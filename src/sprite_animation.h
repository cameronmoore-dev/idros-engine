#pragma once

#include <filesystem>
#include <vector>
#include <cstring>

#include <idlai/math.hpp>

namespace idrs
{
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
        
        void setLoopType(const LoopType type);
        void setSpeed(const u32 speed);
        void setPlaying(const bool status);
        void setReversing(const bool status);

        std::vector<Vec2u> getFrameOffsets();
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