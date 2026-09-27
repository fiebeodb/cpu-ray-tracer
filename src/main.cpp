#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include "common.h"
#include <fstream>

// a point P is on sphere (center C, radius r) surface if:  (P-C)^2 = r^2
// fill in ray equation P(t) = Q + td in the upper equation to get an equation
// of the form  at^2 + bt + c = r^2  determining if the ray hits or not:
float hit_sphere(const point3& center, float radius, const ray& r) {
    vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = h*h - a*c;
    if (discriminant < 0.0f) {
        return -1.0f;
    } else {
        return (h - std::sqrt(discriminant)) / a;
    }
}

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0.0f, infinity, rec)) {
        return 0.5f * (rec.normal + color(1.0f, 1.0f, 1.0f));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - a) * color(1.0f, 1.0f, 1.0f) + a * color(0.5f, 0.7f, 1.0f);
}

int main() {

    // Image
    constexpr auto aspect_ratio = 16.0f / 9.0f;
    constexpr int image_width = 400;
    constexpr int image_height = std::max(1, static_cast<int>(image_width / aspect_ratio));

    // World
    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0.0f, 0.0f, -1.0f), 0.5f));
    world.add(std::make_shared<sphere>(point3(0.0f, -100.5f, -1.0f), 100.0f));

    // Camera
    constexpr auto focal_length = 1.0f; // distance between camera and viewport center
    constexpr auto viewport_height = 2.0f;
    constexpr auto viewport_width = viewport_height * (static_cast<float>(image_width)/image_height);
    constexpr auto camera_center = point3(0, 0, 0);

    // The vectors across the horizontal and down the vertical viewport edge
    constexpr auto viewport_u = vec3(viewport_width, 0, 0);
    constexpr auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate pixel to pixel length (pixel_length = total_length / #pixels)
    constexpr auto pixel_delta_u = viewport_u / image_width;
    constexpr auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel
    constexpr auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    constexpr auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render
    std::ofstream out("image.ppm", std::ios::out | std::ios::binary);
    out << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (static_cast<float>(i) * pixel_delta_u) + (static_cast<float>(j) * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);
            write_color(out, pixel_color);
        }
    }
}
