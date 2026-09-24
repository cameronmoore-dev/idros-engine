#pragma once

#include "core/typedefs.h"

namespace idrs
{
    #define SAMPLES_KHZ 48.0f
    #define INACTIVE_ID UINT32_MAX

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
