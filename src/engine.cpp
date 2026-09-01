#include "engine.h"

namespace idrs
{
    Engine::Engine(IGame *game) : 
        m_game(game)
    {
        g_engine = new EngineContext;
    }

    void Engine::run()
    {
        m_game->start();

        while (g_engine->window.isOpen())
        {
            g_engine->ecs.m_entityManager.addEntities();
            g_engine->time.updateTimestep();

            processEvents();
            fixedUpdate();
            update();
            render();

            g_engine->ecs.m_entityManager.removeEntities();
        }

        shutdown();
    }

    void Engine::processEvents()
    {
        Event e;
        while (g_engine->window.pollEvents(e))
        {
            m_game->processEvents(e);
        }
    }

    void Engine::fixedUpdate()
    {
        while (g_engine->time.doUpdate())
        {
            m_game->fixedUpdate();
        }
    }

    void Engine::update()
    {
        m_game->update();
    }

    void Engine::render()
    {
        g_engine->renderer.clear(0.1f, 0.1f, 0.1f, 1.0f);

        m_game->draw();

        g_engine->window.swapBuffers();
    }

    void Engine::shutdown()
    {
        delete m_game;
        delete g_engine;
    }
}
