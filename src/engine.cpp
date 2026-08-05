#include "engine.h"

#include "ecs/ecs.h"
#include "audio.h"
#include "resource_manager.h"
#include "debug.h"
#include "time.hpp"
#include "random.hpp"

namespace idrs
{
    Engine::Engine(IGame *game) : 
        m_game(game),
        m_window(new Window()),
        m_renderer(nullptr)
    {
        m_window->create("Idros Engine", 1280, 720, Style::Default);
        m_renderer = new Renderer();

        m_game->p_window = m_window;
        m_game->p_renderer = m_renderer;

        ECS::init();
        Audio::init();
        Time::init();
        ResourceManager::init();
        Debug::init();
        Random::init();
    }

    void Engine::run()
    {
        m_game->start();

        while (m_window->isOpen())
        {
            ECS::get().m_entityManager.addEntities();
            Time::updateTimestep();

            processEvents();
            fixedUpdate();
            update();
            render();

            ECS::get().m_entityManager.removeEntities();
        }

        shutdown();
    }

    void Engine::processEvents()
    {
        Event e;
        while (m_window->pollEvents(e))
        {
            m_game->processEvents(e);
        }
    }

    void Engine::fixedUpdate()
    {
        while (Time::doUpdate())
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
        m_renderer->clear(0.1f, 0.1f, 0.1f, 1.0f);

        m_game->draw();

        m_window->swapBuffers();
    }

    void Engine::shutdown()
    {
        delete m_game;
        delete m_window;
        delete m_renderer;
    }
}
