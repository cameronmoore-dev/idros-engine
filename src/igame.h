#pragma once

namespace idrs
{
    class IGame
    {
    public:
        virtual ~IGame() = default;

        virtual void start() = 0;
        virtual void processEvents() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;
    };
}