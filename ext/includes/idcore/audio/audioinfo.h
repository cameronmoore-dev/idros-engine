#pragma once

#include "typedefs.h"

namespace idrs
{
    #define SAMPLES_KHZ 48.0f

    struct AudioInfo
    {
        u32 sampleRate;
        u16 format;
        u16 numChannels;
        u16 bitsPerSample;
    };

    struct AudioLoopInfo
    {
        u32 count;
        u32 start;
        u32 length;
    };
}
