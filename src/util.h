#pragma once

struct Box {
  glm::vec3 min;
  glm::vec3 diagonal;
};

struct StopWatch {
  f64 totalElapsedSeconds;
  f64 deltaSeconds;
  f64 lastKnownFrame;
  StopWatch();
  void update();
};

b32 flagIsSet(b32 flags, b32 queryFlag) { return (flags & queryFlag) ? 1 : 0; } // ensure the values are 0/1
void setFlags(b32* outFlags, b32 newFlags) { *outFlags |= newFlags; }
void removeFlags(b32* outFlags, b32 removeFlags) { *outFlags &= ~removeFlags; }

StopWatch::StopWatch() {
  totalElapsedSeconds = 0.0f;
  lastKnownFrame = getTimeSeconds();
  deltaSeconds = 0.0f;
}

void StopWatch::update() {
  f64 t = getTimeSeconds();
  deltaSeconds = t - lastKnownFrame;
  lastKnownFrame = t;
  totalElapsedSeconds += deltaSeconds;
}