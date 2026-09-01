#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "audioinfo.h"

namespace idrs
{
    class Music
    {
    public:
        u32 id;

    public:
        Music() = default;
        Music(const std::string &path);

        void load(const std::string &path);
        void reset();
        u32 swapMusicBuffers();

        void setVolume(f32 volume);
        void setPitch(f32 pitch);

        const f32 getVolume() { return m_volume; }
        const f32 getPitch()  { return m_pitch; }

        const std::ifstream &file() { return m_file; }
        const std::vector<char> &samples() { return m_playBuffer; }
        const AudioInfo &info() { return m_info; }

        private:
        std::ifstream m_file;
        // std::string m_path;
        std::vector<char> m_playBuffer;
        std::vector<char> m_reserveBuffer;
        AudioInfo m_info = {};
        u32 m_totalSizeBytes = 0;
        u32 m_bufferPos = 0;
        f32 m_volume = 1.0f;
        f32 m_pitch  = 1.0f;

    private:
        void loadWAV();
        void loadChunkIntoBuffer();
    };
}
