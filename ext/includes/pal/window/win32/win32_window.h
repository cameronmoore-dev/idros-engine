#pragma once

#ifndef UNICODE
#define UNICODE 1
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <string>
#include <array>
#include <vector>
#include <queue>
#include <cstdint>
#include <chrono>
#include <thread>

#include "win32_input.h"

namespace idrs
{
    class Window;
    class Win32_Window
    {
    public:
        Win32_Window();

        bool create(Window *wnd, const std::string &title, const u16 width, const u16 height, const u32 style);
        void cleanup();
        void swapBuffers();
        void swapInterval(u8 interval);
        void setTitle(const std::string &title);
        void setFullscreen(bool fullscreen);
        void getPos(s32 &outX, s32 &outY);
        void getSize(s32 &outWidth, s32 &outHeight);
        void setPos(s32 x, s32 y);
        void setSize(s32 width, s32 height);
        void setFlash(UINT count, DWORD timeout, bool sound);
        bool isFocused();
        const u32 nativeStyle(const u32 style);

        void glSetup();
        void registerInputDevices();
        void updateCursor();
        void sleep(const uint64_t duration);

    private:
        Window *m_wnd = nullptr;

        HWND m_window;
        HDC  m_deviceContext;
        RECT m_rect;
        RECT m_windowedRect;
        std::vector<BYTE> m_inputBuffer;
    
    private:
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        LRESULT handleMessages(UINT uMsg, WPARAM wParam, LPARAM lParam);
        LPCWSTR className() const { return L"WindowClass"; }
        void registerWinClass();
        void pollMessages();
    };
}