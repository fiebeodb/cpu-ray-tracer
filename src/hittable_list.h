#pragma once

#include "hittable.h"
#include <memory>
#include <vector>
#include <utility>
#include "interval.h"


class hittable_list : public hittable {
  public:
    std::vector<std::shared_ptr<hittable>> objects;

    hittable_list() = default;
    explicit hittable_list(std::shared_ptr<hittable> object) { add(std::move(object)); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<hittable> object) {
        objects.push_back(std::move(object));
    }

    /**
     * Goes through all objects in the list to find which one the ray hits first.
     * If a hit, it stores the info about the closest hit in rec.
     * 
     * @param r The casted ray
     * @param ray_t the interval in which ray hitpoints are considered
     * @param rec Where the hit (closest) information is stored
     * @return true if the ray hits any object in the list, else false
     */
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object : objects) {
            // call the object's hit function (will put hitpoint info in temp_rec)
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};
