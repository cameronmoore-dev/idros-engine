#pragma once

#include <chrono>

#include "typedefs.h"

namespace strd
{
    using HighResClock  = std::chrono::high_resolution_clock;
    using TimePoint     = std::chrono::high_resolution_clock::time_point;

    inline double getTimeSeconds()
    {
        TimePoint now = HighResClock::now();
        std::chrono::duration<double> time = now.time_since_epoch();
        return time.count();
    }

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