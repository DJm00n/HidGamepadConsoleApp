class HidGamepad
{
public:
    HidGamepad(const winrt::Windows::Devices::Enumeration::DeviceInformation& deviceInformation);
    ~HidGamepad();

    // non-copyable
    HidGamepad(const HidGamepad&) = delete;
    HidGamepad& operator=(const HidGamepad&) = delete;

    // movable
    HidGamepad(HidGamepad&&) = default;
    HidGamepad& operator=(HidGamepad&&) = default;

    void Update();

    const winrt::Windows::Devices::Enumeration::DeviceInformation& GetDeviceInformation() const { return m_DeviceInformation; }

    bool IsError() const { return m_Error; }

private:
    winrt::Windows::Devices::Enumeration::DeviceInformation m_DeviceInformation;
    winrt::Windows::Devices::HumanInterfaceDevice::HidDevice m_Device;
    uint16_t m_VendorId;
    uint16_t m_ProductId;
    uint16_t m_Version;
    uint16_t m_UsagePage;
    uint16_t m_UsageId;
    std::string m_Id;
    std::string m_Name;
    bool m_Error;
};
