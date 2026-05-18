#pragma once

#include <idros_pal.h>

#include "igame.h"

namespace idrs
{
    class Engine
    {
    public:
        Engine(IGame *game);

        void run();

    private:
        IGame    *m_game;
        Window   *m_window;
        Renderer *m_renderer;

    private:
        void processEvents();
        void update();
        void render();

        void shutdown();
    };
}