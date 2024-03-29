#pragma once

void loadOpenGL() {
  // initialize GLAD to acquire function pointers for OpenGL
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    std::cout << "Failed to initialize GLAD!" << std::endl;
    exit(-1);
  }
}

/* Window */
void initWindow(s32 width, s32 height, WINDOW_HANDLE* windowHandle, GL_CONTEXT_HANDLE* glContextHandle) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow(
          "bootstrap",
          SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          width,
          height,
          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
  );
  SDL_CaptureMouse(SDL_TRUE);

  SDL_GLContext context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, context);
  SDL_GL_SetSwapInterval(1); // enable v-sync

  *windowHandle = window;
  *glContextHandle = context;
}

void deinitWindow(WINDOW_HANDLE* window, GL_CONTEXT_HANDLE* glContextHandle) {
  SDL_GL_DeleteContext((SDL_GLContext)*glContextHandle);
  SDL_DestroyWindow((SDL_Window*)*window);
  SDL_Quit();
  *window = nullptr;
  *glContextHandle = nullptr;
}

inline void swapBuffers(WINDOW_HANDLE window) {
  SDL_GL_SwapWindow((SDL_Window*)window);
}

void getWindowDimens(WINDOW_HANDLE window, ivec2* dimens) {
  return getWindowDimens(window, &dimens->x, &dimens->y);
}

void getWindowDimens(WINDOW_HANDLE window, s32* width, s32* height) {
  SDL_GetWindowSize((SDL_Window*)window, width, height);
  assert(*width >= 0 && *height >= 0);
}

inline void hideMouse(bool hide) { SDL_SetRelativeMouseMode(hide ? SDL_TRUE : SDL_FALSE); }

