#include "linux_input.h"
#include "core/window/input.h"

namespace idrs
{
    Linux_Input::Linux_Input(Input *input) : 
        m_input(input)
    {
    }

    bool Linux_Input::isKeyPressed(u32 key) {return false;}
    bool Linux_Input::isMousePressed(u32 btn) {return false;}
    u16 Linux_Input::getKey(u32 key) {return 0;}
    u16 Linux_Input::getMouse(u32 btn) {return 0;}

    void Linux_Input::storeGamepads() {}
    void Linux_Input::pollGamepads(u8 *hidData, std::queue<Event> &events) {}

    // bool isGamepadConnected(u32 slot) {return false;}
    // bool isGamepadButtonPressed(u32 slot, GamepadButtons button) {return false;}
}