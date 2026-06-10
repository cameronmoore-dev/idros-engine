#pragma once

#include "vertexarray.h"
#include "blendmode.h"
#include "typedefs.h"

namespace idrs
{
    class Renderer
    {
    public:
        enum GPUFeatures
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

        void setFeatureActive(GPUFeatures feature, bool enable);

        void clear(const Colour colour);
        void clear(const Vec4f &colour);
        void clear(f32 r, f32 g, f32 b, f32 a);
        void draw(VertexArray &vertices);
        void drawIndex(VertexArray &vertices);

        /* setBlendMode */

    private:
        BlendMode m_blendMode;

    private:
        u32 gpuFeature(GPUFeatures feature);
    };
}
