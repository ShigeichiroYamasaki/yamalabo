# ステッピングモーター 17HS4401

2026/03/11

## ステッピングモーター(17HS4401)

1. 高トルクモデル
2. 相数: 2相
3. ステップ角:（1周200ステップ）
4. 定格電流: 約1.5A 〜 1.7A / 相
5. 保持トルク: 約0.4 N·m 〜 0.5 N·m（4000g·cm〜5000g·cm以上）
6. 配線: バイポーラ（4線）

### 配線色（一般的な例）

* A相: 黒、緑
* B相: 赤、青

### 駆動時の注意点

* ドライバの選定: A4988やDRV8825などのバイポーラステッピングモータドライバが必要です。
* 脱調: 負荷が大きすぎたりパルス周波数が高すぎると、脱調（モーターが回転せず振動する）を起こすため、適切な電流調整（Vref）が必要です。
* 電圧: 通常12V〜24Vで動作します

## ドライバーボード (TB6600)


![配線](./https___qiita-image-store.s3.amazonaws.com_0_10836_d7b5b5b2-7029-c11b-09a1-57b3703b9601.avif)


## Arduino プログラム

* Stepperライブラリを利用
* 「ツール」-> 「ライブラリを管理...」
* Stepper をインストール

```arduino
#include <Stepper.h>

const int stepsPerRevolution = 2048;  // 1回転のステップ数
const int A = 4;
const int B = 6;
const int C = 5;
const int D = 7;

Stepper myStepper(stepsPerRevolution, A, B, C, D);

void setup() {
    myStepper.setSpeed(15);
}

void loop()
{
  myStepper.step(2048);
  delay(1000);
  myStepper.step(-2048);
  delay(1000);
}
```