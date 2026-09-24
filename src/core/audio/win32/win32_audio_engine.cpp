#include "win32_audio_engine.h"
#include "../audio_engine.h"

#include <cassert>
#include <cstdint>
#include <minwindef.h>
#include <winnt.h>

namespace idrs
{
    void Win32AudioEngine::XAudioVoice::OnBufferStart(void *pContext) noexcept
    {
        switch (type)
        {
            case AudioType::Sound:
            break;

            case AudioType::Music:
            break;
        }
    }

    void Win32AudioEngine::XAudioVoice::OnBufferEnd(void *pContext) noexcept
    {
        switch (type)
        {
            case AudioType::Sound:
            {
                Sound &s = *static_cast<Sound*>(pContext);
                s.id = INACTIVE_ID;
                isPlaying = false;
            } break;

            case AudioType::Music:
            {
                if (!isPlaying) return;

                Music &m = *static_cast<Music*>(pContext);
                u32 bytes = m.swapMusicBuffers();

                XAUDIO2_BUFFER buffer = {0};
                buffer.AudioBytes = bytes;
                buffer.pAudioData = (BYTE*)m.samples().data();
                buffer.pContext = static_cast<void*>(&m);

                HRESULT result = voice->SubmitSourceBuffer(&buffer);
                assert(result >= 0);
            } break;
        }
    }

    Win32AudioEngine::Win32AudioEngine(AudioEngine *engine) :
        m_engine(engine)
    {
        HRESULT result = ::CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        assert(result >= 0);

        IXAudio2 *audio = nullptr;
        result = XAudio2Create(&audio, 0, XAUDIO2_DEFAULT_PROCESSOR);
        assert(result >= 0);

        IXAudio2MasteringVoice *master = nullptr;
        result = audio->CreateMasteringVoice(&master);
        assert(result >= 0);

        WAVEFORMATEX wave = {};
        wave.wFormatTag = WAVE_FORMAT_PCM;
        wave.nChannels = 1;
        wave.nSamplesPerSec = 48000;
        wave.wBitsPerSample = 16;
        wave.nBlockAlign = (wave.nChannels * wave.wBitsPerSample) / 8;
        wave.nAvgBytesPerSec = wave.nSamplesPerSec * wave.nBlockAlign;

        for (u16 i = 0; i < MAX_CONCURRENT_CHANNEL_SOUNDS; i++)
        {
            XAudioVoice *voice = &m_voices[i];
            result = audio->CreateSourceVoice(&voice->voice, &wave, 0, XAUDIO2_DEFAULT_FREQ_RATIO, voice, nullptr, nullptr);
            assert(result >= 0);
        }

        wave.nChannels = 2;
        wave.nBlockAlign = (wave.nChannels * wave.wBitsPerSample) / 8;
        wave.nAvgBytesPerSec = wave.nSamplesPerSec * wave.nBlockAlign;
        for (u16 i = MAX_CONCURRENT_CHANNEL_SOUNDS; i < MAX_TOTAL_CONCURRENT_SOUNDS; i++)
        {
            XAudioVoice *voice = &m_voices[i];
            result = audio->CreateSourceVoice(&voice->voice, &wave, 0, XAUDIO2_DEFAULT_FREQ_RATIO, voice, nullptr, nullptr);
            assert(result >= 0);
        }
    }

    Win32AudioEngine::~Win32AudioEngine()
    {
        for (XAudioVoice &voice : m_voices)
        {
            voice.voice->DestroyVoice();
        }
    }

    void Win32AudioEngine::createAndSubmitBuffer(XAudioVoice &voice, Sound &sound)
    {
        XAUDIO2_BUFFER buffer = {};
        buffer.Flags = XAUDIO2_END_OF_STREAM;
        buffer.AudioBytes = sound.samples().size();
        buffer.PlayBegin  = sound.getBegin() * SAMPLES_KHZ;
        buffer.PlayLength = sound.getLength() * SAMPLES_KHZ;
        buffer.LoopCount  = sound.loopInfo().count;
        buffer.LoopBegin  = sound.loopInfo().start;
        buffer.LoopLength = sound.loopInfo().length;
        buffer.pAudioData = (BYTE*)sound.samples().data();
        buffer.pContext = static_cast<void*>(&sound);

        HRESULT result = voice.voice->SubmitSourceBuffer(&buffer);
        assert(result >= 0);
    }

    void Win32AudioEngine::createAndSubmitBuffer(XAudioVoice &voice, Music &music)
    {
        XAUDIO2_BUFFER buffer = {0};
        buffer.Flags = XAUDIO2_END_OF_STREAM;
        buffer.AudioBytes = music.samples().size();
        buffer.pAudioData = (BYTE*)music.samples().data();
        buffer.pContext = static_cast<void*>(&music);

        HRESULT result = voice.voice->SubmitSourceBuffer(&buffer);
        assert(result >= 0);
    }

    void Win32AudioEngine::play(Sound &sound)
    {
        u32 index = getOpenVoiceSlot(sound.info().numChannels);
        XAudioVoice &voice = m_voices[index];

        createAndSubmitBuffer(voice, sound);
        voice.type = AudioType::Sound;
        voice.voice->SetVolume(sound.getVolume());
        voice.voice->SetFrequencyRatio(sound.getPitch());
        voice.voice->Start();
        InterlockedExchange((LONG*)&sound.id, index);
        InterlockedExchange((LONG*)&voice.isPlaying, true);
    }

    void Win32AudioEngine::play(Music &music)
    {
        u32 index = getOpenVoiceSlot(music.info().numChannels);
        XAudioVoice &voice = m_voices[index];

        createAndSubmitBuffer(voice, music);
        voice.type = AudioType::Music;
        voice.voice->SetVolume(music.getVolume());
        voice.voice->SetFrequencyRatio(music.getPitch());
        voice.voice->Start();
        InterlockedExchange((LONG*)&music.id, index);
        InterlockedExchange((LONG*)&voice.isPlaying, true);
    }

    void Win32AudioEngine::pause(u32 id)
    {
        XAudioVoice &voice = m_voices[id];
        voice.voice->Stop();
        voice.isPlaying = false;
    }

    void Win32AudioEngine::stop(u32 &id)
    {
        XAudioVoice &voice = m_voices[id];
        voice.voice->Stop();
        voice.voice->FlushSourceBuffers();
        voice.voice->SetVolume(1.0f);
        voice.voice->SetFrequencyRatio(1.0f);
        voice.isPlaying = false;
        id = INACTIVE_ID;
    }

    u32 Win32AudioEngine::getOpenVoiceSlot(u32 numChannels)
    {
        u32 start = (MAX_CONCURRENT_CHANNEL_SOUNDS * numChannels) - MAX_CONCURRENT_CHANNEL_SOUNDS;
        u32 end = MAX_CONCURRENT_CHANNEL_SOUNDS * numChannels;
        for (u32 i = start; i < end; i++)
        {
            if (!m_voices[i].isPlaying)
            {
                return i;
            }
        }

        return end - 1;
    }

    bool Win32AudioEngine::isPlaying(u32 id)
    {
        return id != INACTIVE_ID;
    }
}
