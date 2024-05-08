# 暗号の数学

最終更新 2024/05/07
Shigeichro Yamasaki

## 事前準備

JavaScript 言語

mochaによるテスト駆動開発


## JavaScript 関数の定義

関数の再帰的定義とイテレータによる定義を説明します．

### ループ制御による関数定義の問題

ループ制御などの手続き的に定義された関数は，バグがないことを確認することが難しいという問題があります．一般的に典型的なデータでテストするだけではバグが無いことを証明することはできません．

数学的な意味や構造を保った形で関数を定義できるとバグが無いことを数学的に証明できるようになります．

そのために，再帰的定義やイテレータを使った定義が有効になることが多いです．

### 関数定義 ＝ 自分がやっている一連の手続きを対象化し名前をつけること

まず，自分がやっていることを外から見る視点を持ってみましょう．
そして，そのやっていること（操作）に名前をつけてみましょう．
関数とは，そのような操作や処理の中身を「箱の中に入れて」抽象化し，入力と出力だけの関係にしたものです．

### 再帰的定義

繰り返し構造を持った関数の定義方法の一つが再帰的定義です．
再帰とは，自分で自分を呼び出すことです．
関数を定義するときに，その中に自分を含めることで繰り返しを表現します．
再帰的定義には，ループ制御と同じように停止条件が必要です．
関数の再帰的定義は，プログラムの動作の視点だけでなく，数学的帰納法による関数の定義（宣言的定義と呼ばれます）と結びつけることができるので，仕様の正しさが証明できるようになります．


### イテレータによる定義

コンピュータが扱うデータ（オブジェクト）は，それ自身が繰り返し構造持っていることが多いです．配列，文字列，ハッシュ，辞書，テキストファイルの行などが代表例です．

### 階乗関数の例

関数の再帰的定義を使う方法

```js
const fact=(n)=>{
  if(n==0) {
    return 1;
  } else {
    return n*fact(n-1);
  }
}


> fact(10)
3628800
> fact(50)
3.0414093201713376e+64
```

Bigint版

```js
const factB=(n)=>{
  if(n==0n) {
    return 1n;
  } else {
    return n*factB(n-1n);
  }
}
> factB(50n)
30414093201713378043612608166064768844377641568960512000000000000n
```

### JavaScriptの範囲オブジェクト

javaScriptには，pythonやRubyの範囲オブジェクトに対応するものが無いので，次のようにして配列を生成する関数として作成することができる

```js
// 0から n-1 の範囲
> const range0 = (n=>{return [...Array(n).keys()]})
> range0(3)
[ 0, 1, 2 ]

// 開始と終了の範囲
> const range = ((from,to) =>{return [...Array(to-from+1)].map((_,i)=>i+from)})
> range(7,20)
[
   7,  8,  9, 10, 11, 12,
  13, 14, 15, 16, 17, 18,
  19, 20
]
```

イテレータに対する reduce 操作を使う方法

```js
const factR=(n)=>{return range(1,n).reduce((x,y)=>x*y,1);}

factR(20)
2432902008176640000
```

### 問題

* 次の集合を実際に作成してください

{x | x < 10 かつ xは素数のべき乗}

* 次の直積集合の外延をタプルで表現してください

{s1, s2, s3} ☓ {e1, e2}


### 課題

* 次の集合の外延を出力するプログラムを作成してください

{x | x < 100 かつ xは素数のべき乗}

#### python

```python
# lim 未満の素数の冪乗
def pow_prime(p,lim):
    list=[]
    e=1
    pp = p**e
    while pp < lim:
        list.append(pp)
        e +=1
        pp = p**e
    else:
        return list
        
#100未満の素数のリスト
primes=[2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]

#100未満の素数のべき乗のリスト
power_prime = sorted(sum(list(map(lambda x: pow_prime(x,100), primes)),[]))

=> [2, 3, 4, 5, 7, 8, 9, 11, 13, 16, 17, 19, 23, 25, 27, 29, 31, 32, 37, 41, 43, 47, 49, 53, 59, 61, 64, 67, 71, 73, 79, 81, 83, 89, 97]
```

