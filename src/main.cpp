#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

// a point P is on sphere (center C, radius r) surface if:  (P-C)^2 = r^2
// fill in ray equation P(t) = Q + td in the upper equation to get an equation
// of the form  at^2 + bt + c = r^2  determining if the ray hits or not:
float hit_sphere(const point3& center, float radius, const ray& r) {
    vec3 oc = center - r.origin();
    auto a = dot(r.direction(), r.direction());
    auto b = -2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - std::sqrt(discriminant) ) / (2.0*a);
    }
}

color ray_color(const ray& r) {
    constexpr auto center = point3(0,0,-1);
    auto t = hit_sphere(center, 0.5, r);
        if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - center); // the normal: center -> hitpoint vector
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1); // 
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5f * (unit_direction.y() + 1.0f);
    return (1.0f - a) * color(1.0f, 1.0f, 1.0f) + a * color(0.5f, 0.7f, 1.0f);
}

int main() {

    constexpr auto aspect_ratio = 16.0f / 9.0f;
    constexpr int image_width = 400;
    constexpr int image_height = std::max(1, static_cast<int>(image_width / aspect_ratio));

    constexpr auto focal_length = 1.0f; // distance between camera and viewport center
    constexpr auto viewport_height = 2.0f;
    constexpr auto viewport_width = viewport_height * (static_cast<float>(image_width)/image_height);
    constexpr auto camera_center = point3(0, 0, 0);

    // the vectors across the horizontal and down the vertical viewport edge
    constexpr auto viewport_u = vec3(viewport_width, 0, 0);
    constexpr auto viewport_v = vec3(0, -viewport_height, 0);

    // calculate pixel to pixel length (pixel_length = total_length / #pixels)
    constexpr auto pixel_delta_u = viewport_u / image_width;
    constexpr auto pixel_delta_v = viewport_v / image_height;

    constexpr auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    constexpr auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (static_cast<float>(i) * pixel_delta_u) + (static_cast<float>(j) * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
}
