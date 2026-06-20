#include "FreeRTOS.h"
#include "task.h"
#include "UART.h"
#include "USB-Driver/inc/usb_lib.h"
#include "usb.hpp"

/* デバイスデスクリプタの例 */
uint8_t MyDeviceDescr[] = {
    0x12,       // bLength
    0x01,       // bDescriptorType (Device)
    0x00, 0x02, // bcdUSB (USB 2.0)
    0x02,       // bDeviceClass (CDC)
    0x00,       // bDeviceSubClass
    0x00,       // bDeviceProtocol
    0x40,       // bMaxPacketSize0
    0x86, 0x1A, // idVendor  -> 0x1A86 (WCHのVID)
    0x0C, 0x80, // idProduct -> 0x800C (サンプルのPID)
    // ... 後略 ...
};

void usb_task(void *pvParameters)
{
    while (1)
    {
        UART2_DataRx_Deal();
        UART2_DataTx_Deal();
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    vTaskDelete(NULL); // タスクを削除（このコードには到達しない）
}
