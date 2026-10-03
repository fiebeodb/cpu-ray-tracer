#pragma once

#include "hittable.h"
#include "color.h"

class material {
  public:
    virtual ~material() = default;

    /**
   * Determines how an incoming ray interacts with the material surface, 
   * calculating the ray's new path and color intensity upon intersection.
   * 
   * @param r_in The incoming ray hitting the object.
   * @param rec The hit_record containing info about the intersection/hitpoint.
   * @param attenuation Stores the albedo (color modifier) applied to the ray, determining light absorbed vs. reflected.
   * @param scattered Stores the new outgoing ray after the bounce.
   * @return true if the ray scatters, false if the material completely absorbs it
   */
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const = 0;
};

class lambertian : public material {
  public:
    lambertian(const color& albedo) : albedo {albedo} {}

    bool scatter(const ray& /*r_in*/, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        auto scatter_direction = rec.normal + random_unit_vector();
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

  private:
    color albedo;
};

class metal : public material {
  public:
    metal(const color& albedo, float fuzz) : albedo {albedo}, fuzz{fuzz < 1 ? fuzz : 1} {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        // if fuzz makes the direction go inside object when normal points outisde,
        // (or outside when normal points inside), just let it absorb it all
        return (dot(scattered.direction(), rec.normal) > 0);
    }

  private:
    color albedo;
    float fuzz;
};
