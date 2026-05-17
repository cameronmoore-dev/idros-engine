#include "engine.h"

namespace idrs
{
    Engine::Engine(IGame *game) : 
        m_game(game),
        m_window(new strd::Window()),
        m_renderer(nullptr)
    {
        if (!m_game)
        {
            std::cerr << "IDROS::Game failed to create!" << std::endl;
            return;
        }
        if (!m_window->create("Idros Engine", 1280, 720, strd::Style::Default))
        {
            std::cerr << "IDROS::Window failed to create!" << std::endl;
            return;
        }
        
        m_renderer = new strd::Renderer();
    }

    void Engine::run()
    {
        m_game->start();

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
        strd::Event e;
        while (m_window->pollEvents(e))
        {
            m_game->processEvents();
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