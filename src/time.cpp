#include "time.hpp"

#include "event_callback_handler.h"

namespace idrs
{
    Time::Time() : 
        m_deltaTime(0.0f),
        m_lag(0.0f),
        m_fixedDT(1.0f / 60.f)
    {
        EventCallbackHandler::subscribe<OnFrameStart>(std::bind(&Time::updateTimestep, this));
    }

    Time &Time::get()
    {
        static Time instance;
        return instance;
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
        return get().m_deltaTime;
    }

    void Time::updateTimestep()
    {
        Time &instance = get();

        instance.m_clock.stop();
        instance.m_deltaTime = instance.m_clock.secondsf();
        instance.m_clock.start();

        if (m_deltaTime >= 0.1f)
        {
            m_deltaTime = 0.1f;
        }

        instance.m_lag += m_deltaTime;
    }
}
