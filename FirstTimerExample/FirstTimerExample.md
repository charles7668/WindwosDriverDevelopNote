# 第一個定時器範例

這是一個透過定時器來手動讀取 IOCTL 的範例 , 功能上與["輸入訊息原樣返回驅動範例"](../EchoDriverExample/EchoDriverExample.md)相同 , 但 IO 請求是藉由 Timer 來讀取 , 會在輸入文字的 3 秒後回傳

# 參考

[Using Timers](https://learn.microsoft.com/en-us/windows-hardware/drivers/wdf/using-timers)
[Configuring Dispatch Mode for an I/O Queue](https://learn.microsoft.com/en-us/windows-hardware/drivers/wdf/configuring-dispatch-mode-for-an-i-o-queue)
[Example Uses of I/O Queues](https://learn.microsoft.com/en-us/windows-hardware/drivers/wdf/example-uses-of-i-o-queues)
