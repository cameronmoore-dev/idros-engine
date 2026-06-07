#pragma once

#include <idlai/idlai.h>

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
        void fixedUpdate();
        void update();
        void render();

        void shutdown();
    };
}