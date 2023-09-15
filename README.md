# 目錄

- [目錄](#目錄)
- [準備工作](#準備工作)
- [配置測試機器(target computer)的環境](#配置測試機器target-computer的環境)
  - [安裝 WDK](#安裝-wdk)
  - [開啟網路探索](#開啟網路探索)
  - [取得測試機器的 hostname](#取得測試機器的-hostname)
- [設定主機的環境](#設定主機的環境)
- [WDF 驅動範例](#wdf-驅動範例)
- [測試驅動](#測試驅動)
- [參考](#參考)

# 準備工作

- visual studio 或更新版本, 安裝在 host computer
- 準備一個安裝 windows 的 VM(VMWare or Virtualbox or Hyper-V)做為 target computer(測試驅動用的機器) , windows 版本建議與主機的 windows 版本相同

# 配置測試機器(target computer)的環境

## 安裝 WDK

- 安裝 最新版[WDK](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk) , 或 [舊版 WDK](https://learn.microsoft.com/en-us/windows-hardware/drivers/other-wdk-downloads) , 依使用的系統版本下載即可
- 在 WDK 安裝資料夾下對應系統架構的資料夾找到 WDK Test Target Setup MSI(預設路徑 : C:\Program Files (x86)\Windows Kits\10\Remote\x64\WDK Test Target Setup x64-x64_en-us.msi)

## 開啟網路探索

- 開始功能表右鍵->設定->網路和網際網路->網路和共用中心->進階共用設定->開啟網路探索

## 取得測試機器的 hostname

- 打開 PowerShell -> 輸入 hostname , 記住這個 hostname, 會在後面主機中的設定用到

# 設定主機的環境

- 安裝 [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/) 及 [WDK](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk) , 或 [舊版 WDK](https://learn.microsoft.com/en-us/windows-hardware/drivers/other-wdk-downloads)
- 打開 Visual Studio -> 延伸模組 -> Driver -> Test -> Configure Devices -> Add New Device
- 設定顯示名稱及測試機器的 hostname 選擇下一步
- 選擇 Debugger 的 Mode 並設定好參數(通常可以不用改動)後按下一步
- 等待測試機器配置完程,過程中測試機器可能會被重新啟動
- 按下一步後按完成

# WDF 驅動範例

1. [第一個 WDF 驅動範例](FirstExample/FirstExample.md)

# 測試驅動

- 在驅動內加入 `kdPrint` 來輸出訊息
- 在測試機器上使用系統管理員打開[DebugView](https://learn.microsoft.com/zh-tw/sysinternals/downloads/debugview) , 並在 `Capture` 內找到 `Capture Kernal` 並打勾
- 使用 [OSR Driver Loader](https://www.osronline.com/article.cfm%5earticle=157.htm)或其它 Driver Loader 來載入及啟動驅動

# 參考

- [Provision a computer for driver deployment and testing (WDK 10)](https://learn.microsoft.com/en-us/windows-hardware/drivers/gettingstarted/provision-a-target-computer-wdk-8-1)
- [Download the Windows Driver Kit (WDK)](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk)
- [Other WDK downloads](https://learn.microsoft.com/en-us/windows-hardware/drivers/other-wdk-downloads)
- [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/)
