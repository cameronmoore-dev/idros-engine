#include "time.hpp"

namespace idrs
{
    Time::Time() : 
        m_deltaTime(0.0f),
        m_timescale(1.0f),
        m_lag(0.0f),
        m_fixedDT(1.0f / 60.f)
    {
    }

    Time &Time::get()
    {
        static Time instance;
        return instance;
    }

    void Time::init()
    {
        get();
    }

    const bool Time::doUpdate()
    {
        Time &instance = get();
        while (instance.m_lag >= instance.m_fixedDT)
        {
            instance.m_lag -= instance.m_fixedDT;
            return true;
        }

        return false;
    }

    const f32 Time::deltaTime()
    {
        return get().m_deltaTime * get().m_timescale;
    }

    const f32 Time::unscaledDeltaTime()
    {
        return get().m_deltaTime;
    }

    void Time::updateTimestep()
    {
        Time &instance = get();

        instance.m_clock.stop();
        instance.m_deltaTime = instance.m_clock.secondsf();
        instance.m_clock.start();

        if (instance.m_deltaTime >= 0.1f)
        {
            instance.m_deltaTime = 0.1f;
        }

        instance.m_lag += instance.m_deltaTime * instance.m_timescale;
    }

    void Time::setTimescale(f32 scale)
    {
        get().m_timescale = scale;
    }
}
