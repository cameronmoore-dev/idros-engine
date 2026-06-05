#include "physicsutils.h"

#include <pal/idrs_math.h>

namespace idrs
{
    void PhysicsUtils::resolveCollision(Entity a, Entity b)
    {
        Transform &at = ECS::get<Transform>(a);
        RectCollider &ac = ECS::get<RectCollider>(a);
        RectCollider &bc = ECS::get<RectCollider>(b);

        f32 xdir = ac.bounds.x - bc.bounds.x;
        f32 ydir = ac.bounds.y - bc.bounds.y;

        f32 ox = (xdir > 0.0f) ? (bc.bounds.x + bc.bounds.w) - ac.bounds.x : 
                                 bc.bounds.x - (ac.bounds.x + ac.bounds.w);

        f32 oy = (ydir > 0.0f) ? (bc.bounds.y + bc.bounds.h) - ac.bounds.y : 
                                 bc.bounds.y - (ac.bounds.y + ac.bounds.h);

        (std::abs(oy) > std::abs(ox)) ? at.position.x += ox : 
                                        at.position.y += oy;
    }

    void PhysicsUtils::updateCollider(Entity entity)
    {
        Transform &transform = ECS::get<Transform>(entity);
        RectCollider &collider = ECS::get<RectCollider>(entity);

        collider.bounds.x = transform.position.x + collider.offset.x;
        collider.bounds.y = transform.position.y + collider.offset.y;
    }

    bool PhysicsUtils::intersection(RectCollider &a, RectCollider& b)
    {
        if (a.bounds.x + a.bounds.w < b.bounds.x || a.bounds.x > b.bounds.x + b.bounds.w) return false;
        if (a.bounds.y + a.bounds.h < b.bounds.y || a.bounds.y > b.bounds.y + b.bounds.h) return false;

        return true;
    }

    bool PhysicsUtils::raycast2D(RaycastResult &result, Vec2f &start, Vec2f &dir, f32 dist)
    {
        Vec2f end = math::normalize(dir) * dist;
        bool hit = false;
        
        ECS::_view<RectCollider>([&hit, &result, &start, &end](Entity e, RectCollider &collider)
        {
            Vec2f c = {};
            Vec2f d = {};
            for (u32 side = 0; side < 4; side++)
            {
                switch (side)
                {
                    case 0:
                    {
                        /* Bottom-left to Top-left */
                        c = { collider.bounds.x, collider.bounds.y };
                        d = { collider.bounds.x, collider.bounds.y + collider.bounds.h };
                    } break;
    
                    case 1:
                    {
                        /* Top-left to Top-right */
                        c = { collider.bounds.x, collider.bounds.y + collider.bounds.h };
                        d = { collider.bounds.x + collider.bounds.w, collider.bounds.y + collider.bounds.h };
                    } break;
    
                    case 2:
                    {
                        /* Top-right to Bottom-right */
                        c = { collider.bounds.x + collider.bounds.w, collider.bounds.y + collider.bounds.h };
                        d = { collider.bounds.x + collider.bounds.w, collider.bounds.y };
                    } break;
    
                    case 3:
                    {
                        /* Bottom-right to Bottom-left */
                        c = { collider.bounds.x + collider.bounds.w, collider.bounds.y };
                        d = { collider.bounds.x, collider.bounds.y };
                    } break;
                    default: break;
                }

                auto orient = [](Vec2f &a, Vec2f& b, Vec2f &c)
                {
                    return math::cross(b-a, c-a);
                };

                f32 oa = orient(c, d, start);
                f32 ob = orient(c, d, end);
                f32 oc = orient(start, end, c);
                f32 od = orient(start, end, d);
               
                if (oa * ob < 0.0f && oc * od < 0.0f)
                {
                    result.hitPoint = (start * ob) - (end * oa) / (ob - oa);
                    result.hitEntity = e;
                    hit = true;
                }
            }
        });

        return hit;
    }
}