#pragma once

#include <typeindex>
#include <variant>
#include <unordered_map>

#include "callbacks/on_frame_start.h"
#include "callbacks/on_frame_end.h"

namespace idrs
{
    using CallbackVar = std::variant<OnFrameStart, OnFrameEnd>;

    class EventCallbackHandler
    {
    public:
        template<typename T>
        static void subscribe(std::function<void()> func);

        template<typename T>
        static void invoke();

    private:
        std::unordered_map<size_t, CallbackVar> m_callbacks;

    private:
        static EventCallbackHandler &get();
        EventCallbackHandler() = default;
    };
}