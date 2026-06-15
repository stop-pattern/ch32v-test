#include "FreeRTOS.h"
#include "task.h"
#include "usb.hpp"

#if defined(USE_TINYUSB)
#if __has_include("Adafruit_TinyUSB.h")
#include "Adafruit_TinyUSB.h"
#define USB_USE_TINYUSB 1
#else
#define USB_USE_TINYUSB 0
#endif
#else
#define USB_USE_TINYUSB 0
#endif

/// @brief USB通信タスクのハンドル
TaskHandle_t usb_task_handle;

void usb_task(void *pvParameters)
{
#if USB_USE_TINYUSB
    if (!TinyUSBDevice.isInitialized())
    {
        TinyUSBDevice.begin(0);
    }

    Serial.begin(115200);

    while (!TinyUSBDevice.mounted())
    {
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    while (1)
    {
#ifdef TINYUSB_NEED_POLLING_TASK
        TinyUSBDevice.task();
#endif

        uint8_t buffer[64];

        if (Serial.available())
        {
            size_t length = Serial.read(buffer, sizeof(buffer));
            if (length > 0)
            {
                Serial.write(buffer, length);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1));
    }
#else
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
#endif

    vTaskDelete(NULL); // タスクを削除（このコードには到達しない）
}
