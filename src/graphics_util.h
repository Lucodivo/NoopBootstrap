#pragma once

// real-time rendering 4.7.2
// ex: screenWidth = 20.0f, screenDist = 30.0f will provide the horizontal field of view
// for a person sitting 30 inches away from a 20 inch screen, assuming the screen is
// perpendicular to the line of sight.
// NOTE: Any units work as long as they are the same. Works for vertical and horizontal.
f32 fieldOfView(f32 screenWidth, f32 screenDist) {
  f32 phi = 2.0f * atanf(screenWidth/(2.0f * screenDist));
  return phi;
}