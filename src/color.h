#pragma once

#include "vec3.h"
#include "interval.h"
#include <iostream>

using color = vec3;

inline float linear_to_gamma(float linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

/**
* Converts a float color (0.0-1.0) into (gamma corrected) RGB values (0 to 255)
* and writes it to the given output stream.
* 
* @param out The stream  to write the color data to
* @param pixel_color The color of the pixel
*/
inline void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply a linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const interval intensity(0.000f, 0.999f);
    // use bytes for less memory & store the bytes in a char
    char rbyte = static_cast<char>(255.999f * intensity.clamp(r));
    char gbyte = static_cast<char>(255.999f * intensity.clamp(g));
    char bbyte = static_cast<char>(255.999f * intensity.clamp(b));

    std::array<char, 3> pixel_bytes = {rbyte, gbyte, bbyte};
    out.write(pixel_bytes.data(), pixel_bytes.size());
}


