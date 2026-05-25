#include "on_frame_start.h"

#include <cstdio>

namespace idrs
{
    void OnFrameStart::subscribe(std::function<void()> func)
    {
        printf("calling subscribe on frame start\n");
        m_subscribers.emplace_back(func);
    }

    void OnFrameStart::invoke()
    {
        for (const auto &sub : m_subscribers)
        {
            printf("calling invoke on frame start\n");
            sub();
        }
    }
}