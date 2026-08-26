#include "audio.h"

namespace idrs
{
    Audio &Audio::get()
    {
        static Audio instance;
        return instance;
    }

    void Audio::init()
    {
        get();
    }

    void Audio::playSound(Sound &sound)
    {
        get().m_audioEngine.play(sound);
    }

    bool Audio::isPlaying(u32 handle)
    {
        return get().m_audioEngine.isPlaying(handle);
    }
}
