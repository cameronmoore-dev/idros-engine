#pragma once

#include <array>

#include "core/typedefs.h"

namespace idrs
{
    using Mat4 = std::array<std::array<f32, 4>, 4>;
    using Mat3 = std::array<std::array<f32, 3>, 3>;
    using Mat2 = std::array<std::array<f32, 2>, 2>;

    inline const f32 pi = 3.14159265359f;
    inline const f32 e = 2.71828f;

    struct Vec2u
    {
        u32 x;
        u32 y;
    };

    struct Vec2i
    {
        s32 x;
        s32 y;

        const Vec2i operator+(const Vec2i &rhs) const { return { x + rhs.x, y + rhs.y }; }
        const Vec2i operator-(const Vec2i &rhs) const { return { x - rhs.x, y - rhs.y }; }
    };

    struct Vec2f
    {
        f32 x;
        f32 y;

        const Vec2f operator+(const Vec2f &rhs) const { return { x + rhs.x, y + rhs.y }; }
        const Vec2f operator-(const Vec2f &rhs) const { return { x - rhs.x, y - rhs.y }; }
        const Vec2f operator/(const Vec2f &rhs) const { return { x / rhs.x, y / rhs.y }; }

        const Vec2f operator*(const f32 &rhs) const { return { x * rhs, y * rhs }; }
        const Vec2f operator/(const f32 &rhs) const { return { x / rhs, y / rhs }; }

        const Vec2f operator+=(const Vec2f &rhs) { return { x += rhs.x, y += rhs.y }; }
        const Vec2f operator-=(const Vec2f &rhs) { return { x -= rhs.x, y -= rhs.y }; }
        const Vec2f operator*=(const Vec2f &rhs) { return { x *= rhs.x, y *= rhs.y }; }
        const Vec2f operator/=(const Vec2f &rhs) { return { x /= rhs.x, y /= rhs.y }; }
    };
    
    struct Vec3f
    {
        f32 x;
        f32 y;
        f32 z;

        const Vec3f operator+=(const Vec3f &rhs) { return { x += rhs.x, y += rhs.y, z += rhs.z }; }
        const Vec3f operator-=(const Vec3f &rhs) { return { x -= rhs.x, y -= rhs.y, z -= rhs.z }; }

        const Vec3f operator+(const Vec3f &rhs) const { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
        const Vec3f operator-(const Vec3f &rhs) const { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
        const Vec3f operator*(const Vec3f &rhs) const { return { x * rhs.x, y * rhs.y, z * rhs.z }; }

        const Vec3f operator*(const f32 &rhs) const { return { x * rhs, y * rhs, z * rhs }; }
    };
    
    struct Vec4f
    {
        f32 x;
        f32 y;
        f32 z;
        f32 w;

        const Vec4f operator/=(const f32 &rhs) { return { x /= rhs, y /= rhs, z /= rhs, w /= rhs }; }
    };

    struct FRect
    {
        f32 x;
        f32 y;
        f32 w;
        f32 h;
    };

    struct URect
    {
        u32 x;
        u32 y;
        u32 w;
        u32 h;
    };

    struct IRect
    {
        s32 x;
        s32 y;
        s32 w;
        s32 h;
    };

    namespace matrix
    {
        const Mat4 perspective(const f32 fov, const f32 aspect, const f32 zNear, const f32 zFar);
        const Mat4 orthographic(const s32 left, const s32 right, const s32 bottom, const s32 top, const f32 zNear, const f32 zFar);
        const Mat4 lookAt(const Vec3f& pos, const Vec3f& forward, const Vec3f& up);
        const Mat4 translate(const Vec3f &vec);
        const Mat4 rotate(const f32 angle, const Vec3f &axis);
        const Mat4 scale(const Vec3f &vec);
        const Mat4 identity();

        void inverse4(Mat4 &out, const Mat4 &matrix);
        const Mat4 transpose(const Mat4 &matrix);

        const Mat4 multiply(const Mat4 &matA, const Mat4 &matB);
        const Vec4f multiply(const Mat4 &mat, const Vec4f &vec);
        const Vec3f multiply(const Mat4 &mat, const Vec3f &vec);
    } // namespace matrix

    namespace math
    {
        const f32 degToRad(const f32 deg);
        const f32 dot(const Vec3f &a, const Vec3f &b);
        const f32 dot(const Vec2f &a, const Vec2f &b);
        const Vec3f normalize(const Vec3f &vec);
        const Vec2f normalize(const Vec2f &vec);
        const Vec3f cross(const Vec3f &a, const Vec3f &b);
        const f32 cross(const Vec2f &a, const Vec2f &b);
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
