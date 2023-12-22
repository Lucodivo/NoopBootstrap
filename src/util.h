#pragma once

struct Box {
  glm::vec3 min;
  glm::vec3 diagonal;
};

struct Stopwatch {
  u64 lastPerfCounter;
  f64 secondsPerPerfCounter;
  f64 totalElapsedSeconds;
  f64 deltaSeconds;
};

bool flagIsSet(b32 flags, b32 queryFlag) { return (flags & queryFlag); } // ensure the values are 0/1
bool flagsAreSet(b32 flags, b32 queryFlags) { return ((flags & queryFlags) == queryFlags); }
void setFlags(b32* outFlags, b32 newFlags) { *outFlags |= newFlags; }
void clearFlags(b32* outFlags, b32 removeFlags) { *outFlags &= ~removeFlags; }

void reset(Stopwatch* stopwatch) {
  stopwatch->secondsPerPerfCounter = 1.0 / getPerformanceCounterFrequencyPerSecond();
  stopwatch->totalElapsedSeconds = 0.0f;
  stopwatch->lastPerfCounter = getPerformanceCounter();
  stopwatch->deltaSeconds = 0.0f;
}

void lap(Stopwatch* stopwatch) {
  u64 currPerfCounter = getPerformanceCounter();
  stopwatch->deltaSeconds = (currPerfCounter - stopwatch->lastPerfCounter) * stopwatch->secondsPerPerfCounter;
  stopwatch->lastPerfCounter = currPerfCounter;
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

template <typename T, u32 maxCount>
struct Stack {
  T slots[maxCount];
  u32 top;

  bool empty() { return top > 0; }
  bool full() { return top == maxCount; }
  u32 count() { return top; }
  T pop() {
    assert(!empty());
    return slots[top--];
  }
  void push(T item) {
    assert(!full());
    slots[top++] = item;
  }
  void pushBatch(std::function<T(u64)>& itemPerIndex, u32 count) {
    u64 newTop = top + count;
    assert(newTop <= maxCount && "ERROR: Filling Stack with too many elements.");
    for(u64 i = 0; i < count; i++) {
      slots[top + i] = itemPerIndex(i);
    }
    top = newTop;
  }
};

namespace Generation {
  template <typename T>
  struct Item {
    T value;
    u64 generation;
  };
  typedef Item<u64> Index;

  template <typename T, u64 maxCount>
  struct Map {
  public:
    Item<T> items[maxCount];
    Stack<u64, maxCount> unusedSlots;

    Map() {
      // Fill stack bottom up so pops go from 0 to maxCount - 1
      u64 lastElement = maxCount - 1;
      unusedSlots.pushBatch([lastElement](u64 index){ return lastElement - index; }, maxCount);
    }

    Index put(T value) {
      assert(!unusedSlots.empty() && "ERROR: Generation::Map is full!");
      u64 slot = unusedSlots.pop();
      items[slot].value = value;
      Index index;
      index.value = slot;
      index.generation = items[slot].generation;
      return index;
    }

    void remove(Index index) {
      assert(index.value < maxCount);
      assert(index.generation == items[index.value].generation);
      items[index.value].generation++;
      unusedSlots.push(index.value);
    }

    T at(Index index) {
      assert(index.value < maxCount);
      Item item = items[index.value];
      assert(item.generation == index.generation && "ERROR: Attempting to access GenerationMap item with expired generation!");
      return item.value;
    }

    u64 count() { return maxCount - unusedSlots.count(); }
  };
}