#### Ruby

ループで実施

```ruby
# lim未満の素数の冪乗
def pow_prime(p,lim)
  list=[]
  e=1
  pp=p**e
  while pp < lim
    list<<pp
    e +=1
    pp=p**e
  end
  return list
end

#100未満の素数のリスト
primes=[2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]

#100未満の素数のべき乗のリスト
power_prime = primes.map{|p|pow_prime(p,100)}.flatten.sort

=> [2, 3, 4, 5, 7, 8, 9, 11, 13, 16, 17, 19, 23, 25, 27, 29, 31, 32, 37, 41, 43, 47, 49, 53, 59, 61, 64, 67, 71, 73, 79, 81, 83, 89, 97]
```

map filter で実施

```ruby
#100未満の素数のリスト
primes=[2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]

def power_prime(primes,lim)
    primes.map{|p|(1..((Math.log2 100).to_i)+1).map{|e|p**e}}.flatten.select{|n|n<lim}.sort
end

power_prime(primes,100)
=> [2, 4, 8, 16, 32, 64, 3, 9, 27, 81, 5, 25, 7, 49, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]
```

#### javascript

```javascript
// lim未満の素数の冪乗
function pow_prime(p,lim) {
  let list=[];
  let e=1;
  let pp=p**e;
  while (pp < lim) {
    list.push(pp);
    e++;
    pp=p**e;
  }
  return list
}

// 100未満の素数のリスト
let primes=[2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97];

// 100未満の素数のべき乗のリスト（★javaScriptのsort()は，辞書順序が基本）
let power_prime = primes.map(p=>pow_prime(p,100)).flat().sort((a, b) => a - b);

=> [2, 3, 4, 5, 7, 8, 9, 11, 13, 16, 17, 19, 23, 25, 27, 29, 31, 32, 37, 41, 43, 47, 49, 53, 59, 61, 64, 67, 71, 73, 79, 81, 83, 89, 97]
```


### 課題

組み合わせ関数の再帰的定義を実際に実装して
n=10 のパスカルの三角形を出力するプログラムを作成してください

#### python

```python
def combi(n,r):
    if n==r or r==0:
        return(1)
    else:
        return(combi(n-1,r) + combi(n-1,r-1))


def pascal(m):
    list(map(lambda n: print(list(map(lambda r: combi(n,r), (range(0,n+1))))), range(0,m+1)))

pascal(10)
[1]
[1, 1]
[1, 2, 1]
[1, 3, 3, 1]
[1, 4, 6, 4, 1]
[1, 5, 10, 10, 5, 1]
[1, 6, 15, 20, 15, 6, 1]
[1, 7, 21, 35, 35, 21, 7, 1]
[1, 8, 28, 56, 70, 56, 28, 8, 1]
[1, 9, 36, 84, 126, 126, 84, 36, 9, 1]
[1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1]
```


#### Ruby

```ruby
def combi(n,r)
  if n==r or r==0 then 1
  else
     combi(n-1,r) + combi(n-1,r-1)
  end
end

(0..10).map{|n|p (0..n).map{|r|combi(n,r)}}
[1]
[1, 1]
[1, 2, 1]
[1, 3, 3, 1]
[1, 4, 6, 4, 1]
[1, 5, 10, 10, 5, 1]
[1, 6, 15, 20, 15, 6, 1]
[1, 7, 21, 35, 35, 21, 7, 1]
[1, 8, 28, 56, 70, 56, 28, 8, 1]
[1, 9, 36, 84, 126, 126, 84, 36, 9, 1]
[1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1]

```

Ruby でメモ化を利用した高速化

```ruby
def combi_memo(memo,n,r)
	memo[n]||={}
	memo[n-1]||={}
	if n==r or r==0 then
		memo[n][r]=1
	else
		memo[n-1][r]||=combi_memo(memo,n-1,r)
		memo[n-1][r-1]||=combi_memo(memo,n-1,r-1)
		memo[n][r]||=memo[n-1][r]+memo[n-1][r-1]
	end
end

memo={}

(0..100).map{|n|p (0..n).map{|r|combi_memo(memo,n,r)}}
```

