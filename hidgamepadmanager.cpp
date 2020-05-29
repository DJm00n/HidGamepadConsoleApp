#include "pch.h"

#include "hidgamepadmanager.h"

using namespace winrt;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Devices::HumanInterfaceDevice;

HidGamepadManager::HidGamepadManager()
    : m_HidDeviceWatcher(nullptr)
{
    m_HidDeviceSelector = HidDevice::GetDeviceSelector(0x01 /*HID_USAGE_PAGE_GENERIC*/, 0x05 /*HID_USAGE_GENERIC_GAMEPAD*/);
    m_HidDeviceWatcher = DeviceInformation::CreateWatcher(m_HidDeviceSelector);
    m_HidAddedToken = m_HidDeviceWatcher.Added(std::bind(&HidGamepadManager::OnDeviceAdded, this, std::placeholders::_2));
    m_HidRemovedToken = m_HidDeviceWatcher.Removed(std::bind(&HidGamepadManager::OnDeviceRemoved, this, std::placeholders::_2));
    m_HidDeviceWatcher.Start();
}

HidGamepadManager::~HidGamepadManager()
{
    m_HidDeviceWatcher.Added(m_HidAddedToken);
    m_HidDeviceWatcher.Removed(m_HidRemovedToken);
}

void HidGamepadManager::Update()
{
    std::lock_guard<std::mutex> lock(m_Mutex);

    for (auto& pad : m_Pads)
    {
        pad->Update();
    }
}

void HidGamepadManager::OnDeviceAdded(const winrt::Windows::Devices::Enumeration::DeviceInformation& args)
{
    std::lock_guard<std::mutex> lock(m_Mutex);

    auto it = find_if(begin(m_Pads), end(m_Pads), [&](const std::unique_ptr<HidGamepad>& pad) { return pad->GetDeviceInformation().Id() == args.Id(); });

    if (it != end(m_Pads))
        return;

    m_Pads.emplace_back(std::make_unique<HidGamepad>(args));
}
void HidGamepadManager::OnDeviceRemoved(const winrt::Windows::Devices::Enumeration::DeviceInformationUpdate& args)
{
    std::lock_guard<std::mutex> lock(m_Mutex);

    auto it = find_if(begin(m_Pads), end(m_Pads), [&](const std::unique_ptr<HidGamepad>& pad) { return pad->GetDeviceInformation().Id() == args.Id(); });

    if (it == end(m_Pads))
        return;

    m_Pads.erase(it);
}
