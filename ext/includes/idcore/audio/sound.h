#pragma once

#include <string>
#include <vector>

#include "typedefs.h"
#include "audioinfo.h"

namespace idrs
{
    class Sound
    {
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
        const AudioInfo &info() const;
        const AudioLoopInfo &loopInfo() const;
        const u64 &id() const;
        
    private:
        std::vector<char> m_data;
        AudioInfo m_info;
        AudioLoopInfo m_loop;
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
