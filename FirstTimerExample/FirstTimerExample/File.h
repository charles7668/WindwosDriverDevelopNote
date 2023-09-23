#pragma once
#include <ntddk.h>
#include <wdf.h>
void EvtFileCreate(WDFDEVICE device, WDFREQUEST request, WDFFILEOBJECT fileObject);
