# 輸入訊息原樣返回驅動範例

此範例是透過 [IOCTL](https://learn.microsoft.com/en-us/windows/win32/devio/device-input-and-output-control-ioctl-) 讓驅動與應用程式進行通訊的範例, 實現的功能是將輸入的東西原樣回傳

# 測試方式

1. 開始功能表右鍵選擇工作管理員
2. 動作->新增傳統硬體
3. 選手動選取
4. 下一步->從磁片安裝
5. 選擇驅動的 Inf 檔案(EchoDriverExample.inf)
6. 完成後可看到在裝置管理員的系統裝置下會出現 EchoDriverExample 的裝置
7. 打開 EchoDriverExampleConsole.exe
8. 輸入文字 , 注意長度不要超過 2048
9. 程式會接收從驅動回傳的 buffer, 內容為輸入的文字

# 參考

- [Device Input and Output Control (IOCTL)](https://learn.microsoft.com/en-us/windows/win32/devio/device-input-and-output-control-ioctl-)
