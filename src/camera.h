#pragma once

#include "common.h"
#include "color.h"
#include "hittable.h"
#include "ray.h"
#include <fstream>

class camera {
  public:
    float aspect_ratio = 1.0f;
    int image_width  = 100;  // Rendered image width in pixel count
    int samples_per_pixel = 10;
    int max_depth = 10; // Max number of ray bounces in scene -> make it lower for games!

    void render(const hittable& world);

  private:
    int image_height;
    float pixel_samples_scale; // Color scale factor for a sum of pixel samples
    point3 center; // Camera center
    point3 pixel00_loc; // Location of pixel 0, 0
    vec3 pixel_delta_u; // Offset/distance to pixel to the right
    vec3 pixel_delta_v; // Offset/distance to pixel below

    void initialize();

    /**
     * Construct a camera ray originating from the origin and directed at randomly sampled
     * point around the pixel location i, j
     * 
     * @param i The horizontal pixel index
     * @param j The vertical pixel index
     * @return the constructed ray
     */
    ray get_ray(int i, int j) const;

    /**
     * @return a random position within a 1x1 square
     */
    vec3 sample_square() const;

    /**
     * Computes the color for a given ray depending on what/if it intersects
     * 
     * @param r The ray cast from the camera through a specific pixel.
     * @param world The collection of all hittable objects in the scene
     * @return The computed RGB color (with components from 0.0f to 1.0f).
     */
    color ray_color(const ray& r, int depth, const hittable& world) const;
};
