#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>

// {08D54C91-A205-4080-AA1F-CDE89AFE92F9}
DEFINE_GUID(GUID_INTERFACE,
            0x8d54c91, 0xa205, 0x4080, 0xaa, 0x1f, 0xcd, 0xe8, 0x9a, 0xfe, 0x92, 0xf9);
NTSTATUS EvtDriverDeviceAdd(WDFDRIVER driver, PWDFDEVICE_INIT deviceInit);
void EvtIoDeviceControl(WDFQUEUE queue, WDFREQUEST request, size_t outputBufferLength, size_t inputBufferLength,
                        ULONG ioControlCode);
// IOCTL code
#define IOCTL_ECHO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

/**
 * \brief this is the entry point of the driver
 * \param driverObject this is a pointer to the driver object
 * \param registryPath  this is a pointer to the registry path
 * \return  STATUS_SUCCESS if success
 */
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT driverObject, _In_ PUNICODE_STRING registryPath)
{
	NTSTATUS status = STATUS_SUCCESS;
	WDFDRIVER driver;
	WDF_DRIVER_CONFIG config;
	WDF_DRIVER_CONFIG_INIT(&config, EvtDriverDeviceAdd);
	status = WdfDriverCreate(driverObject, registryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, &driver);
	if (NT_SUCCESS(status))
	{
		KdPrint(("DriverEntry: WdfDriverCreate success\n"));
	}
	else
	{
		KdPrint(("DriverEntry: WdfDriverCreate failed with status 0x%X\n", status));
	}
	return status;
}

/**
 * \brief this is the callback function for the device add event
 * \param driver driver
 * \param deviceInit  a pointer to a framework-allocated WDFDEVICE_INIT structure
 * \return 
 */
NTSTATUS EvtDriverDeviceAdd(WDFDRIVER driver, PWDFDEVICE_INIT deviceInit)
{
	UNREFERENCED_PARAMETER(driver);
	NTSTATUS status = STATUS_SUCCESS;
	WDFDEVICE device;
	status = WdfDeviceCreate(&deviceInit, WDF_NO_OBJECT_ATTRIBUTES, &device);
	if (NT_SUCCESS(status))
	{
		KdPrint(("EvtDriverDeviceAdd: WdfDeviceCreate success\n"));
	}
	else
	{
		KdPrint(("EvtDriverDeviceAdd: WdfDeviceCreate failed with status 0x%X\n", status));
		goto End;
	}
	WDF_IO_QUEUE_CONFIG queueConfig;
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig, WdfIoQueueDispatchParallel);
	queueConfig.EvtIoDeviceControl = EvtIoDeviceControl;
	WDFQUEUE queue;
	status = WdfIoQueueCreate(device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &queue);
	if (NT_SUCCESS(status))
	{
		KdPrint(("EvtDriverDeviceAdd: WdfIoQueueCreate success\n"));
	}
	else
	{
		KdPrint(("EvtDriverDeviceAdd: WdfIoQueueCreate failed with status 0x%X\n", status));
		goto End;
	}
	status = WdfDeviceCreateDeviceInterface(device, &GUID_INTERFACE, NULL);
	if (NT_SUCCESS(status))
	{
		KdPrint(("EvtDriverDeviceAdd: WdfDeviceCreateDeviceInterface success\n"));
	}
	else
	{
		KdPrint(("EvtDriverDeviceAdd: WdfDeviceCreateDeviceInterface failed with status 0x%X\n", status));
	}
End:
	return status;
}

void EvtIoDeviceControl(WDFQUEUE queue, WDFREQUEST request, size_t outputBufferLength, size_t inputBufferLength,
                        ULONG ioControlCode)
{
	UNREFERENCED_PARAMETER(queue);
	UNREFERENCED_PARAMETER(inputBufferLength);
	UNREFERENCED_PARAMETER(outputBufferLength);
	UNREFERENCED_PARAMETER(ioControlCode);
	KdPrint(("Get IO Request"));
	// WdfRequestComplete(request, STATUS_SUCCESS);
	PVOID inBuffer = NULL;
	PVOID outBuffer = NULL;
	size_t inLength = 0;
	size_t outLength = 0;
	NTSTATUS status = STATUS_SUCCESS;
	// just process the IOCTL_ECHO request
	switch (ioControlCode)
	{
	case IOCTL_ECHO:
		KdPrint(("Get IOCTL_ECHO request\n"));
	// first check output buffer is equal to input buffer
		if (inputBufferLength <= 0 || outputBufferLength != inputBufferLength)
		{
			KdPrint(("EvtIOQueueDeviceControl: outputBufferLength != inputBufferLength\n"));
			WdfRequestComplete(request, STATUS_INVALID_BUFFER_SIZE);
			break;
		}
		KdPrint(
			("EvtIOQueueDeviceControl: inBufferLength = %d , outBufferLength = %d\n" , inputBufferLength ,
				outputBufferLength));
		status = WdfRequestRetrieveInputBuffer(request, inputBufferLength, &inBuffer, &inLength);
		if (NT_SUCCESS(status))
		{
			KdPrint(("EvtIOQueueDeviceControl: WdfRequestRetrieveInputBuffer success\n"));
		}
		else
		{
			KdPrint(("EvtIOQueueDeviceControl: WdfRequestRetrieveInputBuffer failed with status 0x%X\n", status));
			WdfRequestComplete(request, status);
			break;
		}
		status = WdfRequestRetrieveOutputBuffer(request, outputBufferLength, &outBuffer, &outLength);
		if (NT_SUCCESS(status))
		{
			KdPrint(("EvtIOQueueDeviceControl: WdfRequestRetrieveOutputBuffer success\n"));
		}
		else
		{
			KdPrint(("EvtIOQueueDeviceControl: WdfRequestRetrieveOutputBuffer failed with status 0x%X\n", status));
			WdfRequestComplete(request, status);
			break;
		}
		KdPrint(("EvtIOQueueDeviceControl: inBuffer = %s , inLength = %d", (CHAR*)inBuffer, inLength));
		memcpy(outBuffer, inBuffer, inLength);
		KdPrint(("EvtIOQueueDeviceControl: outBuffer = %s , outBufferLength = %d\n" , (CHAR*)outBuffer , outLength));
		WdfRequestCompleteWithInformation(request,STATUS_SUCCESS, outLength);
		break;
	default:
		KdPrint(("EvtIOQueueDeviceControl: invalid IOCTL code\n"));
		WdfRequestComplete(request, STATUS_FWP_INVALID_PARAMETER);
		break;
	}
}
