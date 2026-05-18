#pragma once

#include <array>
#include <cmath>
#include <algorithm>

#include "typedefs.h"

namespace idrs
{
    using Mat4 = std::array<std::array<f32, 4>, 4>;
    using Mat3 = std::array<std::array<f32, 3>, 3>;
    using Mat2 = std::array<std::array<f32, 2>, 2>;

    inline const f32 pi = 3.14159265359f;
    inline const f32 e = 2.71828f;

    struct Vec2i
    {
        u32 x = 0;
        u32 y = 0;
    };

    struct Vec2f
    {
        f32 x = 0.f;
        f32 y = 0.f;
    };
    
    struct Vec3f
    {
        f32 x = 0.f;
        f32 y = 0.f;
        f32 z = 0.f;

        const Vec3f operator+=(const Vec3f &rhs) { return { x += rhs.x, y += rhs.y, z += rhs.z }; }
        const Vec3f operator-=(const Vec3f &rhs) { return { x -= rhs.x, y -= rhs.y, z -= rhs.z }; }

        const Vec3f operator+(const Vec3f &rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
        const Vec3f operator-(const Vec3f &rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
        const Vec3f operator*(const Vec3f &rhs) const { return { x * rhs.x, y * rhs.y, z * rhs.z }; }

        const Vec3f operator*(const f32 &rhs) const { return { x * rhs, y * rhs, z * rhs }; }
    };
    
    struct Vec4f
    {
        f32 x = 0.f;
        f32 y = 0.f;
        f32 z = 0.f;
        f32 w = 0.f;
    };

    struct FRect
    {
        f32 x = 0.f;
        f32 y = 0.f;
        f32 w = 0.f;
        f32 h = 0.f;
    };

    namespace matrix
    {
        const Mat4 perspective(const f32 fov, const f32 aspect, const f32 zNear, const f32 zFar);
        const Mat4 orthographic(const f32 left, const f32 right, const f32 bottom, const f32 top, const f32 zNear, const f32 zFar);
        const Mat4 lookAt(const Vec3f& pos, const Vec3f& forward, const Vec3f& up);
        const Mat4 translate(const Vec3f &vec);
        const Mat4 rotate(const f32 angle, const Vec3f &axis);
        const Mat4 scale(const Vec3f &vec);
        const Mat4 identity();

        const Mat4 multiply(const Mat4 &matA, const Mat4 &matB);
        const Vec4f multiply(const Mat4 &mat, const Vec4f &vec);
        const Vec3f multiply(const Mat4 &mat, const Vec3f &vec);
    } // namespace matrix

    namespace math
    {
        const f32 degToRad(const f32 deg);
        const f32 dot(const Vec3f &a, const Vec3f &b);
        const Vec3f normalize(const Vec3f &vec);
        const Vec3f cross(const Vec3f &a, const Vec3f &b);
    } // namespace math
} // namespace idrs

inline const idrs::Mat4 operator*(const idrs::Mat4 &a, const idrs::Mat4 &b)
{
    return idrs::matrix::multiply(a, b);
}

inline const idrs::Vec4f operator*(const idrs::Mat4 &a, const idrs::Vec4f &b)
{
    return idrs::matrix::multiply(a, b);
}

inline const idrs::Vec3f operator*(const idrs::Mat4 &a, const idrs::Vec3f &b)
{
    return idrs::matrix::multiply(a, b);
}