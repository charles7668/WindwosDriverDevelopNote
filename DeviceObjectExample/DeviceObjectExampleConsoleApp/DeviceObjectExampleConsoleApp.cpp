#include <iostream>
#include <Windows.h>

int main()
{
	// create device object file
	const HANDLE handle = CreateFile(TEXT("\\\\.\\DeviceObjectExample"),
	                                 GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE,
	                                 FILE_SHARE_READ | FILE_SHARE_WRITE,
	                                 nullptr,
	                                 OPEN_EXISTING,
	                                 FILE_ATTRIBUTE_NORMAL, nullptr);
	if (handle == INVALID_HANDLE_VALUE)
	{
		std::cout << "CreateFile failed with error code " << GetLastError() << std::endl;
		return -1;
	}
	std::cout << "CreateFile success" << std::endl;
	std::cin.get();
	return 0;
}
