#include "sound.h"

#include <cassert>
#include <fstream>
#include <algorithm>

#include "format_headers.h"

namespace idrs
{
    Sound::Sound(const std::string &path)
    {
        load(path);
    }

    void Sound::load(const std::string &path)
    {
        std::string fmt = path.substr(path.length() - 3);
        if (fmt == "wav")
        {
            loadWAV(path);
        }
    }

    void Sound::loadWAV(const std::string &path)
    {
        std::ifstream file(path, std::ios::binary);
        assert(file.is_open());

        _priv::WAVheader info;
        file.read(reinterpret_cast<char*>(&info), sizeof(_priv::WAVheader));

        assert(std::string(info.chunkID, 4) == "RIFF");
        assert(info.numChannels == 1 || info.numChannels == 2);
        assert(info.sampleRate == 48000);
        assert(info.bitsPerSample == 16);

        m_info.sampleRate = info.sampleRate;
        m_info.format = info.audioFormat;
        m_info.numChannels = info.numChannels;
        m_info.bitsPerSample = info.bitsPerSample;

        m_data.resize(info.subchunk2Size);
        file.read(m_data.data(), info.subchunk2Size);

        f32 bytesPerSample = (info.numChannels == 1)
            ? 2.0f
            : 4.0f;
        m_playLength = (m_clipLength = (((f32)m_data.size() / bytesPerSample) / SAMPLES_KHZ));

        file.close();
    }

    void Sound::setVolume(f32 volume)
    {
        m_volume = volume;
    }

    void Sound::setPitch(f32 pitch)
    {
        m_pitch = pitch;
    }

    void Sound::setBegin(u32 startPos)
    {
        m_startPos = std::clamp(startPos, (u32)0, m_clipLength);

        /* Set playLength here so that playback doesn't exceed the buffer */
        m_playLength = std::clamp(m_playLength, (u32)0, m_clipLength - m_startPos);
    }

    void Sound::setLength(u32 length)
    {
        m_playLength = std::clamp(length, (u32)0, m_clipLength - m_startPos);
    }

    void Sound::setLoopInfo(u32 count, u32 start, u32 length)
    {
        m_loop.count  = count;
        m_loop.start  = (u32)(start  * SAMPLES_KHZ);
        m_loop.length = (u32)(length * SAMPLES_KHZ);
    }

    void Sound::setLoopCount(u32 count)
    {
        m_loop.count = count;
    }

    void Sound::setLoopStart(u32 start)
    {
        m_loop.start = (u32)(start * SAMPLES_KHZ);
    }

    void Sound::setLoopLength(u32 length)
    {
        m_loop.length = (u32)(length * SAMPLES_KHZ);
    }

    const f32 Sound::getVolume()
    {
        return m_volume;
    }

    const f32 Sound::getPitch()
    {
        return m_pitch;
    }

    const u32 Sound::getBegin()
    {
        return m_startPos;
    }

    const u32 Sound::getLength()
    {
        return m_playLength;
    }

    const std::vector<char> &Sound::samples()
    {
        return m_data;
    }

    const AudioInfo &Sound::info()
    {
        return m_info;
    }

    const AudioLoopInfo &Sound::loopInfo()
    {
        return m_loop;
    }
}
