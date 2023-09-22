#include <array>
#include <iostream>
#include <Windows.h>
#include <initguid.h>
#include <SetupAPI.h>
#include <vector>

DEFINE_GUID(GUID_INTERFACE,
            0x8d54c91, 0xa205, 0x4080, 0xaa, 0x1f, 0xcd, 0xe8, 0x9a, 0xfe, 0x92, 0xf9);
#define IOCTL_ECHO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

LPTSTR GetDevicePath()
{
	const auto deviceInfo = SetupDiGetClassDevs(&GUID_INTERFACE, nullptr, nullptr,
	                                            DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	if (deviceInfo == INVALID_HANDLE_VALUE)
	{
		std::cout << "SetupDiGetClassDevs failed with error " << GetLastError() << std::endl;
		return nullptr;
	}
	SP_DEVICE_INTERFACE_DATA deviceInterface{};
	deviceInterface.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	auto ok = SetupDiEnumInterfaceDevice(deviceInfo, nullptr, &GUID_INTERFACE, 0, &deviceInterface);
	if (!ok)
	{
		std::cout << "SetupDiEnumInterfaceDevice failed with error " << GetLastError() << std::endl;
		return nullptr;
	}
	// allocate enough memory for the device interface detail data
	auto* deviceInterfaceDetailData = static_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(malloc(1024));
	RtlZeroMemory(deviceInterfaceDetailData, 1024);
	deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
	DWORD dwSize = 0;
	ok = SetupDiGetInterfaceDeviceDetail(deviceInfo, &deviceInterface, deviceInterfaceDetailData, 1024, &dwSize,
	                                     nullptr);
	if (!ok)
	{
		std::cout << "SetupDiGetInterfaceDeviceDetail failed with error " << GetLastError() << std::endl;
		return nullptr;
	}
	std::cout << "Device path: " << deviceInterfaceDetailData->DevicePath << std::endl;
	return deviceInterfaceDetailData->DevicePath;
}

int main()
{
	const auto devicePath = GetDevicePath();
	if (devicePath == nullptr)
	{
		return -1;
	}
	printf("%ls\n", devicePath);
	const auto hHandle = CreateFile(L"\\\\?\\ROOT#UNKNOWN#0000#{08d54c91-a205-4080-aa1f-cde89afe92f9}",
	                                GENERIC_READ | GENERIC_READ | GENERIC_EXECUTE,
	                                FILE_SHARE_WRITE | FILE_SHARE_READ, nullptr,
	                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << "CreateFile failed with error " << GetLastError() << std::endl;
		return -1;
	}
	std::cout << "Handle : " << hHandle << std::endl;
	std::cout << "IOCTL : " << std::hex << IOCTL_ECHO << std::dec << std::endl;
	DWORD dwBytesReturned = 0;
	std::array<char, 1024> inBuffer{};
	std::array<char, 1024> outBuffer{};
	std::cout << "send zero buffer ioctl to test";
	auto ok = DeviceIoControl(hHandle, IOCTL_ECHO, nullptr, 0, nullptr, 0, &dwBytesReturned, nullptr);
	if (!ok)
	{
		std::cout << "DeviceIoControl failed with error " << GetLastError() << std::endl;
	}
	std::cout << "enter your input : " << std::endl;
	std::string ch;
	int i = 0;
	std::cin >> ch;
	memcpy(inBuffer.data(), ch.data(), ch.size());
	std::cout << "input : " << inBuffer.data() << std::endl;
	std::cin.get();
	ok = DeviceIoControl(hHandle, IOCTL_ECHO, inBuffer.data(), 1024, outBuffer.data(), 1024, &dwBytesReturned, nullptr);
	if (!ok)
	{
		std::cout << "DeviceIoControl failed with error " << GetLastError() << std::endl;
	}
	else
	{
		std::cout << "Returned bytes : " << dwBytesReturned << std::endl;
		std::cout << "output : " << outBuffer.data() << std::endl;
	}
	CloseHandle(hHandle);
	std::cin.get();
}
