#pragma once

#include "vec3.h"
#include <iostream>

using color = vec3;

/**
* Converts a float color (0.0-1.0) into RGB values (0 to 255)
* and writes it to the given output stream.
* 
* @param out The stream  to write the color data to
* @param pixel_color The color of the pixel
*/
void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    int rbyte = static_cast<int>(255.999 * r);
    int gbyte = static_cast<int>(255.999 * g);
    int bbyte = static_cast<int>(255.999 * b);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}


