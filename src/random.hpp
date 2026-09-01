#pragma once

#include "core/typedefs.h"

namespace idrs
{
    class Random
    {
    friend class Engine;
    public:
        static const u32 generateSeed();
        static const s32 range(s32 min, s32 max, u32 *seed = nullptr);
        static const f32 range(f32 min, f32 max, u32 *seed = nullptr);
        static const u32 xorshift32(u32 &seed);
        static const f32 xorshift32f(u32 &seed);

    private:
        u32 m_seed;

    private:
        static Random &get();
        Random();

        static void init();
    };
}
