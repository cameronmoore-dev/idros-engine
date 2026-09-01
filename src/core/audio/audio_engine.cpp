#include "audio_engine.h"

namespace idrs
{
    AudioEngine::AudioEngine() :
        m_platform(this)
    {
    }

    void AudioEngine::play(Sound &sound)
    {
        m_platform.play(sound);
    }

    void AudioEngine::play(Music &music)
    {
        m_platform.play(music);
    }

    void AudioEngine::pause(u32 id)
    {
        m_platform.pause(id);
    }

    void AudioEngine::stop(u32 id)
    {
        m_platform.stop(id);
    }

    bool AudioEngine::isPlaying(u32 handle)
    {
        return m_platform.isPlaying(handle);
    }
}
