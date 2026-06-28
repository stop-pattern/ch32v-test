#include <array>
#include <algorithm>
#include "FreeRTOS.h"
#include "task.h"
#include "ch32v20x_can.h"
#include "can.h"


/// @brief CAN通信の初期化関数
void canInit() {
    // CANの初期化構造体を設定
    CAN_InitTypeDef CAN_InitStructure;
    CAN_StructInit(&CAN_InitStructure); // デフォルト値で初期化
    CAN_InitStructure.CAN_Prescaler = 16; // 1Mbps
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_NART = DISABLE;

    if (CAN_Init(CAN1, &CAN_InitStructure) != CAN_InitStatus_Success) {
        // エラー処理
        while (1);
    }

    CAN_Init(CAN1, &CAN_InitStructure);
}

/// @copydoc canSendMessage(uint32_t, const std::array<uint8_t, 8>&)
uint8_t canSendMessage(uint32_t id, const std::array<uint8_t, 8> &sendData) {
    bool isExtended = (id > 0x7FF); // 標準IDか拡張IDかを判定
    return canSendMessage(id, sendData, isExtended);
}

/// @copydoc canSendMessage(uint32_t, const std::array<uint8_t, 8>&, bool)
uint8_t canSendMessage(uint32_t id, const std::array<uint8_t, 8> &sendData, bool isExtended) {
    // データ長が8バイトを超える場合は送信失敗
    if (sendData.size() > 8) {
        return -1;
    }

    // CAN送信メッセージ構造体を設定
    CanTxMsg CanTxStructure;
    CanTxStructure.IDE = CAN_Id_Standard;
    CanTxStructure.RTR = CAN_RTR_Data;
    CanTxStructure.DLC = sendData.size();
    // 標準IDまたは拡張IDを設定
    if (isExtended) {
        CanTxStructure.IDE = CAN_Id_Extended;
        CanTxStructure.ExtId = id;
    } else {
        CanTxStructure.StdId = id;
    }
    // データをコピー
    auto temp = std::copy(sendData.begin(), sendData.end(), CanTxStructure.Data);
    if (temp != CanTxStructure.Data + sendData.size()) {
        return -1; // コピー失敗
    }

    // CAN送信
    uint8_t mailbox = CAN_Transmit(CAN1, &CanTxStructure);
    if (mailbox == CAN_TxStatus_NoMailBox) {
        return -1; // 送信失敗
    }

    // 送信カウンタを初期化
    uint16_t transmitCounter = 0;

    // 送信完了を待つ
    while (CAN_TransmitStatus(CAN1, mailbox) != CANTXOK){
        transmitCounter++;
        if (transmitCounter > CAN_TX_TIMEOUT) {
            return -1; // 送信失敗
        }
    };

    return 0; // 送信成功
}

/// @copydoc can_task(void*)
void can_task(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
