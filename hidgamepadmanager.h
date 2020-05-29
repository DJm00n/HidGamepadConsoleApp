
#include "hidgamepad.h"

class HidGamepadManager
{
public:
    HidGamepadManager();
    ~HidGamepadManager();

    void Update();

    void OnDeviceAdded(const winrt::Windows::Devices::Enumeration::DeviceInformation& args);
    void OnDeviceRemoved(const winrt::Windows::Devices::Enumeration::DeviceInformationUpdate& args);

private:
    winrt::hstring m_HidDeviceSelector;
    winrt::Windows::Devices::Enumeration::DeviceWatcher m_HidDeviceWatcher;

    winrt::event_token m_HidAddedToken;
    winrt::event_token m_HidRemovedToken;

    std::mutex m_Mutex;
    std::vector<std::unique_ptr<HidGamepad>> m_Pads;
};
