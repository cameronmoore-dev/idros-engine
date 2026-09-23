#pragma once

#define MAX_ACTIVE_AUDIO_CLIPS 16

#include <pipewire/pipewire.h>
#include <spa/param/audio/format-utils.h>

#include "core/audio/sound.h"
#include "core/audio/music.h"

namespace idrs
{
    struct LinuxAudioData
    {
        void *audioType;
        pw_stream *stream;
        pw_stream_events streamEvents;
        u32 bufferPos;
        bool isPlaying;
    };

    class AudioEngine;
    class LinuxAudioEngine
    {
    public:
        LinuxAudioEngine(AudioEngine *audio);
        ~LinuxAudioEngine();

        void play(Sound &sound);
        void play(Music &music);
        void pause(u32 id);
        void stop(u32 id);

        bool isPlaying(u32 id);
        
    private:
        AudioEngine *m_audioEngine;
        LinuxAudioData m_clips[MAX_ACTIVE_AUDIO_CLIPS];
        pw_thread_loop *m_threadLoop;

    private:
        LinuxAudioData *getFreeSlot(u32 &outID);
        static void onProcessSound(void *userdata);
        static void onProcessMusic(void *userdata);
    };
}