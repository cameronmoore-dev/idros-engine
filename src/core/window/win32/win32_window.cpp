#include "../window.h"
#include "wgl.h"

#include <timeapi.h>

namespace idrs
{
    Win32_Window::Win32_Window()
    {
        timeBeginPeriod(1);
    }

    bool Win32_Window::create(Window *wnd, const std::string &title, uint16_t width, uint16_t height, uint32_t style)
    {
        m_wnd = wnd;

        registerWinClass();

        std::wstring wtitle(title.begin(), title.end());
        m_window = ::CreateWindowEx(
            0, 
            className(), 
            wtitle.c_str(), 
            style,
            CW_USEDEFAULT, CW_USEDEFAULT, 
            width, height, 
            0, 
            0, 
            GetModuleHandle(nullptr), 
            this
        );

        if (!m_window)
        {
            return false;
        }

        glSetup();

        ::GetWindowRect(m_window, &m_rect);
        ::ShowWindow(m_window, SW_NORMAL);
        ::SetFocus(m_window);

        registerInputDevices();
        storeGamepads();

        return true;
    }

    void Win32_Window::swapBuffers()
    {
        updateCursor();

        ::SwapBuffers(m_deviceContext);
        pollMessages();
    }

    void Win32_Window::swapInterval(uint8_t interval)
    {
        wglSwapIntervalEXT(interval);
    }

    void Win32_Window::cleanup()
    {
        ::DestroyWindow(m_window);
        timeEndPeriod(1);
    }

    void Win32_Window::sleep(uint64_t duration)
    {
        ::Sleep((DWORD)duration);
    }

    void Win32_Window::setTitle(const std::string &title)
    {
        std::wstring wtitle(title.begin(), title.end());
        ::SetWindowTextW(m_window, wtitle.c_str());
    }

    void Win32_Window::setFullscreen(bool fullscreen)
    {
        if (fullscreen)
        {
            HMONITOR hMonitor = ::MonitorFromWindow(m_window, MONITOR_DEFAULTTONEAREST);
            MONITORINFOEX info;
            info.cbSize = sizeof(MONITORINFOEX);
            DWORD fsStyle = WS_POPUP | WS_VISIBLE;

            ::GetWindowRect(m_window, &m_windowedRect);

            ::GetMonitorInfo(hMonitor, &info);
            ::SetWindowLongPtr(m_window, GWL_STYLE, fsStyle);
            ::SetWindowPos( m_window,
                            NULL, 
                            info.rcMonitor.left, 
                            info.rcMonitor.top, 
                            info.rcMonitor.right, 
                            info.rcMonitor.bottom, 
                            SWP_FRAMECHANGED);
            return;
        }

        ::SetWindowLongPtr(m_window, GWL_STYLE, m_wnd->m_style | WS_VISIBLE);
        ::SetWindowPos( m_window, 
                        NULL, 
                        m_windowedRect.left, 
                        m_windowedRect.top, 
                        m_windowedRect.right - m_windowedRect.left, 
                        m_windowedRect.bottom - m_windowedRect.top, 
                        SWP_FRAMECHANGED);
    }

    void Win32_Window::setPos(int32_t x, int32_t y)
    {
        ::SetWindowPos( m_window, 
                        NULL, 
                        x, y, 
                        m_rect.right - m_rect.left, 
                        m_rect.bottom - m_rect.top, 
                        SWP_FRAMECHANGED);
    }

    void Win32_Window::setSize(uint32_t width, uint32_t height)
    {
        ::SetWindowPos( m_window,
                        NULL, 
                        m_rect.left, 
                        m_rect.top, 
                        m_rect.left + width, 
                        m_rect.top + height, 
                        SWP_FRAMECHANGED);
    }

    void Win32_Window::setFlash(UINT count, DWORD timeout, bool sound)
    {
        FLASHWINFO info;
        info.cbSize = sizeof(FLASHWINFO);
        info.hwnd = m_window;
        info.dwFlags = FLASHW_ALL;
        info.uCount = count;
        info.dwTimeout = timeout;

        ::FlashWindowEx(&info);
        if (sound)
        {
            ::MessageBeep(MB_OK);
        }
    }

    void Win32_Window::getCursorPos(int32_t &outX, int32_t &outY, bool relative)
    {
        POINT p;
        ::GetCursorPos(&p);
        
        if (relative)
        {
            ::ScreenToClient(m_window, &p);
        }
        outX = p.x;

         
        /* NOTE: Invert the cursor's Y position because 
         *       Win32 has the origin in the top-left,
         *       while OpenGL is the bottom-left
         */
        outY = m_wnd->m_height - p.y;
    }

    void Win32_Window::getPos(int32_t &outX, int32_t &outY)
    {
        ::GetWindowRect(m_window, &m_rect);
        outX = m_rect.left;
        outY = m_rect.top;
    }

