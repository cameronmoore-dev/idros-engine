#include "engine.h"

#include "enginecontext.h"

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
            g_engine->input.pumpGamepadEvents(g_engine->window.m_events);
            
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
            switch (e.type)
            {
                case Event::Type::DeviceChanged:
                {
                    g_engine->input.storeGamepads();
                    continue;
                };

                case Event::Type::_DeviceInput:
                {
                    g_engine->input.pollGamepads(
                        e._deviceInputPlatformInternal, 
                        *static_cast<std::queue<Event>*>(&g_engine->window.m_events));
                    continue;
                };

                case Event::Type::KeyPressed:
                {
                    g_engine->input.setInputState(e.keyCode, true);
                } break;

                case Event::Type::KeyReleased:
                {
                    g_engine->input.setInputState(e.keyCode, false);
                } break;

                case Event::Type::MousePressed:
                {
                    g_engine->input.setInputState(e.mouseButton, true);
                } break;

                case Event::Type::MouseReleased:
                {
                    g_engine->input.setInputState(e.mouseButton, false);
                } break;
            }
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
