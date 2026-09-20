#pragma once

namespace idrs
{
    struct Event
    {
        enum class Type
        {
            Default = 0,
            KeyPressed,
            KeyReleased,
            MousePressed,
            MouseReleased,
            MouseMove,
            MouseScroll,
            GamepadButtonPressed,
            GamepadButtonReleased,
            GamepadAxisMoved,
            GamepadTrigger,

            WindowResize,
            DeviceChanged,

            _DeviceInput
        };

        Type type = Type::Default;

        union
        {
            int keyCode;
            int mouseButton;
            struct
            {
                int x;
                int y;
            } mousePos;

            int scrollDelta;

            int gamepadButton;
            struct
            {
                int axis;
                int value;
            } axisInfo;

            struct
            {
                int trigger;
                int value;
            } triggerInfo;

            struct
            {
                int width;
                int height;
            } wndResizeInfo;

            unsigned char *_deviceInputPlatformInternal;
        };
    };
}
