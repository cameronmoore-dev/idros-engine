#pragma once

#include <idcore/typedefs.h>
#include <idcore/clock.h>

namespace idrs
{
    class Time
    {
    friend class Engine;
    public:
        static const f32 deltaTime();
        static const f32 unscaledDeltaTime();
        static const bool doUpdate();

        static void setTimescale(f32 scale);

    private:
        Clock m_clock;
        f32 m_deltaTime;
        f32 m_timescale;
        f32 m_lag;
        f32 m_fixedDT;

    private:
        static Time &get();
        Time();

        static void init();
        static void updateTimestep();
    };
}
