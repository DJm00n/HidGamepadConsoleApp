#include "pch.h"

#include "hidgamepad.h"

using namespace winrt;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Devices::HumanInterfaceDevice;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

HidGamepad::HidGamepad(const DeviceInformation& deviceInformation)
    : m_DeviceInformation(deviceInformation)
    , m_Device(nullptr)
    , m_VendorId(0)
    , m_ProductId(0)
    , m_Version(0)
    , m_UsagePage(0)
    , m_UsageId(0)
    , m_Error(false)
{
    std::cout << "*****Device Init Start*******" << std::endl;

    if (!m_DeviceInformation)
        return;

    try {
        m_Id = to_string(m_DeviceInformation.Id());
        m_Name = to_string(m_DeviceInformation.Name());

        std::cout << "New HID device connected:" << std::endl;
        std::cout << "Id: " << m_Id << std::endl;
        std::cout << "Name: " << m_Name << std::endl;

        m_Device = HidDevice::FromIdAsync(m_DeviceInformation.Id(), FileAccessMode::ReadWrite).get();

        if (!m_Device)
        {
            std::cout << "Cannot open device for writing! Trying read-only..." << std::endl;
            m_Device = HidDevice::FromIdAsync(m_DeviceInformation.Id(), FileAccessMode::Read).get();
        }

        if (!m_Device)
        {
            std::cout << "Cannot open device for reading!" << std::endl;
            throw;
        }

        m_VendorId = m_Device.VendorId();
        m_ProductId = m_Device.ProductId();
        m_Version = m_Device.Version();
        m_UsagePage = m_Device.UsagePage();
        m_UsageId = m_Device.UsageId();

        std::cout << "VendorId: " << m_VendorId << std::endl;
        std::cout << "ProductId: " << m_ProductId << std::endl;
        std::cout << "UsagePage: " << m_UsagePage << std::endl;
        std::cout << "UsageId: " << m_UsageId << std::endl;
    }
    catch (...)
    {
        std::cout << "Something went wrong while device init! :(" << std::endl;
    }

    std::cout << "*****Device Init End*******" << std::endl;
}

HidGamepad::~HidGamepad()
{
    std::cout << "*****Device Remove Start*******" << std::endl;
    std::cout << "Id: " << m_Id << std::endl;
    std::cout << "*****Device Remove End*******" << std::endl;
}

void HidGamepad::Update()
{
    if (m_Error)
        return;

    std::cout << "*****Device Update Start*******" << std::endl;
    std::cout << "Id: " << m_Id << std::endl;

    try {

        // Try send vibration

        // 0x05 for Stadia gamepad.
        // https://github.com/chromium/chromium/blob/master/device/gamepad/hid_haptic_gamepad.cc#L51
        // https://github.com/chromium/chromium/blob/master/device/gamepad/hid_haptic_gamepad.h#L43
        HidOutputReport outputReport = m_Device.CreateOutputReport(0x05);

        uint16_t vibration1 = 0xffff; // 0x0000 - 0xffff
        uint16_t vibration2 = 0xffff; // 0x0000 - 0xffff

        DataWriter writer = DataWriter();
        writer.WriteUInt16(vibration1);
        writer.WriteUInt16(vibration2);
        writer.WriteBuffer(outputReport.Data());

        std::cout << "Trying to send HID vibration report:" << std::endl;

        uint32_t bytesSent = m_Device.SendOutputReportAsync(outputReport).get();

        std::cout << bytesSent << " bytes sent." << std::endl;

    }
    catch (...)
    {
        std::cout <<"Error while sending." << std::endl;
        m_Error = true;
    }

    std::cout << "*****Device Update End*******" << std::endl;
}
