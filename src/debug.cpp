#include "debug.h"

#include <cstdarg>

namespace idrs
{
    Debug::Debug() : 
        m_drawables(VertexArray::Primitive::Line, VertexArray::Usage::Static, 0)
    {
        EARLY_OUT();

        m_drawables.generate();
        m_drawables.setVertexAttrib(0, 2, VertexArray::VertexAttribType::Float, false, offsetof(Vertex, position));
    }

    void Debug::initDebugText(Font &font)
    {
        Debug &instance = get();
        
        instance.m_text.setFont(font);
        instance.m_text.setPosition(10.0f, 650.0f);
    }

    void Debug::drawText(const char *text, ...)
    {
        EARLY_OUT();

        va_list args;
        va_start(args, text);

        char buf[128];
        vsnprintf(buf, sizeof(buf), text, args);
        std::string t(buf);
        
        get().m_accumText += (t + "\n");
        
        va_end(args);
    }

    void Debug::drawLine(f32 x1, f32 y1, f32 x2, f32 y2)
    {
        EARLY_OUT();

        Debug &instance = get();

        Vertex v1 = {};
        Vertex v2 = {};
        v1.position = { x1, y1 };
        v2.position = { x2, y2 };

        instance.m_drawables.append(v1);
        instance.m_drawables.append(v2);
    }
    
    void Debug::clear()
    {
        EARLY_OUT();

        Debug &instance = get();

        instance.m_drawables.clear();
        instance.m_accumText.clear();
    }

    void Debug::draw(Renderer &renderer)
    {
        EARLY_OUT();

        // drawGeo(renderer);
        // drawText();

        clear();
    }

    void Debug::_drawGeo(Renderer &renderer)
    {
        Debug &instance = get();

        instance.m_drawables.update();
        renderer.draw(instance.m_drawables);
    }

    void Debug::_drawText()
    {
        Debug &instance = get();

        instance.m_text.setText(instance.m_accumText);
        instance.m_text.draw();

        // clear();
    }

    Debug &Debug::get()
    {
        static Debug instance;
        return instance;
    }
}