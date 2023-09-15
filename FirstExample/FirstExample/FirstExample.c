#include <ntddk.h>
#include <wdf.h>

// �X�ʵ{�������ƥ�
void EvtDriverUnload(_In_ WDFDRIVER driver)
{
	UNREFERENCED_PARAMETER(driver);
	KdPrint(("Driver Unload Called\n"));
}

/*
* DriverEntry
* @driver_object: ���V�X�ʵ{�����󪺫���
* @registry_path: ���V���U����|������
* 
* �o�Ө禡�|�Q I/O �޲z���I�s�Ӫ�l���X�ʵ{���C�o�O�X�ʵ{�����Ĥ@�ӳQ�I�s���禡�C
* 
* Return Value: STATUS_SUCCESS if successful, STATUS_UNSUCCESSFUL otherwise
*/
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT driver_object,_In_ PUNICODE_STRING register_path)
{
	// WDF Driver �t�m����
	WDF_DRIVER_CONFIG config;
	// ��l�� WDF Driver �t�m���� , �ĤG�ӰѼƬO�@�Ӧ^�ը禡�A�ΨӳB�zPnP�޲z�����i�˸m�s�b���ƥ�
	WDF_DRIVER_CONFIG_INIT(&config, WDF_NO_EVENT_CALLBACK);
	// �]�w WDF Driver �t�m����Flags , �o�̳]�w���DPnP�X�ʵ{��
	config.DriverInitFlags |= WdfDriverInitNonPnpDriver;
	// �]�w WDF Driver �t�m����EvtDriverUnload(�X�ʨ����ƥ�)
	config.EvtDriverUnload = EvtDriverUnload;
	// �I�s WdfDriverCreate �禡�ӫإ� WDF Driver ����
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
