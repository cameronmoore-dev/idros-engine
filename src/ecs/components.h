#pragma once

#include <pal/idrs_math.h>
#include <pal/graphics/texture.h>
#include <pal/graphics/vertexarray.h>

#include "sprite_animation.h"

namespace idrs
{
    struct Transform
    {
        Vec3f position;
        Vec3f rotation;
        Vec3f scale;
    };

    struct Sprite
    {
        VertexArray vertices;
        FRect textureRect;
        Texture *texture;
        f32 width;
        f32 height;
        bool flipx;
        bool flipy;
    };

    struct RectCollider
    {
        FRect bounds;
        Vec2f offset;
        bool trigger;
        bool isStatic;
    };

    struct Tag
    {
        char string[32];
    };

    struct SpriteAnimator
    {
        SpriteAnimation *current;
        f32 currentFrame;
        bool playing;
    };
}