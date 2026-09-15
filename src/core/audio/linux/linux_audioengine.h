#pragma once

#include "core/audio/sound.h"
#include "core/audio/music.h"

namespace idrs
{
    class AudioEngine;
    class LinuxAudioEngine
    {
    public:
        LinuxAudioEngine(AudioEngine *audio);

        void play(Sound &sound);
        void play(Music &music);
        void pause(u32 id);
        void stop(u32 id);

        bool isPlaying(u32 id);
        
    private:
        AudioEngine *m_audioEngine = nullptr;
    };
}