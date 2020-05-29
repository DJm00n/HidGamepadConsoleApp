#include "pch.h"

#include "hidgamepadmanager.h"

int main()
{
    winrt::init_apartment();

    HidGamepadManager manager;

    while (true)
    {
        manager.Update();
    }

    winrt::uninit_apartment();

    return 0;
}
