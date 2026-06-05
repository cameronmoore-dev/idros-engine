#pragma once

#include "ecs/ecs.h"
#include "ecs/components.h"

namespace idrs
{
    struct RaycastResult
    {
        Vec2f hitPoint;
        Entity hitEntity;
    };

    class PhysicsUtils
    {
    public:
        static void resolveCollision(Entity a, Entity b);
        static void updateCollider(Entity entity);
        static bool intersection(RectCollider &a, RectCollider& b);
        static bool raycast2D(RaycastResult& result, Vec2f &start, Vec2f &dir, f32 dist);
    };
}