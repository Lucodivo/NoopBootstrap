#include "main.h"

#define INIT_WINDOW_WIDTH 640
#define INIT_WINDOW_HEIGHT 480
#define INIT_ASPECT (f32)INIT_WINDOW_WIDTH / INIT_WINDOW_HEIGHT

void scene(WINDOW_HANDLE windowHandle);

int main(int argc, char* argv[]) {
  WINDOW_HANDLE windowHandle;
  GL_CONTEXT_HANDLE glContextHandle;
  initWindow(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT, &windowHandle, &glContextHandle);
  loadOpenGL();
  initImgui(windowHandle, glContextHandle);
  scene(windowHandle);
  deinitWindow(windowHandle);
  return 0;
}

void scene(WINDOW_HANDLE windowHandle) {

  const glm::vec3 worldUp = glm::vec3{0.0f, 1.0f, 0.0f};

  s32 windowWidth, windowHeight;
  windowExtent(windowHandle, &windowWidth, &windowHeight);

  // load 2d textures
  GLuint spiritTexture, birdTexture;
  s32 spiritTexWidth, spiritTexHeight, birdTexWidth, birdTexHeight;
  load2DTexture("data/textures/seed_spirit.png", &spiritTexture, &spiritTexWidth, &spiritTexHeight, LoadTextureFlags::CHUNKY_PIXELS);
  load2DTexture("data/textures/bird_guy.png", &birdTexture, &birdTexWidth, &birdTexHeight, LoadTextureFlags::CHUNKY_PIXELS);

  // load simple vertex attributes for cube
  VertexAtt cubeVertAtt = initializeCubePosNormTexVertexAttBuffers();
  // setup cube's initial model matrix
  glm::vec3 cubePosition = glm::vec3{0.0f, 0.0f, 0.0f};
  glm::vec3 cubeScale = glm::vec3(1.5f);
  glm::vec3 cubeInitRotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 cubeActiveRotationAxis = worldUp;
  f32 cubeActiveRotationPerSecond = 20.0f * RadiansPerDegree;
  glm::mat4 cubeScaleRotationMat = glm::rotate(glm::scale(glm::mat4(), cubeScale),  -45.0f * RadiansPerDegree, cubeInitRotationAxis);
  glm::mat4 cubeTranslationMat = glm::translate(glm::mat4(), cubePosition);

  // load model (w/ vertex attributes) through a gltf file
  Model quadModel;
  loadModel("data/models/quad.glb", &quadModel);
  // setup quad's initial model matrix
  glm::vec3 quadPosition = glm::vec3{-0.8f, -0.55f, -2.0f};
  f32 qScale = 0.2f;
  glm::vec3 quadScale = glm::vec3(qScale);
  glm::mat4 quadScaleMat = glm::scale(glm::mat4(), quadScale);

  // camera and projection initial values
  glm::vec3 cameraPosition = glm::vec3{0.0f, 0.0f, -5.0f};
  Camera camera;
  lookAt(cameraPosition, cubePosition, &camera);
  glm::mat4 projMat = perspective(fieldOfView(13.5f, 25.0f), INIT_ASPECT, 0.01f, 100.0f);

  // create shader program
  ShaderProgram shaderProgram = createShaderProgram("shaders/pos.vert", "shaders/single_color.frag");
  glUseProgram(shaderProgram.id);

  // Create uniform buffer object for model-view-projection matrices=
  // - generate id for view-model-proj buffer
  GLuint modelViewProjUboId;
  glGenBuffers(1, &modelViewProjUboId);
  // - allocate memory for view-model-proj buffer
  glBindBuffer(GL_UNIFORM_BUFFER, modelViewProjUboId);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(ModelViewProjUBO), NULL, GL_STREAM_DRAW);
  // - upload the projection matrix data to the buffer
  glBufferSubData(GL_UNIFORM_BUFFER, offsetof(ModelViewProjUBO, proj), sizeof(glm::mat4), &projMat);
  // - attach buffer to correct binding point (as specified in shader. ex: "binding = 0")
  glBindBufferRange(GL_UNIFORM_BUFFER, modelViewProjUBOBindingIndex, modelViewProjUboId, 0, sizeof(ModelViewProjUBO));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  // Desired gl default state
  glViewport(0, 0, windowWidth, windowHeight);
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

  const f32 cameraWalkMoveSpeedPerSecond = 0.3f;
  const f32 cameraRunMoveSpeedPerSecond = cameraWalkMoveSpeedPerSecond * 2.0f;
  const f32 cameraPitchRotationSpeedPerSecond = 0.01f;
  const f32 quadWalkMoveSpeedPerSecond = 0.3f;
  const f32 quadRunMoveSpeedPerSecond = quadWalkMoveSpeedPerSecond * 3.0f;

  InputState inputState = {};
  bool showDemoWindow = false;
  StopWatch stopWatch = StopWatch();
  while(!inputState.quit && !flagIsSet(inputState.released, InputType::ESC)) {
    stopWatch.update();
    getKeyboardInput(&inputState);

    // Update camera
    const f32 cameraMoveSpeedPerSecond = flagIsSet(inputState.down, InputType::SHIFT) ? cameraRunMoveSpeedPerSecond : cameraWalkMoveSpeedPerSecond;
    glm::vec3 cameraPosDelta = glm::vec3(
            stopWatch.deltaSeconds * cameraMoveSpeedPerSecond * static_cast<f32>(flagIsSet(inputState.down, InputType::D) - flagIsSet(inputState.down, InputType::A)),
            0.0f,
            stopWatch.deltaSeconds * cameraMoveSpeedPerSecond * static_cast<f32>(flagIsSet(inputState.down, InputType::W) - flagIsSet(inputState.down, InputType::S))
            );
    cameraPosition += cameraPosDelta; // TODO: THIS IS NOT CORRECT. WE NEED CAMERA FORWARD/RIGHT/UP.
    f32 cameraPitchDelta = stopWatch.deltaSeconds * cameraPitchRotationSpeedPerSecond * inputState.mouseDeltaY;
    glm::mat4 viewMat = updateCamera(&camera, glm::vec3(cameraPosDelta), cameraPitchDelta, 0.0f);

    // Use keyboard input to move our quad
    const f32 quadMoveSpeedPerSecond = flagIsSet(inputState.down, InputType::SHIFT) ? quadRunMoveSpeedPerSecond : quadWalkMoveSpeedPerSecond;
    glm::vec3 quadDelta = glm::vec3(
            stopWatch.deltaSeconds * quadMoveSpeedPerSecond * static_cast<f32>(flagIsSet(inputState.down, InputType::RIGHT) - flagIsSet(inputState.down, InputType::LEFT)),
            stopWatch.deltaSeconds * quadMoveSpeedPerSecond * static_cast<f32>(flagIsSet(inputState.down, InputType::UP) - flagIsSet(inputState.down, InputType::DOWN)),
            0.0f
            );
    quadPosition += quadDelta;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindBuffer(GL_UNIFORM_BUFFER, modelViewProjUboId);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(ModelViewProjUBO, view), sizeof(glm::mat4), &viewMat);

    // draw cube
    glm::mat4 cubeFrameModelMat = cubeTranslationMat * glm::rotate(cubeScaleRotationMat, static_cast<f32>(cubeActiveRotationPerSecond * getTimeSeconds()), cubeActiveRotationAxis);
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(ModelViewProjUBO, model), sizeof(glm::mat4), &cubeFrameModelMat);
    glDisable(GL_CULL_FACE);
    setSampler2D(shaderProgram.id, "albedoTex", spiritTexIndex);
    drawTriangles(cubeVertAtt);

    // draw quad
    glm::mat4 quadTranslationMat = glm::translate(glm::mat4(), quadPosition);
    glm::mat4 quadFrameModelMat = quadTranslationMat * quadScaleMat;
    glBufferSubData(GL_UNIFORM_BUFFER, offsetof(ModelViewProjUBO, model), sizeof(glm::mat4), &quadFrameModelMat);
    setSampler2D(shaderProgram.id, "albedoTex", birdTexIndex);
    drawModel(quadModel);
    glEnable(GL_CULL_FACE);

    // draw Dear ImGui
    newFrameImGui();
    {
      if (ImGui::BeginMainMenuBar())
      {
        if (ImGui::BeginMenu("View"))
        {
          if (ImGui::MenuItem("Demo Window ", NULL)) {
            showDemoWindow = !showDemoWindow;
          }
          ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
      }
      if(showDemoWindow) {
        ImGui::ShowDemoWindow(&showDemoWindow);
      }
    }
    renderImGui();

    swapBuffers(windowHandle);
  }

  // cleanup vertex attributes/models
  deleteVertexAtts(&cubeVertAtt);
  deleteModels(&quadModel);
}