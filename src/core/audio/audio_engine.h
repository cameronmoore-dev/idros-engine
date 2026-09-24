#pragma once

#if defined(_WIN32)
    #include "win32/win32_audio_engine.h"
    #define PLATFORM_AUDIO_ENGINE idrs::Win32AudioEngine
    using PlatformAudioEngine = idrs::Win32AudioEngine;
#endif
#if defined (__linux__)
    #include "linux/linux_audioengine.h"
    #define PLATFORM_AUDIO_ENGINE idrs::LinuxAudioEngine;
    using PlatformAudioEngine = idrs::LinuxAudioEngine;
#endif

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

        void play(AudioInfo &info);
        void pause(u32 id);
        void resume(u32 id);
        void stop(u32 &id);

        bool isPlaying(u32 handle);

    private:
        PlatformAudioEngine m_platform;
    };
}
