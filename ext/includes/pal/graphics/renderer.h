#pragma once

#include "vertexarray.h"
#include "blendmode.h"
#include "typedefs.h"

namespace idrs
{
    class Renderer
    {
    public:
        enum class Features
        {
            Blend = 0,
            CullFace,
            DepthTest,
            StencilTest,
            ScissorTest,
            sRGB_Framebuffer,
            Multisample,
            SeamlessCubeMap,
            Dither
        };

    public:
        Renderer();

        void setFeatureActive(const Features feature, const bool enable);

        void clear(const u32 colour);
        void clear(const Vec4f &colour);
        void clear(const f32 r, const f32 g, const f32 b, const f32 a);
        void draw(const VertexArray &vertices);
        void drawIndex(const VertexArray &vertices);

        /* setBlendMode */

    private:
        BlendMode m_blendMode;

    private:
        const u32 gpuFeature(const Features feature) const;
    };
}