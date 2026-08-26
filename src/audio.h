#pragma once

#include <idcore/audio/audio_engine.h>

namespace idrs
{
    class Audio
    {
    friend class Engine;
    public:
        static void playSound(Sound &sound);
        static void playMusic(Music &music);

        static bool isPlaying(u32 handle);

    private:
        AudioEngine m_audioEngine;

    private:
        static Audio &get();
        Audio() = default;

        static void init();
    };
}