#### javascript

```javascript
const combi=(n,r)=>{
  if (n==r || r==0) {
   return 1;
  } else {
     return combi(n-1,r) + combi(n-1,r-1);
  }
}

const range = (n)=> (Array.from(Array(n).keys()))

range(10).map(n => range(n+1).map(r => combi(n,r)))

[
  [ 1 ],
  [ 1, 1 ],
  [ 1, 2, 1 ],
  [ 1, 3, 3, 1 ],
  [ 1, 4, 6, 4, 1 ],
  [ 1, 5, 10, 10, 5, 1 ],
  [
     1, 6, 15, 20,
    15, 6,  1
  ],
  [
     1,  7, 21, 35,
    35, 21,  7,  1
  ],
  [
     1,  8, 28, 56, 70,
    56, 28,  8,  1
  ],
  [
      1,  9, 36, 84, 126,
    126, 84, 36,  9,   1
  ]
]

```

### 課題

縦棒３本のあみだくじを一つ作成します。それをfとします。
（どのように横線を書いてもいいです）
同一のf を6回合成したもの f○f○f○f○f○f を作成してください
 f○f○f○f○f○f ＝ e　となることを確認してください 

#### python

```python
def f1(a):
    return [a[0],a[2],a[1]]

def f2(a):
    return [a[1],a[0],a[2]]

def f3(a):
    return [a[1],a[2],a[0]]

def f4(a):
    return [a[2],a[0],a[1]]

def f5(a):
    return [a[2],a[1],a[0]]

a=['a','b','c']

f1(a)
["a", "c", "b"]

f2(a)
["b", "a", "c"]

f3(a)
["b", "c", "a"]

f4(a)
["c", "a", "b"]

f5(a)
["c", "b", "a"]

f1(f1(f1(f1(f1(f1(a))))))
=> ["a", "b", "c"]

f2(f2(f2(f2(f2(f2(a))))))
=> ["a", "b", "c"]

f3(f3(f3(f3(f3(f3(a))))))
=> ["a", "b", "c"]

f4(f4(f4(f4(f4(f4(a))))))
=> ["a", "b", "c"]

f5(f5(f5(f5(f5(f5(a))))))
=> ["a", "b", "c"]
```

#### Ruby

```ruby
def f1(a) [a[0],a[2],a[1]] end
def f2(a) [a[1],a[0],a[2]] end
def f3(a) [a[1],a[2],a[0]] end
def f4(a) [a[2],a[0],a[1]] end
def f5(a) [a[2],a[1],a[0]] end

a=['a','b','c']
 f1(a)
=> ["a", "c", "b"]
 f2(a)
=> ["b", "a", "c"]
 f3(a)
=> ["b", "c", "a"]
 f4(a)
=> ["c", "a", "b"]
 f5(a)
=> ["c", "b", "a"]

f1(f1(f1(f1(f1(f1(a))))))
=> ["a", "b", "c"]
f2(f2(f2(f2(f2(f2(a))))))
=> ["a", "b", "c"]
f3(f3(f3(f3(f3(f3(a))))))
=> ["a", "b", "c"]
f4(f4(f4(f4(f4(f4(a))))))
=> ["a", "b", "c"]
f5(f5(f5(f5(f5(f5(a))))))
=> ["a", "b", "c"]

```

#### javascript

```javascript
const f1=(a)=> [a[0],a[2],a[1]];
const f2=(a)=> [a[1],a[0],a[2]];
const f3=(a)=> [a[1],a[2],a[0]];
const f4=(a)=> [a[2],a[0],a[1]];
const f5=(a)=> [a[2],a[1],a[0]];

const a=['a','b','c'];
 f1(a)
=> ["a", "c", "b"]
 f2(a)
=> ["b", "a", "c"]
 f3(a)
=> ["b", "c", "a"]
 f4(a)
=> ["c", "a", "b"]
 f2(a)
=> ["c", "b", "a"]

f1(f1(f1(f1(f1(f1(a))))))
=> ["a", "b", "c"]
f2(f2(f2(f2(f2(f2(a))))))
=> ["a", "b", "c"]
f3(f3(f3(f3(f3(f3(a))))))
=> ["a", "b", "c"]
f4(f4(f4(f4(f4(f4(a))))))
=> ["a", "b", "c"]
f5(f5(f5(f5(f5(f5(a))))))
=> ["a", "b", "c"]

```
	
