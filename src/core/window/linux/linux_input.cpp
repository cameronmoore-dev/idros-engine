#include "linux_input.h"
#include "core/window/input.h"

namespace idrs
{
    Linux_Input::Linux_Input(Input *input) : 
        m_input(input)
    {
        storeGamepads();
    }

    u16 Linux_Input::getKey(u32 key)
    {
        return keyToXKB(key);
    }

    u16 Linux_Input::getMouse(u32 btn)
    {
        return mouseToXKB(btn);
    }

    void Linux_Input::storeGamepads() {}
    void Linux_Input::pollGamepads(u8 *hidData, std::queue<Event> &events) {}

    bool isGamepadConnected(u32 slot) {return false;}
    bool isGamepadButtonPressed(u32 slot, GamepadButtons button) {return false;}

    u16 Linux_Input::mouseToXKB(u32 btn)
    {
        switch ((Mouse)btn)
        {
            case Mouse::LeftButton:     return 1;
            case Mouse::MiddleButton:   return 2;
            case Mouse::RightButton:    return 3;
            case Mouse::ThumbButton0:   return 8;
            case Mouse::ThumbButton1:   return 9;
            default: return 0;
        }
    }

    u16 Linux_Input::keyToXKB(u32 key)
    {
        switch ((Key)key)
        {
            case Key::A:          return XKB_KEY_A;
            case Key::B:          return XKB_KEY_B;
            case Key::C:          return XKB_KEY_C;
            case Key::D:          return XKB_KEY_D;
            case Key::E:          return XKB_KEY_E;
            case Key::F:          return XKB_KEY_F;
            case Key::G:          return XKB_KEY_G;
            case Key::H:          return XKB_KEY_H;
            case Key::I:          return XKB_KEY_I;
            case Key::J:          return XKB_KEY_J;
            case Key::K:          return XKB_KEY_K;
            case Key::L:          return XKB_KEY_L;
            case Key::M:          return XKB_KEY_M;
            case Key::N:          return XKB_KEY_N;
            case Key::O:          return XKB_KEY_O;
            case Key::P:          return XKB_KEY_P;
            case Key::Q:          return XKB_KEY_Q;
            case Key::R:          return XKB_KEY_R;
            case Key::S:          return XKB_KEY_S;
            case Key::T:          return XKB_KEY_T;
            case Key::U:          return XKB_KEY_U;
            case Key::V:          return XKB_KEY_V;
            case Key::W:          return XKB_KEY_W;
            case Key::X:          return XKB_KEY_X;
            case Key::Y:          return XKB_KEY_Y;
            case Key::Z:          return XKB_KEY_Z;
            case Key::Num0:       return XKB_KEY_0;
            case Key::Num1:       return XKB_KEY_1;
            case Key::Num2:       return XKB_KEY_2;
            case Key::Num3:       return XKB_KEY_3;
            case Key::Num4:       return XKB_KEY_4;
            case Key::Num5:       return XKB_KEY_5;
            case Key::Num6:       return XKB_KEY_6;
            case Key::Num7:       return XKB_KEY_7;
            case Key::Num8:       return XKB_KEY_8;
            case Key::Num9:       return XKB_KEY_9;
            case Key::Escape:     return XKB_KEY_Escape;
            case Key::LControl:   return XKB_KEY_Control_L;
            case Key::LShift:     return XKB_KEY_Shift_L;
            case Key::LAlt:       return XKB_KEY_Alt_L;
            case Key::LSystem:    return XKB_KEY_Super_L;
            case Key::RControl:   return XKB_KEY_Control_R;
            case Key::RShift:     return XKB_KEY_Shift_R;
            case Key::RAlt:       return XKB_KEY_Alt_R;
            case Key::RSystem:    return XKB_KEY_Super_R;
            case Key::Menu:       return XKB_KEY_Menu;
            case Key::LBracket:   return XKB_KEY_bracketleft;
            case Key::RBracket:   return XKB_KEY_bracketright;
            case Key::Semicolon:  return XKB_KEY_semicolon;
            case Key::Comma:      return XKB_KEY_comma;
            case Key::Period:     return XKB_KEY_period;
            case Key::Apostrophe: return XKB_KEY_apostrophe;
            case Key::Slash:      return XKB_KEY_slash;
            case Key::Backslash:  return XKB_KEY_backslash;
            case Key::Grave:      return XKB_KEY_grave;
            case Key::Equal:      return XKB_KEY_equal;
            case Key::Hyphen:     return XKB_KEY_minus;
            case Key::Space:      return XKB_KEY_space;
            case Key::Enter:      return XKB_KEY_Return;
            case Key::Backspace:  return XKB_KEY_backslash;
            case Key::Tab:        return XKB_KEY_Tab;
            case Key::PageUp:     return XKB_KEY_Page_Up;
            case Key::PageDown:   return XKB_KEY_Page_Down;
            case Key::End:        return XKB_KEY_End;
            case Key::Home:       return XKB_KEY_Home;
            case Key::Insert:     return XKB_KEY_Insert;
            case Key::Delete:     return XKB_KEY_Delete;
            case Key::Add:        return XKB_KEY_KP_Add;
            case Key::Subtract:   return XKB_KEY_KP_Subtract;
            case Key::Multiply:   return XKB_KEY_KP_Multiply;
            case Key::Divide:     return XKB_KEY_KP_Divide;
            case Key::Left:       return XKB_KEY_Left;
            case Key::Right:      return XKB_KEY_Right;
            case Key::Up:         return XKB_KEY_Up;
            case Key::Down:       return XKB_KEY_Down;
            case Key::Numpad0:    return XKB_KEY_KP_0;
            case Key::Numpad1:    return XKB_KEY_KP_1;
            case Key::Numpad2:    return XKB_KEY_KP_2;
            case Key::Numpad3:    return XKB_KEY_KP_3;
            case Key::Numpad4:    return XKB_KEY_KP_4;
            case Key::Numpad5:    return XKB_KEY_KP_5;
            case Key::Numpad6:    return XKB_KEY_KP_6;
            case Key::Numpad7:    return XKB_KEY_KP_7;
            case Key::Numpad8:    return XKB_KEY_KP_8;
            case Key::Numpad9:    return XKB_KEY_KP_9;
            case Key::F1:         return XKB_KEY_F1;
            case Key::F2:         return XKB_KEY_F2;
            case Key::F3:         return XKB_KEY_F3;
            case Key::F4:         return XKB_KEY_F4;
            case Key::F5:         return XKB_KEY_F5;
            case Key::F6:         return XKB_KEY_F6;
            case Key::F7:         return XKB_KEY_F7;
            case Key::F8:         return XKB_KEY_F8;
            case Key::F9:         return XKB_KEY_F9;
            case Key::F10:        return XKB_KEY_F10;
            case Key::F11:        return XKB_KEY_F11;
            case Key::F12:        return XKB_KEY_F12;
            case Key::F13:        return XKB_KEY_F13;
            case Key::F14:        return XKB_KEY_F14;
            case Key::F15:        return XKB_KEY_F15;
            case Key::Pause:      return XKB_KEY_Pause;
            default:              return 0;
        }
    }
}