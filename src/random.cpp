#include "random.hpp"

#include <random>

namespace idrs
{
    Random::Random() :
        m_seed(generateSeed())
    {
    }

    const u32 Random::generateSeed()
    {
        std::random_device rd;
        return rd();
    }

    const s32 Random::range(s32 min, s32 max, u32 *seed)
    {
        s32 result = seed != nullptr
            ? xorshift32(*seed)
            : xorshift32(m_seed);

        return (min + result % (max - min + 1));
    }

    const f32 Random::range(f32 min, f32 max, u32 *seed)
    {
        f32 result = seed != nullptr
            ? xorshift32f(*seed)
            : xorshift32f(m_seed);

        return (min + result * (max - min));
    }

    const u32 Random::xorshift32(u32 &seed)
    {
        u32 x = seed;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;

        return seed = x;
    }

    const f32 Random::xorshift32f(u32 &seed)
    {
        u32 x = seed;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        seed = x;

        return (f32)x / (f32)UINT32_MAX;
    }
}

