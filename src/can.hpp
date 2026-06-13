#pragma once

/// @brief CAN通信タスク関数
/// @param pvParameters タスクに渡されるパラメータ（今回は使用しない）
/// @note タスクは無限ループで動作し、100msごとにCAN通信の処理を行う
void can_task(void *pvParameters);
