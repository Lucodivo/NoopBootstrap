#include "main.h"

#define INIT_WINDOW_WIDTH 1920
#define INIT_WINDOW_HEIGHT 1080
#define INIT_ASPECT (f32)INIT_WINDOW_WIDTH / INIT_WINDOW_HEIGHT

const glm::vec4 debugColor = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);

void scene(WINDOW_HANDLE windowHandle, AUDIO_HANDLE audioHandle);

struct AppState {
  WINDOW_HANDLE windowHandle;
  AUDIO_HANDLE audioHandle;
  ivec2 windowDimens;
  bool hiddenMouse;
};

int main(int argc, char* argv[]) {
  // platform initialization
  WINDOW_HANDLE windowHandle;
  GL_CONTEXT_HANDLE glContextHandle;
  initWindow(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT, &windowHandle, &glContextHandle);
  AUDIO_HANDLE audioHandle;
  initAudio(&audioHandle);
  loadOpenGL();
  initImgui(windowHandle, glContextHandle);

  // program
  scene(windowHandle, audioHandle);

  // platform breakdown
  deinitAudio(&audioHandle);
  deinitWindow(&windowHandle, &glContextHandle);
  return 0;
}

struct CollisionBox2D {
  glm::vec2 lowerLeft;
  glm::vec2 width;
  glm::vec2 height;
};

void logV(bool* pOpen, const char* fmt, ...) {
  va_list argptr;
  va_start(argptr, fmt);
  if(*pOpen) {
    if(ImGui::Begin("General Debug", pOpen)) {
      ImGui::TextV(fmt, argptr);
    }ImGui::End();
  }
  va_end(argptr);
}

