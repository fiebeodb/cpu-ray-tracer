#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <numbers>

// Constants
constexpr float infinity = std::numeric_limits<float>::infinity();
constexpr float pi = std::numbers::pi_v<float>;

// Utility Functions
inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0f;
}

// Common Headers
#include "color.h"
#include "ray.h"
#include "vec3.h"
