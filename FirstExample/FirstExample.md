# 第一個驅動範例

這個專案包含最基本的 wdf 驅動的初始化

- [第一個驅動範例](#第一個驅動範例)
- [使用的函數及程式說明](#使用的函數及程式說明)
  - [WDF\_DRIVER\_CONFIG](#wdf_driver_config)
  - [WDF\_DRIVER\_CONFIG\_INIT](#wdf_driver_config_init)
  - [WdfDriverCreate](#wdfdrivercreate)
- [參考](#參考)

# 使用的函數及程式說明

## WDF_DRIVER_CONFIG

- 這是一個 包含 WDF Driver 設定的結構體

- 更多詳細資訊可參考以下連結 [WDF_DRIVER_CONFIG structure](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdfdriver/ns-wdfdriver-_wdf_driver_config)

## WDF_DRIVER_CONFIG_INIT

- 用來初始化 `WDF_DRIVER_CONFIG` 物件的函式
- 更多詳細資訊可參考以下連結[WDF_DRIVER_CONFIG_INIT function](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdfdriver/nf-wdfdriver-wdf_driver_config_init)

## WdfDriverCreate

- 為呼叫的 Driver 創建一個 WDF Driver 物件
- 更多詳細資訊可參考以下連結 [WdfDriverCreate function (wdfdriver.h)](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdfdriver/nf-wdfdriver-wdfdrivercreate)

# 參考

- [WDF_DRIVER_CONFIG structure](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdfdriver/ns-wdfdriver-_wdf_driver_config)
- [WDF_DRIVER_CONFIG_INIT function](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdfdriver/nf-wdfdriver-wdf_driver_config_init)
- [WdfDriverCreate function (wdfdriver.h)](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wdfdriver/nf-wdfdriver-wdfdrivercreate)
