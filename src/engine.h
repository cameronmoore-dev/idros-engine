#pragma once

#include <stryde.h>

#include "igame.h"

namespace idrs
{
    class Engine
    {
    public:
        Engine(IGame *game);

        void run();

    private:
        IGame *m_game;
        strd::Window *m_window;
        strd::Renderer *m_renderer;

    private:
        void processEvents();
        void update();
        void render();

        void shutdown();
    };
}