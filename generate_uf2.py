import os
import sys
import struct

# PlatformIOの環境変数を取得
Import("env")

# UF2フォーマットの標準マジックナンバー
UF2_MAGIC_START0 = 0x0A324655
UF2_MAGIC_START1 = 0x9E5D5157
UF2_MAGIC_END    = 0x0AB41357

def bin_to_uf2(target, source, env):
    # SConsの仕様に合わせ、target[0] から生成された .bin のパスを取得
    bin_path = str(target[0])
    uf2_path = os.path.splitext(bin_path)[0] + ".uf2"

    print(f"\n--- Converting BIN to UF2 ---")
    print(f"Source: {bin_path}")
    print(f"Target: {uf2_path}")

    # ⚠️ お使いのUF2ブートローダーの仕様に合わせて設定してください
    # 通常のFlash開始アドレスは 0x08000000 です。
    # ブートローダーが自身を避けるためにオフセットを要求する場合は、0x08002000 等に変更してください。
    BASE_ADDRESS = 0x08000000  
    FAMILY_ID = 0x00000000     # 特定のファミリーID検証が必要な場合はその値を指定（不要なら0）

    try:
        with open(bin_path, "rb") as f:
            bin_data = f.read()

        # 1ブロックあたり256バイトのデータを入れる
        num_blocks = (len(bin_data) + 255) // 256

        with open(uf2_path, "wb") as f:
            for block_no in range(num_blocks):
                addr = BASE_ADDRESS + (block_no * 256)
                data = bin_data[block_no * 256 : (block_no + 1) * 256]
                
                # 256バイトに満たない最終ブロックを0埋め
                if len(data) < 256:
                    data = data.ljust(256, b"\x00")

                flags = 0x00002000 if FAMILY_ID != 0 else 0x00000000

                # UF2ヘッダー (32バイト)
                header = struct.pack(
                    "<IIIIIIII",
                    UF2_MAGIC_START0,
                    UF2_MAGIC_START1,
                    flags,
                    addr,
                    256,          # ペイロードサイズ
                    block_no,
                    num_blocks,
                    FAMILY_ID     # familyID / fileSize
                )

                # UF2フッター (4バイト)
                footer = struct.pack("<I", UF2_MAGIC_END)
                
                # 残りのパディング (512 - 32 - 256 - 4 = 220バイト)
                padding = b"\x00" * 220

                # 512バイトのUF2ブロックを書き込み
                f.write(header + data + padding + footer)

        print("UF2 generation successfully completed.\n")
        print("Error: invalid configuration", file=sys.stderr)  # エラーの例
        sys.exit(1)
    except Exception as e:
        print(f"Error generating UF2: {e}\n")
        sys.exit(1)

# firmware.bin の生成が成功した直後に実行
env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", bin_to_uf2)