#include "../blendmode.h"

#include <glad/gl.h>

namespace idrs
{
    BlendMode::BlendMode(const Factor srcf, const Factor dstf, const Equation equation) :
        m_srcf(srcf),
        m_dstf(dstf),
        m_eqtn(equation)
    {
        glBlendFunc(getSrcFactor(), getDstFactor());
        glBlendEquation(getEquation());
    }

    void BlendMode::setSrcFactor(const Factor srcf)
    {
        m_srcf = srcf;
    }

    void BlendMode::setDstFactor(const Factor dstf)
    {
        m_dstf = dstf;
    }

    void BlendMode::setEquation(const Equation equation)
    {
        m_eqtn = equation;
    }

    const u32 BlendMode::getSrcFactor() const
    {
        switch (m_srcf)
        {
            case Factor::Zero: return GL_ZERO; break;
            case Factor::One: return GL_ONE; break;
            case Factor::SrcColour: return GL_SRC_COLOR; break;
            case Factor::DstColour: return GL_DST_COLOR; break;
            case Factor::SrcAlpha: return GL_SRC_ALPHA; break;
            case Factor::DstAlpha: return GL_DST_ALPHA; break;
            case Factor::OneMinusSrcColour: return GL_ONE_MINUS_SRC_COLOR; break;
            case Factor::OneMinusDstColour: return GL_ONE_MINUS_DST_COLOR; break;
            case Factor::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA; break;
            case Factor::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA; break;
        }
        return -1;
    }

    const u32 BlendMode::getDstFactor() const
    {
        switch (m_dstf)
        {
            case Factor::Zero: return GL_ZERO; break;
            case Factor::One: return GL_ONE; break;
            case Factor::SrcColour: return GL_SRC_COLOR; break;
            case Factor::DstColour: return GL_DST_COLOR; break;
            case Factor::SrcAlpha: return GL_SRC_ALPHA; break;
            case Factor::DstAlpha: return GL_DST_ALPHA; break;
            case Factor::OneMinusSrcColour: return GL_ONE_MINUS_SRC_COLOR; break;
            case Factor::OneMinusDstColour: return GL_ONE_MINUS_DST_COLOR; break;
            case Factor::OneMinusSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA; break;
            case Factor::OneMinusDstAlpha: return GL_ONE_MINUS_DST_ALPHA; break;
        }
        return -1;
    }

    const u32 BlendMode::getEquation() const
    {
        switch (m_eqtn)
        {
            case Equation::Add: return GL_FUNC_ADD; break;
            case Equation::Subtract: return GL_FUNC_SUBTRACT; break;
            case Equation::ReverseSubtract: return GL_FUNC_REVERSE_SUBTRACT; break;
            case Equation::Min: return GL_MIN; break;
            case Equation::Max: return GL_MAX; break;
        }
        return -1;
    }
}