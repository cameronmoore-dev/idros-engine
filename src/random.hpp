#pragma once

#include "core/typedefs.h"

namespace idrs
{
    class Random
    {
    public:
        Random();

        const u32 generateSeed();
        const s32 range(s32 min, s32 max, u32 *seed = nullptr);
        const f32 range(f32 min, f32 max, u32 *seed = nullptr);
        const u32 xorshift32(u32 &seed);
        const f32 xorshift32f(u32 &seed);

    private:
        u32 m_seed;
    };
}
