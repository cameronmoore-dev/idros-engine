#pragma once

#include <idlai/window/window.h>
#include <idlai/graphics/renderer.h>

namespace idrs
{
    class IGame
    {
        friend class Engine;

    public:
        virtual ~IGame() = default;

        virtual void start() = 0;
        virtual void processEvents(const Event &event) = 0;
        virtual void fixedUpdate() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;

    protected:
        Window *p_window = nullptr;
        Renderer *p_renderer = nullptr;
    };
}