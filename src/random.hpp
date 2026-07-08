#pragma once

#include <idcore/typedefs.h>

namespace idrs
{
    class Random
    {
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
    };
}
