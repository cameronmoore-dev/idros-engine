#include "time.hpp"

#include "event_callback_handler.h"

namespace idrs
{
    Time::Time() : 
        m_deltaTime(0.0f)
    {
        EventCallbackHandler::subscribe<OnFrameStart>(std::bind(&Time::updateTimestep, this));
    }

    Time &Time::get()
    {
        static Time instance;
        return instance;
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
    }
}