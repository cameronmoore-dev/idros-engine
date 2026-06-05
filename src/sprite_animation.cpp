#include "sprite_animation.h"

#include "serialization.h"

namespace idrs
{
    void SpriteAnimation::load(const std::string &path)
    {
        Serialization reader(path, Serialization::IO::ReadText);

        strncpy(m_name, reader.readString("/Name").c_str(), 32);
        m_loop = (LoopType)reader.readInt("/LoopType");
        m_rows = reader.readInt("/Rows");
        m_columns = reader.readInt("/Columns");
        m_frames = reader.readInt("/TotalFrames");
        m_speed = reader.readFlt("/Speed");

        std::array<u32, 2> startPos;
        reader.readArrayInt<u32>("/StartPosition", startPos.data());
        m_start = { startPos[0], startPos[1] };

        calculateFrameOffsets();
    }

    void SpriteAnimation::calculateFrameOffsets()
    {
        u32 parsed = 0;
        m_frameOffsets.resize(m_frames);

        for (u32 y = 0; y < m_rows; y++)
        {
            for (u32 x = 0; x < m_columns; x++)
            {
                m_frameOffsets[parsed] = { x, y };
                if (parsed++ == m_frames)
                {
                    break;
                }
            }
        }
    }

    void SpriteAnimation::setLoopType(const SpriteAnimation::LoopType type) { m_loop = type; }
    void SpriteAnimation::setSpeed(const u32 speed)         { m_speed = speed; }
    void SpriteAnimation::setReversing(const bool status)   { m_reversing = status; }

    std::vector<Vec2u> SpriteAnimation::getFrameOffsets() { return m_frameOffsets; }
    const Vec2u SpriteAnimation::getStartFrame()    { return m_start; }
    const char *SpriteAnimation::getName()          { return m_name; }
    const SpriteAnimation::LoopType SpriteAnimation::getLoopType()  { return m_loop; }
    const f32 SpriteAnimation::getSpeed()           { return m_speed; }
    const u32 SpriteAnimation::getRows()            { return m_rows; }
    const u32 SpriteAnimation::getColumns()         { return m_columns; }
    const u32 SpriteAnimation::getTotalFrames()     { return m_frames; }
    const bool SpriteAnimation::isReversing()       { return m_reversing; }
}