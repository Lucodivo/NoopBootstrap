#pragma once

const glm::vec3 defaultUp{0.0f, 1.0f, 0.0f};
const f32 maxPitchFirstPerson = Radians(85.0f);
const f32 minPitchFirstPerson = Radians(-85.0f);

struct Camera {
  glm::vec3 origin;
  glm::vec3 forward;
  glm::vec3 right;
  glm::vec3 up;
};

void lookAt(glm::vec3 origin, glm::vec3 focus, Camera* camera) {
  camera->origin = origin;
  camera->forward = normalize(focus - origin);
  assert(camera->forward.y < cos(Radians(5.0f)) && camera->forward.y > cos(Radians(175.0f)));
  camera->right = normalize(cross(defaultUp, camera->forward));
  camera->up = cross(camera->forward, camera->right);
}

glm::mat4 updateCamera(Camera* camera, glm::vec3 posOffset, f32 pitchOffset, f32 yawOffset) {
  camera->origin += posOffset;

  glm::mat4 translation{
          1.0f,             0.0f,       0.0f,       0.0f,
          0.0f,             1.0f,       0.0f,       0.0f,
          0.0f,             0.0f,       1.0f,       0.0f,
          -camera->origin.x, -camera->origin.y, -camera->origin.z, 1.0f
  };

  glm::mat4 measure{
          camera->right.x, camera->up.x, camera->forward.x, 0.0f,
          camera->right.y, camera->up.y, camera->forward.y, 0.0f,
          camera->right.z, camera->up.z, camera->forward.z, 0.0f,
          0.0f,        0.0f,              0.0f, 1.0f
  };

  return measure * translation;
}

// real-time rendering 4.7.2
// ex: screenWidth = 20.0f, screenDist = 30.0f will provide the horizontal field of view
// for a person sitting 30 inches away from a 20 inch screen, assuming the screen is
// perpendicular to the line of sight.
// NOTE: Any units work as long as they are the same. Works for vertical and horizontal.
f32 fieldOfView(f32 screenWidth, f32 screenDist) {
  f32 phi = 2.0f * atanf(screenWidth/(2.0f * screenDist));
  return phi;
}

// real-time rendering 4.7.2
// aspect ratio is equivalent to width / height
inline glm::mat4 perspective(f32 fovVert, f32 aspect, f32 n, f32 f) {
  glm::mat4 result;

  const f32 c = 1.0f / tanf(fovVert / 2.0f);
  result[0] = {(c / aspect), 0.0f, 0.0f, 0.0f};
  result[1] = {0.0f, c, 0.0f, 0.0f};
  result[2] = {0.0f, 0.0f, (f + n) / (f - n), 1.0f};
  result[3] = {0.0f, 0.0f, -(2.0f * f * n) / (f - n), 0.0f};

  return result;
}