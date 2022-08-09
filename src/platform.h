#pragma once

typedef void* WINDOW_HANDLE;
typedef void* FILE_HANDLE;
typedef void* GL_CONTEXT_HANDLE;

enum InputType {
  UP = 1 << 0,
  DOWN = 1 << 1,
  LEFT = 1 << 2,
  RIGHT = 1 << 3,
  SHIFT = 1 << 4
};

struct InputState {
  b32 down;
  b32 activated;
  b32 released;
  bool quit;
};

/* OpenGL */
void loadOpenGL();

/* WINDOW */
void initWindow(u32 width, u32 height, WINDOW_HANDLE* windowHandle, GL_CONTEXT_HANDLE* glContextHandle);
void deinitWindow(WINDOW_HANDLE window);
inline void swapBuffers(WINDOW_HANDLE window);
void windowExtent(WINDOW_HANDLE window, s32* width, s32* height);

/* INPUT */
void getKeyboardInput(InputState* prevState);

/* FILE */
void openFile(const char* fileName, FILE_HANDLE* outFile, size_t* readInBytes);
const char* fileBytes(FILE_HANDLE file);
void closeFile(FILE_HANDLE file);

/* TIME */
inline f64 getTimeMilliseconds();
inline f64 getTimeSeconds();

/* IMGUI */
void initImgui(WINDOW_HANDLE windowHandle, GL_CONTEXT_HANDLE glContextHandle);
void newFrameImGui();
void renderImGui();