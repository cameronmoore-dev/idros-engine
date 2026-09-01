#include "../view3d.h"

#include <cmath>
#include <algorithm>

#include <glad/gl.h>

namespace idrs
{
    View3D::View3D(const f32 fov, const f32 aspect, const f32 zNear, const f32 zFar) : 
        m_projection(matrix::perspective(fov, aspect, zNear, zFar)),
        m_view(matrix::identity()),
        m_state({ fov, aspect, zNear, zFar }),
        m_position({ 0, 0, 0 }),
        m_angle({ 0, -90, 0 }),
        m_forward({ 0, 0, -1 })
    {
    }

    void View3D::move(const Vec3f &offset)
    {
        Vec3f up = { 0, 1, 0 };
        Vec3f right = idrs::math::normalize(idrs::math::cross(m_forward, up));

        m_position -= right * offset.x;
        m_position -= up * offset.y;
        m_position -= m_forward * offset.z;
        
        update();
    }

    void View3D::rotate(const Vec3f &offset)
    {
        m_angle.y += offset.x;
        m_angle.x += offset.y;
        m_angle.x = std::clamp(m_angle.x, -89.f, 89.f);

        applyRotation();
        update();
    }

    void View3D::setPosition(const Vec3f &pos)
    {
        m_position = pos;
        update();
    }

    void View3D::setRotation(const Vec3f &angle)
    {
        m_angle = angle;
        m_angle.x = std::clamp(m_angle.x, -89.f, 89.f);

        applyRotation();
        update();
    }

    void View3D::setViewport(const u32 x, const u32 y, const u32 w, const u32 h)
    {
        glViewport(x, y, w, h);
    }

    void View3D::setViewport(const FRect &rect)
    {
        glViewport(rect.x, rect.y, rect.w, rect.h);
    }

    void View3D::setFov(const f32 fov)
    {
        m_state.fov = fov;
        m_projection = matrix::perspective(m_state.fov, m_state.aspect, m_state.zNear, m_state.zFar);
    }

    void View3D::applyRotation()
    {
        f32 radYaw = math::degToRad(m_angle.y);
        f32 radPitch = math::degToRad(m_angle.x);

        Vec3f dir;
        dir.x = std::cos(radYaw) * std::cos(radPitch);
        dir.y = std::sin(radPitch);
        dir.z = std::sin(radYaw) * std::cos(radPitch);
        m_forward = math::normalize(dir);
    }

    void View3D::update()
    {
        m_view = matrix::lookAt(m_position, m_position + m_forward, { 0, 1, 0 });
    }

    const Mat4 &View3D::projection() const
    {
        return m_projection;
    }

    const Mat4 &View3D::view() const
    {
        return m_view;
    }
}