#include "common.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {
    hittable_list world;

    auto material_ground = std::make_shared<lambertian>(color(0.8f, 0.8f, 0.0f));
    auto material_center = std::make_shared<lambertian>(color(0.1f, 0.2f, 0.5f));
    auto material_left   = std::make_shared<dielectric>(1.50f);
    auto material_bubble = std::make_shared<dielectric>(1.00f / 1.50f);
    auto material_right  = std::make_shared<metal>(color(0.8f, 0.6f, 0.2f), 1.0f);

    world.add(std::make_shared<sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f, material_ground));
    world.add(std::make_shared<sphere>(point3(0.0f,    0.0f, -1.2f),   0.5f, material_center));
    world.add(std::make_shared<sphere>(point3(-1.0f,   0.0f, -1.0f),   0.5f, material_left));
    world.add(std::make_shared<sphere>(point3(-1.0f,   0.0f, -1.0f),   0.4f, material_bubble));
    world.add(std::make_shared<sphere>(point3(1.0f,    0.0f, -1.0f),   0.5f, material_right));

    camera cam;
    cam.aspect_ratio      = 16.0f / 9.0f;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20.0f;
    cam.lookfrom = point3(-2.0f, 2.0f, 1.0f);
    cam.lookat   = point3(0.0f, 0.0f, -1.0f);
    cam.vup      = vec3(0.0f, 1.0f, 0.0f);

    cam.render(world);
}
