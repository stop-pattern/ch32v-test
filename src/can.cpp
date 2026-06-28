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

/// @copydoc can_task(void*)
void can_task(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
