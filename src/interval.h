#pragma once

#include "common.h"

class interval {
  public:
    float min;
    float max;

    constexpr interval() : min {+infinity}, max{-infinity} {} // Default interval is empty

    interval(double min, double max) : min(min), max(max) {}

    constexpr double size() const {
        return max - min;
    }

    constexpr bool contains(double x) const {
        return min <= x && x <= max;
    }

    constexpr bool surrounds(double x) const {
        return min < x && x < max;
    }

    static const interval empty;
    static const interval universe;
};

inline const interval interval::empty = interval(+infinity, -infinity);
inline const interval interval::universe = interval(-infinity, +infinity);
