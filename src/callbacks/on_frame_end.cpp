#include "on_frame_end.h"

namespace idrs
{
    void OnFrameEnd::subscribe(std::function<void()> func)
    {
        m_subscribers.emplace_back(func);
    }

    void OnFrameEnd::invoke()
    {
        for (const auto &sub : m_subscribers)
        {
            sub();
        }
    }
}
