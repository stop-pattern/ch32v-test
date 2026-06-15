#include "debug.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usb.hpp"

/// @brief exampleタスクのハンドル
TaskHandle_t example_task_handle;
/// @brief USBタスクのハンドル
TaskHandle_t usb_task_handle;

/// @brief タスク関数
/// @param pvParameters タスクに渡されるパラメータ（今回は使用しない）
/// @note タスクは無限ループで動作し、250msごとにメッセージを出力する
void example_task(void *pvParameters)
{
    while (1)
    {
        printf("Hello FreeRTOS!\n");
        vTaskDelay(250);
    }
    vTaskDelete(NULL); // タスクを削除（このコードには到達しない）
}

/// @brief メイン関数
/// @return 常に0を返す
int main(void)
{
    // 割り込みの優先順位グループをNVIC_PriorityGroup_1に設定
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    // システムクロックの更新
    SystemCoreClockUpdate();

    // デバッグ用の初期化
    Delay_Init();

    // USARTの初期化
    USART_Printf_Init(115200);

    // exampleタスクの作成
    xTaskCreate((TaskFunction_t)example_task,
        (const char *)"example",
        (uint16_t)256,
        (void *)NULL,
        (UBaseType_t)5,
        (TaskHandle_t *)&example_task_handle);

    // USBタスクの作成
    xTaskCreate((TaskFunction_t)usb_task,
        (const char *)"usb",
        (uint16_t)256,
        (void *)NULL,
        (UBaseType_t)3,
        (TaskHandle_t *)NULL);

    // スケジューラの開始
    vTaskStartScheduler();

    // スケジューラが終了した場合はここに到達する
    while (1)
    {
        printf("shouldn't run at here!!\n");
    }
}
