#include "window.h"

#include <glad/gl.h>

#include <cstdio>

namespace idrs
{
    Window::Window() :
        m_style(0),
        m_width(0),
        m_height(0),
        m_cursorState(0),
        m_isOpen(false),
        m_targetFramerate(0),
        m_frameStart(std::chrono::high_resolution_clock::now())
    {
        /* Set default values to window hints */
        setHint(ColourBits, 32);
        setHint(DepthBits, 24);
        setHint(StencilBits, 8);
        setHint(Samples, 0);
        setHint(sRGBCapable, 0);
        setHint(DoubleBuffer, 1);
        setHint(ContextVerMajor, 4);
        setHint(ContextVerMinor, 1);
        setHint(ContextProfile, GLContextProfile::Core);
    }

    Window::Window(const std::string &title, uint16_t width, uint16_t height, uint32_t style) : 
        m_style(0),
        m_width(0),
        m_height(0),
        m_cursorState(0),
        m_isOpen(false),
        m_targetFramerate(0),
        m_frameStart(std::chrono::high_resolution_clock::now())
    {
        /* Set default values to window hints */
        setHint(ColourBits, 32);
        setHint(DepthBits, 24);
        setHint(StencilBits, 8);
        setHint(Samples, 0);
        setHint(sRGBCapable, 0);
        setHint(DoubleBuffer, 1);
        setHint(ContextVerMajor, 4);
        setHint(ContextVerMinor, 1);
        setHint(ContextProfile, GLContextProfile::Core);

        create(title, width, height, style);
    }

    Window::~Window()
    {
        m_platform.cleanup();
    }

    void Window::setHint(Hints index, uint8_t value)
    {
        m_hints[index] = (Hints)value;
    }

    bool Window::pollEvents(Event &event)
    {
        if (m_events.empty())
        {
            return false;
        }
        event = m_events.front();
        m_events.pop();
        return true;
    }

    void Window::cursorEnableFlags(const uint8_t state)
    {
        m_cursorState |= state;
    }

    void Window::cursorDisableFlags(const uint8_t state)
    {
        m_cursorState &= ~(state);
    }

    void Window::close()
    {
        m_isOpen = false;
    }

    const bool Window::isOpen()
    {
        return m_isOpen;
    }

    /* TODO: Change this to get the native screen size from the platform,
             as this is out of date if the window size changes
     */
    void Window::getScreenSize(uint32_t &outWidth, uint32_t &outHeight)
    {
        outWidth  = m_width;
        outHeight = m_height;
    }

    bool Window::create(const std::string &title, uint16_t width, uint16_t height, uint32_t style)
    {
        m_title = title;
        m_width = width;
        m_height = height;
        m_style = style;

        bool success = m_platform.create(this, title, width, height, style);
        if (!success)
        {
            printf("Failed to create platform window!");
            return false;
        }
        if (!gladLoaderLoadGL())
        {
            printf("Failed to load GLAD!\n");
            return false;
        }

        return m_isOpen = true;
    }

    void Window::sleep()
    {
        auto frameLength = std::chrono::duration<double>(1.0 / m_targetFramerate);
        auto nextFrameTime = m_frameStart + frameLength;
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::milliseconds sleepDuration = std::chrono::duration_cast<std::chrono::milliseconds>((nextFrameTime - now) - std::chrono::milliseconds(1));

        /* OS thread sleep */
        if (sleepDuration.count() > 0)
        {
            m_platform.sleep(sleepDuration.count());
        }

        /* Spin wait */
        // std::chrono::high_resolution_clock::time_point spinStart = std::chrono::high_resolution_clock::now();
        // std::chrono::microseconds minThreshold = std::chrono::microseconds(5);
        while (std::chrono::high_resolution_clock::now() < nextFrameTime)
        {
            /* Prevents an infinite while loop due to a potential negative number occuring */
            // if ((std::chrono::high_resolution_clock::now() - spinStart) > minThreshold)
            // {
            //     break;
            // }
        }

        m_frameStart = std::chrono::high_resolution_clock::now();
    }

    void Window::swapBuffers()
    {
        m_platform.swapBuffers();
        if (m_targetFramerate > 0)
        {
            sleep();
        }
    }

    void Window::swapInterval(uint8_t interval)
    {
        m_platform.swapInterval(interval);
    }

    void Window::setFramerate(uint64_t framerate)
    {
        m_targetFramerate = framerate;
    }

    void Window::setTitle(const std::string &title)
    {
        m_platform.setTitle(title);
    }

    void Window::setFullscreen(bool fullscreen)
    {
        m_platform.setFullscreen(fullscreen);
    }

    void Window::setPos(int32_t x, int32_t y)
    {
        m_platform.setPos(x, y);
    }

    void Window::setSize(uint32_t width, uint32_t height)
    {
        m_platform.setSize(width, height);
    }

    void Window::setFlash(uint32_t count, unsigned long timeout, bool sound)
    {
        m_platform.setFlash(count, timeout, sound);
    }

    void Window::getCursorPos(int32_t &outX, int32_t &outY, bool relative)
    {
        m_platform.getCursorPos(outX, outY, relative);
    }

    void Window::getPos(int32_t &outX, int32_t &outY)
    {
        m_platform.getPos(outX, outY);
    }

    void Window::getSize(uint32_t &outWidth, uint32_t &outHeight)
    {
        m_platform.getSize(outWidth, outHeight);
    }

    const bool Window::isFocused()
    {
        return m_platform.isFocused();
    }

    const Style Window::nativeStyle(const Style style)
    {
        return (Style)m_platform.nativeStyle(style);
    }
}
