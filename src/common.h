#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <numbers>
#include <random>

// Constants
constexpr float infinity = std::numeric_limits<float>::infinity();
constexpr float pi = std::numbers::pi_v<float>;

// Utility Functions
inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0f;
}

inline float random_float() {
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float random_float(float min, float max) {
    return min + (max-min)*random_float();
}
