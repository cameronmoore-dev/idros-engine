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

#include <cstdint>
#include <string>
// #include <array>
#include <vector>
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

        bool create(Window *wnd, const std::string &title, uint16_t width, uint16_t height, uint32_t style);
        void cleanup();
        void swapBuffers();
        void swapInterval(uint8_t interval);
        void setTitle(const std::string &title);
        void setFullscreen(bool fullscreen);
        void setPos(int32_t x, int32_t y);
        void setSize(uint32_t width, uint32_t height);
        void setFlash(UINT count, DWORD timeout, bool sound);
        void getCursorPos(int32_t &outX, int32_t &outY, bool relative);
        void getPos(int32_t &outX, int32_t &outY);
        void getSize(uint32_t &outWidth, uint32_t &outHeight);
        bool isFocused();
        const uint32_t nativeStyle(uint32_t style);

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