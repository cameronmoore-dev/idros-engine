#include <windows.h>
#include <xinput.h>
#include <cstdio>
#include <algorithm>

#include "../input.h"

namespace strd
{
    const bool isKeyPressed(Key key);
    const uint16_t getKey(Key key);
    const uint16_t strdToVK(Key key);

    void storeGamepads();
    void pollGamepads(RAWHID &hidData, std::queue<Event> &events);

    void xboxToStrd(Gamepad &gamepad, XINPUT_STATE &state);
    void dualshockToStrd(Gamepad &gamepad, BYTE *rawData);
}