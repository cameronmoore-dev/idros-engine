#include "debug.h"

#include <cstdarg>

namespace idrs
{
    Debug::Debug() :
        m_drawables(VertexArray::Primitive::Line, VertexArray::Usage::Dynamic, 0)
    {
        EARLY_OUT();

        m_drawables.generate();
        m_drawables.setVertexAttrib(0, 2, VertexArray::VertexAttribType::Float, false, offsetof(Vertex, position));
        m_drawables.update();
    }

    void Debug::initDebugText(Font &font, const Vec2f &textPos)
    {
        m_text.setFont(font);
        m_text.setPosition(textPos.x, textPos.y);
    }

    void Debug::drawText(const char *text, ...)
    {
        EARLY_OUT();

        va_list args;
        va_start(args, text);

        char buf[128];
        vsnprintf(buf, sizeof(buf), text, args);
        std::string t(buf);

        m_accumText += (t + "\n");

        va_end(args);
    }

    void Debug::drawLine(f32 x1, f32 y1, f32 x2, f32 y2)
    {
        EARLY_OUT();

        Vertex v1 = {};
        Vertex v2 = {};
        v1.position = { x1, y1 };
        v2.position = { x2, y2 };

        m_drawables.append(v1);
        m_drawables.append(v2);
    }

    void Debug::clear()
    {
        EARLY_OUT();

        m_drawables.clear();
        m_accumText.clear();
    }

    void Debug::draw(Renderer &renderer, Shader &textShader, Shader &lineShader)
    {
        EARLY_OUT();

        m_drawables.update();
        renderer.draw(m_drawables, &lineShader);
        textShader.use();
        m_text.setText(m_accumText);
        m_text.draw();

        clear();
    }
}