void scene(WINDOW_HANDLE windowHandle, AUDIO_HANDLE audioHandle) {
  AppState appState;
  appState.windowHandle = windowHandle;
  appState.audioHandle = audioHandle;

  getWindowDimens(windowHandle, &appState.windowDimens);
  assert(INIT_WINDOW_WIDTH == appState.windowDimens.x && INIT_WINDOW_HEIGHT == appState.windowDimens.y);

  appState.hiddenMouse = false;
  hideMouse(appState.hiddenMouse);

  // load 2d textures
  GLuint spiritTexture, birdTexture;
  ivec2 spiritTexDimens, birdTexDimens;
  load2DTexture("data/textures/seed_spirit.png", &spiritTexture, &spiritTexDimens.x, &spiritTexDimens.y, LoadTextureFlags::CHUNKY_PIXELS);
  load2DTexture("data/textures/bird_guy.png", &birdTexture, &birdTexDimens.x, &birdTexDimens.y, LoadTextureFlags::CHUNKY_PIXELS);

  // load sounds
  loadUpSong(audioHandle, "data/sounds/songs/fairy_loop.wav");
  loadUpSoundEffect(audioHandle, "data/sounds/clips/echo.wav");

  // load models
  VertexAtt cubeVertAtt = initializeCubePosNormTexVertexAttBuffers();
  Model quadModel; loadModel("data/models/quad.glb", &quadModel);

  // setup cube's initial model matrix
  glm::vec3 cubePosition = glm::vec3{0.0f, 0.0f, 0.0f};
  glm::vec3 cubeScale = glm::vec3(1.5f);
  glm::vec3 cubeInitRotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 cubeActiveRotationAxis = worldUp;
  f64 cubeActiveRotationPerSecond = Radians(20.0f);
  glm::mat4 cubeScaleRotationMat = glm::rotate(glm::scale(glm::mat4(), cubeScale),  Radians(-45.0f), cubeInitRotationAxis);
  glm::mat4 cubeTranslationMat = glm::translate(glm::mat4(), cubePosition);

  // setup quad's initial model matrix
  ivec2 pixelUpscale{8, 8}; // NOTE: Ensure resolution is equally divisible by upscale value
  ivec2 emulatedPixelResolution{appState.windowDimens.x / pixelUpscale.x, appState.windowDimens.y / pixelUpscale.y};
  glm::vec2 spriteTilePosition = glm::vec2{emulatedPixelResolution.x * 0.5f, emulatedPixelResolution.y * 0.5f};
  const f32 spriteWalkTilesPerSecond = 16.0f;
  const f32 spriteRunTilesPerSecond = spriteWalkTilesPerSecond * 3.0f;

  // camera and projection initial values
  glm::vec3 cameraPosition = glm::vec3{0.0f, 0.0f, -5.0f};
  Camera camera;
  lookAt(cameraPosition, cubePosition, &camera);
  glm::mat4 projMat = perspective(fieldOfView(13.5f, 25.0f), INIT_ASPECT, 0.01f, 100.0f);

  ShaderProgram texShaderProgram = createShaderProgram("shaders/pos.vert", "shaders/texture.frag");
  glUseProgram(texShaderProgram.id);

  ModelViewProjUniformBuffer mvpUbo;
  mvpUbo.setProjection(&projMat);

  ShaderProgram spriteShaderProgram = createShaderProgram("shaders/sprite.vert", "shaders/texture.frag");
  glUseProgram(spriteShaderProgram.id);

  PixelUniformBuffer pixelUbo;
  pixelUbo.setSpriteDimens(&birdTexDimens);
  pixelUbo.setEmulatedWindowRes(&emulatedPixelResolution);

  // Desired gl default state
  glViewport(0, 0, appState.windowDimens.x, appState.windowDimens.y);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  glm::vec3 clearColor = glm::vec3(0.7f);
  glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);

  // bind our 2d textures to specific active indices
  s32 spiritTexIndex = 0, birdTexIndex = 1;
  bindActiveTexture2d(spiritTexIndex, spiritTexture);
  bindActiveTexture2d(birdTexIndex, birdTexture);

  const f32 cameraWalkMoveSpeedPerSecond = 0.8f;
  const f32 cameraRunMoveSpeedPerSecond = cameraWalkMoveSpeedPerSecond * 3.0f;
  const f32 cameraPitchRotationSpeedPerSecond = 0.04f;
  const f32 cameraYawRotationSpeedPerSecond = 0.04f;

  InputState inputState{};
  bool showNavBar = true, showDemoWindow = false, showFPS = true, showDebug = true, playMusic = false;
  RingSampler fpsSampler = RingSampler();
  Stopwatch stopwatch{};
  reset(&stopwatch);
  while(!inputState.quit && !flagIsSet(inputState.released, InputType::ESC)) {
    lap(&stopwatch);
    getKeyboardInput(&inputState);

    auto toggleMouseAndCameraControl = [&]() {
      appState.hiddenMouse = !appState.hiddenMouse;
      hideMouse(appState.hiddenMouse);
    };

    auto toggleMusic = [&]() {
      playMusic = !playMusic;
      pauseSong(audioHandle, !playMusic);
    };

    // Toggle mouse capture
    if(flagIsSet(inputState.released, InputType::TAB)) {
      toggleMouseAndCameraControl();
    }

    // Toggle nav bar
    if(flagIsSet(inputState.released, InputType::ALT)) {
      showNavBar = !showNavBar;
    }

    // Toggle music
    if(flagIsSet(inputState.released, InputType::Q)) {
      toggleMusic();
    }

    if(flagIsSet(inputState.released, InputType::E)) {
      playSoundEffect(audioHandle);
    }

    // Update camera
    const f32 cameraMoveSpeedPerSecond = flagIsSet(inputState.down, InputType::SHIFT) ? cameraRunMoveSpeedPerSecond : cameraWalkMoveSpeedPerSecond;
    f32 forwardDeltaUnits = static_cast<f32>(stopwatch.deltaSeconds * cameraMoveSpeedPerSecond * (flagIsSet(inputState.down, InputType::W) - flagIsSet(inputState.down, InputType::S)));
    f32 rightDeltaUnits = static_cast<f32>(stopwatch.deltaSeconds * cameraMoveSpeedPerSecond * (flagIsSet(inputState.down, InputType::D) - flagIsSet(inputState.down, InputType::A)));
    glm::vec3 cameraPosDelta = glm::vec3(
            forwardDeltaUnits * camera.forward.x + rightDeltaUnits * camera.right.x,
            0.0f,
            forwardDeltaUnits * camera.forward.z + rightDeltaUnits * camera.right.z
            );
    cameraPosition += cameraPosDelta;
    f32 cameraPitchDelta = appState.hiddenMouse ? static_cast<f32>(stopwatch.deltaSeconds * cameraPitchRotationSpeedPerSecond * inputState.mouseDeltaY) : 0.0f;
    f32 cameraYawDelta = appState.hiddenMouse ? static_cast<f32>(stopwatch.deltaSeconds * cameraYawRotationSpeedPerSecond * -inputState.mouseDeltaX) : 0.0f;
    glm::mat4 viewMat = updateCamera(&camera, glm::vec3(cameraPosDelta), cameraPitchDelta, cameraYawDelta);

    // Use keyboard input to move our quad
    const f32 spriteMoveSpeedPerSecond = flagIsSet(inputState.down, InputType::SHIFT) ? spriteRunTilesPerSecond : spriteWalkTilesPerSecond;
    glm::vec2 spriteDelta = glm::vec2(
            stopwatch.deltaSeconds * spriteMoveSpeedPerSecond * static_cast<f32>(flagIsSet(inputState.down, InputType::RIGHT) - flagIsSet(inputState.down, InputType::LEFT)),
            stopwatch.deltaSeconds * spriteMoveSpeedPerSecond * static_cast<f32>(flagIsSet(inputState.down, InputType::UP) - flagIsSet(inputState.down, InputType::DOWN))
            );
      spriteTilePosition += spriteDelta;
    glm::vec2 birdPixelDimens{23.0f, 32.0f}; // Note: THESE NUMBERS WERE MANUALLY PULLED AFTER INSPECTING THE BIRD SPRITE TEXTURE
    ivec2 birdImagePixelOffset{1, 0}; // Note: THESE NUMBERS WERE MANUALLY PULLED AFTER INSPECTING THE BIRD SPRITE TEXTURE
    spriteTilePosition.x = Clamp(spriteTilePosition.x, 0.0f, emulatedPixelResolution.x - birdTexDimens.x);
    spriteTilePosition.y = Clamp(spriteTilePosition.y, 0.0f, emulatedPixelResolution.y - birdTexDimens.y);

    // Set view matrix in Model-View-Projection uniform buffer
    mvpUbo.setView(&viewMat);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw cube
    glm::mat4 cubeFrameModelMat = cubeTranslationMat * glm::rotate(cubeScaleRotationMat, static_cast<f32>(cubeActiveRotationPerSecond * stopwatch.totalElapsedSeconds), cubeActiveRotationAxis);
    mvpUbo.setModel(&cubeFrameModelMat);
    glUseProgram(texShaderProgram.id);
    glDisable(GL_CULL_FACE);
    setSampler2D(texShaderProgram.id, "albedoTex", spiritTexIndex);
    drawTriangles(cubeVertAtt);
    glEnable(GL_CULL_FACE);

    // draw sprite
    glUseProgram(spriteShaderProgram.id);
    pixelUbo.setPos(&spriteTilePosition);
    setSampler2D(spriteShaderProgram.id, "albedoTex", birdTexIndex);
    drawModel(quadModel);

    // draw Dear ImGui
    newFrameImGui();
    {
      if(showNavBar) {
        if (ImGui::BeginMainMenuBar())
        {
          if (ImGui::BeginMenu("Edit"))
          {
            if (ImGui::MenuItem("Toggle Camera/Mouse", "Tab")) {
              toggleMouseAndCameraControl();
            }
            if (ImGui::MenuItem("Toggle Music", "Q")) {
              toggleMusic();
            }
            if (ImGui::MenuItem("Play Sound Effect", "E")) {
              playSoundEffect(audioHandle);
            }
            ImGui::EndMenu();
          }
          if (ImGui::BeginMenu("View"))
          {
            if(ImGui::MenuItem("Navigation Bar", "Alt")) {
              showNavBar = !showNavBar;
            }
            if (ImGui::MenuItem("Demo Window", NULL)) {
              showDemoWindow = !showDemoWindow;
            }
            if (ImGui::MenuItem("FPS", NULL)) {
              showFPS = !showFPS;
            }
            if (ImGui::MenuItem("Toggle Music", NULL)) {
              playMusic = !playMusic;
              pauseSong(audioHandle, !playMusic);
            }
            ImGui::EndMenu();
          }
          ImGui::EndMainMenuBar();
        }
      }
      if(showDemoWindow) {
        ImGui::ShowDemoWindow(&showDemoWindow);
      }
      fpsSampler.addValue(stopwatch.deltaSeconds);
      const ImGuiWindowFlags textNoFrills = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize;
      if(showFPS){
        if(ImGui::Begin("FPS", &showFPS, textNoFrills)) {
          ImGui::Text("%5.1f ms | %3.1f fps", fpsSampler.average() * 1000.0, 1.0 / fpsSampler.average());
        }ImGui::End();
      }

      logV(&showDebug, "Pos: {X = %2.2f, Y = %2.2f}", spriteTilePosition.x, spriteTilePosition.y);
    }
    renderImGui();

    swapBuffers(windowHandle);
  }

  // cleanup vertex attributes/models
  deleteVertexAtts(&cubeVertAtt);
  deleteModels(&quadModel);
}