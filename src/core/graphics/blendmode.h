#pragma once

#include "core/typedefs.h"

namespace idrs
{
    class BlendMode
    {
    public:
        enum class Factor
        {
            Zero = 0,
            One,
            SrcColour,
            DstColour,
            SrcAlpha,
            DstAlpha,
            OneMinusSrcColour,
            OneMinusDstColour,
            OneMinusSrcAlpha,
            OneMinusDstAlpha
        };

        enum class Equation
        {
            Add = 0,
            Subtract,
            ReverseSubtract,
            Min,
            Max
        };

    public:
        BlendMode(const Factor srcf, const Factor dstf, const Equation equation);

        void setSrcFactor(const Factor src);
        void setDstFactor(const Factor dst);
        void setEquation(const Equation equation);

        const u32 getSrcFactor() const;
        const u32 getDstFactor() const;
        const u32 getEquation() const;
        
    private:
        Factor m_srcf;
        Factor m_dstf;
        Equation m_eqtn;
    };
}