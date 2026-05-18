#pragma once

#include <pal/window/window.h>
#include <pal/graphics/renderer.h>

namespace idrs
{
    class IGame
    {
    public:
        virtual ~IGame() = default;

        virtual void start(Window &window) = 0;
        virtual void processEvents(const Event &event) = 0;
        virtual void update() = 0;
        virtual void draw(Renderer &renderer) = 0;
    };
}