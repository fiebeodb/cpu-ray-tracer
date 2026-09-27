#pragma once

#include "hittable.h"
#include <memory>
#include <vector>
#include <utility>


class hittable_list : public hittable {
  public:
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list() = default;
    hittable_list(std::shared_ptr<hittable> object) { add(std::move(object)); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<hittable> object) {
        objects.push_back(object);
    }
    /**
     * Goes through all objects in the list to find which one the ray hits first.
     * If a hit, it stores the info about the closest hit in rec.
     * 
     * @param r The casted ray
     * @param ray_tmin The closest distance allowed for a hit
     * @param ray_tmax The furthest distance allowed for a hit
     * @param rec Where the hit (closest) information is stored
     * 
     * @return true if the ray hits any object in the list, else false
     */
    bool hit(const ray& r, float ray_tmin, float ray_tmax, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_tmax;

        for (const auto& object : objects) {
            // call the object's hit function (will put hitpoint info in temp_rec)
            if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};
