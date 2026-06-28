#pragma once

constexpr uint16_t CAN_TX_TIMEOUT = 1000; // CAN送信タイムアウト

/// @brief CAN通信タスクのハンドル
TaskHandle_t can_task_handle;

/// @brief CANメッセージを送信する関数
/// @param id 送信するCAN ID
/// @param sendData 送信するデータ
/// @return 0: 送信成功, -1: 送信失敗
/// @note CAN IDが標準IDか拡張IDかは、idの値によって判定されます。
uint8_t canSendMessage(uint32_t id, const std::array<uint8_t, 8> &sendData);

/// @brief CANメッセージを送信する関数
/// @param id 送信するCAN ID
/// @param sendData 送信するデータ
/// @param isExtended 拡張IDを使用するかどうか（デフォルトはfalse）
/// @return 0: 送信成功, -1: 送信失敗
uint8_t canSendMessage(uint32_t id, const std::array<uint8_t, 8> &sendData, bool isExtended = false);


/// @brief CAN通信タスクの関数
/// @param pvParameters タスクに渡されるパラメータ（今回は使用しない）
void can_task(void *pvParameters);
