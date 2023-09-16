#include <ntddk.h>
#include <wdf.h>

// define driver unload callback
void EvtDriverUnload(_In_ WDFDRIVER);
void EvtFileCreateCallback(_In_ WDFDEVICE, _In_ WDFREQUEST, _In_ WDFFILEOBJECT);

/*
 * DriverEntry
 * this is the first function called after the driver is loaded
 * @pDriverObject: a pointer to the driver object created by the system
 * @pRegistryPath: a pointer to the driver registry path
 * return: STATUS_SUCCESS if successful, STATUS_UNSUCCESSFUL otherwise
 */
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT pDriverObject,_In_ PUNICODE_STRING pRegistryPath)
{
	// write a debug message
	KdPrint(("DriverEntry called\n"));
	// driver related variables
	WDF_DRIVER_CONFIG config;
	WDFDRIVER driver;
	// device object related variables
	WDFDEVICE device;
	WDF_FILEOBJECT_CONFIG fileObjectConfig;
	// device name and symbolic link
	DECLARE_CONST_UNICODE_STRING(deviceName, L"\\Device\\DeviceObjectExample");
	DECLARE_CONST_UNICODE_STRING(symbolicLink, L"\\??\\DeviceObjectExample");

	// init driver config
	WDF_DRIVER_CONFIG_INIT(&config, WDF_NO_EVENT_CALLBACK);
	// set driver init flags to non-pnp driver
	config.DriverInitFlags = WdfDriverInitNonPnpDriver;
	// set driver unload callback
	config.EvtDriverUnload = EvtDriverUnload;

	NTSTATUS status = STATUS_SUCCESS;
	// create driver
	status = WdfDriverCreate(pDriverObject, pRegistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, &driver);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("WdfDriverCreate failed with status 0x%08X\n", status));
		goto End;
	}
	else
	{
		KdPrint(("WdfDriverCreate success\n"));
	}

	// allocate a device init object , and set SDDL(Security Descriptor Definition Language) string
	PWDFDEVICE_INIT pDeviceInit = WdfControlDeviceInitAllocate(driver, &SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_RWX_RES_RWX);
	if (pDeviceInit == NULL)
	{
		KdPrint(("WdfControlDeviceInitAllocate failed\n"));
		goto End;
	}
	// assign device name to device init object
	status = WdfDeviceInitAssignName(pDeviceInit, &deviceName);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("WdfDeviceInitAssignName failed with status 0x%08X\n", status));
		goto End;
	}
	// init file object config , and set call back functions, for now we just set Create callback
	WDF_FILEOBJECT_CONFIG_INIT(&fileObjectConfig, EvtFileCreateCallback, NULL, NULL);

	// set file object config to device init object
	WdfDeviceInitSetFileObjectConfig(pDeviceInit, &fileObjectConfig, WDF_NO_OBJECT_ATTRIBUTES);

	// create device
	status = WdfDeviceCreate(&pDeviceInit, WDF_NO_OBJECT_ATTRIBUTES, &device);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("WdfDeviceCreate failed with status 0x%08X\n", status));
		goto End;
	}

	// create symbolic link
	status = WdfDeviceCreateSymbolicLink(device, &symbolicLink);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("WdfDeviceCreateSymbolicLink failed with status 0x%08X\n", status));
		goto End;
	}
	// finish initializing device
	WdfControlFinishInitializing(device);

End:
	// return success
	return status;
}

/*
 * EvtDriverUnload
 * this is the driver unload callback
 * @driver: driver object
 */
VOID EvtDriverUnload(_In_ WDFDRIVER driver)
{
	UNREFERENCED_PARAMETER(driver);
	KdPrint(("EvtDriverUnload called\n"));
}

/*
 * EvtFileCreateCallback
 * this is the file create callback
 * @device: device object
 * @request: request object
 * @fileObject: file object
 */
VOID EvtFileCreateCallback(_In_ WDFDEVICE device, _In_ WDFREQUEST request, _In_ WDFFILEOBJECT fileObject)
{
	KdPrint(("EvtFileCreateCallback called\n"));
	UNREFERENCED_PARAMETER(device);
	UNREFERENCED_PARAMETER(fileObject);
	// complete the request
	WdfRequestComplete(request, STATUS_SUCCESS);
}
