#pragma once

#include "ecs/entity.h"
#include "ecs/components.h"

namespace idrs
{
    class SpriteUtils
    {
    public:
        static void constructSprite(Transform &transform, Sprite &sprite);
        static void setTextureCoords(Sprite &sprite, f32 x, f32 y, f32 w, f32 h);
        static void updateDrawPosition(Transform &transform, Sprite &sprite);
        static void flipX(Entity entity);
        static void flipY(Entity entity);
    };
}
