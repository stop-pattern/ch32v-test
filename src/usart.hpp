#pragma once

/// @brief USART通信を管理するクラス
class UsartManager {
public:
    /// @brief 受信バッファのサイズ
    static constexpr uint16_t RxBufferSize = 128;

    /// @brief コンストラクタ
    UsartManager() : txMutex(nullptr), rxQueue(nullptr) {}

    /// @brief 初期化メソッド
    void init(uint32_t baudrate);

    /// @brief 文字列送信メソッド（どのタスクからでも呼べるグローバル関数用）
    void sendString(const char* str);

    /// @brief 割り込みハンドラから呼ばれる受信用メソッド
    void handleRxFromISR();

    /// @brief 受信タスクのメインループ（内部で呼び出す）
    void rxTaskLoop();

private:
    /// @brief 送信ミューテックス
    SemaphoreHandle_t txMutex;

    /// @brief 受信キュー
    QueueHandle_t rxQueue;
};

// グローバル空間からアクセスできるようにインスタンスを外部宣言
extern UsartManager g_Usart;
