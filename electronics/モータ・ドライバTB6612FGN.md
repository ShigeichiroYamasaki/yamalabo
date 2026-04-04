# モータ・ドライバ TB6612FGN

## 接続

* AIN1 = 11
* AIN2 = 7
* PWMA = 33

* BIN1 = 29
* BIN2 = 31
*PWMB = 35


## プログラム

```python
import RPi.GPIO as GPIO

#timeライブラリをインポート
import time

#ピン番号の割り当て方式を「コネクタのピン番号」に設定
GPIO.setmode(GPIO.BOARD)

#使用するピン番号を代入
AIN1 = 15
AIN2 = 13
PWMA = 33

BIN1 = 29
BIN2 = 31
PWMB = 35

#各ピンを出力ピンに設定
GPIO.setup(AIN1, GPIO.OUT, initial = GPIO.LOW)
GPIO.setup(AIN2, GPIO.OUT, initial = GPIO.LOW)
GPIO.setup(PWMA, GPIO.OUT, initial = GPIO.LOW)

GPIO.setup(BIN1, GPIO.OUT, initial = GPIO.LOW)
GPIO.setup(BIN2, GPIO.OUT, initial = GPIO.LOW)
GPIO.setup(PWMB, GPIO.OUT, initial = GPIO.LOW)

#PWMオブジェクトのインスタンスを作成
#出力ピン：12,26  周波数：100Hz
p_a = GPIO.PWM(PWMA,100)
p_b = GPIO.PWM(PWMB,100)

#PWM信号を出力
p_a.start(0)
p_b.start(0)

#デューティを設定(0~100の範囲で指定)
#速度は80%で走行する。
val = 80

#デューティ比を設定
p_a.ChangeDutyCycle(val)
p_b.ChangeDutyCycle(val)

#ブレーキする関数
def func_brake():
    GPIO.output(AIN1, GPIO.HIGH)
    GPIO.output(AIN2, GPIO.HIGH)
    GPIO.output(BIN1, GPIO.HIGH)
    GPIO.output(BIN2, GPIO.HIGH)


#前進する関数
def func_forward():
    GPIO.output(AIN1, GPIO.LOW)
    GPIO.output(AIN2, GPIO.HIGH)
    GPIO.output(BIN1, GPIO.LOW)
    GPIO.output(BIN2, GPIO.HIGH)


#後進する関数
def func_back():
    GPIO.output(AIN1, GPIO.HIGH)
    GPIO.output(AIN2, GPIO.LOW)
    GPIO.output(BIN1, GPIO.HIGH)
    GPIO.output(BIN2, GPIO.LOW)


#右回転する関数
def func_right():
    GPIO.output(AIN1, GPIO.LOW)
    GPIO.output(AIN2, GPIO.HIGH)
    GPIO.output(BIN1, GPIO.HIGH)
    GPIO.output(BIN2, GPIO.LOW)


#左回転する関数
def func_left():
    GPIO.output(AIN1, GPIO.HIGH)
    GPIO.output(AIN2, GPIO.LOW)
    GPIO.output(BIN1, GPIO.LOW)
    GPIO.output(BIN2, GPIO.HIGH)


#メインプログラム
while True:
    #3秒前進する
    func_forward()
    time.sleep(3.0)
    func_brake()
    time.sleep(1.0)
    func_right()
    time.sleep(2.0)
    func_brake()
    time.sleep(1.0)
    func_left()
    time.sleep(2.0)
    func_brake()
    time.sleep(1.0)
    #3秒後進する
    func_back()
    time.sleep(3.0)
    #3秒ブレーキ
    func_brake()
    time.sleep(3.0)
    break

#PWM信号を停止
p_a.stop()
p_b.stop()

#GPIOを開放
GPIO.cleanup()

#プログラム終了
print("End of program")

```