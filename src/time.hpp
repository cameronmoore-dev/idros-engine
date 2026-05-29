#pragma once

#include <pal/typedefs.h>
#include <pal/clock.h>

namespace idrs
{
    class Time
    {
    public:
        static const f32 deltaTime();

    private:
        Clock m_clock;
        f32 m_deltaTime;

    private:
        static Time &get();
        Time();

        void updateTimestep();
    };
}