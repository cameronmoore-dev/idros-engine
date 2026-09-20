#pragma once

#include <libevdev-1.0/libevdev/libevdev.h>

#include <queue>

#include "core/typedefs.h"

namespace idrs
{
    struct InputEventContext;

    class Input;
    struct Event;
    struct Gamepad;
    class Linux_Input
    {
    public:
        Linux_Input(Input *input);
        ~Linux_Input();

        u16 getKey(u32 key);
        u16 getMouse(u32 btn);

        void storeGamepads();
        void pollGamepads(u8 *hidData, std::queue<Event> &events);
        void pumpGamepadEvents(std::queue<Event> &events);

    private:
        Input *m_input;
        input_event m_inputBuffer[16];

    private:
        u16 keyToXKB(u32 key);
        u16 mouseToXKB(u32 btn);
        void rawToGamepad(Gamepad &gamepad, u8 *hidData);
    };
}