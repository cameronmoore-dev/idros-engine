#pragma once

#if defined(_WIN32)
    #include "win32/win32_audio_engine.h"
    #define PLATFORM_AUDIO_ENGINE strd::Win32AudioEngine;
    using PlatformAudioEngine = strd::Win32AudioEngine;
#endif
#if defined (__linux__)
    
#endif

#include "sound.h"

namespace strd
{
    class AudioEngine
    {
        friend class PLATFORM_AUDIO_ENGINE;

    public:
        AudioEngine();
        
        void play(const Sound &sound);
        void play(const Music &music);
        void pause(const Sound &sound);
        void pause(const Music &music);
        void stop(const Sound &sound);
        void stop(const Music &music);
        void setVolume(const f32 volume);
        void setPitch(const f32 pitch);

    private:
        PlatformAudioEngine m_impl;
    };
}