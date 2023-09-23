#include "File.h"

void EvtFileCreate(WDFDEVICE device, WDFREQUEST request, WDFFILEOBJECT fileObject)
{
	UNREFERENCED_PARAMETER(device);
	UNREFERENCED_PARAMETER(fileObject);
	PAGED_CODE()
	KdPrint(("File Created"));
	WdfRequestComplete(request, STATUS_SUCCESS);
}
