#pragma once

/// @brief USB通信タスク関数
/// @param pvParameters タスクに渡されるパラメータ（今回は使用しない）
/// @note USBを通してCDCでシリアル通信の処理を行う
void usb_task(void *pvParameters);
