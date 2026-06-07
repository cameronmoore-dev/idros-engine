#pragma once

#include <idlai/typedefs.h>
#include <idlai/clock.h>

namespace idrs
{
    class Time
    {
    public:
        static const f32 deltaTime();

        static const bool doUpdate();

    private:
        Clock m_clock;
        f32 m_deltaTime;

        f32 lag;
        f32 fixedDT;

    private:
        static Time &get();
        Time();

        void updateTimestep();
    };
}