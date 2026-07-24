#pragma once

#include <functional>

#include <idcore/graphics/texture.h>
#include <idcore/graphics/vertexarray.h>
#include <idcore/graphics/view2d.h>
#include <idcore/graphics/font.h>
#include <idcore/graphics/shader.h>

#include "ecs/entity.h"
#include "graphics/colour.h"
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
        VertexArray vertices;
        FRect textureRect;
        Texture *texture;
        Vec4f colour = { 1, 1, 1, 1 };
        f32 width;
        f32 height;
        bool flipx;
        bool flipy;
    };

    struct Material
    {
        Shader *shader;
        Texture *albedo;
        Colour colour = colour::white;
    };

    struct SortingLayer
    {
        s32 layer;
        s32 modifier;
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
        View2D *view;
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
