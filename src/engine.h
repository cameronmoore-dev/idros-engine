#pragma once

#include "igame.h"
#include "enginecontext.h"

namespace idrs
{
    class Engine
    {
    public:
        Engine(IGame *game);

        void run();

    private:
        IGame *m_game;

    private:
        void processEvents();
        void fixedUpdate();
        void update();
        void render();

        void shutdown();
    };
}
