#pragma once

#include <X11/Xlib.h>
#include <xcb/xcb.h>
#include <libevdev-1.0/libevdev/libevdev.h>

#include <string>

#include "core/typedefs.h"

struct RendererState;

namespace idrs
{
    class Window;
    class X11_Window
    {
    public:
        X11_Window();

        bool create(Window *wnd, const std::string &title, u32 width, u32 height, u32 style);
        void cleanup();
        void sleep(const uint64_t duration);
        void pollMessages();

        void swapBuffers();
        void swapInterval(uint8_t interval);
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
        RendererState *m_rndState;

        Display *m_display;
        xcb_connection_t *m_connection;
        xcb_window_t m_window;
        xcb_screen_t *m_screen;
        xcb_atom_t wm_deleteWindow;

        /* TODO: Probably just wrap all platform specific types 
                 into an opaque struct that gets defined in source file
         */
        input_event inputBuffer[16];
        libevdev *dev;
        s32 evdev_result;

    private:
        void eglSetup();
        xcb_atom_t xcbInternAtom(xcb_connection_t &conn, const std::string &atomName);
    };
}