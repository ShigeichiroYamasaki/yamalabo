# raspberry pi DHT22 温度湿度センサー

## ３個所の温度湿度を記録

1. 屋根外気: roof
2. 室内: inside
3. 床下: floor

## DHT22

* DAT: GPIO
* VCC: 3.3V
* GND: グラウンド

## raspberry pi GPIO pin

![](https://qiita-user-contents.imgix.net/https%3A%2F%2Fwww.raspberrypi.com%2Fdocumentation%2Fcomputers%2Fimages%2FGPIO-Pinout-Diagram-2.png?ixlib=rb-4.0.0&auto=format&gif-q=60&q=75&s=24a05c59dc3e56961a5c25fbfafbcfc6)

### 使用するピン

* 1:  3.3V
* 3: GPIO2
* 5: GPIO3
* 6:  GND
* 7: GPIO4

### センサーとのピン接続

* roof ->   1,6,3 (GPIO2)
* inside -> 1,6,5 (GPIO3)
* floor ->  1,6,7 (GPIO4)

## raspbeery pi のセットアップ

```bash
sudo apt install git pip
sudo apt update
sudo apt install python3-pip
sudo python3 -m pip install --upgrade pip setuptools wheel
```

### DHT ドライバ

```bash
sudo pip3 install Adafruit_DHT

sudo git clone https://github.com/adafruit/Adafruit_Python_DHT.git
cd Adafruit_Python_DHT
sudo python setup.py install
```

#### Adafruit_DHTモジュールの基本的な使い方

* SENSOR_TYPE = DHT.DHT22
* DHT_GPIO = 2 # (GPIO2) pin は 3

```python
import Adafruit_DHT as DHT
humid,temp = DHT.read_retry(SENSOR_TYPE, DHT_GPIO)
```

## プログラム例

```python
import Adafruit_DHT as DHT
import statistics as st
import datetime

#センサータイプを指定
SENSOR_TYPE=DHT.DHT22
#接続したGPIOピンを指定
DHT_GPIO_roof=2
DHT_GPIO_inside=3
DHT_GPIO_floor=4
# 温度と湿度の測定
dt = datetime.datetime.now()
t_roof,h_roof     = DHT.read_retry(SENSOR_TYPE,DHT_GPIO_roof)
t_inside,h_inside = DHT.read_retry(SENSOR_TYPE,DHT_GPIO_inside)
t_floor,h_floor   = DHT.read_retry(SENSOR_TYPE,DHT_GPIO_floor)
# 出力

print("屋根 "+dt+":温度:"+t_roof+",湿度:"+h_roof)
print("室内 "+dt+":温度:"+t_inside+",湿度:"+h_inside)
print("床下 "+dt+":温度:"+t_floor+",湿度:"+h_floor)

```