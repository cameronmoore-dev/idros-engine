#include "clock.h"

namespace idrs
{
    Clock::Clock()
    {
        start();
    }

    void Clock::start()
    {
        m_start = HighResClock::now();
    }

    void Clock::stop()
    {
        m_end = HighResClock::now();
    }

    u64 Clock::seconds()
    {
        return std::chrono::duration_cast<std::chrono::seconds>(m_end - m_start).count();
    }

    f32 Clock::secondsf()
    {
        return std::chrono::duration_cast<std::chrono::duration<f32>>(m_end - m_start).count();
    }

    u64 Clock::milliseconds()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start).count();
    }

    f32 Clock::millisecondsf()
    {
        return std::chrono::duration_cast<std::chrono::duration<f32, std::milli>>(m_end - m_start).count();
    }

    u64 Clock::microseconds()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(m_end - m_start).count();
    }

    f32 Clock::microsecondsf()
    {
        return std::chrono::duration_cast<std::chrono::duration<f32, std::micro>>(m_end - m_start).count();
    }
}