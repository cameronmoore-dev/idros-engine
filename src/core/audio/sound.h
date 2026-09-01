#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "audioinfo.h"

namespace idrs
{
    class Sound
    {
    public:
        u32 id = UINT32_MAX;

    public:
        Sound() = default;
        Sound(const std::string &path);

        void load(const std::string &path);

        void setVolume(f32 volume);
        void setPitch(f32 pitch);
        void setBegin(u32 startPos);
        void setLength(u32 length);
        void setLoopInfo(u32 count, u32 start, u32 length);
        void setLoopCount(u32 count);
        void setLoopStart(u32 start);
        void setLoopLength(u32 length);

        const f32 getVolume();
        const f32 getPitch();
        const u32 getBegin();
        const u32 getLength();
        const std::vector<char> &samples();
        const AudioInfo &info();
        const AudioLoopInfo &loopInfo();

    private:
        std::vector<char> m_data;
        AudioInfo m_info = {};
        AudioLoopInfo m_loop = {};
        u32 m_startPos = 0;
        u32 m_playLength = 0;
        u32 m_clipLength = 0;
        f32 m_volume = 1.0f;
        f32 m_pitch  = 1.0f;

    private:
        void loadWAV(const std::string &path);
        /* void loadOGG */
        /* void loadMP3 */
    };
}
