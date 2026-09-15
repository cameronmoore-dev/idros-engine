#include "wl_window.h"
#include "core/window/window.h"

#include "egl.h"

#include <sys/mman.h>

#include <unistd.h>
#include <ctime>

namespace idrs
{
    WL_Window::WL_Window()
    {
    }

    bool WL_Window::create(Window *wnd, const std::string &title, u32 width, u32 height, u32 style)
    {
        m_wnd = wnd;
    }

    void WL_Window::cleanup()
    {
    }

    void WL_Window::pollMessages()
    {
    }

    void WL_Window::sleep(const uint64_t duration){}

    void WL_Window::swapBuffers(){}
    void WL_Window::swapInterval(uint8_t interval){}
    void WL_Window::setFramerate(uint64_t framerate){}
    void WL_Window::setTitle(const std::string &title){}
    void WL_Window::setFullscreen(bool fullscreen){}
    void WL_Window::setPos(int32_t x, int32_t y){}
    void WL_Window::setSize(uint32_t width, uint32_t height){}
    void WL_Window::setFlash(uint32_t count, unsigned long timeout, bool sound){}
    void WL_Window::getCursorPos(int32_t &outX, int32_t &outY, bool relative){}
    void WL_Window::getPos(int32_t &outX, int32_t &outY){}
    void WL_Window::getSize(uint32_t &outWidth, uint32_t &outHeight){}
    const bool WL_Window::isFocused(){return true;}
    const uint32_t WL_Window::nativeStyle(const uint32_t style){return 0;}
}
