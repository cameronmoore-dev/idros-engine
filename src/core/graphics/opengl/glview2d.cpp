#include "../view2d.h"

#include <cmath>

#include <glad/gl.h>

namespace idrs
{
    View2D::View2D(const IRect &rect, const f32 zNear, const f32 zFar) : 
        m_projection(matrix::orthographic(rect.x, rect.w, rect.y, rect.h, zNear, zFar)),
        m_state({ rect.x, rect.w, rect.y, rect.h, zNear, zFar }),
        m_view(matrix::identity()),
        m_roll(0.f)
    {
        update();
    }

    void View2D::update()
    {
        Vec3f localUp = { std::sin(math::degToRad(m_roll)), std::cos(math::degToRad(m_roll)), 0.f };
        Vec3f pos = { (f32)m_state.x, (f32)m_state.y, 0.f };
        Vec3f fwd = { 0.f, 0.f, 1.f };
        
        m_view = matrix::lookAt(pos, pos + fwd, localUp);
    }

    void View2D::move(const Vec2f &offset)
    {
        m_state.x += offset.x;
        m_state.y += offset.y;

        update();
    }

    void View2D::rotate(const f32 zOffset)
    {
        m_roll += zOffset;

        update();
    }

    void View2D::setPosition(const Vec2i &pos)
    {
        m_state.x = pos.x;
        m_state.y = pos.y;

        update();
    }

    void View2D::setRotation(const f32 zAngle)
    {
        m_roll = zAngle;

        update();
    }

    void View2D::setViewport(const s32 x, const s32 y, const s32 w, const s32 h)
    {
        m_state.x = x;
        m_state.y = y;
        m_state.w = w;
        m_state.h = h;
        glViewport(x, y, w, h);
    }

    void View2D::setViewport(const IRect &rect)
    {
        m_state.x = rect.x;
        m_state.y = rect.y;
        m_state.w = rect.w;
        m_state.h = rect.h;
        glViewport(rect.x, rect.y, rect.w, rect.h);
    }

    const Vec2f View2D::screenToWorld(Vec2f &point)
    {
        f32 px = point.x;
        f32 py = point.y;
        const IRect &viewport = getViewport();

        Vec4f pointNDC =
        { 
            (2.0f * ((px / viewport.w) - 0.5f)), 
            (2.0f * ((py / viewport.h) - 0.5f)),
            -1.0f,
            1.0f
        };

        Mat4 ivp;
        matrix::inverse4(ivp, (m_projection * m_view));

        Vec4f pointWorld = ivp * pointNDC;
        pointWorld /= pointWorld.w;

        return { pointWorld.x, pointWorld.y };
    }

    const IRect View2D::getViewport() const
    {
        return { m_state.x, m_state.y, m_state.w, m_state.h };
    }

    const Mat4 &View2D::projection() const
    {
        return m_projection;
    }

    const Mat4 &View2D::view() const
    {
        return m_view;
    }
}
