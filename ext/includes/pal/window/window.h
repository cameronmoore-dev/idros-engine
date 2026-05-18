#pragma once

#if defined(_WIN32)
    #include "win32/win32_window.h"
    #define PLATFORM_WINDOW idrs::Win32_Window;
    using PlatformWindow = idrs::Win32_Window;
#endif
#if defined (__linux__)
    #include "linux/x11_window.h"
    #define PLATFORM_WINDOW idrs::X11_Window;
    using PlatformWindow = idrs::X11_Window;
#endif

#include <chrono>
#include <thread>

#include "event.h"
#include "typedefs.h"

namespace idrs
{
    enum GLContextProfile
    {
        Core          = 0x1,
        Compatibility = 0x2
    };

    enum Style
    {
        Titlebar,
        Resizable,
        Maximised,
        Maximisable,
        Minimisable,
        Default
    };
    
    enum CursorFlags
    {
        Hidden  = 0x1,
        Locked  = 0x2,
        Clamped = 0x4
    };

    enum Hints
    {
        ColourBits = 0,
        DepthBits,
        StencilBits,
        Samples,
        sRGBCapable,
        DoubleBuffer,
        ContextVerMajor,
        ContextVerMinor,
        ContextProfile,
        HintsCount
    };

    class Window
    {
        friend class PLATFORM_WINDOW;

    public:
        Window();
        ~Window();

        bool pollEvents(Event &event);
        void setHint(Hints index, u8 value);
        void cursorEnableFlags(const u8 flags);
        void cursorDisableFlags(const u8 flags);
        void getScreenSize(s32 &outWidth, s32 &outHeight);
        void close();
        const bool isOpen();

        bool create(const std::string &title, uint16_t width, uint16_t height, uint32_t style);
        void swapBuffers();
        void swapInterval(u8 interval);
        void setFramerate(const u64 framerate);
        void setTitle(const std::string &title);
        void setFullscreen(bool fullscreen);
        void getPos(s32 &outX, s32 &outY);
        void getSize(s32 &outWidth, s32 &outHeight);
        void setPos(s32 x, s32 y);
        void setSize(s32 width, s32 height);
        void setFlash(u32 count, unsigned long timeout, bool sound);
        const bool isFocused();
        const Style nativeStyle(const Style style);
    
    private:
        PlatformWindow m_impl;
        
        std::queue<Event> m_events;
        std::array<Hints, HintsCount> m_hints;
        std::string m_title;
        std::chrono::high_resolution_clock::time_point m_frameStart;
        u64 m_targetFramerate;
        u32 m_style;
        u16 m_width;
        u16 m_height;
        u8 m_cursorState;
        bool m_isOpen = false;
    
    private:
        void glSetup();
        void registerInputDevices();
        void updateCursor();
        void sleep();
    };
}