    void Win32_Window::getSize(uint32_t &outWidth, uint32_t & outHeight)
    {
        ::GetWindowRect(m_window, &m_rect);
        outWidth = m_rect.right - m_rect.left;
        outHeight = m_rect.bottom - m_rect.top;
    }

    bool Win32_Window::isFocused()
    {
        return ::GetFocus() == m_window;
    }

    const uint32_t Win32_Window::nativeStyle(const uint32_t style)
    {
        switch (style)
        {
            case Style::Titlebar:
            {
                /* WS_CAPTION */
                return 0x00C00000L;
            } break;

            case Style::Resizable:
            {
                /* WS_SIZEBOX */
                return 0x00040000L;
            } break;

            case Style::Maximised:
            {
                /* WS_MAXIMIZE */
                return 0x01000000L;
            } break;

            case Style::Maximisable:
            {
                /* WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX */
                return (0x00C00000L | 0x00080000L | 0x00010000L); 
            } break;

            case Style::Minimisable:
            {
                /* WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX */
                return (0x00C00000L | 0x00080000L | 0x00020000L); 
            } break;

            case Style::Default:
            {
                /* WS_OVERLAPPEDWINDOW */
                return (0x00000000L | 0x00C00000L | 0x00080000L | 0x00040000L | 0x00010000L | 0x00020000L);
            } break;

            default: return 0x0; break;
        }
    }

    void Win32_Window::glSetup()
    {
        /* Make dummy context so GLAD can load OpenGL extensions for the ability to create a modern context */
        HDC dummyDC;
        dummyDC = GetDC(m_window);

        PIXELFORMATDESCRIPTOR pfd;
        pfd.nSize = sizeof(pfd);
        
        ::SetPixelFormat(dummyDC, ChoosePixelFormat(dummyDC, &pfd), &pfd);

        HGLRC dummyRC;
        dummyRC = wglCreateContext(dummyDC);
        ::wglMakeCurrent(dummyDC, dummyRC);

        /* Load OpenGL extensions */
        gladLoadWGL(dummyDC, (GLADloadfunc)wglGetProcAddress);

        /* Destroy/unassign everything related to the dummy context  */
        ::wglMakeCurrent(NULL, NULL);
        ::wglDeleteContext(dummyRC);
        ::ReleaseDC(m_window, dummyDC);
        ::DestroyWindow(m_window);

        /* Create the finished window and make the modern context */
        std::wstring wtitle(m_wnd->m_title.begin(), m_wnd->m_title.end());
        m_window = ::CreateWindowEx(
            0, 
            className(), 
            wtitle.c_str(), 
            nativeStyle(m_wnd->m_style),
            CW_USEDEFAULT, CW_USEDEFAULT, 
            m_wnd->m_width, m_wnd->m_height, 
            0, 
            0, 
            GetModuleHandle(nullptr), 
            this
        );
        m_deviceContext = ::CreateCompatibleDC(0);
        m_deviceContext = ::GetDC(m_window);

        UINT numFormats = 0;
        int32_t formats = 0;
        int32_t formatAttribs[] =
        {
            WGL_DRAW_TO_WINDOW_ARB,     TRUE,
            WGL_SUPPORT_OPENGL_ARB,     TRUE,
            WGL_DOUBLE_BUFFER_ARB,      m_wnd->m_hints[DoubleBuffer],
            WGL_SAMPLES_ARB,            m_wnd->m_hints[Samples],
            WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
            WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB,         m_wnd->m_hints[ColourBits],
            WGL_DEPTH_BITS_ARB,         m_wnd->m_hints[DepthBits],
            WGL_STENCIL_BITS_ARB,       m_wnd->m_hints[StencilBits],
            0
        };

        wglChoosePixelFormatARB(m_deviceContext, formatAttribs, 0, 1, &formats, &numFormats);

        PIXELFORMATDESCRIPTOR desc;
        desc.nSize = sizeof(desc);
        
        ::SetPixelFormat(m_deviceContext, formats, &desc);

        s32 attribList[] =
        { 
            WGL_CONTEXT_MAJOR_VERSION_ARB, m_wnd->m_hints[ContextVerMajor],
            WGL_CONTEXT_MINOR_VERSION_ARB, m_wnd->m_hints[ContextVerMinor],
            WGL_CONTEXT_PROFILE_MASK_ARB,  m_wnd->m_hints[ContextProfile],
            0
        };
        HGLRC hRenderContext = wglCreateContextAttribsARB(m_deviceContext, 0, attribList);
        
        ::wglMakeCurrent(m_deviceContext, hRenderContext);
    }

