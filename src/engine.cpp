#include "engine.h"

namespace idrs
{
    Engine::Engine(IGame *game) : 
        m_game(game),
        m_window(new Window()),
        m_renderer(nullptr)
    {
        if (!m_game)
        {
            std::cerr << "IDROS::Game failed to create!" << std::endl;
            return;
        }
        if (!m_window->create("Idros Engine", 1280, 720, Style::Default))
        {
            std::cerr << "IDROS::Window failed to create!" << std::endl;
            return;
        }
        
        m_renderer = new Renderer();
    }

    void Engine::run()
    {
        m_game->start(*m_window);

        while (m_window->isOpen())
        {
            processEvents();
            update();
            render();
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

    void Engine::update()
    {
        m_game->update();
    }

    void Engine::render()
    {
        m_renderer->clear(0.1f, 0.1f, 0.1f, 1.0f);

        m_game->draw(*m_renderer);

        m_window->swapBuffers();
    }

    void Engine::shutdown()
    {
        delete m_game;
        delete m_window;
        delete m_renderer;
    }
}