## 02　線形代数

*  線型関数
* ベクトル
* 内積
* 単位ベクトル
*  線形独立と次元
* 行列
* 行列の足し算
* ０行列
* 行列の掛け算
* 単位行列
* 逆行列
* 行列式

### 課題

#### python

```python
# ベクトルの和、差、スカラー倍
a=[3,5,1,-2,0,5,1,1]
b=[6,2,-9,3,1,7,2,9]
k=10

# ベクトルの和
def vector_sum(a,b):
    return list(map(sum,zip(a,b))

# ベクトルの差
def vector_diff(a,b):
    c=map(lambda x: -x, b)
    return vector_sum(a,c)

# ベクトルのスカラー倍
def vector_scalar(k,a):
    return list(map(lambda x: k*x, a))

vector_sum(a,b)
vector_diff(a,b)
vector_scalar(k,a)
```

#### Ruby

```ruby
# ベクトルの和、差、スカラー倍
a=[3,5,1,-2,0,5,1,1]
b=[6,2,-9,3,1,7,2,9]
k=10

def vector_sum(a,b)
	a.zip(b).map{|x,y|x+y}
end 

def vector_diff(a,b)
	a.zip(b).map{|x,y|x-y}
end 

def vector_scalar(k,a)
	a.map {|x|k*x}
end 

vector_sum(a,b)
vector_diff(a,b)
vector_scalar(k,a)
```

### 行列の転置

```ruby
# 3,5行列の場合
m=[[1,1,1,1,1],[2,2,2,2,2],[3,3,3,3,3]]

def transpose(m)
	r=m.size
	c=m[0].size
	(0..(c-1)).map do |i|
		(0..(r-1)).map{|j|m[j][i]}
	end
end

transpose(m)
=> [[1, 2, 3], [1, 2, 3], [1, 2, 3], [1, 2, 3], [1, 2, 3]]
```
ベクトルの転置

```ruby
# 行ベクトルを1☓n 行列と考える
v=[[1,2,3,4,5]]

transpose(v)
=> [[1], [2], [3], [4], [5]]
```

## ベクトルの内積


```ruby
v1=[3,5,1]
v2=[6,2,9]

def inner_product(a,b)
    a.zip(b).map{|x,y|x*y}.sum
end

a=[1,0,0,1,1,0,1,0,0,0,0,0,0,0,0,1]
b=[1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0]


p=inner_product(v1,v2)
p
#=> 37
```

## 行列の和、差、スカラー倍



```ruby
m1=[[1,2],[3,4]]
m2=[[5,6],[7,8]]

def matrix_sum(a,b)
    a.zip(b).map do |x,y|
        x.zip(y).map do |u,v|
            u+v
        end
    end
end
def matrix_diff(a,b)
    a.zip(b).map do |x,y|
        x.zip(y).map do |u,v|
            u-v
        end
    end
end


m=matrix_sum(m1,m2)
m
#=> [[6, 8], [10, 12]]
```
## 行列の積

```ruby
m1=[[1,2],[3,4]]
m2=[[5,6],[7,8]]

def matrix_product(a,b)
	a.map do |x|
		b.transpose.map do |y|
			inner_product(x,y)
		end
	end
end


m3=matrix_product(m1,m2)
m3

e3=[[1,0,0,],[0,1,0],[0,0,1]]
m4=[[1,2,3],[3,4,5],[5,6,7]]

m5=matrix_product(e3,m4)
m5

```

```ruby
# n次の単位行列の生成

def unit(n)
	(1..n).map do |i|
		(1..n).map do |j|
		    if i==j then 1
		    else 0
		    end
	    end
	end
end

def matrix_pow(m,e)
	u=unit(m[0].size)
	e.times do
		u=matrix_product(u,m)
	end
	return u
end

f1=[[1,0,0],[0,0,1],[0,1,0]]
f2=[[0,1,0],[1,0,0],[0,0,1]]
f3=[[0,1,0],[0,0,1],[1,0,0]]
f4=[[0,0,1],[1,0,0],[0,1,0]]
f5=[[0,0,1],[0,1,0],[1,0,0]]

matrix_pow(f1,2)
```

