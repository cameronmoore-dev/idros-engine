#pragma once

#if defined(_WIN32)
    #include "win32/win32_audio_engine.h"
    #define PLATFORM_AUDIO_ENGINE idrs::Win32AudioEngine
    using PlatformAudioEngine = idrs::Win32AudioEngine;
#endif
#if defined (__linux__)
    
#endif

#include "sound.h"

namespace idrs
{
    class AudioEngine
    {
        friend class PLATFORM_AUDIO_ENGINE;

    public:
        AudioEngine();
        
        void play(Sound &sound);
        void play(Music &music);
        void pause(Sound &sound);
        void pause(Music &music);
        void stop(Sound &sound);
        void stop(Music &music);

        bool isPlaying(u32 handle);

        // void setVolume(const f32 volume);
        // void setPitch(const f32 pitch);

    private:
        PlatformAudioEngine m_platform;
    };
}
