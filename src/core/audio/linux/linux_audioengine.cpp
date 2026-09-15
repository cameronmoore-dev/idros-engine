#include "linux_audioengine.h"
#include "core/audio/audio_engine.h"

namespace idrs
{
    LinuxAudioEngine::LinuxAudioEngine(AudioEngine *audio) : 
        m_audioEngine(audio)
    {
    }

    void LinuxAudioEngine::play(Sound &sound)
    {
    }

    void LinuxAudioEngine::play(Music &music)
    {
    }

    void LinuxAudioEngine::pause(u32 id)
    {
    }

    void LinuxAudioEngine::stop(u32 id)
    {
    }

    bool LinuxAudioEngine::isPlaying(u32 id)
    {
        return id != UINT32_MAX;
    }
}