```ruby
def inv(m)
	a,b,c,d=m[0][0],m[0][1],m[1][0],m[1][1]
	minv=[]
	minv[0]=[]
	minv[1]=[]
	f=1.0/(a*d - b*c)
	minv[0][0]=f*d
	minv[0][1]=-f*b
	minv[1][0]=-f*c
	minv[1][1]=f*a
	minv
end
```
    
##03 整数論1


* 合同式
* 剰余類
* 一方向関数
* 組み合わせ爆発
* RSA暗号

```ruby
p=41
q=89
n=p*q
e=3
f=(p-1)*(q-1)
k=2
d=(f*k+1)/e
# メッセージ
m=1010
#　暗号化
c=m**e %n

# 復号化
p=c**d %n
p
=> 1010

```


### 再帰プログラム　gcd

```ruby
def gcd(a,b)
  if a%b==0 then b
  else  gcd(b,a%b)
  end
end

# 実行
gcd(5355,924)
# => 21
```

* ディオファントス方程式
	* ベズー方程式
* 拡張ユークリッドの互除法

p=11
q=13
r=17


```ruby
def egcd(a,b)							
  if b==0 then [0,1,a]  # 再帰終了a*0+b*1=b 
  else
    xp,yp,d = egcd(b,a%b) # 再帰     bx'+a%by'=d
    [yp,xp-(a/b)*yp,d]    # 復帰処理 x=y', y=x'-(a/b)*y'
  end
end
```
javascript
```js
const egcd=(a,b)=>{
  if (b==0) {return [0,1,a];}
  else {
    r= egcd(b,a%b);
    xp=r[0];
    yp=r[1];
    d=r[2];
    return [yp,xp-(a/b)*yp,d]
  }
}
```


* フェルマーの小定理

* オイラーのΦ関数

* 中国剰余定理

(b*s*p + a*t*q) mod p*q = x mod p*q


```ruby
def crt(a,b,p,q)
   s,t,d=egcd(p,q)
   (b*s*p+a*t*q)%(p*q)
end


# 実行
crt(5,3,7,11)
#=> 47

47%7
#=>5
47%11
#=> 3
```

* オイラーの定理






* 順列と組み合わせ
* 遅延評価プログラム

* 素因数分解
* エラトステネスのふるい
* 生成・テスト（ジェネレート・アンド・テスト）法と遅延評価プログラム


##04  整数論2

* フェルマーの小定理
* オイラーの定理
* 平方剰余の相互定理
* 素因数分解
* 冪乗計算
* 離散対数問題
* 誕生日問題
* ρ法による素因数分解問題への攻撃
* ρ法による離散対数への攻撃

##05 ガロア理論

* 群の例
* ハッセ図
* 巡回群
* コセット分割
* 有限体
* １のn乗根
* ユークリッドの互除法と有限体の逆元
* フェルマーの小定理と有限体の逆数
* 有限体上の多項式
* ガロア拡大
* ガロア群とガロア体
* 拡大体の元の逆元


##06 楕円曲線

* 楕円曲線
* トーラス上の関数
* 楕円離散対数問題
* 楕円DHと楕円ElGamal暗号
* 有限体上の楕円曲線
* ワイエルシュトラウスの$\wp$関数
* ペアリング
* IDベース暗号
* エドワード曲線

##07 確率論

* 確率事象
* 確率変数
* 確率分布
* 期待値
* 分散
* モーメント
* 大数の法則
* 正規分布
* ポアソン分布
* カイ二乗分布
* 推定と検定
* t検定
* シャピロ・ウィルク検定
* シミュレーション

##08 シャノン理論

* 情報源と通信路のモデル
* 情報源のエントロピー
* シャノンの符号化定理
* 最小距離
* 線形符号
* ハミング符号
* リードソロモン符号