/* INPUT */
void getKeyboardInput(InputState* prevState) {
  prevState->activated = 0;
  prevState->released = 0;

  SDL_Event event;
  while( SDL_PollEvent( &event ) ){
    ImGui_ImplSDL2_ProcessEvent(&event);

    /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
    switch( event.type ){
      case SDL_KEYDOWN:
        switch( event.key.keysym.sym ){
#define InputType(name,index,sdlCode) \
          case sdlCode:               \
            setFlags(&prevState->activated, InputType::##name); \
            break;
#include "InputType.incl"
#undef InputType
          default:
            break;
        }
        break;
      case SDL_KEYUP:
        switch( event.key.keysym.sym ){
#define InputType(name,index,sdlCode) \
          case sdlCode:               \
            setFlags(&prevState->released, InputType::##name); \
            break;
#include "InputType.incl"
#undef InputType
          default:
            break;
        }
        break;
      case SDL_QUIT:
        prevState->quit = true;
        break;
      case SDL_MOUSEMOTION:
      default:
        break;
    }
  }

  SDL_GetRelativeMouseState(&prevState->mouseDeltaX, &prevState->mouseDeltaY);
  prevState->mouseDeltaY = -prevState->mouseDeltaY;

  setFlags(&prevState->down, prevState->activated);
  clearFlags(&prevState->down, prevState->released);
}

/* FILE */
void openFile(const char* fileName, FILE_HANDLE* outFile, size_t* readInBytes) {
  *outFile = SDL_LoadFile(fileName, readInBytes);
  assert(*outFile != nullptr);
}

const char* fileBytes(FILE_HANDLE file) {
  return (const char*)file;
}

void closeFile(FILE_HANDLE file) {
  SDL_free(file);
}

/* AUDIO: Currently only supports WAV */
enum AudioFlags {
  ACTIVE = 1 << 0,
  PAUSED = 1 << 1,
  LOOPS = 1 << 2,
};

struct Sound {
  SDL_AudioSpec audioSpec;
  u8* buffer;
  u32 length;
  u32 readPos;
  b32 audioFlags;

  bool active() const { return audioFlags & AudioFlags::ACTIVE; }
  bool loops() const { return audioFlags & AudioFlags::LOOPS; }
};

struct AudioState {
  Sound song;
  Sound soundEffect;
  SDL_AudioSpec audioSpec;
  SDL_AudioDeviceID deviceId;
};

/*
  - SDL2 is not great for audio and SDL_MixAudioFormat is not meant for mixing more than two streams.
      - A separate library is necessary for more complex audio.
*/
void sdlAudioCallback(void* userdata, u8* stream, s32 bytesRequested) {
  AudioState* audioState = static_cast<AudioState*>(userdata);

  // insert song into buffer if available
  Sound& song = audioState->song;
  if(flagIsSet(song.audioFlags, AudioFlags::ACTIVE) && !flagIsSet(song.audioFlags, AudioFlags::PAUSED)) {
    s32 remainingAudio = (s32)song.length - (s32)song.readPos;
    u32 bytesToWrite = Min(bytesRequested, remainingAudio);
    memcpy(stream, song.buffer + song.readPos, bytesToWrite);
    song.readPos += bytesToWrite;

    // check to see if we reached the end of the song
    if(song.readPos == song.length) {
      u32 remainingLen = bytesRequested - bytesToWrite;
      if(flagIsSet(song.audioFlags, AudioFlags::LOOPS)) {
        memcpy(stream + bytesToWrite, song.buffer, remainingLen);
        song.readPos = remainingLen;
      } else {
        memset(stream + bytesToWrite, 0, remainingLen);
        setFlags(&song.audioFlags, AudioFlags::PAUSED);
      }
    }
  } else {
    // clear the audio stream
    memset(stream, 0, bytesRequested);
  }

  // play sound effect if available
  Sound& soundEffect = audioState->soundEffect;
  if(flagIsSet(soundEffect.audioFlags, AudioFlags::ACTIVE) &&
          !flagIsSet(soundEffect.audioFlags, AudioFlags::PAUSED) &&
          soundEffect.readPos != soundEffect.length) {
    s32 remainingAudio = (s32)soundEffect.length - (s32)soundEffect.readPos;
    u32 bytesToWrite = Min(bytesRequested, remainingAudio);
    SDL_MixAudioFormat(stream, soundEffect.buffer + soundEffect.readPos, soundEffect.audioSpec.format, bytesToWrite, 128);
    soundEffect.readPos += bytesToWrite;
  }
}

void initAudio(AUDIO_HANDLE* handle) {
  SDL_Init(SDL_INIT_AUDIO);

  AudioState* audioState = new AudioState();

  memset(audioState, 0, sizeof(AudioState));

  SDL_AudioSpec desiredAudioSpec{};
  desiredAudioSpec.freq = 44100;
  desiredAudioSpec.format = AUDIO_S32;
  desiredAudioSpec.channels = 2;
  desiredAudioSpec.samples = 4096;
  desiredAudioSpec.callback = sdlAudioCallback;
  desiredAudioSpec.userdata = audioState;

  audioState->deviceId = SDL_OpenAudioDevice(nullptr, 0, &desiredAudioSpec, &audioState->audioSpec, 0);
  SDL_PauseAudioDevice(audioState->deviceId, 0);

  *handle = audioState;
}

void deinitAudio(AUDIO_HANDLE* handle) {
  AudioState* audioState = static_cast<AudioState*>(*handle);
  Sound& song = audioState->song;
  Sound& soundEffect = audioState->soundEffect;

  SDL_PauseAudioDevice(audioState->deviceId, 1);

  if(flagIsSet(song.audioFlags, AudioFlags::ACTIVE)) {
    SDL_FreeWAV(song.buffer);
  }

  if(flagIsSet(soundEffect.audioFlags, AudioFlags::ACTIVE)) {
    SDL_FreeWAV(soundEffect.buffer);
  }

  delete audioState;
  *handle = nullptr;
}

void loadUpSong(AUDIO_HANDLE handle, const char* fileName) {
  AudioState* audioState = static_cast<AudioState*>(handle);
  Sound& song = audioState->song;

  if(flagIsSet(song.audioFlags, AudioFlags::ACTIVE)) {
    SDL_FreeWAV(song.buffer);
  }

  if (SDL_LoadWAV(fileName, &song.audioSpec, &song.buffer, &song.length) == nullptr) {
    fprintf(stderr, "Could not open wav sound file (%s fileName): %s\n", fileName, SDL_GetError());
  }

  // TODO: Potentially adjust some aspect of the audio spec for device

  song.audioFlags = 0;
  setFlags(&song.audioFlags, AudioFlags::ACTIVE | AudioFlags::LOOPS | AudioFlags::PAUSED);
}

void pauseSong(AUDIO_HANDLE handle, bool pause) {
  AudioState* audioState = static_cast<AudioState*>(handle);
  Sound& song = audioState->song;
  assert(song.active());
  if(pause) {
    setFlags(&song.audioFlags, AudioFlags::PAUSED);
  } else {
    clearFlags(&song.audioFlags, AudioFlags::PAUSED);
  }
}

void loadUpSoundEffect(AUDIO_HANDLE handle, const char* fileName) {
  AudioState* audioState = static_cast<AudioState*>(handle);
  Sound& soundEffect = audioState->soundEffect;

  if(soundEffect.active()) {
    SDL_FreeWAV(soundEffect.buffer);
  }

  if (SDL_LoadWAV(fileName, &soundEffect.audioSpec, &soundEffect.buffer, &soundEffect.length) == nullptr) {
    fprintf(stderr, "Could not open wav sound file (%s fileName): %s\n", fileName, SDL_GetError());
  }

  // TODO: Potentially adjust some aspect of the audio spec for device

  soundEffect.readPos = 0;
  soundEffect.audioFlags = 0;
  setFlags(&soundEffect.audioFlags, AudioFlags::ACTIVE | AudioFlags::PAUSED);
}

void playSoundEffect(AUDIO_HANDLE handle) {
  AudioState* audioState = static_cast<AudioState*>(handle);
  Sound& soundEffect = audioState->soundEffect;
  assert(soundEffect.active());
  clearFlags(&soundEffect.audioFlags, AudioFlags::PAUSED);
  soundEffect.readPos = 0;
}

/* TIME */
inline u64 getPerformanceCounter() { return SDL_GetPerformanceCounter(); }
inline u64 getPerformanceCounterFrequencyPerSecond() { return SDL_GetPerformanceFrequency(); }

/* IMGUI */
void initImgui(WINDOW_HANDLE windowHandle, GL_CONTEXT_HANDLE glContextHandle) {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)windowHandle, (SDL_GLContext*)glContextHandle);
  ImGui_ImplOpenGL3_Init(nullptr);
}

void newFrameImGui() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
}

void renderImGui() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}