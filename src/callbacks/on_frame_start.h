#pragma once

#include <vector>
#include <functional>

namespace idrs
{
    class OnFrameStart
    {
    public:
        void subscribe(std::function<void()> func);
        void invoke();

    private:
        std::vector<std::function<void()>> m_subscribers;
    };
}