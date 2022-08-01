#pragma once

typedef void* WINDOW_HANDLE;
typedef void* FILE_HANDLE;

enum InputType {
  UP = 1 << 0,
  DOWN = 1 << 1,
  LEFT = 1 << 2,
  RIGHT = 1 << 3
};

struct InputState {
  b32 down;
  b32 activated;
  b32 released;
  bool quit;
};

void loadOpenGL() {
  // initialize GLAD to acquire function pointers for OpenGL
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    std::cout << "Failed to initialize GLAD!" << std::endl;
    exit(-1);
  }
}

/* INPUT */
WINDOW_HANDLE initWindow(u32 width, u32 height) {
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
  SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);

  SDL_GLContext context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, context);

  return window;
}

void deinitWindow(WINDOW_HANDLE window) {
  SDL_DestroyWindow((SDL_Window*)window);
  SDL_Quit();
}

inline void swapBuffers(WINDOW_HANDLE window) {
  SDL_GL_SwapWindow((SDL_Window*)window);
}

/* INPUT */
void getKeyboardInput(InputState* prevState) {
  prevState->activated = 0;
  prevState->released = 0;

  SDL_Event event;
  while( SDL_PollEvent( &event ) ){
    /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
    switch( event.type ){
      case SDL_KEYDOWN:
        switch( event.key.keysym.sym ){
          case SDLK_LEFT:
            setFlags(&prevState->activated, InputType::LEFT);
            break;
          case SDLK_RIGHT:
            setFlags(&prevState->activated, InputType::RIGHT);
            break;
          case SDLK_UP:
            setFlags(&prevState->activated, InputType::UP);
            break;
          case SDLK_DOWN:
            setFlags(&prevState->activated, InputType::DOWN);
            break;
          default:
            break;
        }
        break;
      case SDL_KEYUP:
        switch( event.key.keysym.sym ){
          case SDLK_LEFT:
            setFlags(&prevState->released, InputType::LEFT);
            break;
          case SDLK_RIGHT:
            setFlags(&prevState->released, InputType::RIGHT);
            break;
          case SDLK_UP:
            setFlags(&prevState->released, InputType::UP);
            break;
          case SDLK_DOWN:
            setFlags(&prevState->released, InputType::DOWN);
            break;
          case SDLK_ESCAPE:
            prevState->quit = true;
            break;
          default:
            break;
        }
        break;
      case SDL_QUIT:
        prevState->quit = true;
        break;
      default:
        break;
    }
  }

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
f64 getElapsedTime() {
  return SDL_GetTicks() / 1000.0; // ticks are in milliseconds
}