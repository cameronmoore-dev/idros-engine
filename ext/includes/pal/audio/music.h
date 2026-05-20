#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <cmath>
#include <algorithm>

#include "typedefs.h"
#include "idrs_assert.h"
#include "format_headers.h"

namespace idrs
{
    class Music
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
        Music() = default;
        Music(const std::string &path);

        void load(const std::string &path);
        void reset();
        u32 swapMusicBuffers();

        std::ifstream &file() { return m_file; }

        const std::vector<char> &samples() const { return m_playBuffer; }
        const u64 &id() const {return m_id;}
        
        private:
        std::ifstream m_file;
        // std::string m_path;
        std::vector<char> m_playBuffer;
        std::vector<char> m_reserveBuffer;
        SoundInfo m_info;
        u64 m_id;
        u32 m_totalSizeBytes;
        u32 m_bufferPos;
        
    private:
        void loadWAV();
        
        void loadChunkIntoBuffer();
        u64 hashID(const std::string &path);
    };
}