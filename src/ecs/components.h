#pragma once

#include <functional>

#include "core/graphics/texture.h"
#include "core/graphics/vertexarray.h"
#include "core/graphics/view2d.h"
#include "core/graphics/font.h"
#include "core/graphics/shader.h"
#include "core/graphics/colour.h"

#include "ecs/entity.h"
#include "sprite_animation.h"

#define SEEK 0

namespace idrs
{
    struct Transform
    {
        Vec3f position;
        Vec3f rotation;
        Vec3f scale = { 1, 1, 1 };
    };

    struct Sprite
    {
        struct
        {
            Shader *shader;
            Texture *texture;
            Colour colour;
        } material;
        VertexArray vertices;
        FRect textureRect;
        struct
        {
            s32 layer;
            s32 modifier;
        } sortLayer;
        f32 width;
        f32 height;
        bool flipx;
        bool flipy;
    };

    struct Velocity
    {
        f32 x, y;
    };

    struct RectCollider
    {
        FRect bounds;
        Vec2f offset;
        bool trigger;
        bool isStatic;
        bool ignoreRaycast;
    };

    struct Child
    {
        Entity parent;
        Vec3f localPosition;
        Vec3f localRotation;
        Vec3f localScale = { 1, 1, 1 };
    };

    struct Tag
    {
        char string[32];
        u32 hash;
    };

    struct SpriteAnimator
    {
        SpriteAnimation *current;
        f32 currentFrame;
        bool playing;
    };

    struct Camera2D
    {
        View2D view;
        Entity target;
        f32 smoothing;
    };

    struct Pathfinder
    {
        Entity target;
        f32 smoothing;
        u32 steerBehaviour;
    };

    struct UIButton
    {
        FRect bounds;

        std::function<void()> on_click;
        std::function<void()> on_hover;
    };

    struct UIText
    {
        std::string text;
        VertexArray vertices;
        Font *font;
        Vec2f charPadding;
    };

    struct UISprite
    {
        VertexArray vertices;
        FRect textureRect;
        Texture *texture;
        Vec4f colour = { 1, 1, 1, 1 };
        f32 width;
        f32 height;
        bool flipx;
        bool flipy;
    };
}
