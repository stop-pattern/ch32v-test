#include "debug.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ch32v20x_can.h"
#include "can.hpp"

// CAN初期化構造体
CAN_InitTypeDef can_init_struct;
// CAN送信メッセージ構造体
CanTxMsg TxMessage;

void can_task(void *pvParameters)
{
    // CANの初期化
    CAN_StructInit(&can_init_struct);
    can_init_struct.CAN_Prescaler = 16; // 16分周
    can_init_struct.CAN_Mode = CAN_Mode_Normal; // ノーマルモード
    can_init_struct.CAN_SJW = CAN_SJW_1tq; // 同期ジャンプ幅1TQ
    can_init_struct.CAN_BS1 = CAN_BS1_4tq; // ビットセグメント1を4TQ
    can_init_struct.CAN_BS2 = CAN_BS2_3tq; // ビットセグメント2を3TQ
    can_init_struct.CAN_NART = ENABLE; // 自動再送信を無効化
    if (CAN_Init(CAN1, &can_init_struct) != CAN_InitStatus_Success)
    {
        printf("CAN initialization failed!\n");
        vTaskDelete(NULL); // タスクを削除
    }

    
    while (1)
    {
        // CAN通信の処理をここに記述
        TxMessage.StdId = 0x123; // 標準ID
        TxMessage.ExtId = 0x01; // 拡張ID（使用しない場合は0）
        TxMessage.IDE = CAN_Id_Standard; // 標準IDを使用
        TxMessage.RTR = CAN_RTR_Data; // データフレーム
        TxMessage.DLC = 8; // データ長コード
        TxMessage.Data[0] = 0xDE;
        TxMessage.Data[1] = 0xAD;
        TxMessage.Data[2] = 0xBE;
        TxMessage.Data[3] = 0xEF;
        TxMessage.Data[4] = 0x01;
        TxMessage.Data[5] = 0x23;
        TxMessage.Data[6] = 0x45;
        TxMessage.Data[7] = 0x67;
        uint8_t mailbox = CAN_Transmit(CAN1, &TxMessage);
        if (mailbox == CAN_TxStatus_NoMailBox)
        {
            printf("No empty mailbox for CAN transmission!\n");
        }
        vTaskDelay(100); // 100msごとに処理を行う
    }
    vTaskDelete(NULL); // タスクを削除（このコードには到達しない）
}