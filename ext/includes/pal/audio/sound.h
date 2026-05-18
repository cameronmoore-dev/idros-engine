#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <cmath>
#include <algorithm>

#include "idrs_assert.h"
#include "typedefs.h"
#include "format_headers.h"

namespace idrs
{
    class Sound
    {
    public:
        struct SoundInfo
        {
            u32 sampleRate;
            u16 format;
            u16 numChannels;
            u16 bitsPerSample;
        };

        struct LoopInfo
        {
            u32 count;
            u32 start;
            u32 length;
        };

    public:
        Sound() = default;
        Sound(const std::string &path);

        void load(const std::string &path);

        void setBegin(const u32 startPos);
        void setLength(const u32 length);
        void setLoopInfo(const u32 count, const u32 start, const u32 length);
        void setLoopCount(const u32 count);
        void setLoopStart(const u32 start);
        void setLoopLength(const u32 length);

        const u32 getBegin() const;
        const u32 getLength() const;
        const std::vector<char> &samples() const;
        const SoundInfo &info() const;
        const LoopInfo &loopInfo() const;
        const u64 &id() const;
        
    private:
        std::vector<char> m_data;
        SoundInfo m_info;
        LoopInfo m_loop;
        u64 m_id;
        u32 m_startPos;
        u32 m_playLength;
        u32 m_clipLength;

    private:
        void loadWAV(const std::string &path);
        /* void loadOGG */
        /* void loadMP3 */

        u64 hashID(const std::string &path);
    };
}