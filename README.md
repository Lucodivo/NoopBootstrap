I have a tendency to want to spin up a new project and try something new. This repository is meant to make that process
simple without fun-sucker that is dealing with dependencies and CMake.

### Included Dependencies
- glad: [OpenGL loader](https://github.com/Dav1dde/glad)
- SDL2: ["Simple DirectMedia Layer is a cross-platform development library designed to provide low level access to audio, 
keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D"](https://www.libsdl.org/)
  - Included as a submodule. Builds alongside the CMake project.
- GLM: ["GLM provides classes and functions designed and implemented with the same naming conventions and functionality 
than GLSL so that anyone who knows GLSL, can use GLM as well in C++."](https://github.com/g-truc/glm)

### Building
1) Build "bootstrap-dependencies" using the CMakeLists.txt found in the external directory.
2) Build "bootstrap" using the CMakeLists.txt in the root directory