#pragma once

#include "typedefs.h"

namespace idrs::_priv
{
    /* http://soundfile.sapp.org/doc/WaveFormat/ */
    #pragma pack(push, 1)
    struct WAVheader
    {
        char chunkID[4];        /* Should be "RIFF" */
        u32 chunkSize = 0;      /* Size of the entire file minus 8 bytes */
        char format[4];         /* Should be "WAVE" */
        char subchunk1ID[4];    /* Should be "fmt " */
        u32 subchunk1Size;      /* 16 for PCM. This is the size of the rest of the Subchunk which follows this number. */
        u16 audioFormat;        /* PCM = 1 (i.e. Linear quantization). Values other than 1 indicate some form of compression. */
        u16 numChannels;        /* Mono = 1, Stereo = 2 */
        u32 sampleRate;         /* 8000, 44100, etc. */
        u32 byteRate;           /* == SampleRate * NumChannels * BitsPerSample / 8 */
        u16 blockAlign;         /* == NumChannels * BitsPerSample / 8 (The number of bytes for one sample including all channels) */
        u16 bitsPerSample;      /* 8, 16, 24, or 32 */
        char subchunk2ID[4];    /* Should be "data" */
        u32 subchunk2Size;      /* == NumSamples * NumChannels * BitsPerSample / 8 (This is the number of bytes in the data) */
    };
    #pragma pack(pop)

    // #pragma pack(push, 1)
    // struct OGGheader
    // {

    // };
    // #pragma pack(pop)

    // #pragma pack(push, 1)
    // struct MP3header
    // {

    // };
    // #pragma pack(pop)
}