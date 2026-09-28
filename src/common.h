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
    thread_local static uint32_t state = 123456789 ^ std::random_device{}();
    
    // fast Xorshift32 algorithm
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    
    // Multiply by (1.0 / 2^32) to map the random uint32 to a float between 0.0 and 1.0
    return state * 2.3283064365386963e-10f; 
}

inline float random_float(float min, float max) {
    return min + (max-min)*random_float();
}
