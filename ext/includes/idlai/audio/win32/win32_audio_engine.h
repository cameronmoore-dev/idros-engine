#pragma once

#include <algorithm>

#include <xaudio2.h>

#include "typedefs.h"
#include "audio/sound.h"
#include "audio/music.h"

#define STRD_SOUND_LOOP_INFINITE XAUDIO2_LOOP_INFINITE

namespace idrs
{
    class AudioEngine;
    class Win32AudioEngine
    {
    private:
        enum class AudioType
        {
            Sound = 0,
            Music
        };

    private:
        struct XAudioVoice : IXAudio2VoiceCallback
        {
            IXAudio2SourceVoice *voice;
            size_t id = 0;
            AudioType type;
            bool isPlaying = false;

            void OnBufferStart(void *pContext) override;
            void OnBufferEnd(void *pContext) override;
            void OnStreamEnd() override {}
            void OnVoiceProcessingPassStart(UINT32 BytesRequired) override {}
            void OnVoiceProcessingPassEnd() override {}
            void OnLoopEnd(void *pContext) override {}
            void OnVoiceError(void *pContext, HRESULT Error) override {}
        };

    public:
        Win32AudioEngine(AudioEngine *engine);
        ~Win32AudioEngine();

        void play(const Sound &sound);
        void play(const Music &music);
        void pause(const Sound &sound);
        void pause(const Music &music);
        void stop(const Sound &sound);
        void stop(const Music &music);
        void setVolume(const f32 volume);
        void setPitch(const f32 pitch);

    private:
        AudioEngine *m_engine = nullptr;
        std::vector<XAudioVoice> m_voices;
        std::vector<char> m_samplesCopy;

    private:
        XAudioVoice &getValidVoiceSlot(const u64 soundID);
        void createAndSubmitBuffer(const XAudioVoice &voice, const Sound &sound);
        void createAndSubmitBuffer(const XAudioVoice &voice, const Music &sound);
    };
}