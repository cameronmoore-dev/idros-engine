#include <windows.h>
#include <xinput.h>

#include "../input.h"

namespace idrs
{
    const bool isKeyPressed(Key key);
    const bool isMousePressed(Mouse btn);
    const u16 getKey(Key key);
    const u16 getMouse(Mouse btn);
    const u16 strdToVK(Key key);
    const u16 idrsToVK(Mouse btn);

    void storeGamepads();
    void pollGamepads(RAWHID &hidData, std::queue<Event> &events);

    void xboxToStrd(Gamepad &gamepad, XINPUT_STATE &state);
    void dualshockToStrd(Gamepad &gamepad, BYTE *rawData);
}
