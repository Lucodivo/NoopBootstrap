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
void initWindow(u32 width, u32 height, WINDOW_HANDLE* windowHandle, GL_CONTEXT_HANDLE* glContextHandle) {
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

void deinitWindow(WINDOW_HANDLE window) {
  SDL_DestroyWindow((SDL_Window*)window);
  SDL_Quit();
}

inline void swapBuffers(WINDOW_HANDLE window) {
  SDL_GL_SwapWindow((SDL_Window*)window);
}

void getWindowExtent(WINDOW_HANDLE window, s32* width, s32* height) {
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
        break;
      default:
        break;
    }
  }

  SDL_GetRelativeMouseState(&prevState->mouseDeltaX, &prevState->mouseDeltaY);
  prevState->mouseDeltaY = -prevState->mouseDeltaY;

  setFlags(&prevState->down, prevState->activated);
  removeFlags(&prevState->down, prevState->released);
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
  ImGui_ImplOpenGL3_Init(NULL);
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