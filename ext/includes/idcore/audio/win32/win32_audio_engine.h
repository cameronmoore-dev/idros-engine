#pragma once

#include <xaudio2.h>

#include <array>

#include "typedefs.h"
#include "audio/sound.h"
#include "audio/music.h"

#define MAX_CONCURRENT_SOUNDS 16

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
            AudioType type;
            bool isPlaying;

            void OnBufferStart(void *pContext) noexcept override;
            void OnBufferEnd(void *pContext) noexcept override;

            void OnStreamEnd() noexcept override {}
            void OnVoiceProcessingPassStart(UINT32 BytesRequired) noexcept override {}
            void OnVoiceProcessingPassEnd() noexcept override {}
            void OnLoopEnd(void *pContext) noexcept override {}
            void OnVoiceError(void *pContext, HRESULT Error) noexcept override {}
        };

    public:
        Win32AudioEngine(AudioEngine *engine);
        ~Win32AudioEngine();

        void play(Sound &sound);
        void play(Music &music);
        void pause(Sound &sound);
        void pause(Music &music);
        void stop(Sound &sound);
        void stop(Music &music);

        bool isPlaying(u32 handle);

    private:
        AudioEngine *m_engine = nullptr;
        std::array<XAudioVoice, MAX_CONCURRENT_SOUNDS> m_monoVoices;
        std::array<XAudioVoice, MAX_CONCURRENT_SOUNDS> m_stereoVoices;

    private:
        void createAndSubmitBuffer(XAudioVoice &voice, Sound &sound);
        void createAndSubmitBuffer(XAudioVoice &voice, Music &sound);

        u32 getOpenVoiceSlot(u32 numChannels);
        std::array<XAudioVoice, MAX_CONCURRENT_SOUNDS> &getVoiceArrayByChannelCount(u32 numChannels);
    };
}
