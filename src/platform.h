#pragma once

typedef void* WINDOW_HANDLE;
typedef void* FILE_HANDLE;
typedef void* GL_CONTEXT_HANDLE;
typedef void* AUDIO_HANDLE;

enum InputType {
#define InputType(name,index,sdlCode) name = 1 << index,
#include "InputType.incl"
#undef InputType
};

struct InputState {
  b32 down;
  b32 activated;
  b32 released;
  s32 mouseDeltaX;
  s32 mouseDeltaY; // positive is up, negative is down
  bool quit;
};

/* OpenGL */
void loadOpenGL();

/* WINDOW */
void initWindow(u32 width, u32 height, OUT WINDOW_HANDLE* windowHandle, OUT GL_CONTEXT_HANDLE* glContextHandle);
void deinitWindow(WINDOW_HANDLE* window, GL_CONTEXT_HANDLE* glContextHandle);
inline void swapBuffers(WINDOW_HANDLE window);
void getWindowDimens(WINDOW_HANDLE window, OUT ivec2* dimens);
void getWindowDimens(WINDOW_HANDLE window, OUT s32* width, OUT s32* height);

/* INPUT */
void getKeyboardInput(InputState* prevState);

/* FILE */
void openFile(const char* fileName, OUT FILE_HANDLE* outFile, OUT size_t* readInBytes);
const char* fileBytes(FILE_HANDLE file);
void closeFile(FILE_HANDLE file);

/* AUDIO: Currently only supports WAV */
void initAudio(AUDIO_HANDLE* handle);
void deinitAudio(AUDIO_HANDLE* handle);
void loadUpSong(AUDIO_HANDLE handle, const char* fileName);
void pauseSong(AUDIO_HANDLE handle, bool pause = true);
void loadUpSoundEffect(AUDIO_HANDLE handle, const char* filename);
void playSoundEffect(AUDIO_HANDLE handle);

/* TIME */
u64 getPerformanceCounter();
u64 getPerformanceCounterFrequencyPerSecond();

/* IMGUI */
void initImgui(WINDOW_HANDLE windowHandle, GL_CONTEXT_HANDLE glContextHandle);
void newFrameImGui();
void renderImGui();