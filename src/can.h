#pragma once

/// @brief CAN通信タスクのハンドル
TaskHandle_t can_task_handle;

/// @brief CAN通信タスクの関数
/// @param pvParameters タスクに渡されるパラメータ（今回は使用しない）
void can_task(void *pvParameters);
