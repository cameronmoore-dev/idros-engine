#pragma once

#include <vector>
#include <cstdint>

#include "event.h"
#include "core/typedefs.h"

#if defined (_WIN32)
    #include "core/window/win32/win32_input.h"
    #define PLATFORM_INPUT idrs::Win32_Input;
    using PlatformInput = idrs::Win32_Input;
#elif defined (__linux__)
#endif

/* https://www.psdevwiki.com/ps4/DS4-USB#Data_Format */
namespace idrs
{
    enum class Key
    {
        Invalid = -1, //!< Unhandled key
        A       = 0,  //!< The A key
        B,            //!< The B key
        C,            //!< The C key
        D,            //!< The D key
        E,            //!< The E key
        F,            //!< The F key
        G,            //!< The G key
        H,            //!< The H key
        I,            //!< The I key
        J,            //!< The J key
        K,            //!< The K key
        L,            //!< The L key
        M,            //!< The M key
        N,            //!< The N key
        O,            //!< The O key
        P,            //!< The P key
        Q,            //!< The Q key
        R,            //!< The R key
        S,            //!< The S key
        T,            //!< The T key
        U,            //!< The U key
        V,            //!< The V key
        W,            //!< The W key
        X,            //!< The X key
        Y,            //!< The Y key
        Z,            //!< The Z key
        Num0,         //!< The 0 key
        Num1,         //!< The 1 key
        Num2,         //!< The 2 key
        Num3,         //!< The 3 key
        Num4,         //!< The 4 key
        Num5,         //!< The 5 key
        Num6,         //!< The 6 key
        Num7,         //!< The 7 key
        Num8,         //!< The 8 key
        Num9,         //!< The 9 key
        Escape,       //!< The Escape key
        LControl,     //!< The left Control key
        LShift,       //!< The left Shift key
        LAlt,         //!< The left Alt key
        LSystem,      //!< The left OS specific key: window (Windows and Linux), apple (macOS), ...
        RControl,     //!< The right Control key
        RShift,       //!< The right Shift key
        RAlt,         //!< The right Alt key
        RSystem,      //!< The right OS specific key: window (Windows and Linux), apple (macOS), ...
        Menu,         //!< The Menu key
        LBracket,     //!< The [ key
        RBracket,     //!< The ] key
        Semicolon,    //!< The ; key
        Comma,        //!< The , key
        Period,       //!< The . key
        Apostrophe,   //!< The ' key
        Slash,        //!< The / key
        Backslash,    //!< The \ key
        Grave,        //!< The ` key
        Equal,        //!< The = key
        Hyphen,       //!< The - key (hyphen)
        Space,        //!< The Space key
        Enter,        //!< The Enter/Return keys
        Backspace,    //!< The Backspace key
        Tab,          //!< The Tabulation key
        PageUp,       //!< The Page up key
        PageDown,     //!< The Page down key
        End,          //!< The End key
        Home,         //!< The Home key
        Insert,       //!< The Insert key
        Delete,       //!< The Delete key
        Add,          //!< The + key
        Subtract,     //!< The - key (minus, usually from numpad)
        Multiply,     //!< The * key
        Divide,       //!< The / key
        Left,         //!< Left arrow
        Right,        //!< Right arrow
        Up,           //!< Up arrow
        Down,         //!< Down arrow
        Numpad0,      //!< The numpad 0 key
        Numpad1,      //!< The numpad 1 key
        Numpad2,      //!< The numpad 2 key
        Numpad3,      //!< The numpad 3 key
        Numpad4,      //!< The numpad 4 key
        Numpad5,      //!< The numpad 5 key
        Numpad6,      //!< The numpad 6 key
        Numpad7,      //!< The numpad 7 key
        Numpad8,      //!< The numpad 8 key
        Numpad9,      //!< The numpad 9 key
        F1,           //!< The F1 key
        F2,           //!< The F2 key
        F3,           //!< The F3 key
        F4,           //!< The F4 key
        F5,           //!< The F5 key
        F6,           //!< The F6 key
        F7,           //!< The F7 key
        F8,           //!< The F8 key
        F9,           //!< The F9 key
        F10,          //!< The F10 key
        F11,          //!< The F11 key
        F12,          //!< The F12 key
        F13,          //!< The F13 key
        F14,          //!< The F14 key
        F15,          //!< The F15 key
        Pause,        //!< The Pause key
    };

    enum class Mouse
    {
        LeftButton = 0,
        RightButton,
        MiddleButton,
        ThumbButton0,
        ThumbButton1
    };

    enum GamepadButtons
    {
        DpadUp = 0,
        DpadDown,
        DpadLeft,
        DpadRight,
        Start,
        Back,
        LeftStick,
        RightStick,
        LeftShoulder,
        RightShoulder,
        A = 12,
        B,
        X,
        Y,
        ButtonCount
    };

    enum GamepadAxis
    {
        LX = 0,
        LY,
        RX,
        RY,
        AxesCount,
    };

    enum GamepadVendorID
    {
        Xbox = 0x45E,
        Dualshock4 = 0X54C,
        SwitchPro = 0x57E
    };

    enum GamepadDeadzone
    {
        JoystickPercent = 20,
        Trigger = 30
    };

    struct Gamepad
    {
        u16 vendorId;
        u16 buttons;
        s16 stickAxes[4];
        u16 triggers;
        u16 maxAxisValue = INT16_MAX;
    };

    class Input
    {
    friend class PLATFORM_INPUT
    friend class Engine;
    public:
        Input();

        bool isKeyPressed(Key key);
        bool isMousePressed(Mouse btn);
        u16 getKey(Key key);
        u16 getMouse(Mouse btn);

        bool isGamepadConnected(u32 slot);
        bool isGamepadButtonPressed(u32 slot, GamepadButtons button);

    private:
        PlatformInput m_platform;
        std::vector<Gamepad> m_gamepads;

    private:
        f32 controllerAxisValue(u32 slot, GamepadAxis axis);
        void compareGamepadStates(Gamepad &current, Gamepad &previous, std::queue<Event> &events);
        void storeGamepads();
        void pollGamepads(u8 *hidData, std::queue<Event> &events);
    };
}
