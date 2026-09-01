#pragma once

#include <functional>
#include <vector>
#include <string>

#include "core/math.hpp"

namespace idrs
{
    struct AnimationEvent
    {
        std::function<void(void*)> callback;
        void *data;
        u32 frame;
        bool triggered;
    };

    class SpriteAnimation
    {
    public:
        enum class LoopType
        {
            None,
            Repeat,
            PingPong
        };

    public:
        void load(const std::string &path);
        void addEvent(AnimationEvent &event);
        void resetEventsTriggeredFlag();

        void setLoopType(const LoopType type);
        void setSpeed(const u32 speed);
        void setPlaying(const bool status);
        void setReversing(const bool status);

        std::vector<Vec2u> getFrameOffsets();
        std::vector<AnimationEvent> &getEvents();
        const Vec2u getStartFrame();
        const char *getName();
        const LoopType getLoopType();
        const f32 getSpeed();
        const u32 getRows();
        const u32 getColumns();
        const u32 getTotalFrames();
        const bool isReversing();

    private:
        std::vector<Vec2u> m_frameOffsets;
        std::vector<AnimationEvent> m_events;
        char m_name[32];
        Vec2u m_start;
        LoopType m_loop;
        f32 m_speed;
        u32 m_rows;
        u32 m_columns;
        u32 m_frames;
        bool m_reversing;

    private:
        void calculateFrameOffsets();
    };
}
