#pragma once

#include <string>
#include <cstdio>

#include <idlai/graphics/renderer.h>
#include <idlai/graphics/vertexarray.h>
#include <idlai/graphics/font.h>
#include <idlai/graphics/text.h>
#include <idlai/graphics/shader.h>

#if defined(MSC_VER)
    #define BREAK __debugbreak
    #define FILE  __FILE__
#elif defined(__GNUC__)
    #define BREAK __builtin_trap() /* NOTE: causes an illegal instruction, since gcc has no debug breakpoint intrinsic */
    #define FILE  __FILE_NAME__
#elif defined(__clang__)
    #define BREAK __builtin_debugtrap
    #define FILE  __FILE__
#endif

namespace idrs
{
#ifndef _NDEBUG
    #define LOG(mes, ...) \
    { \
        std::fprintf(stderr, "[LOG]: " mes " -- %s:%d \n", ##__VA_ARGS__, FILE, __LINE__); \
    }

    #define ASSERT(cond, mes, ...) \
    { \
        if (!(cond)) \
        { \
            std::fprintf(stderr, "[ASSERT FAILED]: " mes " -- %s:%d \n", ##__VA_ARGS__, FILE, __LINE__); \
            BREAK; \
        } \
    }

    #define EARLY_OUT() {}

#else
    #define LOG(mes, ...)
    #define ASSERT(cond, mes, ...)
    #define EARLY_OUT() { return; }
#endif

    class Debug
    {
    public:
        static void initDebugText(Font &font, const Vec2f &textPos);
        static void drawText(const char *text, ...);
        static void drawLine(f32 x1, f32 y1, f32 x2, f32 y2);
        
        static void draw(Renderer &renderer, Shader &textShader, Shader &lineShader);

    private:
        VertexArray m_drawables;
        std::string m_accumText;
        Text m_text;
        
    private:
        static void clear();

    private:
        static Debug &get();
        Debug();
    };
}
