#pragma once

#include "core/window/event.h"

namespace idrs
{
    class IGame
    {
    public:
        virtual ~IGame() = default;

        virtual void start() = 0;
        virtual void processEvents(const Event &event) = 0;
        virtual void fixedUpdate() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;
    };
}
