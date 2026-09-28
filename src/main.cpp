#include "common.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

int main() {
    hittable_list world;

    world.add(std::make_shared<sphere>(point3(0.0f, 0.0f, -1.0f), 0.5f));
    world.add(std::make_shared<sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f));

    camera cam;

    cam.aspect_ratio = 16.0f / 9.0f;
    cam.image_width  = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world);
}
