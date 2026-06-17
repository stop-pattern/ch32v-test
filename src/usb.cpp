#include "FreeRTOS.h"
#include "task.h"
#include "usb.hpp"

void usb_task(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelete(NULL); // タスクを削除（このコードには到達しない）
}
