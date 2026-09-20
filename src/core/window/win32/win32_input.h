#include <windows.h>
#include <xinput.h>

#include <queue>

#include "core/typedefs.h"

namespace idrs
{
    class Input;
    struct Event;
    struct Gamepad;
    class Win32_Input
    {
    public:
        Win32_Input(Input *input);

        bool isKeyPressed(u32 key);
        bool isMousePressed(u32 btn);
        u16 getKey(u32 key);
        u16 getMouse(u32 btn);

        void storeGamepads();
        void pollGamepads(u8 *hidData, std::queue<Event> &events);
        void pumpGamepadEvents(std::queue<Event> &events) {}

    private:
        Input *m_input = nullptr;

    private:
        u16 mouseToVK(u32 btn);
        u16 keyToVK(u32 key);
        void xboxToIdrs(Gamepad &gamepad, XINPUT_STATE &state);
        void dualshockToIdrs(Gamepad &gamepad, BYTE *rawData);
    };
}
