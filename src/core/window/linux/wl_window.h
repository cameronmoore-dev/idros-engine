#pragma once

#include <string>

#include "core/typedefs.h"

namespace idrs
{
    class Window;
    class WL_Window
    {
    public:
        WL_Window();

        bool create(Window *wnd, const std::string &title, u32 width, u32 height, u32 style);
        void cleanup();
        void sleep(const uint64_t duration);

        void swapBuffers();
        void swapInterval(uint8_t interval);
        void setFramerate(uint64_t framerate);
        void setTitle(const std::string &title);
        void setFullscreen(bool fullscreen);
        void setPos(int32_t x, int32_t y);
        void setSize(uint32_t width, uint32_t height);
        void setFlash(uint32_t count, unsigned long timeout, bool sound);
        void getCursorPos(int32_t &outX, int32_t &outY, bool relative = true);
        void getPos(int32_t &outX, int32_t &outY);
        void getSize(uint32_t &outWidth, uint32_t &outHeight);
        const bool isFocused();
        const uint32_t nativeStyle(const uint32_t style);

    private:
        Window *m_wnd = nullptr;

    private:
        void pollMessages();
    };
}
