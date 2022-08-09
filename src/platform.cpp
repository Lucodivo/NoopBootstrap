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
  SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);

  SDL_GLContext context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, context);

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

void windowExtent(WINDOW_HANDLE window, s32* width, s32* height) {
  SDL_GetWindowSize((SDL_Window*)window, width, height);
  assert(*width >= 0 && *height >= 0);
}

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
          case SDLK_LSHIFT:
            setFlags(&prevState->activated, InputType::SHIFT);
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
          case SDLK_LSHIFT:
            setFlags(&prevState->released, InputType::SHIFT);
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
inline f64 getTimeMilliseconds() { return SDL_GetTicks(); }
inline f64 getTimeSeconds() { return getTimeMilliseconds() / 1000.0; }

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