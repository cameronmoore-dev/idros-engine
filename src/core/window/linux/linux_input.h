#pragma once

#include <queue>

#include "core/typedefs.h"

namespace idrs
{
    class Input;
    struct Event;
    struct Gamepad;
    class Linux_Input
    {
    public:
        Linux_Input(Input *input);

        bool isKeyPressed(u32 key);
        bool isMousePressed(u32 btn);
        u16 getKey(u32 key);
        u16 getMouse(u32 btn);

        void storeGamepads();
        void pollGamepads(u8 *hidData, std::queue<Event> &events);

        // bool isGamepadConnected(u32 slot);
        // bool isGamepadButtonPressed(u32 slot, GamepadButtons button);

    private:
        Input *m_input;
    };
}