#include <SDL.h>
#include <glad/glad.h>

#include <iostream>

#include "glm/vec3.hpp"

#include "types.h"
#include "cube.h"

#define INIT_WINDOW_WIDTH 640
#define INIT_WINDOW_HEIGHT 480

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

void setFlags(b32* outFlags, b32 newFlags) { *outFlags |= newFlags; }

void removeFlags(b32* outFlags, b32 removeFlags) { *outFlags &= ~removeFlags; }

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


int main(int argc, char* argv[]) {
  glm::vec3 screenColor = glm::vec3{0.5f, 0.0f, 0.0f};

  SDL_Init(SDL_INIT_VIDEO);
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

  SDL_GLContext context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, context);

  // initialize GLAD to acquire function pointers for OpenGL
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    std::cout << "Failed to initialize GLAD!" << std::endl;
    exit(-1);
  }

  VertexAtt cubeVertAtt = initCubePosVertexAttBuffers();
  InputState inputState = {};
  while(!inputState.quit) {
    getKeyboardInput(&inputState);

    glClearColor(screenColor.r, screenColor.g, screenColor.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT );

    SDL_GL_SwapWindow(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}