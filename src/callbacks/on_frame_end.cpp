#include "on_frame_end.h"

#include <cstdio>

namespace idrs
{
    void OnFrameEnd::subscribe(std::function<void()> func)
    {
        printf("calling subscribe on frame end\n");
        m_subscribers.emplace_back(func);
    }

    void OnFrameEnd::invoke()
    {
        for (const auto &sub : m_subscribers)
        {
            printf("calling invoke on frame end\n");
            sub();
        }
    }
}