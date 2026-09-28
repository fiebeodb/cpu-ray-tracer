#pragma once

#include <array>
#include <cmath>
#include <iostream>

class vec3 {
  public:
    std::array<float, 3> e{0.0f, 0.0f, 0.0f};

    constexpr vec3() = default;
    constexpr vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}

    constexpr float x() const { return e[0]; }
    constexpr float y() const { return e[1]; }
    constexpr float z() const { return e[2]; }

    constexpr vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    constexpr float operator[](int i) const { return e[i]; }
    constexpr float& operator[](int i) { return e[i]; }

    constexpr vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    constexpr vec3& operator*=(float t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    constexpr vec3& operator/=(float t) {
        return *this *= 1/t;
    }

    float length() const {
        return std::sqrt(length_squared());
    }

    constexpr float length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    static vec3 random() {
        return vec3(random_float(), random_float(), random_float());
    }

    static vec3 random(float min, float max) {
        return vec3(random_float(min,max), random_float(min,max), random_float(min,max));
    }
};

using point3 = vec3;

// Overload these operators outside the class so the operators can be used
// with both objects being equal/explicit

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

constexpr inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

constexpr inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

constexpr inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

constexpr inline vec3 operator*(float t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

constexpr inline vec3 operator*(const vec3& v, float t) {
    return t * v;
}

constexpr inline vec3 operator/(const vec3& v, float t) {
    return (1/t) * v;
}

constexpr inline float dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

constexpr inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

constexpr inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

/**
 * Generates random vectors within a 2x2 unit square, until we find one that
 * falls within the radius 1 circle in this square, then return it normalized.
 * 
 * @return the random normalized vector
 */
inline vec3 random_unit_vector() {
    while (true) {
        auto p = vec3::random(-1,1);
        auto lensq = p.length_squared();
        // norm will be 0 if all 3 coords small enough: reject
        if (1e-160 < lensq && lensq <= 1)
            return p / sqrt(lensq);
    }
}

/**
 * Generates random vector in the correct hemisphere (= same as the normal).
 * If the randomly generated vector isn't, it is inverted so it now is.
 * 
 * @param normal The normal vector for a (hit)point on the object's surface
 * @return The randomly generated vector
 */
inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}
