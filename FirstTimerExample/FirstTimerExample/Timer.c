#include "Timer.h"
#include "Device.h"

NTSTATUS TimerCreate(WDFTIMER* timer, WDFDEVICE device)
{
	UNREFERENCED_PARAMETER(device);
	UNREFERENCED_PARAMETER(timer);
	PAGED_CODE()
	WDF_TIMER_CONFIG timerConfig;
	WDF_OBJECT_ATTRIBUTES timerAttributes;
	WDF_TIMER_CONFIG_INIT(&timerConfig, EvtTimerFunc);
	WDF_OBJECT_ATTRIBUTES_INIT(&timerAttributes);
	timerAttributes.ParentObject = device;
	const NTSTATUS status = WdfTimerCreate(&timerConfig, &timerAttributes, timer);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("WdfTimerCreate failed with status 0x%x\n", status));
		goto End;
	}
End:
	return status;
}

void EvtTimerFunc(WDFTIMER timer)
{
	UNREFERENCED_PARAMETER(timer);
	KdPrint(("Timer tirggered"));
	WDFREQUEST request;
	WDFDEVICE device = WdfTimerGetParentObject(timer);
	PDEVICE_CONTEXT context = DeviceGetContext(device);
	NTSTATUS status = WdfIoQueueRetrieveNextRequest(context->Queue, &request);
	if (NT_SUCCESS(status) == FALSE)
	{
		KdPrint(("WdfIoQueueRetrieveNextRequest failed with status 0x%x\n", status));
		goto End;
	}

	size_t inputBufferLength = context->InBufferLength;
	size_t outputBufferLength = context->OutBufferLength;
	PVOID inBuffer = NULL;
	PVOID outBuffer = NULL;
	size_t inLength = 0;
	size_t outLength = 0;
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
		goto End;
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
		goto End;
	}
	KdPrint(("EvtIOQueueDeviceControl: inBuffer = %s , inLength = %d", (CHAR*)inBuffer, inLength));
	memcpy(outBuffer, inBuffer, inLength);
	KdPrint(("EvtIOQueueDeviceControl: outBuffer = %s , outBufferLength = %d\n" , (CHAR*)outBuffer , outLength));
	WdfRequestCompleteWithInformation(request,STATUS_SUCCESS, outLength);
End:
	return;
}
