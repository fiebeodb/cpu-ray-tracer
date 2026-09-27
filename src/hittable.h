#pragma once

#include "ray.h"

class hit_record {
  public:
    point3 p; // the ray-hittable hitpoint
    vec3 normal;
    float t;
};

class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, float ray_tmin, float ray_tmax, hit_record& rec) const = 0;
};
