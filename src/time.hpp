#pragma once

#include "core/typedefs.h"
#include "core/clock.h"

namespace idrs
{
    class Time
    {
    friend class Engine;
    public:
        Time();

        const f32 deltaTime();
        const f32 unscaledDeltaTime();
        const bool doUpdate();

        void setTimescale(f32 scale);

    private:
        Clock m_clock;
        f32 m_deltaTime;
        f32 m_timescale;
        f32 m_lag;
        f32 m_fixedDT;

    private:
        void updateTimestep();
    };
}
