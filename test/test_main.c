#include <unity.h>

// テスト前の準備処理（空でOK）
void setUp(void) {
    // 毎回テストが実行される前に呼ばれる
}

// テスト後の後片付け処理（空でOK）
void tearDown(void) {
    // 毎回テストが終了した後に呼ばれる
}

// 実際のテストケース（1 + 1 = 2 を検証するだけ）
void test_calculation_simple(void) {
    TEST_ASSERT_EQUAL(2, 1 + 1);
}

int main(int argc, char **argv) {
    UNITY_BEGIN(); // テスト開始の合図
    
    RUN_TEST(test_calculation_simple); // テストの実行
    
    return UNITY_END(); // テスト終了と結果の返却
}
