#include "physicsutils.h"

#include <idlai/math.hpp>

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

    bool PhysicsUtils::raycast2D(RaycastResult &result, Vec2f &start, Vec2f &dir, f32 dist, Entity exclude)
    {
        Vec2f end = math::normalize(dir) * dist;
        f32 length = 1.0f;
        Entity hit;

        std::vector<Entity> colliders = ECS::getEntities<RectCollider>();
        for (Entity e : colliders)
        {
            if (e == exclude)
            {
                continue;
            }

            RectCollider& collider = ECS::get<RectCollider>(e);

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

				Vec2f s = d - c;
				Vec2f cma = c - start;
				f32 rxs = math::cross(end, s);

				/* The scalar value of the ray being cast */
				f32 t = math::cross(cma, s) / rxs;

				/* The scalar value of the line between c and d */
				f32 u = math::cross(cma, end) / rxs;

				/* 
                 * If both scalar values are less then the normalised distance between their respective points,
                 * then there is an intersection
                 */
				if ((t >= 0 && t <= 1) && (u >= 0 && u <= 1))
				{
					/* If the intersection point's distance is closer then the previous value */
					if (t < length)
					{
						length = t;
						hit = e;
					}
				}
            }
        }

        /* 
         * Once all of the entities are looped through,
		 * return the closest distance and the entity that was hit
         */
		if (length < 1.0f)
		{
            result.hitEntity = hit;
            result.hitPoint = start + end * length;
			return true;
		}

        return false;
    }
}