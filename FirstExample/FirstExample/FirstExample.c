#include <ntddk.h>
#include <wdf.h>

// 驅動程式卸載事件
void EvtDriverUnload(_In_ WDFDRIVER driver)
{
	UNREFERENCED_PARAMETER(driver);
	KdPrint(("Driver Unload Called\n"));
}

/*
* DriverEntry
* @driver_object: 指向驅動程式物件的指標
* @registry_path: 指向註冊表路徑的指標
* 
* 這個函式會被 I/O 管理員呼叫來初始化驅動程式。這是驅動程式中第一個被呼叫的函式。
* 
* Return Value: STATUS_SUCCESS if successful, STATUS_UNSUCCESSFUL otherwise
*/
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT driver_object,_In_ PUNICODE_STRING register_path)
{
	// WDF Driver 配置物件
	WDF_DRIVER_CONFIG config;
	// 初始化 WDF Driver 配置物件 , 第二個參數是一個回調函式，用來處理PnP管理器報告裝置存在的事件
	WDF_DRIVER_CONFIG_INIT(&config, WDF_NO_EVENT_CALLBACK);
	// 設定 WDF Driver 配置物件的Flags , 這裡設定為非PnP驅動程式
	config.DriverInitFlags |= WdfDriverInitNonPnpDriver;
	// 設定 WDF Driver 配置物件的EvtDriverUnload(驅動卸載事件)
	config.EvtDriverUnload = EvtDriverUnload;
	// 呼叫 WdfDriverCreate 函式來建立 WDF Driver 物件
	NTSTATUS status = WdfDriverCreate(driver_object, register_path, WDF_NO_OBJECT_ATTRIBUTES, &config, WDF_NO_HANDLE);
	if (NT_SUCCESS(status))
	{
		KdPrint(("wdf driver create success\n"));
	}
	else
	{
		KdPrint(("WdfDriverCreate failed with status 0x%x\n", status));
	}
	return status;
}
