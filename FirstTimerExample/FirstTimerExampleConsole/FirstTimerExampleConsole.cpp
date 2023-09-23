#include <array>
#include <iostream>
#include <Windows.h>
#include <initguid.h>
#include <SetupAPI.h>
#include <limits>

// {08D54C91-A205-4080-AA1F-CDE89AFE92F9}
DEFINE_GUID(GUID_INTERFACE,
            0x8d54c91, 0xa205, 0x4080, 0xaa, 0x1f, 0xcd, 0xe8, 0x9a, 0xfe, 0x92, 0xf9);

#define IOCTL_ECHO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

LPTSTR GetDevicePath()
{
	const auto h_device = SetupDiGetClassDevs(&GUID_INTERFACE, nullptr, nullptr, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	if (h_device == INVALID_HANDLE_VALUE)
	{
		std::cout << "SetupDiGetClassDevs failed: " << GetLastError() << std::endl;
		return nullptr;
	}
	SP_DEVICE_INTERFACE_DATA device_interface{};
	device_interface.cbSize = sizeof(device_interface);
	auto ok = SetupDiEnumInterfaceDevice(h_device, nullptr, &GUID_INTERFACE, 0, &device_interface);
	if (!ok)
	{
		std::cout << "SetupDiEnumInterfaceDevice failed: " << GetLastError() << std::endl;
		return nullptr;
	}
	auto* device_interface_detail_data = static_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(malloc(1024));
	RtlZeroMemory(device_interface_detail_data, 1024);
	device_interface_detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
	DWORD required_size = 0;
	ok = SetupDiGetInterfaceDeviceDetail(h_device, &device_interface, device_interface_detail_data, 1024,
	                                     &required_size, nullptr);
	if (!ok)
	{
		std::cout << "SetupDiGetInterfaceDeviceDetail failed: " << GetLastError() << std::endl;
		return nullptr;
	}
	std::cout << "Device Path : " << device_interface_detail_data->DevicePath << std::endl;
	return device_interface_detail_data->DevicePath;
}

int main()
{
	const auto device_path = GetDevicePath();
	if (device_path == nullptr)
	{
		std::cout << "GetDevicePath failed" << std::endl;
		return -1;
	}
	const auto handle = CreateFile(device_path,GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE,
	                               FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
	                               nullptr);
	if (handle == INVALID_HANDLE_VALUE)
	{
		std::cout << "CreateFile failed: " << GetLastError() << std::endl;
		std::cin.get();
		return -1;
	}
	std::cout << std::hex << "Handle : " << handle << std::dec << std::endl;
	std::cout << std::hex << "IOCTL : " << IOCTL_ECHO << std::dec << std::endl;
	DWORD returned_bytes = 0;
	std::array<char, 1024> inBuffer{};
	std::array<char, 1024> outBuffer{};
	std::cout << "Send zero buffer length to test driver" << std::endl;
	auto ok = DeviceIoControl(handle, IOCTL_ECHO, inBuffer.data(), 0, outBuffer.data(), 0, &returned_bytes, nullptr);
	if (!ok)
	{
		std::cout << "DeviceIoControl failed: " << GetLastError() << std::endl;
	}
	std::string user_input;
	std::cout << "Enter a string to send to the driver: ";
	std::cin >> user_input;
	memcpy(inBuffer.data(), user_input.c_str(), user_input.size());
	std::cout << "Your input is : " << inBuffer.data() << std::endl;
	ok = DeviceIoControl(handle, IOCTL_ECHO, inBuffer.data(), 1024, outBuffer.data(), 1024, &returned_bytes, nullptr);
	if (!ok)
	{
		std::cout << "DeviceIoControl failed: " << GetLastError() << std::endl;
	}
	std::cout << "Driver returned : " << outBuffer.data() << std::endl;
	std::cin.clear();
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
	std::cin.get();
}
