#pragma once

#include "math.hpp"

namespace idrs
{
    class View3D
    {
        struct ProjectionMatrixState
        {
            f32 fov;
            f32 aspect;
            f32 zNear;
            f32 zFar;
        };

    public:
        View3D() = default;
        View3D(const f32 fov, const f32 aspect, const f32 zNear, const f32 zFar);

        void move(const Vec3f &offset);
        void rotate(const Vec3f &offset);
        void setPosition(const Vec3f &pos);
        void setRotation(const Vec3f &angle);
        void setViewport(const u32 x, const u32 y, const u32 w, const u32 h);
        void setViewport(const FRect &rect);
        void setFov(const f32 fov);

        const Mat4 &projection() const;
        const Mat4 &view() const;

    private:
        Mat4 m_projection;
        Mat4 m_view;
        ProjectionMatrixState m_state;
        Vec3f m_position;
        Vec3f m_forward;
        Vec3f m_angle;

    private:
        void update();
        void applyRotation();
    };
}