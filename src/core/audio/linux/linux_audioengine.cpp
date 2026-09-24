#include "linux_audioengine.h"
#include "core/audio/audio_engine.h"

#include "time.hpp"

#include <spa/param/audio/format-utils.h>
#include <spa/param/props.h>

namespace idrs
{
    LinuxAudioEngine::LinuxAudioEngine(AudioEngine *audio) : 
        m_audioEngine(audio),
        m_threadLoop(nullptr)
    {
        pw_init(nullptr, nullptr);

        m_threadLoop = pw_thread_loop_new("audio-thread", nullptr);
        pw_thread_loop_start(m_threadLoop);

        memset(m_clips, 0, sizeof(m_clips));
    }

    LinuxAudioEngine::~LinuxAudioEngine()
    {
        pw_thread_loop_destroy(m_threadLoop);
        pw_deinit();
    }

    void LinuxAudioEngine::play(Sound &sound)
    {
        if (sound.id != INACTIVE_ID)
        {
            return;
        }

        LinuxAudioData *data = getFreeSlot(sound.id);
        data->streamEvents.version = PW_VERSION_STREAM_EVENTS;
        data->streamEvents.process = onProcessSound;

        pw_thread_loop_lock(m_threadLoop);

        if (!data->isPlaying && data->stream != nullptr)
        {
            pw_stream_destroy(data->stream);
            data->stream = nullptr;
        }

        const spa_pod *params[2];
        uint8_t buffer[1024];
        spa_pod_builder pod = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));
        pw_properties *props = pw_properties_new(
            PW_KEY_MEDIA_TYPE, "Audio",
            PW_KEY_MEDIA_CATEGORY, "Playback",
            PW_KEY_MEDIA_ROLE, "Game", NULL
        );

        data->audioType = (void *)&sound;
        data->stream = pw_stream_new_simple(
            pw_thread_loop_get_loop(m_threadLoop),
            "sound", props, 
            &data->streamEvents, data
        );

        spa_audio_info_raw raw = SPA_AUDIO_INFO_RAW_INIT();
        raw.format = SPA_AUDIO_FORMAT_S16;
        raw.channels = sound.info().numChannels;
        raw.rate = sound.info().sampleRate * sound.getPitch();
        params[0] = spa_format_audio_raw_build(&pod, SPA_PARAM_EnumFormat, &raw);
        params[1] = (spa_pod *)spa_pod_builder_add_object(&pod,
            SPA_TYPE_OBJECT_Props, SPA_PARAM_Props,
            SPA_PROP_volume, SPA_POD_Float(sound.getVolume())
        );

        pw_stream_flags flags = (pw_stream_flags)(PW_STREAM_FLAG_AUTOCONNECT | PW_STREAM_FLAG_MAP_BUFFERS | PW_STREAM_FLAG_RT_PROCESS);
        pw_stream_connect(
            data->stream,
            PW_DIRECTION_OUTPUT, PW_ID_ANY,
            flags, params, 2
        );

        data->isPlaying = true;
        pw_thread_loop_unlock(m_threadLoop);
    }

    void LinuxAudioEngine::play(Music &music)
    {
        if (music.id != INACTIVE_ID)
        {
            return;
        }

        LinuxAudioData *data = getFreeSlot(music.id);
        music.reset();

        data->streamEvents.version = PW_VERSION_STREAM_EVENTS;
        data->streamEvents.process = onProcessMusic;
        data->streamEvents.control_info = onControlInfo;

        pw_thread_loop_lock(m_threadLoop);

        if (!data->isPlaying && data->stream != nullptr)
        {
            pw_stream_destroy(data->stream);
            data->stream = nullptr;
        }

        const spa_pod *params[2];
        uint8_t buffer[1024];
        spa_pod_builder pod = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));

        pw_properties *props = pw_properties_new(
            PW_KEY_MEDIA_TYPE, "Audio",
            PW_KEY_MEDIA_CATEGORY, "Playback",
            PW_KEY_MEDIA_ROLE, "Music",
            NULL
        );

        data->audioType = (void *)&music;
        data->stream = pw_stream_new_simple(
            pw_thread_loop_get_loop(m_threadLoop),
            "music", props, 
            &data->streamEvents, data
        );

        spa_audio_info_raw raw = SPA_AUDIO_INFO_RAW_INIT();
        raw.format = SPA_AUDIO_FORMAT_S16;
        raw.channels = music.info().numChannels;
        raw.rate = music.info().sampleRate * music.getPitch();
        params[0] = spa_format_audio_raw_build(&pod, SPA_PARAM_EnumFormat, &raw);
        params[1] = (spa_pod *)spa_pod_builder_add_object(&pod,
            SPA_TYPE_OBJECT_Props, SPA_PARAM_Props,
            SPA_PROP_volume, SPA_POD_Float(music.getVolume())
        );

        pw_stream_flags flags = (pw_stream_flags)(PW_STREAM_FLAG_AUTOCONNECT | PW_STREAM_FLAG_MAP_BUFFERS | PW_STREAM_FLAG_RT_PROCESS);
        pw_stream_connect(
            data->stream,
            PW_DIRECTION_OUTPUT, PW_ID_ANY,
            flags, params, 2
        );

        data->isPlaying = true;
        pw_thread_loop_unlock(m_threadLoop);

        // NOTE: Setting stream controls only takes effect when the stream state is STREAMING.
        //       At this point it's just CONNECTING
        pw_stream_state state = pw_stream_get_state(data->stream, nullptr);
    }

    void LinuxAudioEngine::pause(u32 id)
    {
        LinuxAudioData *data = &m_clips[id];
        pw_stream_state state = pw_stream_get_state(data->stream, nullptr);
        if (state != PW_STREAM_STATE_PAUSED && 
            state != PW_STREAM_STATE_UNCONNECTED)
        {
            pw_thread_loop_lock(m_threadLoop);
            pw_stream_set_active(data->stream, false);
            pw_thread_loop_unlock(m_threadLoop);
        }
    }

    void LinuxAudioEngine::resume(u32 id)
    {
        LinuxAudioData *data = &m_clips[id];
        pw_stream_state state = pw_stream_get_state(data->stream, nullptr);
        if (state == PW_STREAM_STATE_PAUSED && 
            state != PW_STREAM_STATE_UNCONNECTED)
        {
            pw_thread_loop_lock(m_threadLoop);
            pw_stream_set_active(data->stream, true);
            pw_thread_loop_unlock(m_threadLoop);
        }
    }

    void LinuxAudioEngine::stop(u32 &id)
    {
        if (id == INACTIVE_ID)
        {
            return;
        }
        
        LinuxAudioData *data = &m_clips[id];
        pw_thread_loop_lock(m_threadLoop);
        pw_stream_disconnect(data->stream);
        pw_stream_destroy(data->stream);
        pw_thread_loop_unlock(m_threadLoop);

        data->stream = nullptr;
        data->isPlaying = false;
        id = INACTIVE_ID;
    }

    bool LinuxAudioEngine::isPlaying(u32 id)
    {
        if (id != INACTIVE_ID)
        {
            LinuxAudioData *data = &m_clips[id];
            pw_stream_state state = pw_stream_get_state(data->stream, nullptr);
            return (state != PW_STREAM_STATE_PAUSED);
        }
        return false;
    }

    LinuxAudioData *LinuxAudioEngine::getFreeSlot(u32 &outID)
    {
        for (u32 i = 0; i < MAX_ACTIVE_AUDIO_CLIPS; i++)
        {
            if (!m_clips[i].isPlaying)
            {
                outID = i;
                return &m_clips[i];
            }
        }
        outID = MAX_ACTIVE_AUDIO_CLIPS - 1;
        return &m_clips[MAX_ACTIVE_AUDIO_CLIPS - 1];
    }

    void LinuxAudioEngine::onControlInfo(void *userdata, u32 id, const pw_stream_control *control)
    {
        if (id == SPA_PROP_volume)
        {
            LinuxAudioData *data = (LinuxAudioData *)userdata;
            pw_stream_set_control(data->stream, id, control->n_values, control->values);
        }
    }

    // TODO: Handle repeating/looping sound effects
    void LinuxAudioEngine::onProcessSound(void *userdata)
    {
        LinuxAudioData &data = *(LinuxAudioData *)userdata;
        Sound &sound = *(Sound *)data.audioType;

        pw_buffer *pw_buf = pw_stream_dequeue_buffer(data.stream);
        spa_buffer *spa_buf = pw_buf->buffer;

        // NOTE: Pointer to the beginning of the sound's pcm buffer
        char *dst = (char *)spa_buf->datas[0].data;
        u32 stride = sizeof(int16_t) * sound.info().numChannels;
        u32 bytesNeeded = spa_buf->datas[0].maxsize;

        if (pw_buf->requested)
        {
            bytesNeeded = SPA_MIN(pw_buf->requested * stride, bytesNeeded);
        }

        u32 bytesRemaining = sound.samples().size() - data.bufferPos;
        u32 bytesToCopy = SPA_MIN(bytesNeeded, bytesRemaining);

        if (bytesToCopy > 0)
        {
            auto currentItrPos = sound.samples().begin() + data.bufferPos;
            std::copy(currentItrPos, currentItrPos + bytesToCopy, dst);

            data.bufferPos += bytesToCopy;
            spa_buf->datas[0].chunk->offset = 0;
            spa_buf->datas[0].chunk->stride = stride;
            spa_buf->datas[0].chunk->size = bytesToCopy;
            pw_stream_queue_buffer(data.stream, pw_buf);
        }
        else
        {
            sound.id = INACTIVE_ID;
            data.bufferPos = 0;
            data.isPlaying = false;
            spa_buf->datas[0].chunk->size = 0;
        }
    }

    void LinuxAudioEngine::onProcessMusic(void *userdata)
    {
        LinuxAudioData &data = *(LinuxAudioData *)userdata;
        Music &music = *(Music *)data.audioType;

        pw_buffer *pw_buf = pw_stream_dequeue_buffer(data.stream);
        spa_buffer *spa_buf = pw_buf->buffer;
        char *dst = (char *)spa_buf->datas[0].data;
        u32 stride = sizeof(int16_t) * music.info().numChannels;
        u32 bytesNeeded = spa_buf->datas[0].maxsize;

        if (pw_buf->requested)
        {
            bytesNeeded = SPA_MIN(pw_buf->requested * stride, bytesNeeded);
        }

        u32 written = 0;
        while (written < bytesNeeded)
        {
            u32 bytesRemaining = music.samples().size() - data.bufferPos;
            u32 bytesToCopy = SPA_MIN(bytesNeeded, bytesRemaining);
            if (bytesToCopy > 0)
            {
                auto currentItrPos = music.samples().begin() + data.bufferPos;
                std::copy(currentItrPos, currentItrPos + bytesToCopy, dst + written);

                data.bufferPos += bytesToCopy;
                written += bytesToCopy;
            }
            else
            {
                if (data.bufferPos >= music.samples().size())
                {
                    music.swapMusicBuffers();
                    data.bufferPos = 0;
                }
            }
        }

        spa_buf->datas[0].chunk->offset = 0;
        spa_buf->datas[0].chunk->stride = stride;
        spa_buf->datas[0].chunk->size = written;
        pw_stream_queue_buffer(data.stream, pw_buf);
    }
}
