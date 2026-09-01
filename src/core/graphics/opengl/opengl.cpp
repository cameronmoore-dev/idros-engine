#include "../renderer.h"
#include "../vertexarray.h"

#include <glad/gl.h>

namespace idrs
{
    Renderer::Renderer() : 
        m_blendMode(BlendMode::Factor::SrcAlpha, BlendMode::Factor::OneMinusSrcAlpha, BlendMode::Equation::Add)
    {
    }

    void Renderer::setFeatureActive(GPUFeatures feature, bool enable)
    {
        if (enable) glEnable(gpuFeature(feature));
        else        glDisable(gpuFeature(feature));
    }

    void Renderer::clear(const Colour colour)
    {
        Vec4f c = colour::normalizeColour(colour);

        glClearColor(c.x, c.y, c.z, c.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::clear(const Vec4f &colour)
    {
        glClearColor(colour.x, colour.y, colour.z, colour.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::clear(f32 r, f32 g, f32 b, f32 a)
    {
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::draw(VertexArray &vertices, Shader *shader, Texture *texture)
    {
        if (shader != nullptr)  shader->use();
        if (texture != nullptr) texture->bind();

        glBindVertexArray(vertices.id());
        glDrawArrays(vertices.primitive(), 0, vertices.count());
    }

    void Renderer::drawIndex(VertexArray &vertices, Shader *shader, Texture *texture)
    {
        if (shader != nullptr)  shader->use();
        if (texture != nullptr) texture->bind();

        glBindVertexArray(vertices.id());
        glDrawElements(vertices.primitive(), vertices.indices().size(), GL_UNSIGNED_INT, 0);
    }

    u32 Renderer::gpuFeature(GPUFeatures feature)
    {
        switch (feature)
        {
            case GPUFeatures::Blend:            return GL_BLEND; break;
            case GPUFeatures::CullFace:         return GL_CULL_FACE; break;
            case GPUFeatures::DepthTest:        return GL_DEPTH_TEST; break;
            case GPUFeatures::Dither:           return GL_DITHER; break;
            case GPUFeatures::Multisample:      return GL_MULTISAMPLE; break;
            case GPUFeatures::ScissorTest:      return GL_SCISSOR_TEST; break;
            case GPUFeatures::SeamlessCubeMap:  return GL_TEXTURE_CUBE_MAP_SEAMLESS; break;
            case GPUFeatures::sRGB_Framebuffer: return GL_FRAMEBUFFER_SRGB; break;
            case GPUFeatures::StencilTest:      return GL_STENCIL_TEST; break;
        }
        return -1;
    }
}
