#include "pch.h"

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Devices.Enumeration.h>
#include <winrt/Windows.Devices.HumanInterfaceDevice.h>
#include <winrt/Windows.Storage.h>

#include <string>
#include <iostream>
#include <functional>

using namespace std;
using namespace winrt;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Devices::HumanInterfaceDevice;
using namespace Windows::Storage;

void OnDeviceAdded(const winrt::Windows::Devices::Enumeration::DeviceInformation& deviceInformation)
{
    std::cout << "*****Device Init End*******" << std::endl;

    try {
        string id = to_string(deviceInformation.Id());
        string name = to_string(deviceInformation.Name());

        std::cout << "New HID device connected:" << std::endl;
        std::cout << "Id: " << id << std::endl;
        std::cout << "Name: " << name << std::endl;

        HidDevice m_Device = HidDevice::FromIdAsync(deviceInformation.Id(), FileAccessMode::ReadWrite).get();

        if (!m_Device)
        {
            std::cout << "Cannot open device for writing! Trying read-only..." << std::endl;
            m_Device = HidDevice::FromIdAsync(deviceInformation.Id(), FileAccessMode::Read).get();
        }

        if (!m_Device)
        {
            std::cout << "Cannot open device for reading!" << std::endl;
            throw;
        }

        uint16_t vendorId = m_Device.VendorId();
        uint16_t productId = m_Device.ProductId();
        uint16_t version = m_Device.Version();
        uint16_t usagePage = m_Device.UsagePage();
        uint16_t usageId = m_Device.UsageId();

        std::cout << "VendorId: " << vendorId << std::endl;
        std::cout << "ProductId: " << productId << std::endl;
        std::cout << "Version: " << version << std::endl;
        std::cout << "UsagePage: " << usagePage << std::endl;
        std::cout << "UsageId: " << usageId << std::endl;
    }
    catch (...)
    {
        std::cout << "Something went wrong while device init! :(" << std::endl;
    }

    std::cout << "*****Device Init End*******" << std::endl;
}

winrt::hstring hidDeviceSelector;
winrt::Windows::Devices::Enumeration::DeviceWatcher hidDeviceWatcher = nullptr;
winrt::event_token hidAddedToken;

int main()
{
    winrt::init_apartment();

    hidDeviceSelector = HidDevice::GetDeviceSelector(0x01 /*HID_USAGE_PAGE_GENERIC*/, 0x05 /*HID_USAGE_GENERIC_GAMEPAD*/);
    hidDeviceWatcher = DeviceInformation::CreateWatcher(hidDeviceSelector);
    hidAddedToken = hidDeviceWatcher.Added(std::bind(&OnDeviceAdded, std::placeholders::_2));
    hidDeviceWatcher.Start();

    while (true)
    {
        // wait
    }

    hidDeviceWatcher.Added(hidAddedToken);

    winrt::uninit_apartment();

    return 0;
}
