I have a tendency to want to spin up a new project and try something new. This repository is meant to make that process
simple without the fun-suckers that are dealing with dependencies and builds. The intention of this code is to keep things
as simple as possible and avoid any sort of concrete architecture. Since architecture is something that I also like to 
explore in interactive projects, I'd like the starting point to not really take a stance on any specific ways of doing
things.

### Included Dependencies
- glad: [OpenGL loader](https://github.com/Dav1dde/glad)
- SDL2: ["Simple DirectMedia Layer is a cross-platform development library designed to provide low level access to audio, 
keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D"](https://www.libsdl.org/)
- GLM: ["GLM provides classes and functions designed and implemented with the same naming conventions and functionality 
than GLSL so that anyone who knows GLSL, can use GLM as well in C++."](https://github.com/g-truc/glm)
- Dear ImGui: [Debug immediate-mode GUI](https://github.com/ocornut/imgui)
- tinygltf: [Used to parse models in the gltf format (.gltf or .glb extensions)](https://github.com/syoyo/tinygltf)
- nlohmann/json: [A simple json parser also used by tinygltf.](https://github.com/nlohmann/json)
- stb_image/stb_image_write: [Single header libraries used for reading/writing images.](https://github.com/nothings/stb)

### Building
1) Build "bootstrap-dependencies" using the CMakeLists.txt found in the external directory.
2) Build "bootstrap" using the CMakeLists.txt in the root directory

### Running
The program must run with root as the working directory. Ensure that the executable and the SDL2 dynamic library (dll) 
are in the same directory.