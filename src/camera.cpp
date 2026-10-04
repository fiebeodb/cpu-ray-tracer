#include "camera.h"
#include "material.h"
#include <vector>
#include <thread>
#include <atomic>

void camera::render(const hittable& world) {
    initialize();

    std::vector<color> image_buffer(image_width * image_height); // all in RAM
    int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 4; // basically all modern pc's have >4 cores
    std::vector<std::jthread> threads;

    // the the rendering in a lambda function to give it to each thread
    // so multiple rows of pixels can be rendered at the same time:
    std::atomic<int> next_row{0};
    auto render_rows = [&]() {
        int j;
        // a thread asks for a (next) row to renders, does so, then asks for next,... 
        while ((j = next_row.fetch_add(1)) < image_height) {
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0.0f,0.0f,0.0f);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                // the index of the pixel in the 1D vector that represents the image
                int pixel_index = j * image_width + i;
                // no mutex needed: each thread only modifies it's 'own index' in the array
                image_buffer[pixel_index] = pixel_samples_scale * pixel_color;
            }
        }
    };

    for (int t = 0; t < num_threads; t++) {
        threads.emplace_back(render_rows);
    }

    // Clearing the vector will call .join() on each of the threads: wait until all done
    threads.clear(); 

    std::ofstream out("image.ppm", std::ios::out | std::ios::binary);
    out << "P6\n" << image_width << " " << image_height << "\n255\n"; // P6: raw bytes

    for (const auto& pixel : image_buffer) {
        write_color(out, pixel);
    }
}

void camera::initialize() {
    image_height = std::max(1, static_cast<int>(image_width / aspect_ratio));

    pixel_samples_scale = 1.0f / samples_per_pixel;

    center = lookfrom;

    // Determine viewport dimensions
    auto focal_length = (lookfrom - lookat).length();
    auto theta = degrees_to_radians(vfov);
    auto h = std::tan(theta/2.0f);
    auto viewport_height = 2.0f * h * focal_length;
    float viewport_width = viewport_height * (static_cast<float>(image_width) / static_cast<float>(image_height));

    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    vec3 viewport_u = viewport_width * u;
    vec3 viewport_v = viewport_height * -v;

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    pixel_delta_u = viewport_u / static_cast<float>(image_width);
    pixel_delta_v = viewport_v / static_cast<float>(image_height);

    // Calculate the location of the upper left pixel
    auto viewport_upper_left = center - (focal_length * w) - viewport_u/2 - viewport_v/2;
    pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
}

ray camera::get_ray(int i, int j) const {
    auto offset = sample_square();
    auto pixel_sample = pixel00_loc
                      + ((i + offset.x()) * pixel_delta_u)
                      + ((j + offset.y()) * pixel_delta_v);

    auto ray_origin = center;
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
}

vec3 camera::sample_square() const {
    return vec3(random_float() - 0.5f, random_float() - 0.5f, 0.0f);
}

color camera::ray_color(const ray& r, int depth, const hittable& world) const {
    color through(1.0f, 1.0f, 1.0f);
    color accumulated(0.0f, 0.0f, 0.0f);
    ray current_ray = r;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    for (int bounce = 0; bounce < depth; ++bounce) {
        hit_record rec;
        // 0.004 instead of 0 to get rid of shadow acne (due to floating point errors)
        if (world.hit(current_ray, interval(0.004f, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(current_ray, rec, attenuation, scattered)) {
                current_ray = scattered;
                through *= attenuation;
            } else {
                // Ray was completely absorbed by the material
                break; 
            }
        } else { // background
            vec3 unit_direction = unit_vector(current_ray.direction());
            auto a = 0.5f * (unit_direction.y() + 1.0f);
            color sky = (1.0f - a) * color(1.0f, 1.0f, 1.0f) + a * color(0.4f, 0.6f, 1.0f);
            accumulated += through * sky;
            break;
        }
    }
    return accumulated;
}
