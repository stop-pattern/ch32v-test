#pragma once

/// @brief USART通信を管理するクラス
class UsartManager {
public:
    /// @brief 受信バッファのサイズ
    static constexpr uint16_t RxBufferSize = 128;

    /// @brief コンストラクタ
    UsartManager() : txMutex(nullptr), rxQueue(nullptr) {}

    /// @brief 初期化メソッド
    /// @param baudrate ボーレート
    void init(uint32_t baudrate);

    /// @brief 文字列送信メソッド
    /// @param str 送信する文字列
    void sendString(const char* str);

    /// @brief 受信用メソッド
    /// @note 受信割り込みから呼び出される
    void handleRxFromISR();

    /// @brief 受信タスクのメインループ
    /// @note 内部で呼び出す
    void rxTaskLoop();

private:
    /// @brief 送信ミューテックス
    SemaphoreHandle_t txMutex;

    /// @brief 受信キュー
    QueueHandle_t rxQueue;
};

// グローバル空間からアクセスできるようにインスタンスを外部宣言
extern UsartManager g_Usart;
