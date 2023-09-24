# 目錄

- [目錄](#目錄)
- [準備工作](#準備工作)
- [配置測試機器(target computer)的環境](#配置測試機器target-computer的環境)
  - [安裝 WDK](#安裝-wdk)
  - [開啟網路探索](#開啟網路探索)
  - [啟用 Debugging](#啟用-debugging)
  - [啟用測試簽章](#啟用測試簽章)
  - [設定主機的 ip 位置](#設定主機的-ip-位置)
  - [取得測試機器的 hostname](#取得測試機器的-hostname)
- [設定主機的環境](#設定主機的環境)
- [WDF 驅動範例](#wdf-驅動範例)
- [測試驅動](#測試驅動)
- [相關工具](#相關工具)
- [疑難排解](#疑難排解)
- [參考](#參考)

# 準備工作

- visual studio 或更新版本, 安裝在 host computer
- 準備一個安裝 windows 的 VM(VMWare or Virtualbox or Hyper-V)做為 target computer(測試驅動用的機器) , windows 版本建議與主機的 windows 版本相同
- 此外以下的配置全以 x64 為主

# 配置測試機器(target computer)的環境

## 安裝 WDK

- 安裝 最新版[WDK](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk) , 或 [舊版 WDK](https://learn.microsoft.com/en-us/windows-hardware/drivers/other-wdk-downloads) , 依使用的系統版本下載即可
- 在 WDK 安裝資料夾下對應系統架構的資料夾找到 WDK Test Target Setup MSI(預設路徑 : C:\Program Files (x86)\Windows Kits\10\Remote\x64\WDK Test Target Setup x64-x64_en-us.msi)

## 開啟網路探索

- 開始功能表右鍵->設定->網路和網際網路->網路和共用中心->進階共用設定->開啟網路探索

## 啟用 Debugging

- 使用系統管理員打開 CMD -> 輸入 `bcdedit /set {default} DEBUG YES`

## 啟用測試簽章

- 使用系統管理員打開 CMD -> 輸入 `bcdedit /set TESTSIGNING ON `

## 設定主機的 ip 位置

- 這裡假設使用的是虛擬機,並沒額外調整網路設定的狀況
- 輸入 ipconfig , 確認 ip 位址前三碼 ex : 192.168.60
- 輸入以下指令, hostip 的前三碼根據自己上一步查到的位置進行替換 , port 跟 key 如有需要也可以更換,以達到增加安全性的目的
  > bcdedit /dbgsettings net hostip:192.168.60.1 port:50000 key:2steg4fzbj2sz.23418vzkd4ko3.1g34ou07z4pev.1sp3yo9yz874p
- 確認設定是否正確, 輸入以下命令
  > bcdedit /dbgsettings
- 可能回傳以下資訊
  > key 2steg4fzbj2sz.23418vzkd4ko3.1g34ou07z4pev.1sp3yo9yz874p  
  > debugtype NET  
  > hostip 192.168.60.1  
  > port 50000  
  > dhcp Yes

## 取得測試機器的 hostname

- 打開 PowerShell -> 輸入 hostname , 記住這個 hostname, 會在後面主機中的設定用到

# 設定主機的環境

- 使用系統管理員打開 cmd,並進入 WinDbg 所在的目錄
  > cd C:\Program Files (x86)\Windows Kits\10\Debuggers\x64
- 使用以下命令通過遠程用戶調試打開 WinDbg。 密鑰和端口的值與之前在目標計算機上使用 BCDEdit 設置的值匹配。
  > WinDbg –k net:port=50000,key=2steg4fzbj2sz.23418vzkd4ko3.1g34ou07z4pev.1sp3yo9yz874p
- 重新啟動目標系統

# WDF 驅動範例

1. [第一個 WDF 驅動範例](FirstExample/FirstExample.md)
2. [簡易設備物件的驅動範例](DeviceObjectExample/DeviceObjectExample.md)
3. [簡易 PnP 裝置驅動](FirstPnPExample/FirstPnPExample.md)
4. [輸入訊息原樣返回驅動範例](EchoDriverExample/EchoDriverExample.md)
5. [第一個定時器範例](FirstTimerExample/FirstTimerExample.md)

# 測試驅動

- 使用 [OSR Driver Loader](https://www.osronline.com/article.cfm%5earticle=157.htm)或其它 Driver Loader 來載入及啟動驅動
- 可透過[WinObj](https://learn.microsoft.com/en-us/sysinternals/downloads/winobj)來確認驅動的安裝是否成功
- 在驅動內加入 `kdPrint` 來輸出訊息
- 在測試機器上使用系統管理員打開[DebugView](https://learn.microsoft.com/zh-tw/sysinternals/downloads/debugview) , 並在 `Capture` 內找到 `Capture Kernal` 並打勾
- 如想進行更進一步的調試, 可參考 [Debug Windows drivers step-by-step lab (echo kernel mode)](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/debug-universal-drivers---step-by-step-lab--echo-kernel-mode-)

# 相關工具

- [WinDbg](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/)
- [DebugView](https://learn.microsoft.com/en-us/sysinternals/downloads/debugview)
- [WinObj](https://learn.microsoft.com/en-us/sysinternals/downloads/winobj)
- [OSR Driver Loader](https://www.osronline.com/article.cfm%5earticle=157.htm)

# 疑難排解

- [點我前往 Q&A](Q&A/Q&A.md)

# 參考

- [Provision a computer for driver deployment and testing (WDK 10)](https://learn.microsoft.com/en-us/windows-hardware/drivers/gettingstarted/provision-a-target-computer-wdk-8-1)
- [Download the Windows Driver Kit (WDK)](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk)
- [Other WDK downloads](https://learn.microsoft.com/en-us/windows-hardware/drivers/other-wdk-downloads)
- [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/)
- [Debug Windows drivers step-by-step lab (echo kernel mode)](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/debug-universal-drivers---step-by-step-lab--echo-kernel-mode-)
- [Install the Windows debugger](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/)
- [DebugView](https://learn.microsoft.com/en-us/sysinternals/downloads/debugview)
- [WinObj](https://learn.microsoft.com/en-us/sysinternals/downloads/winobj)
- [OSR Driver Loader](https://www.osronline.com/article.cfm%5earticle=157.htm)
- [Windows Driver Frameworks](https://learn.microsoft.com/en-us/windows-hardware/drivers/wdf/)
