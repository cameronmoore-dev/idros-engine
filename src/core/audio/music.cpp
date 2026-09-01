#include "music.h"

#include <cassert>
#include <algorithm>

#include "format_headers.h"

namespace idrs
{
    #define BUFFER_SIZE 65536U

    Music::Music(const std::string &path)
    {
        load(path);
    }

    void Music::reset()
    {
        m_file.seekg(0);
        loadWAV();
    }

    void Music::load(const std::string &path)
    {
        m_file = std::ifstream(path, std::ios::binary);
        assert(m_file);

        std::string fmt = path.substr(path.length() - 3);
        if (fmt == "wav")
        {
            loadWAV();
        }
    }

    void Music::loadWAV()
    {
        _priv::WAVheader info;
        m_file.read(reinterpret_cast<char*>(&info), sizeof(_priv::WAVheader));

        assert(std::string(info.chunkID, 4) == "RIFF");
        assert(info.numChannels == 1 || info.numChannels == 2);
        assert(info.sampleRate == 48000);
        assert(info.bitsPerSample == 16);

        m_info.sampleRate = info.sampleRate;
        m_info.format = info.audioFormat;
        m_info.numChannels = info.numChannels;
        m_info.bitsPerSample = info.bitsPerSample;

        m_playBuffer.resize(BUFFER_SIZE);
        m_reserveBuffer.resize(BUFFER_SIZE);
        m_file.read(m_playBuffer.data(), BUFFER_SIZE);
        m_file.read(m_reserveBuffer.data(), BUFFER_SIZE);

        m_totalSizeBytes = info.subchunk2Size;
        m_bufferPos = BUFFER_SIZE * 2;
    }

    u32 Music::swapMusicBuffers()
    {
        std::swap(m_playBuffer, m_reserveBuffer);

        u32 remaining = m_totalSizeBytes - m_bufferPos;
        if (remaining >= BUFFER_SIZE)
        {
            m_file.seekg(m_bufferPos);
            m_file.read(m_reserveBuffer.data(), BUFFER_SIZE);
            m_bufferPos += BUFFER_SIZE;
        }
        else
        {
            u32 wrapBytes = BUFFER_SIZE - remaining;

            m_file.seekg(m_bufferPos);
            m_file.read(m_reserveBuffer.data(), remaining);
            m_file.seekg(sizeof(_priv::WAVheader));
            m_file.read(m_reserveBuffer.data() + remaining, wrapBytes);

            m_bufferPos = sizeof(_priv::WAVheader) + wrapBytes;
        }

        return BUFFER_SIZE;
    }

    void Music::setVolume(f32 volume)
    {
        m_volume = volume;
    }

    void Music::setPitch(f32 pitch)
    {
        m_pitch = pitch;
    }
}
