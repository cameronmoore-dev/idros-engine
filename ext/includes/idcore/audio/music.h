#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "typedefs.h"
#include "audioinfo.h"

namespace idrs
{
    class Music
    {
    public:
        Music() = default;
        Music(const std::string &path);

        void load(const std::string &path);
        void reset();
        u32 swapMusicBuffers();

        std::ifstream &file() { return m_file; }

        const std::vector<char> &samples() const { return m_playBuffer; }
        const AudioInfo &info() const { return m_info; }
        const u64 &id() const {return m_id;}
        
        private:
        std::ifstream m_file;
        // std::string m_path;
        std::vector<char> m_playBuffer;
        std::vector<char> m_reserveBuffer;
        AudioInfo m_info;
        u64 m_id;
        u32 m_totalSizeBytes;
        u32 m_bufferPos;
        
    private:
        void loadWAV();
        
        void loadChunkIntoBuffer();
        u64 hashID(const std::string &path);
    };
}
