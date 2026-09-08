#pragma once

#if defined(_WIN32)
    #include "win32/win32_window.h"
    #define PLATFORM_WINDOW idrs::Win32_Window;
    using PlatformWindow = idrs::Win32_Window;
#elif defined (__linux__)
    #include "linux/x11_window.h"
    #define PLATFORM_WINDOW idrs::X11_Window;
    using PlatformWindow = idrs::X11_Window;
#endif

#include <array>
#include <chrono>
#include <queue>

#include "event.h"

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
        Window(const std::string &title, uint16_t width, uint16_t height, uint32_t style);
        ~Window();

        bool pollEvents(Event &event);
        void setHint(Hints index, uint8_t value);
        void cursorEnableFlags(const uint8_t flags);
        void cursorDisableFlags(const uint8_t flags);
        void getScreenSize(uint32_t &outWidth, uint32_t &outHeight);
        void close();
        const bool isOpen();

        bool create(const std::string &title, uint16_t width, uint16_t height, uint32_t style);
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
        const Style nativeStyle(const Style style);

    private:
        PlatformWindow m_platform;

        std::queue<Event> m_events;
        std::array<Hints, HintsCount> m_hints;
        std::string m_title;
        std::chrono::high_resolution_clock::time_point m_frameStart;
        uint64_t m_targetFramerate;
        uint32_t m_style;
        uint16_t m_width;
        uint16_t m_height;
        uint8_t m_cursorState;
        bool m_isOpen = false;

    private:
        void glSetup();
        void registerInputDevices();
        void updateCursor();
        void sleep();
    };
}
