#pragma once
#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>

// {08D54C91-A205-4080-AA1F-CDE89AFE92F9}
DEFINE_GUID(GUID_INTERFACE,
            0x8d54c91, 0xa205, 0x4080, 0xaa, 0x1f, 0xcd, 0xe8, 0x9a, 0xfe, 0x92, 0xf9);

#define IOCTL_ECHO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)


typedef struct DEVICE_CONTEXT_
{
	WDFTIMER Timer;
	WDFQUEUE Queue;
	size_t InBufferLength;
	size_t OutBufferLength;
} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

NTSTATUS EvtDeviceAdd(_In_ WDFDRIVER driver, _In_ PWDFDEVICE_INIT deviceInit);

void EvtIoDeviceControl(WDFQUEUE queue, WDFREQUEST request, size_t outputBufferLength,
                        size_t inputBufferLength, ULONG ioControlCode);

void EvtIoCanceledOnQueue(WDFQUEUE queue, WDFREQUEST request);
