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

    const bool Time::doUpdate()
    {
        while (m_lag >= m_fixedDT)
        {
            m_lag -= m_fixedDT;
            return true;
        }

        return false;
    }

    const f32 Time::deltaTime()
    {
        return m_deltaTime * m_timescale;
    }

    const f32 Time::unscaledDeltaTime()
    {
        return m_deltaTime;
    }

    void Time::updateTimestep()
    {
        m_clock.stop();
        m_deltaTime = m_clock.secondsf();
        m_clock.start();

        if (m_deltaTime >= 0.1f)
        {
            m_deltaTime = 0.1f;
        }

        m_lag += m_deltaTime * m_timescale;
    }

    void Time::setTimescale(f32 scale)
    {
        m_timescale = scale;
    }
}
