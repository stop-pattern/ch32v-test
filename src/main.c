#include "debug.h"
#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t example_task_handle;
void example_task(void *pvParameters)
{
    while (1)
    {
        printf("Hello FreeRTOS!\n");
        vTaskDelay(250);
    }
}

int main(void)
{
    temp;   // for debug
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    xTaskCreate((TaskFunction_t)example_task,
        (const char *)"example",
        (uint16_t)256,
        (void *)NULL,
        (UBaseType_t)5,
        (TaskHandle_t *)&example_task_handle);
    vTaskStartScheduler();
    while (1)
    {
        printf("shouldn't run at here!!\n");
    }
}