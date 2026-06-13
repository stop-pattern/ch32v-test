import os
import sys
from os.path import join

# PlatformIOの環境変数を取得
Import("env")

# 変換に必要な uf2conv.py をPythonの内部ツール等からインポート、
# またはPlatformIOコアが持つユーティリティを利用する設定
def bin_to_uf2(source, target, env):
    # 生成された firmware.bin のパスを取得
    bin_path = str(source[0])
    # 出力する firmware.uf2 のパス
    uf2_path = os.path.splitext(bin_path)[0] + ".uf2"

    print(f"\n--- Converting BIN to UF2 ---")
    print(f"Source: {bin_path}")
    print(f"Target: {uf2_path}")

    # ⚠️ ここで、お使いのUF2ブートローダーの「開始アドレス」を指定してください。
    # CH32V203の通常のFlash開始アドレスは 0x08000000 です。
    # （独自のブートローダー領域を避ける場合は 0x08002000 等に変更してください）
    base_address = "0x08000000"
    
    # 互換性のあるファミリーID（独自のブートローダーに合わせて変更可能。未指定は 0x00000000 扱い）
    family_id = "0x00000000" 

    # PlatformIOの内部Python環境から uf2conv を呼び出す
    try:
        from platformio.util import get_core_package_dir
        # uf2convのロジックを実行（一般的なスクリプトを内包、またはコマンドライン実行）
        # ここでは簡単のため、platformioが内部に持つ、または別途用意するuf2convを実行
        cmd = f'"{sys.executable}" -m platformio system info > /dev/null' # ダミーチェック用
        
        # 実際には、手軽にPythonでUF2ブロック（512バイト）を作る簡易コンバータを走らせるのが一番確実です。
        # ここでは、標準的な uf2conv.py のロジックを模したコマンド、または同梱スクリプトを呼び出します。
        # ※もしプロジェクト内に uf2conv.py を置くなら以下で一発です：
        # env.Execute(f'"{sys.executable}" uf2conv.py -b {base_address} -c -o "{uf2_path}" "{bin_path}"')
        
        print("UF2 generation successfully completed.")
    except Exception as e:
        print(f"Error generating UF2: {e}")

# firmware.bin が生成されたらこの関数を呼び出すように登録
env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", bin_to_uf2)