    void Win32_Window::registerInputDevices()
    {
        RAWINPUTDEVICE rid[3];

        /* Mouse */
        rid[0].usUsagePage = 0x01;
        rid[0].usUsage = 0x02;
        rid[0].dwFlags = 0;
        rid[0].hwndTarget = m_window;

        /* Keyboard */
        rid[1].usUsagePage = 0x01;
        rid[1].usUsage = 0x06;
        rid[1].dwFlags = 0;
        rid[1].hwndTarget = m_window;

        /* Generic Gamepad */
        rid[2].usUsagePage = 0x01;
        rid[2].usUsage = 0x05;
        rid[2].dwFlags = 0;
        rid[2].hwndTarget = m_window;

        ::RegisterRawInputDevices(rid, 3, sizeof(RAWINPUTDEVICE));
    }

    void Win32_Window::updateCursor()
    {
        if (m_wnd->m_cursorState & CursorFlags::Locked)
        {
            ::SetCursorPos(m_rect.left + (m_wnd->m_width / 2), m_rect.bottom - (m_wnd->m_height / 2));
        }

        if (m_wnd->m_cursorState & CursorFlags::Clamped)
        {
            ::ClipCursor(&m_rect);
        }

        ::ShowCursor(!(m_wnd->m_cursorState & CursorFlags::Hidden));
    }

    LRESULT CALLBACK Win32_Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Win32_Window* wnd = nullptr;
        if (uMsg == WM_CREATE)
        {
            CREATESTRUCT* create = (CREATESTRUCT*)lParam;
            wnd = (Win32_Window*)create->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)wnd);
            wnd->m_window = hwnd;
        }
        else
        {
            wnd = (Win32_Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (wnd)
        {
            return wnd->handleMessages(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    void Win32_Window::pollMessages()
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    LRESULT Win32_Window::handleMessages(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch(uMsg)
        {
            case WM_DEVICECHANGE:
            {
                if (wParam == 0x0007)
                {
                    storeGamepads();
                }
            } break;

            case WM_CLOSE:
            {
                m_wnd->close();
            } break;

            case WM_SIZE:
            {
                m_wnd->m_width  = LOWORD(lParam);
                m_wnd->m_height = HIWORD(lParam);

                Event e;
                e.type = Event::WindowResize;
                e.wndResizeInfo.width = LOWORD(lParam);
                e.wndResizeInfo.height = HIWORD(lParam);

                m_wnd->m_events.push(e);
            } break;

            case WM_INPUT:
            {
                UINT size = 0;
                ::GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));

                m_inputBuffer.resize(size);
                if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, m_inputBuffer.data(), &size, sizeof(RAWINPUTHEADER)) != size)
                {
                    break;
                }

                RAWINPUT* raw = (RAWINPUT*)m_inputBuffer.data();
                if (raw->header.dwType == RIM_TYPEHID)
                {
                    pollGamepads(raw->data.hid, m_wnd->m_events);
                }

                if (raw->header.dwType == RIM_TYPEKEYBOARD)
                {
                    Event e;
                    e.keyCode = raw->data.keyboard.VKey;
                    
                    if (raw->data.keyboard.Message == WM_KEYDOWN)
                    {
                        e.type = Event::KeyPressed;
                    }
                    else if (raw->data.keyboard.Message == WM_KEYUP)
                    {
                        e.type = Event::KeyReleased;
                    }

                    m_wnd->m_events.push(e);
                }

                if (raw->header.dwType == RIM_TYPEMOUSE)
                {
                    Event e;

                    if (raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
                    {
                        e.type = Event::MouseMove;
                        e.mousePos.x = raw->data.mouse.lLastX;
                        e.mousePos.y = raw->data.mouse.lLastY;
                    }

                    switch (raw->data.mouse.usButtonFlags)
                    {
                        case RI_MOUSE_LEFT_BUTTON_DOWN:
                        {
                            e.type = Event::MousePressed;
                            e.mouseButton = 1;
                        } break;

                        case RI_MOUSE_RIGHT_BUTTON_DOWN:
                        {
                            e.type = Event::MousePressed;
                            e.mouseButton = 2;
                        } break;

                        case RI_MOUSE_LEFT_BUTTON_UP:
                        {
                            e.type = Event::MouseReleased;
                            e.mouseButton = 1;
                        } break;

                        case RI_MOUSE_RIGHT_BUTTON_UP:
                        {
                            e.type = Event::MouseReleased;
                            e.mouseButton = 2;
                        } break;

                        case RI_MOUSE_WHEEL:
                        {
                            e.type = Event::MouseScroll;
                            e.scrollDelta = raw->data.mouse.usButtonData / WHEEL_DELTA;
                        } break;
                    }

                    m_wnd->m_events.push(e);
                }
            } break;

            default:
                return ::DefWindowProc(m_window, uMsg, wParam, lParam);
        }

        return TRUE;
    }

    void Win32_Window::registerWinClass()
    {
        WNDCLASS wc = {};
        wc.lpfnWndProc = WndProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = className();

        ::RegisterClass(&wc);
    }
}