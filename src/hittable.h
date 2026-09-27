#pragma once

#include "ray.h"
#include "interval.h"

class hit_record {
  public:
    point3 p; // the ray-hittable hitpoint
    vec3 normal;
    float t;
    bool front_face; // true-false if ray comes from outside-inside the object

    /**
     * Determines if the ray comes from the outside or inside of the object, 
     * and makes sure the surface normal always points against the incoming ray.
     * 
     * @param r The ray that hit the object
     * @param outward_normal The normal vector pointing outwards from the object's center
     */
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        // make normal always face opposite direction of the ray
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
  public:
    virtual ~hittable() = default;

    /**
     * Checks if the given ray intersects with this object (within a specific t range)
     * 
     * @param r the ray
     * @param ray_t the interval in which ray hitpoints are considered
     * @param rec Where the hit information is stored (if hit)
     * 
     * @return true if the ray hits the object, else false
     */
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};
