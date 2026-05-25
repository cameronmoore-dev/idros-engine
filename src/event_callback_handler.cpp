#include "event_callback_handler.h"

#include <cstdio>

namespace idrs
{
    EventCallbackHandler &EventCallbackHandler::get()
    {
        static EventCallbackHandler instance;
        return instance;
    }

    template<typename T>
    void EventCallbackHandler::subscribe(std::function<void()> func)
    {
        EventCallbackHandler &instance = get();

        size_t key = std::type_index(typeid(T)).hash_code();
        auto it = instance.m_callbacks.find(key);
        if (it == instance.m_callbacks.end())
        {
            instance.m_callbacks.emplace(key, T());
        }
        T &ev = std::get<T>(instance.m_callbacks.at(key));
        ev.subscribe(func);
    }

    template <typename T>
    void EventCallbackHandler::invoke()
    {
        EventCallbackHandler &instance = get();

        size_t key = std::type_index(typeid(T)).hash_code();
        auto it = instance.m_callbacks.find(key);
        if (it != instance.m_callbacks.end())
        {
            T &ev = std::get<T>(it->second);
            ev.invoke();
        }
    }

    template void EventCallbackHandler::subscribe<OnFrameStart>(std::function<void()>);
    template void EventCallbackHandler::subscribe<OnFrameEnd>(std::function<void()>);

    template void EventCallbackHandler::invoke<OnFrameStart>();
    template void EventCallbackHandler::invoke<OnFrameEnd>();
}
