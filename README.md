# ch32v-test

CH32V203 向けの PlatformIO プロジェクトです。USB、CAN、Lチカ、書き込み方法の確認をまとめて試せるようにしています。

## 概要

- 対象: CH32V203 系マイコン
- 開発環境: PlatformIO
- 目的: 初期化、GPIO 制御、シリアル出力、USB 通信の動作確認

## 機能

- USB 通信
  - USB FS
  - CDC Serial
  - MSC Download
- CAN 通信
  - CAN - Serial
- LED 点滅
- 書き込み手段
  - USB
  - WCH
  - UF2

## ビルド

```bash
pio run
```

## 書き込み

### 1. GitHub Actions から取得する

1. コードを commit & push する
2. Actions で生成されたバイナリをダウンロードする

### 2. WCHLink を使う

1. WCHLink を PC と書き込み対象に接続する
2. 自動でファームが書き込まれる

### 3. ISP を使う

1. Zadig でドライバーを WinUSB に変更する  初回のみ
2. BOOT ボタンを押しながら NRST ボタンで再起動する
3. 自動でファームが書き込まれる

### 4. PlatformIO CLI を使う

ビルドから書き込みまでまとめて実行できます。

```bash
pio run --target upload
```

1. BOOT ボタンを押しながら NRST ボタンで再起動する
2. 初回は ISP と同様にドライバー変更が必要
3. 自動でファームが書き込まれる

### 5. UF2 を使う

1. NRST を 500ms 以内に 2 回以上連打して MSC モードを起動する
2. UF2 ファイルを USB MSC ドライブにコピーする
3. 自動でファームが書き込まれる

## 使い方

1. PlatformIO をインストールする
2. このプロジェクトを開く
3. `src` 配下のコードを編集する
4. ビルドしてボードへ書き込む

## プロジェクト構成

- `src/`: アプリケーション本体
- `lib/`: 共通ライブラリ
- `test/`: テストコード
- `.github/workflows/`: CI と release workflow

## 注意

- ボード設定や書き込み方法は、実際の配線と環境に合わせて調整してください。
- 詳細な仕様はソースコードを参照してください。
