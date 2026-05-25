#include "on_frame_start.h"

#include <cstdio>

namespace idrs
{
    void OnFrameStart::subscribe(std::function<void()> func)
    {
        m_subscribers.emplace_back(func);
    }

    void OnFrameStart::invoke()
    {
        for (const auto &sub : m_subscribers)
        {
            sub();
        }
    }
}