#include "input.h"

#include <cmath>

namespace idrs
{
    Input::Input() :
        m_platform(this),
        m_inputmap{}
    {
    }

    void Input::setInputState(u32 inputCode, bool pressed)
    {
        if (inputCode > UINT16_MAX)
        {
            return;
        }
        m_inputmap[inputCode] = pressed;
    }

    bool Input::isKeyPressed(Key key)
    {
        return m_inputmap[getKey(key)];
    }

    bool Input::isMousePressed(Mouse btn)
    {
        return m_inputmap[getMouse(btn)];
    }

    u16 Input::getKey(Key key)
    {
        return m_platform.getKey((u32)key);
    }

    u16 Input::getMouse(Mouse btn)
    {
        return m_platform.getMouse((u32)btn);
    }

    bool Input::isGamepadConnected(u32 slot)
    {
        return slot < m_gamepads.size();
    }

    bool Input::isGamepadButtonPressed(u32 slot, GamepadButtons button)
    {
        if (slot >= m_gamepads.size())
        {
            return false;
        }

        return ((m_gamepads[slot].buttons >> button) & 0x1);
    }

    void Input::storeGamepads()
    {
        m_platform.storeGamepads();
    }

    void Input::pollGamepads(u8 *hidData, std::queue<Event> &events)
    {
        m_platform.pollGamepads(hidData, events);
    }

    f32 Input::controllerAxisValue(u32 slot, GamepadAxis axis)
    {
        if (slot >= m_gamepads.size())
        {
            return -1.f;
        }

        Gamepad &gamepad = m_gamepads[slot];
        f32 axisValue = (f32)gamepad.stickAxes[(int)axis];
        f32 whole = (f32)gamepad.maxAxisValue;
        f32 axisPercent = std::floor(axisValue / whole * 100.f);

        if (std::abs(axisPercent) > GamepadDeadzone::JoystickPercent)
        {
            /* Normalise value between -1.0 and 1.0 */
            return (axisValue / whole);
        }

        return 0.f;
    }

    void Input::compareGamepadStates(Gamepad &current, Gamepad &previous, std::queue<Event> &events)
    {
        if (current.buttons == previous.buttons)
        {
            goto compareAxes;
        }

        for (u8 i = 0; i < GamepadButtons::ButtonCount; i++)
        {
            if (((current.buttons >> i) ^ (previous.buttons >> i)) == 1)
            {
                Event e;
                e.type = ((current.buttons >> i) & 0x1) 
                    ? Event::Type::GamepadButtonPressed 
                    : Event::Type::GamepadButtonReleased;

                e.gamepadButton = i;
                events.push(e);
            }
        }

    compareAxes:
        for (u8 i = 0; i < GamepadAxis::AxesCount; i++)
        {
            if (current.stickAxes[i] == previous.stickAxes[i])
            {
                continue;
            }

            float axisPercent = (float)current.stickAxes[i] / (float)current.maxAxisValue * 100.f;
            if (std::abs(axisPercent) > GamepadDeadzone::JoystickPercent)
            {
                Event e;
                e.type = Event::Type::GamepadAxisMoved;
                e.axisInfo.axis = i;
                e.axisInfo.value = current.stickAxes[i];

                events.push(e);
            }
        }

        if (current.triggers == previous.triggers)
        {
            return;
        }

        Event e;
        if (((current.triggers >> 8) & 0xFF) != ((previous.triggers >> 8) & 0xFF))
        {
            e.type = Event::Type::GamepadTrigger;
            e.triggerInfo.trigger = 0;
            e.triggerInfo.value = ((current.triggers >> 8) & 0xFF);
        }

        if ((current.triggers & 0xFF) != (previous.triggers & 0xFF))
        {
            e.type = Event::Type::GamepadTrigger;
            e.triggerInfo.trigger = 1;
            e.triggerInfo.value = (current.triggers & 0xFF);
        }

        events.push(e);
    }
}
