#pragma once

#include "common.h"

class interval {
  public:
    float min;
    float max;

    constexpr interval() : min {+infinity}, max{-infinity} {} // Default interval is empty

    interval(float min, float max) : min{min}, max{max} {}

    constexpr float size() const {
        return max - min;
    }

    constexpr bool contains(float x) const {
        return min <= x && x <= max;
    }

    constexpr bool surrounds(float x) const {
        return min < x && x < max;
    }

    float clamp(float x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const interval empty;
    static const interval universe;
};

inline const interval interval::empty = interval(+infinity, -infinity);
inline const interval interval::universe = interval(-infinity, +infinity);
