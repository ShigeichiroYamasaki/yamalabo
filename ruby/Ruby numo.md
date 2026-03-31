# numo (NUmerical MOdules)

2026/03/31

Shigeichiro Yamasaki

## 概要

numo は python の numpy などに対応する数値処理ライブラリです

* numo-narray : 行列計算などの多次元配列の高速処理を目的にしたデータ処理ライブラリ
* numo-gnuplot : Ruby 言語用Gnuplot インターフェース
* numo-linalg :Numo::NArray を利用したBLAS/LAPACK 線形代数ライブラリ

## インストール

* MacOSX

```bash
brew install lapack
brew install openblas
```

```bash
gem update --system
gem update

gem install specific_install
gem specific_install https://github.com/ruby-numo/numo-narray.git
gem install numo-linalg -- --with-openblas-dir=/usr/local/opt/openblas
gem install numo-gnuplot 

gem install numo-pocketfft

```

## [Numo::NArray](https://github.com/ruby-numo/numo-narray?tab=readme-ov-file#numonarray)

[クラスライブラリの仕様](https://ruby-numo.github.io/numo-narray/yard/index.html)

[NArray日本語ドキュメント](https://github.com/ruby-numo/numo-narray/wiki/Numo::NArray-Overview-(Japanese))

### NArrayの型

Rubyの配列と違い、NArrayの配列には型があります

|整数| 	符号なし整数| 	小数 |	複素数|
|:--:|:--:|:--:|:--:|
|Numo::Int8| 	Numo::UInt8 |	Numo::SFloat 	|Numo::SComplex|
|Numo::Int16| 	Numo::UInt16| 	Numo::DFloat |	Numo::DComplex|
|Numo::Int32 |	Numo::UInt32 		|||
|Numo::Int64 	|Numo::UInt64 		|||


```ruby
require "numo/narray"
include Numo

i = Int32[1,2,3]

i
=> 
Numo::Int32#shape=[3]
[1, 2, 3]

a = [[0, 1], [2, 3]]
Int32[*a]

a
=> [[0, 1], [2, 3]]


 b=Int32[1..5]

 b
 => 
Numo::Int32#shape=[5]
[1, 2, 3, 4, 5]

b+1
=> 
Numo::Int32#shape=[5]
[2, 3, 4, 5, 6]

# 行列生成 3×3 の昇順値行列
c = Int32.new(3,3).seq

c
=> 
Numo::Int32#shape=[3,3]
[[0, 1, 2], 
 [3, 4, 5], 
 [6, 7, 8]]

# 行列へのスカラー加算
d = c+1

d
=> 
Numo::Int32#shape=[3,3]
[[1, 2, 3], 
 [4, 5, 6], 
 [7, 8, 9]]

# 行列の和
e = c+d

e
=> 
Numo::Int32#shape=[3,3]
[[1, 3, 5], 
 [7, 9, 11], 
 [13, 15, 17]]

# 行列の積 
f = c * d

f
=> 
Numo::Int32#shape=[3,3]
[[0, 2, 6], 
 [12, 20, 30], 
 [42, 56, 72]]

 # 単位行列の生成

i3 = Int32.eye(3,3)

i3
=> 
Numo::Int32#shape=[3,3]
[[1, 0, 0], 
 [0, 1, 0], 
 [0, 0, 1]]

# ゼロ行列

zero3 = Int32.zeros(3,3)

zero3
=> 
Numo::Int32#shape=[3,3]
[[0, 0, 0], 
 [0, 0, 0], 
 [0, 0, 0]]
```

### 数学関数 NMath

```ruby
data  = DFloat.linspace(-20,20,11)
data
=> 
Numo::DFloat#shape=[11]
[-20, -16, -12, -8, -4, 0, 4, 8, 12, 16, 20]

NMath.sin(data)
NMath.cos(data)
NMath.tan(data)
NMath.tanh(data)
NMath.atan(data)

```



### FFT (pocketfft) 高速フーリエ変換

* NumPy と同じ pocketfft の利用

```ruby
require 'numo/pocketfft'

a = Numo::DFloat.new(100).rand
a
=> 
Numo::DFloat#shape=[100]
[0.165089, 0.0508827, 0.108065, 0.0687079, 0.904121, 0.478644, 0.342969, ...]

# a のフーリエ変換
x = Numo::Pocketfft.rfft(a)
x
=> 
Numo::DComplex#shape=[51]
[43.8049+0i, 0.0968008+2.93079i, -1.12478+1.7676i, 0.450905-0.222611i, ...]

# x の逆フーリエ変換
Numo::Pocketfft.irfft(x)
=> 
Numo::DFloat#shape=[100]
[0.165089, 0.0508827, 0.108065, 0.0687079, 0.904121, 0.478644, 0.342969, ...]
```

## Numo::Gnuplot

```ruby
require 'numo/narray'
require 'numo/gnuplot'

# ステップ関数
def step(x)
  x > 0 # Numo::Bit を返す
end

# シグモイド関数
def sigmoid(x)
  1 / (1 + Numo::NMath.exp(-x)) # Numo::DFloat を返す
end

# ReLU (Rectified Linear Unit) 関数
def relu(x)
  y = Numo::DFloat[x] # コピー
  y[y < 0] = 0 # 0より小さい値の場合は0を代入する
  y
end

# データの作成
x = Numo::DFloat.new(100).seq(-5.0, 0.1)
y1 = step(x)
y2 = sigmoid(x)
y3 = relu(x)

# グラフの描画
g = Numo::gnuplot do
  set term: {png: {size: [480, 640]}} # 画像サイズ
  set output: 'ruby_graph.png'
  set title: 'Step, Sigmoid, ReLU' # タイトル
  set key: 'box left top'
  set xrange: -5.5...5.5 # x軸の範囲
  set yrange: -0.2...5.2 # y軸の範囲
  set offset: [0, 0, 0, 0]
  plot x, y1, {w: 'lines', lw: 3, title: 'Step'},
       x, y2, {w: 'lines', lw: 3, title: 'Sigmoid'},
       x, y3, {w: 'lines', lw: 3, title: 'ReLU'}
end
```