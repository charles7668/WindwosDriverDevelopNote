#include <ntddk.h>
#include <wdf.h>
#include "Driver.h"
#include "Device.h"

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT driverObject,_In_ PUNICODE_STRING registerPath)
{
	WDF_DRIVER_CONFIG config;
	WDFDRIVER driver;
	WDF_DRIVER_CONFIG_INIT(&config, EvtDeviceAdd);
	config.EvtDriverUnload = EvtDriverUnload;
	const NTSTATUS status = WdfDriverCreate(driverObject, registerPath, WDF_NO_OBJECT_ATTRIBUTES, &config, &driver);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("WdfDriverCreate failed with status 0x%x\n", status));
	}
	return status;
}

void EvtDriverUnload(WDFDRIVER driver)
{
	UNREFERENCED_PARAMETER(driver);
	PAGED_CODE()
	KdPrint(("Driver Unloaded"));
}
