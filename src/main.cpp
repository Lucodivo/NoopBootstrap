#include <SDL.h>
#include <SDL_opengl.h>

#include "glm/vec3.hpp"

#include "types.h"

#define INIT_WINDOW_WIDTH 640
#define INIT_WINDOW_HEIGHT 480

SDL_Window* initSDL(){
  SDL_Window* window = SDL_CreateWindow(
          "bootstrap",
          SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
          INIT_WINDOW_WIDTH,
          INIT_WINDOW_HEIGHT,
          SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
  );
  SDL_CaptureMouse(SDL_TRUE);
  SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
  return window;
}

int main(int argc, char* argv[]) {
  glm::vec3 screenColor = glm::vec3{0.5f, 0.0f, 0.0f};

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = initSDL();
  SDL_GLContext context = SDL_GL_CreateContext(window);

  SDL_GL_MakeCurrent(window, context);
  glClearColor(screenColor.r, screenColor.g, screenColor.b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT );
  SDL_GL_SwapWindow(window);

  SDL_Delay(3000);

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}