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
    };

    struct SpriteAnimator
    {
        SpriteAnimation *current;
        f32 currentFrame;
        bool playing;
    };

    struct Pathfinder
    {

    };

    struct PathfindingNode
    {
        std::vector<PathfindingNode*> neighbours;
        PathfindingNode *parent;
        Vec2f position;
        bool visited;
    };
}