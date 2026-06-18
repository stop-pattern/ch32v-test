#include "ch32v20x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "usart.hpp"

void UsartManager::init(uint32_t baudrate) {
    // FreeRTOS リソースの生成
    txMutex = xSemaphoreCreateMutex();
    rxQueue = xQueueCreate(RxBufferSize, sizeof(uint8_t));

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    // クロック有効化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    // TX (PA9) の設定
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // RX (PA10) の設定
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART1 設定
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);

    // 受信割り込み有効化
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    // NVIC 設定
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // USART1 有効化
    USART_Cmd(USART1, ENABLE);
}

void UsartManager::sendString(const char* str) {
    // 引数チェック
    if (txMutex == nullptr || str == nullptr) return;

    // ミューテックスを取得して送信権を確保
    if (xSemaphoreTake(txMutex, portMAX_DELAY) == pdTRUE) {
        while (*str) {
            USART_SendData(USART1, static_cast<uint8_t>(*str));
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
            str++;
        }
        // ミューテックスを解放
        xSemaphoreGive(txMutex);
    }
}

// 割り込み内からデータをキューへ送る処理
void UsartManager::handleRxFromISR() {
    // 割り込み内でのタスク切り替えを考慮して、必要に応じてコンテキストスイッチを要求するためのフラグ
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // 受信割り込みが発生した場合の処理
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        // 受信データを読み取る
        uint8_t data = static_cast<uint8_t>(USART_ReceiveData(USART1));
        // キューにデータを送信
        if (rxQueue != nullptr) {
            xQueueSendFromISR(rxQueue, &data, &xHigherPriorityTaskWoken);
        }
    }

    // 必要に応じてコンテキストスイッチを要求
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
}

/// @brief USART1の割り込みハンドラ
/// @note C言語の割り込みハンドラからC++のメソッドを呼び出すためのラッパー関数
extern "C" void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART1_IRQHandler(void) {
    // グローバルインスタンス経由でC++のメソッドを呼び出す
    g_Usart.handleRxFromISR();
}

/// @brief 受信タスクのメインループ
/// @note 内部で呼び出す
void UsartManager::rxTaskLoop() {
    uint8_t rxBuffer[RxBufferSize];
    uint16_t rxIndex = 0;
    uint8_t receivedChar;

    while (true) {
        if (xQueueReceive(rxQueue, &receivedChar, portMAX_DELAY) == pdTRUE) {
            if (receivedChar == '\n' || receivedChar == '\r') {
                if (rxIndex > 0) {
                    rxBuffer[rxIndex] = '\0';
                    
                    // エコーバック処理（グローバル送信メソッドの呼び出し）
                    sendString("Echo: ");
                    sendString(reinterpret_cast<char*>(rxBuffer));
                    sendString("\r\n");
                    
                    rxIndex = 0;
                }
            } else {
                if (rxIndex < (RxBufferSize - 1)) {
                    rxBuffer[rxIndex++] = receivedChar;
                } else {
                    rxIndex = 0;
                }
            }
        }
    }
}

/// @brief 受信タスクのラッパー関数
/// @param pvParameters タスクに渡されるパラメータ（UsartManagerのインスタンスポインタ）
extern "C" void vRxTaskWrapper(void* pvParameters) {
    // 渡されたインスタンスポインタを使ってループを実行
    static_cast<UsartManager*>(pvParameters)->rxTaskLoop();
}

// グローバルインスタンスの初期化
UsartManager g_Usart;
