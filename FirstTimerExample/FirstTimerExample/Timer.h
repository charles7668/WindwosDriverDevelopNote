#pragma once
#include <ntddk.h>
#include <wdf.h>
NTSTATUS TimerCreate(_In_ WDFTIMER* timer, _In_ WDFDEVICE device);
void EvtTimerFunc(WDFTIMER timer);
