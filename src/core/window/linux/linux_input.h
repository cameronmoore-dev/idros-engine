#pragma once

#include <xkbcommon/xkbcommon.h>

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

        u16 getKey(u32 key);
        u16 getMouse(u32 btn);

        void storeGamepads();
        void pollGamepads(u8 *hidData, std::queue<Event> &events);

    private:
        Input *m_input;

    private:
        u16 keyToXKB(u32 key);
        u16 mouseToXKB(u32 btn);
    };
}