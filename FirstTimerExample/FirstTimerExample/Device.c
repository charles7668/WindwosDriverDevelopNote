#include "Device.h"
#include "File.h"
#include "Timer.h"

NTSTATUS EvtDeviceAdd(WDFDRIVER driver, PWDFDEVICE_INIT deviceInit)
{
	UNREFERENCED_PARAMETER(driver);
	PAGED_CODE()
	WDF_OBJECT_ATTRIBUTES attributes;
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, DEVICE_CONTEXT);
	attributes.SynchronizationScope = WdfSynchronizationScopeDevice;
	WDFDEVICE device;
	NTSTATUS status = WdfDeviceCreate(&deviceInit, &attributes, &device);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("WdfDeviceCreate failed with status 0x%x\n", status));
		goto End;
	}
	PDEVICE_CONTEXT context = DeviceGetContext(device);
	status = TimerCreate(&context->Timer, device);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("TimerCreate failed with status 0x%x\n", status));
		goto End;
	}
	WDF_IO_QUEUE_CONFIG queueConfig;
	WDFQUEUE queue;
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig, WdfIoQueueDispatchParallel);
	queueConfig.EvtIoDeviceControl = EvtIoDeviceControl;
	status = WdfIoQueueCreate(device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &queue);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("Fisrt WdfIoQueueCreate Failed with status 0x%x\n" , status));
		goto End;
	}
	WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);
	queueConfig.EvtIoCanceledOnQueue = EvtIoCanceledOnQueue;
	status = WdfIoQueueCreate(device, &queueConfig,WDF_NO_OBJECT_ATTRIBUTES, &context->Queue);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("Second WdfIoQueueCreate Failed with status 0x%x\n" , status));
		goto End;
	}
	status = WdfDeviceCreateDeviceInterface(device, &GUID_INTERFACE, NULL);
End:
	return status;
}

void EvtIoDeviceControl(WDFQUEUE queue, WDFREQUEST request, size_t outputBufferLength, size_t inputBufferLength,
                        ULONG ioControlCode)
{
	UNREFERENCED_PARAMETER(queue);
	UNREFERENCED_PARAMETER(outputBufferLength);
	UNREFERENCED_PARAMETER(inputBufferLength);
	UNREFERENCED_PARAMETER(ioControlCode);
	KdPrint(("Get IO Request"));
	// WdfRequestComplete(request, STATUS_SUCCESS);
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
		WDFDEVICE device = WdfIoQueueGetDevice(queue);
		PDEVICE_CONTEXT context = DeviceGetContext(device);
		context->InBufferLength = inputBufferLength;
		context->OutBufferLength = outputBufferLength;
		status = WdfRequestForwardToIoQueue(request, context->Queue);
		if (NT_SUCCESS(status) == FALSE)
		{
			KdPrint(("WdfRequestForwardToIoQueue failed with status 0x%x\n", status));
			WdfRequestCompleteWithInformation(request, status, 0);
			break;
		}
		WdfTimerStart(context->Timer, WDF_REL_TIMEOUT_IN_SEC(3));
		break;
	default:
		KdPrint(("EvtIOQueueDeviceControl: invalid IOCTL code\n"));
		WdfRequestCompleteWithInformation(request, STATUS_FWP_INVALID_PARAMETER, 0);
		break;
	}
}

void EvtIoCanceledOnQueue(WDFQUEUE queue, WDFREQUEST request)
{
	UNREFERENCED_PARAMETER(queue);
	PAGED_CODE()
	KdPrint(("Io Cancel Request"));
	WdfRequestCompleteWithInformation(request, STATUS_CANCELLED, 0);
	WDFDEVICE device = WdfIoQueueGetDevice(queue);
	PDEVICE_CONTEXT context = DeviceGetContext(device);
	WdfTimerStop(context->Timer, FALSE);
}
