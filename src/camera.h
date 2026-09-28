#include "common.h"
#include "color.h"
#include "hittable.h"
#include "ray.h"
#include <fstream>

class camera {
  public:
    float aspect_ratio = 1.0f;
    int image_width  = 100;  // Rendered image width in pixel count
    int samples_per_pixel = 10;
    int max_depth = 10; // Max number of ray bounces into scene

    void render(const hittable& world) {
        initialize();

        std::ofstream out("image.ppm", std::ios::out | std::ios::binary);
        out << "P3\n" << image_width << " " << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0.0f,0.0f,0.0f);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(out, pixel_samples_scale * pixel_color);
            }
        }
    }

  private:
    int image_height;
    float pixel_samples_scale; // Color scale factor for a sum of pixel samples
    point3 center; // Camera center
    point3 pixel00_loc; // Location of pixel 0, 0
    vec3 pixel_delta_u; // Offset/distance to pixel to the right
    vec3 pixel_delta_v; // Offset/distance to pixel below

    void initialize() {
        image_height = std::max(1, static_cast<int>(image_width / aspect_ratio));

        pixel_samples_scale = 1.0f / samples_per_pixel;

        center = point3(0.0f, 0.0f, 0.0f);

        // Determine viewport dimensions
        float focal_length = 1.0f;
        float viewport_height = 2.0f;
        float viewport_width = viewport_height * (static_cast<float>(image_width) / static_cast<float>(image_height));

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        auto viewport_u = vec3(viewport_width, 0.0f, 0.0f);
        auto viewport_v = vec3(0.0f, -viewport_height, 0.0f);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        pixel_delta_u = viewport_u / static_cast<float>(image_width);
        pixel_delta_v = viewport_v / static_cast<float>(image_height);

        // Calculate the location of the upper left pixel
        auto viewport_upper_left = center - vec3(0.0f, 0.0f, focal_length) - (viewport_u / 2.0f) - (viewport_v / 2.0f);
        pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
    }

    /**
     * Construct a camera ray originating from the origin and directed at randomly sampled
     * point around the pixel location i, j
     * 
     * @param i The horizontal pixel index
     * @param j The vertical pixel index
     * @return the constructed ray
     */
    ray get_ray(int i, int j) const {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                          + ((i + offset.x()) * pixel_delta_u)
                          + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    /**
     * @return a random position within a 1x1 square
     */
    vec3 sample_square() const {
        return vec3(random_float() - 0.5f, random_float() - 0.5f, 0.0f);
    }

    /**
     * Computes the color for a given ray depending on what/if it intersects
     * 
     * @param r The ray cast from the camera through a specific pixel.
     * @param world The collection of all hittable objects in the scene
     * @return The computed RGB color (with components from 0.0f to 1.0f).
     */
    color ray_color(const ray& r, int depth, const hittable& world) const {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0.0f,0.0f,0.0f);
        
        hit_record rec;

        // 0.001 instead of 0 to get rid of shadow acne (due to floating point errors)
        if (world.hit(r, interval(0.001f, infinity), rec)) {
            vec3 direction = rec.normal + random_unit_vector();
            return 0.6f * ray_color(ray(rec.p, direction), depth - 1, world);
        }

        // background
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5f*(unit_direction.y() + 1.0f); // from [-1,1] to [0,1]
        return (1.0f-a)*color(1.0f, 1.0f, 1.0f) + a*color(0.4f, 0.6f, 1.0f);
    }
};
