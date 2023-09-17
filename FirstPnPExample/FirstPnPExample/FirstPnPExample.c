#include <ntddk.h>
#include <wchar.h>
#include <initguid.h>
#include <wdf.h>

// {08D54C91-A205-4080-AA1F-CDE89AFE92F9}
DEFINE_GUID(GUID_INTERFACE,
            0x8d54c91, 0xa205, 0x4080, 0xaa, 0x1f, 0xcd, 0xe8, 0x9a, 0xfe, 0x92, 0xf9);

NTSTATUS
EvtDriverDeviceAdd(
	_In_
	WDFDRIVER driver,
	_Inout_
	PWDFDEVICE_INIT deviceInit
);

void EvtIoDeviceControl(WDFQUEUE queue, WDFREQUEST request, size_t outputBufferLength,
                        size_t inputBufferLength, ULONG ioControlCode);

/**
 * \brief this is the entry point of the driver
 * \param driverObject this is a pointer to the driver object
 * \param registryPath  this is a pointer to the registry path
 * \return  STATUS_SUCCESS if success
 */
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT driverObject, _In_ PUNICODE_STRING registryPath)
{
	NTSTATUS status = STATUS_SUCCESS;
	// init the driver config and set the device add event function
	WDF_DRIVER_CONFIG config;
	WDF_DRIVER_CONFIG_INIT(&config, EvtDriverDeviceAdd);
	WDFDRIVER driver;
	// create a driver
	status = WdfDriverCreate(driverObject, registryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, &driver);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("WdfDriverCreate failed\n"));
		goto End;
	}
	KdPrint(("WdfDriverCreate success\n"));
End:
	return status;
}

/**
 * \brief this is the callback function for the device add event
 * \param driver driver
 * \param deviceInit  a pointer to a framework-allocated WDFDEVICE_INIT structure
 * \return 
 */
NTSTATUS
EvtDriverDeviceAdd(
	_In_
	WDFDRIVER driver,
	_Inout_
	PWDFDEVICE_INIT deviceInit
)
{
	UNREFERENCED_PARAMETER(driver);
	WDFDEVICE device;
	// create a device
	NTSTATUS status = WdfDeviceCreate(&deviceInit, WDF_NO_OBJECT_ATTRIBUTES, &device);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("WdfDeviceCreate failed\n"));
		goto End;
	}
	// init the queue config , set the queue dispatch type to sequential
	WDF_IO_QUEUE_CONFIG queueConfig;
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig, WdfIoQueueDispatchSequential);
	// set io device control callback
	queueConfig.EvtIoDeviceControl = EvtIoDeviceControl;
	// create a queue
	WDFQUEUE queue;
	status = WdfIoQueueCreate(device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &queue);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("WdfIoQueueCreate failed\n"));
		goto End;
	}
	else
	{
		KdPrint(("WdfIoQueueCreate success\n"));
	}
	status = WdfDeviceCreateDeviceInterface(device, &GUID_INTERFACE, NULL);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("WdfDeviceCreateDeviceInterface failed\n"));
		goto End;
	}
	KdPrint(("WdfDeviceCreateDeviceInterface success\n"));
End:
	return status;
}


/**
 * \brief io device control callback
 * \param queue  queue
 * \param request  request
 * \param outputBufferLength  output buffer length
 * \param inputBufferLength  input buffer length
 * \param ioControlCode  io control code
 */
void EvtIoDeviceControl(WDFQUEUE queue, WDFREQUEST request, size_t outputBufferLength,
                        size_t inputBufferLength, ULONG ioControlCode)
{
	UNREFERENCED_PARAMETER(queue);
	UNREFERENCED_PARAMETER(outputBufferLength);
	UNREFERENCED_PARAMETER(inputBufferLength);
	UNREFERENCED_PARAMETER(ioControlCode);
	WdfRequestComplete(request, STATUS_SUCCESS);
}
