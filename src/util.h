#pragma once

struct Box {
  glm::vec3 min;
  glm::vec3 diagonal;
};

struct Stopwatch {
  f64 totalElapsedSeconds;
  f64 deltaSeconds;
  f64 lastKnownFrame;
};

b32 flagIsSet(b32 flags, b32 queryFlag) { return (flags & queryFlag) ? 1 : 0; } // ensure the values are 0/1
void setFlags(b32* outFlags, b32 newFlags) { *outFlags |= newFlags; }
void removeFlags(b32* outFlags, b32 removeFlags) { *outFlags &= ~removeFlags; }

void reset(Stopwatch* stopwatch) {
  stopwatch->totalElapsedSeconds = 0.0f;
  stopwatch->lastKnownFrame = getTimeSeconds();
  stopwatch->deltaSeconds = 0.0f;
}

void lap(Stopwatch* stopwatch) {
  f64 t = getTimeSeconds();
  stopwatch->deltaSeconds = t - stopwatch->lastKnownFrame;
  stopwatch->lastKnownFrame = t;
  stopwatch->totalElapsedSeconds += stopwatch->deltaSeconds;
}

struct RingSampler {
  f64 values[6] = {};
  u32 newEntryIndex = 0;
  RingSampler() = default;
  void addValue(f64 newVal) {
    values[newEntryIndex] = newVal;
    newEntryIndex = (newEntryIndex + 1) % 6;
  }
  f64 average() {
    f64 total = 0.0;
    for(u32 i = 0; i < 6; ++i) {
      total += values[0];
    }
    return total / 6.0;
  }
};