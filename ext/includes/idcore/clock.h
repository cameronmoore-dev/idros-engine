#pragma once

#include <chrono>

#include "typedefs.h"

namespace idrs
{
    using HighResClock  = std::chrono::high_resolution_clock;
    using TimePoint     = std::chrono::high_resolution_clock::time_point;

    class Clock
    {
    public:
        Clock();
        void start();
        void stop();

        u64 seconds();
        f32 secondsf();
        u64 milliseconds();
        f32 millisecondsf();
        u64 microseconds();
        f32 microsecondsf();

    private:
        TimePoint m_start;
        TimePoint m_end;
    };
}