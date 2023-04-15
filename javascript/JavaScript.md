# JavaScript言語入門

2023/04/14 Shigeichiro Yamasaki

## インストール

node.jsをインストールしておく

ubuntu 22.04LTS

```bash
sudo apt update
sudo apt install curl
curl -sL https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list
sudo apt install yarn -y

curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash - 
sudo apt-get install gcc g++ make
sudo apt-get install -y nodejs
sudo apt install -y npm

yarn add node-fetch@2.6.6
yarn add -D @types/node-fetch@2.x
```

MacOSX

```bash
brew install nodejs
brew install yarn 
brew install npm

yarn add node-fetch@2.6.6
yarn add -D @types/node-fetch@2.x
```

## node.jsのインタープリタの起動と終了

起動

```bash
$ node
Welcome to Node.js v16.13.2.
Type ".help" for more information.
> 
```

終了

(コントロールｄ）


## 文法

### 式

セミコロンで終わるのが基本
改行で代替できる

```js
> 1+2;
3
> a=3; b=4
> c=5
```

### コメント

```js
// 1行のコメント
/* ここから
ここまでがコメント */
```

## 予約語

以下は予約語なので変数名などの識別子として利用することができない

```js
let
static
implements
interface
package
private
protected
public
await
break
case
catch
class
const
continue
debugger
default
delete
do
else
enum
export
extends
false
finally
for
function
if
import
in
instanceof
new
null
return
super
switch
this
throw
true
try
typeof
var
void
while
with
yield
enum
```

## 型

### 基本型

イミュータブル（生成後，変更することができないオブジェクト）

* 数値
* 文字列
* 論理値
* null, undefined
* Symbol

### オブジェクト型

プロパティ（名前と値を持ち，値として基本型やオブジェクトを保持できる）の集合体
ミュータブル（生成後，変更することが可能）

* 配列 (Array)
* Set （集合 同じ要素は同一になる）
* Map （キーと値の対の集まり）
* RegExp (正規表現 パターマッチング)
* Date （日付と時間）
* 関数
* クラス

## リテラル

### 数値

64ビット浮動小数点形式が基本

* 整数リテラル

```js
// 10進整数
> 123
// 16進整数
> 0xCAFEBEEF
// 2進数整数
> 0b1010110

// _ を使って区切ってよい
> 12_233_111==12233111
true
```

* 浮動小数点リテラル

10進数の指数表記

```js
> 3.14
> 6.02e+23
> 2e-123
```

### 数値演算

```js
> 1+1
2
> 3*2
6
> 2**8
256
> 2**125
4.253529586511731e+37
> 2.0+3
5
> 2-0.5
1.5
> 6/5
1.2
> 15%4
3
> (2**125)%3
2
> (2+5)*7
49
```

特殊な定数

```js
> 1/0
Infinity
> -1/0
-Infinity
> Infinity+1
Infinity
> "a"/2
NaN     // Not a number
```

### 数学的演算

Math オブジェクトのプロパティを利用して計算する

```js
> Math.pow(2,128)   // べき乗
> Math.random()     // 0.0~1.0 の乱数生成
> Math.PI           // 円周率
> Math.E            // 自然対数の底
> Math.sqrt(2)      // 平方根
> Math.sin(Math.PI/2) // sin
> Math.cos(Math.PI/2) // cos
> Math.log(Math.E)  // 自然対数
> Math.log10(1000)  // 常用対数
> Math.log2(2048)   // 底が2の対数
```

### BigInt （任意精度整数）

数値の最後に小文字のnをつける
16進数，8進数，2進数でも同様
BigInt の計算に普通の数値リテラルを混在させることはできない

```js
> 123456n
> 0xcafebeefn
> 0b101101010111n
> 10n/3n
3n

> 2n**1024n
179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137216n

> 3n/2
Uncaught TypeError: Cannot mix BigInt and other types, use explicit conversions
```

## 文字リテラル

Unicodeの文字

' または " または ` で囲まれた文字列


```js
> "hello"
'hello'
> "近畿大学"
'近畿大学'
> "近畿大学 123"
'近畿大学 123'
> 'hello'+'world'
'helloworld'
> 'hello "hello" hello'
'hello "hello" hello'
> "hello 'hello' hello"
"hello 'hello' hello"
```

`（バッククート）はテンプレートリテラルと呼ばれ，改行を含む複数行テキストや${}によるJavaScript式の展開が可能になります

```
> "I am ${10*6} years old"
'I am ${10*6} years old'

> `I am ${10*6} years old`
'I am 60 years old'

> 
```

エスケープシーケンス

\ に続けて文字を記述することで ' や " など文字列内に記述できない文字を記述できる

```js
> "hello \"hello\" hello"
'hello "hello" hello'

// \n 改行記号
// \t タブ
// \r 復帰記号
> "hello \n hello"
```

### 文字リテラルへの操作

```js
> "hello"+"world"
'helloworld'
> "hello".length
5
> "近畿大学".length
4
>  "近畿大学123".length
7
```

## 論理値

tureとfalseの2つの真偽値

```js
> true
true
> false
false
> 2>1
true
> 2<1
false
```

```js
> 1==1
true
> 1!=1
false
> 1>2
false
> 1<=2
true
> 1>=2
false
> 1<2 && 3<5
true
> 1<2 && 3==5
false
> 1<2 && 3!=5
true
> 1<2 || 3==5
true
> 1<(1+1) || 3==5
true
> "b">"z"
false
> "b"<"z"
true
```

JavaScriptは自動的に型変換を行う
== による比較は自動型変換の結果で比較される．
=== による比較は自動型変換を行わない．

```js
> 4==4n
true
> 4===4n
false

> "4"==4
true
> "4"===4
false
```

## 代入

### 変数へのバインディング

* ローカル変数の宣言は let を使用
* let で宣言された変数のスコープはブロックスコープ（{ } で囲まれた範囲のこと）
* let 同じ名前の変数をスコープ内で複数回宣言することができません.スコープが異なれ再宣言ば可能です．

```js
> let x=2;
undefined
> x
2
> x*10
20
> let x=100
Uncaught SyntaxError: Identifier 'x' has already been declared
> x=100

> let c=1
> if (c==1) {
    let c=2
    console.log(c)
}

2
```

未定義の変数への操作はエラーになります

```js
> xx
Uncaught ReferenceError: xx is not defined
```

変数への再バインディング

```js
> x
2
> x=100
100
> x
100
```

古い仕様である var による変数は複数回宣言できますが，望ましくありません
トップレベルで var で宣言されば変数のスコープはグローバルスコープになります

```js
> var y=1
> var y=2
```

### 定数定義

```js
> const z=5;
undefined
> z
5
```

定数は再バインディングできません

```js
> z=1000
Uncaught TypeError: Assignment to constant variable.
```

## 変数名の文法

* アルファベットの大文字でも小文字でも使えます
* 数字から始まってはいけません
* $ と _ は使えますが、それ以外の句読点や特殊文字は使えません
* 予約語は使えません


## 標準出力への出力

print文にあたるもの

```js
> console.log('hello')
hello
undefined

```


## 配列

### 配列リテラルによる配列の生成

```js
> let arr=[1,2,3,4,5,6];
> arr
[ 1, 2, 3, 4, 5, 6 ]

> arr1=[]
[]
> arr1
[]

> let arr2=[1,,,,,,'hello']
> arr2
[ 1, <5 empty items>, 'hello' ]
```

### Array() コンストラクタによる配列の生成

```js
> let arr3=new Array()
undefined
> arr3
[]
> let arr4=new Array(5)
> arr4
[ <5 empty items> ]
```


### インデックスによる配列要素へのアクセス

```js
> arr[0]
1
> arr[1]
2
> arr[3]
4
```

### 配列への操作

```js
// 長さ
> arr.length;
6

// ソート
> arr=[9,3,1,7,8,3,5];
> arr.sort()
[
  1, 3, 3, 5,
  7, 8, 9
]

> arr.sort().reverse()
[
  9, 8, 7, 5,
  3, 3, 1
]

// フラット化
> arr = [[1,2],[3,4]]
> arr.flat()
[ 1, 2, 3, 4 ]

// プッシュ
> arr=[1,3,5]
> arr.push(7)
4
> arr
[ 1, 3, 5, 7 ]

// ポップ
> arr.pop()
7
> arr
[ 1, 3, 5 ]

// シフト
> arr.shift()
1
> arr
[ 3, 5 ]

// アンシフト
> arr.unshift(100)
3
> arr
[ 100, 3, 5 ]

// データの削除
> delete arr[1]
true
> arr
[ 100, <1 empty item>, 5 ]
> arr.length
3
```

### スプレッド演算子

... は配列リテラルの展開を意味する
配列リテラルの中に配列要素を入れることに利用できる

```js
> let a=[1,2,3]
> let b=[0, ...a,4]
> b
[ 0, 1, 2, 3, 4 ]

// 多重代入での利用
> [car, ...cdr]=[1,2,3,4,5,6]
[ 1, 2, 3, 4, 5, 6 ]
> car
1
> cdr
[ 2, 3, 4, 5, 6 ]
```

## オブジェクト

### オブジェクトの生成

```js
// オブジェクトリテラルによるオブジェクトの生成
> let obj={}
> obj
{}

> let point={x: 12, y: 333}
> point
{ x: 12, y: 333 }

// new によるオブジェクトの生成

> let obj0 = new Object()
> obje0
{}

> let arr0 = new Array()
> arr0
[]

> let map0 = new Map()
> map0
Map(0) {}

> let set0 = new Set()
> set0
Set(0) {}

> let date0 = new Date()
> date0
2023-04-13T01:56:38.510Z
```

### オブジェクトのプロパティへのアクセス

JavaScriptのオブジェクトにはプロパティという情報が格納されています．
プロパティの読み出しと書き込みの方法には，メソッド型と連想配列型の2種類があります．

```js
> point
{ x: 12, y: 333 }

// プロパティの読み出し
> point.x
12
> point.y
333

> point['x']
12
> point['y']
333

// プロパティの書き込み
> point.x=10000
10000
> point.y=9999
9999
> point
{ x: 10000, y: 9999 }

> point['x']=5555
5555
> point['y']=88888
88888
> point
{ x: 5555, y: 88888 }

```

### Map オブジェクト

キーに対応付けられた値にアクセスするためのオブジェクト
マップは高速なので，処理の高速化手法として有効
キーには任意のオブジェクトを利用できる

```js
> let map1=new Map()
undefined

// set
> map1.set(1,1)
Map(1) { 1 => 1 }
> map1.set(2,1)
Map(2) { 1 => 1, 2 => 1 }
> map1.set(3,2)
Map(3) { 1 => 1, 2 => 1, 3 => 2 }
> map1
Map(3) { 1 => 1, 2 => 1, 3 => 2 }

// get
> map1.get(2)
1
> map1.get(3)
2

// keys()
> map1.keys()
[Map Iterator] { 1, 2, 3 }

// スプレッド演算子を利用してキーの配列をつくる
> [...map1.keys()]
[ 1, 2, 3 ]

// values()
> map1.values()
[Map Iterator] { 1, 1, 2 }

// スプレッド演算子を利用して値の配列をつくる
> [...map1.values()]
[ 1, 1, 2 ]

// clear
> map1.clear()
undefined
> map1
Map(0) {}

// キーと値の対応が定義されているか？
> map1.has(1)
true
> map1.has(4)
false
```

### プロトタイプ・オブジェクトから新しいオブジェクトを生成

javaScript では，プロトタイプを使って新しいオブジェクトを生成できる．
生成されたオブジェクトは，プロトタイプのプロパティを継承する

```js
> point
{ x: 5555, y: 88888 }
> let p1=Object.create(point)

// 生成されたオブジェクト自体にはプロパティはまだ存在していない
> p1
{}

// 継承によるプロパティへのアクセス（プロトタイプのプロパティにアクセスできる）
> p1.x
5555
> p1.y
88888

// 生成されたオブジェクトのプロパティの更新
> p1.x=0
0
> p1
{ x: 0 }

// p1 への書き込みは プロトタイプの pointには影響しない
> point
{ x: 5555, y: 88888 }
```

### プロパティのテスト

オブジェクトがそのプロパティを持っているかテストできる

```js
> p1
{ x: 0 }
> 'y' in p1
true
```

### オブジェクトの型

```js
> typeof 1
'number'
> typeof 1.1
'number'
> typeof 'aaa'
'string'
> typeof [1,2,3]
'object'
> typeof  { '1': 1, '2': 1, '3': 2 }
'object'
> typeof true
'boolean'
> typeof null
'object'
> typeof Infinity
'number'
```


## 制御構造

### if文

```js
> if (1+1 == 2) "正しい";

'正しい'

> let p=8
> if (p%2==0) {
    "偶数";
    } else {
    "奇数";
    }
    
'偶数'

> let q=15;
> if (q%2) {
 "2の倍数";
 } else if (q%3) {
 "3の倍数";
 } else if (q%5) {
 "5の倍数";
 } else {
 "それ以外";
 }
 
'3の倍数'
```

### switch文

```js
> let m=2

> switch (m) {
    case 1:
        console.log('one')
        break;
    case 2:
        console.log('two')
        break;
    case 3:
        console.log('three')
        break;
    default:
        console.log('out of range')
}

two
```

### while文

```js
> let n=1;
> let s=0;> while (n<=10) {
 s=s+n;
 n=n+1;
 }
 
11
> s;
55
```

### do while文

```js
> let m=0;
undefined
> do {
     r=Math.random()*100;
     m=Math.ceil(r);
 } while(m<80)
 
95
> m;
95
```

### for文 （ループ制御）

```js
> let t=1;
> for(let i=1; i<=3; i+=1){
     t=t*i;
 }
6
> 
> t
6
```

### for in文 （イテレータによる繰り返し制御）

オブジェクトは，in によって「イテラブル」（繰り返し構造を持ったデータ）になる

```js
> let week={mon: 10, tue: 11, wed: 13, thu: 11, fri: 9, sat: 8}
> for (let key in week){
   console.log(key);
 }
 
mon
tue
wed
thu
fri
sat
undefined

> for (let k in week){
   console.log(week[k]);
 }

10
11
13
11
9
8
undefined
```

これはループではなく，ブロック（ {} で囲まれた処理）の方が繰り返し構造を持ったデータに毎回適用されているので，このブロックの中の 'key' や 'k' は変更可能な変数ではなく定数でもかまいません．

```js
> for (const key in week){
   console.log(key);
 }
 
mon
tue
wed
thu
fri
sat
```

### for of文（イテレータによる繰り返し制御）

配列はもともと イテラブルなので，inを使う必要がありません．
このブロック内に毎回渡されるデータは，変数でも定数でもかまいません．

```js
> let data=[2,5,1,8,3,4]

> for (const x of data){
  console.log(x);
}

2
5
1
8
3
4

```

### イテレータ

forEach, map, filter, reduce

```js
> arr = [1,2,3,4,5,6]

// forEach
> arr.forEach(x=>console.log(3*x))
3
6
9
12
15
18

// map
> arr.map(x=>3*x)
[ 3, 6, 9, 12, 15, 18 ]

// filter
> arr.filter(x=>x>3)
[ 4, 5, 6 ]

// redude
> arr.reduce((s,x)=>s+x,0)
21

> arr.reduce((p,x)=>p*x,1)
720
```

インデックス付きイテレータ（第2引数は配列のインデックスになる）

```js
// map
> arr.map((x,i)=>[x,i])
[ [ 1, 0 ], [ 2, 1 ], [ 3, 2 ], [ 4, 3 ], [ 5, 4 ], [ 6, 5 ] ]

// filter
> arr.filter((x,i)=>i%2==0)
[ 1, 3, 5 ]
```

`

#### 文字列に対するイテレータ

```js
let str="abcdefghijklmnopqrstu"

Array.from(str).map((x)=>{return x+x;})
[
  'aa', 'bb', 'cc', 'dd',
  'ee', 'ff', 'gg', 'hh',
  'ii', 'jj', 'kk', 'll',
  'mm', 'nn', 'oo', 'pp',
  'qq', 'rr', 'ss', 'tt',
  'uu'
]

> Array.from(str).reduce((s,x)=>s+x+x)
'abbccddeeffgghhiijjkkllmmnnooppqqrrssttuu'
```

## 関数

### 関数定義

関数はオブジェクトです

#### function文による関数定義（定義文）

```js
> function sum(x) {
    let arr=[...Array(x+1).keys()]
    return arr.reduce((s,y)=>s+y,0)
}

> function sum(x) {
    let s=0;
    do {
        s+=x;
        x-=1;
    } while (x>0)
    return s;
}

> sum(10)
55
> sum(100)
5050

> typeof sum
'function'
```

### アロー演算子 => による関数定義（定義式）

```js
// 生成された関数を fact という定数に代入する
> const fact=(x)=>{
    if(x==0) {
        return 1;
    } else {
        return x*fact(x-1);
    }
}


> fact(3)
6
> fact(9)
362880
> fact(20)
2432902008176640000
> fact(50)
3.0414093201713376e+64
```

BigIntの場合

```js
> const factn=(x)=>{
    if(x==0n) {
        return 1n;
    } else {
        return x*factn(x-1n);
    }
}

> factn(100n)
93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000n
```

無名関数

```js
> ((x) => x*x)(10)
100

> ((a,b)=> a+b)(2,3)
5
```

### 範囲オブジェクトに対応する配列の生成の例

javaScriptには，pythonやRubyの範囲オブジェクトに対応するものが無いので，次のようにして配列を生成する関数として作成することができる

```js
// 0から n-1 の範囲
> let range0 = (n=>{return [...Array(n).keys()]})
> range0(3)
[ 0, 1, 2 ]

// 開始と終了の範囲
> let range = ((from,to) =>{return [...Array(to-from+1)].map((_,i)=>i+from)})
> range(7,20)
[
   7,  8,  9, 10, 11, 12,
  13, 14, 15, 16, 17, 18,
  19, 20
]
```

### メモ化

```js
> let memo=new Map()
> const factMemo = (x,memo) => {
    // すでにメモに定義されていればそれを返しておわり
    if (memo.has(x)) {
        return memo.get(x);
    // そうでなければ実際に計算して，その結果をメモに登録する
    } else {
        if (x==0n) {
        memo.set(0n,1n);
        return 1n;
        } else {
        memo.set(x,x*factMemo(x-1n,memo));
        return x*factMemo(x-1n,memo);
        }
    }
}

// 10000 の階乗を求めるとスタックオーバーフローになる
> factMemo(10000n,memo)
Uncaught RangeError: Maximum call stack size exceeded
    at Map.has (<anonymous>)
    at factMemo (REPL14:2:14)
    at factMemo (REPL14:9:22)
    at factMemo (REPL14:9:22)
    at factMemo (REPL14:9:22)
    at factMemo (REPL14:9:22)
    at factMemo (REPL14:9:22)
    at factMemo (REPL14:9:22)
    at factMemo (REPL14:9:22)
    at factMemo (REPL14:9:22)

// 5000の階乗から計算結果をメモに登録していくと 20000 の階乗でも成功する

> factMemo(5000n,memo)
> factMemo(6000n,memo)
> factMemo(7000n,memo)
> factMemo(8000n,memo)
> factMemo(9000n,memo)
> factMemo(10000n,memo)
> factMemo(15000n,memo)
> factMemo(20000n,memo)
```

### コールバック関数と高階関数

関数の引数として渡される関数オブジェクト
コールバック関数を引数にとる関数（関数をインプットにする関数）を公開関数といいます．

forEach, map, filter, reduce などのイテレータは高階関数で，コールバック関数を引数にとっていました．

```js
> arr = [1,2,3,4,5,6]
// map
> arr.map(x=>3*x)
[ 3, 6, 9, 12, 15, 18 ]

// コールバック関数
> const cb = x=>3*x
> arr.map(cb)
[ 3, 6, 9, 12, 15, 18 ]
```

### クロージャ

関数の変数環境は関数オブジェクトの一部として管理され，クロージャと呼ばれます．
クロージャは，関数が呼ばれたときの変数の束縛環境を保持します．

```js
// 関数を出力する関数（n の y乗の関数）
> const powN = x=> {
     let n=x;
     return y=>n**y;
 }

// pow2 は 2のべき乗関数
> let pow2=powN(2);
// pow3 は 3のべき乗関数
> let pow3=powN(3);

> pow2(10)
1024
> pow3(10)
59049

// カウンター
> const counter=()=>{
    let c=0;
    return ()=>{return c+=1}
}

> let count=counter()

> count()
1
> count()
2
> count()
3
> count()
4
> count()
5
```

### ジェネレータ

イテラブルなオブジェクトを生成する関数
functionのあとに * をつけるとジェネレータ（オブジェクト）になる．

ジェネレータに対して next() メソッドを適用すると，次の計算が行われる．
毎回の計算は，現在の位置から yield まで．
yield 文は return 文のように関数のアウトプットを指定する．

```js
// フィボナッチ数列を無限に生成するジェネレータ

> function* fib() {
    let x=0, y=1;
    for(;;){
        yield y;
        [x,y]=[y,x+y]
    }
}

// ジェネレータの生成
> f=fib()
Object [Generator] {}

// next() メソッドを適用してみる
> f.next()['value']
1
> f.next()['value']
1
> f.next()['value']
2
> f.next()['value']
3
> f.next()['value']
5
> f.next()['value']
8
```


## 文字列処理

```js
> let w="hello world";
undefined
> 
> w[0]
'h'
> w[1]
'e'
> w[3]
'l'
> w.length
11
> w.slice(0, 3);
'hel'
> w.substr(1, 3);
'ell'
> w.substr(1, 4);
'ello'
> w.replace('world', 'japan');
'hello japan'


> let j="こんにちは世界さん"
undefined
> j[0]
'こ'
> j[1]
'ん'
> j.length
9

> j.slice(3, 7);
'ちは世界'
```

連接

```js
> "hello" + "world"
'helloworld'

> ['Hello', 'World'].join(' ')
'Hello World'

```

分割

```js
> let data = '2018, 2019, 2020';
undefined
> data.split(',')
[ '2018', ' 2019', ' 2020' ]
```

トリム

```js
> line= '  08F399 \n';
'  08F399 \n'
> line.trim()
'08F399'
```



## 非同期処理

あるイベントが完了するまで、処理の実行を保留しておく制御を非同期処理という
イベントに反応して実行される処理は通常コールバック関数として登録される

```
    イベント  →  コールバック関数（イベント情報）
```
### イベント駆動型プログラムの例

#### ブラウザ上のユーザ操作とイベントハンドラー

ブラウザ上のユーザのキーボード操作やマウスクリックなどのイベントに反応する処理をコールバック関数として登録するプログラミング．webブラウザではこのようなコールバック関数をイベントリスナーという．
イベントハンドラーのコールバック関数には，イベントに関する情報（マウスポインタの座標や時刻など）が渡される．

```
// イベントリスナー
    event1 → callback1
    event2 → callback2
    ...
```

#### ネットワークイベントのコールバック関数

ネットワークに対してリクエストを送り，その応答をイベントとして駆動されるコールバック関数．セッションを openし， send によってリクエストを送り，onload で応答を得る．
ネットワークイベントは成功するとは限らないので，異常事態に対するコールバック関数も必要になる．

```
    event1→ (成功) callback1
             └→ （失敗）callback2
    ...
```
#### ファイル入出力イベントのコールバック関数

ネットワークイベントと同様に，ファイルの読み込みや書き込みに対する結果をイベントとして実行されるコールバック関数．
異常事態に対するコールバック関数も必要になる．

#### コールバックチェーン

コールバック関数の実行によって次のイベントとそれに対する新たなコールバック関数が定義されることがあり，さらにそれが深く連鎖することがあります．
しかもどれかのイベントが失敗することもあるので，それを捕捉するコールバック関数も必要です．

```
    event1→ (成功) callback11 : event2 → （成功）callback21：event3 → ...
             └→ （失敗）callback12          └→ （失敗）callback22
```


### タイマーによる非同期処理

`setTimeout()`

一定時間が経過したというイベントの後に関数を実行する
delayは、ミリ秒

```js
//  コールバック関数
> const f1=()=>console.log("処理1")
> const f2=()=>console.log("処理2")
> const f3=()=>console.log("処理3")

// タイムアウト・イベントの登録
> let t1=setTimeout(f1, 30000);
> let t2=setTimeout(f2, 20000);
> let t3=setTimeout(f3, 10000);

> 処理3
処理2
処理1
```

`setInterval()`

一定時間が経過したというイベントの後に関数を実行する
delayは、ミリ秒

```js
//  コールバック関数
> const f4=()=>console.log("5秒経過")

// タイムアウト・イベントの登録
> let t4=setInterval(f4, 5000);

> 5秒経過
5秒経過
5秒経過
5秒経過

// イベントループの停止
> clearInterval(t4)
```

### Node.js のネットワークイベント

webページを取得する場合，リクエストとレスポンスの２段階のイベントとコールバック関数の非同期処理が実行される
ここでは，リクエストに対する非同期処理のみを記述する
Node.js では，コールバック関数を登録するメソッドは on() 

```js
> const https = require("https")
> let url = "https://www.kindai.ac.jp"

// HTTP GET メソッドでリクエストを送る
> let req = https.get(url)
// HTTP レスポンスのコールバック関数
> const responce = resp=>{
    let body=""
    resp.on("data", chunk=>{body+=chunk;})
    resp.on("end", ()=>console.log(body))
}
> req.on("response",responce)

// 以下省略

```

### Promiseによる非同期処理

非同期処理は，処理がどのような順番で結果が返ってくるかわかりません．
これを同期処理のように順番どおりに実行するしくみがpromiseです．
特に複雑化しやすいエラー処理を含むコールバックチェーンを簡潔に記述するために javaScript コアのオブジェクトに備わっています．

#### Promise オブジェクトのメソッド

* then()    : イベント成功時のコールバック関数の登録
* catch()   ：イベント失敗時のコールバック関数の登録
* finally() ：イベントが成功しても失敗しても最終的に行われるコールバック関数の登録
* all()     ：並行実行される複数のイベントがすべて完了したときのコールバック関数の登録
* race()    ：並行実行される複数のイベントのうち最初のものが完了したときのコールバック関数の登録

#### Promise チェーン

then() メソッドで登録されたコールバック関数の戻り値は promise なのでチェーンを構成できる．
このthen()メソッドのチェーンによって非同期処理を順番どおりに実行させることができます．
また，このチェーンのどこかでエラーが発生した場合は，それを chach() メソッドで捕捉できます．
finally() メソッドで登録されたコールバック関数は，promiseチェーンが成功でも失敗でも最終的に実行される．

```
promise.then(callback1).then(callback2).catch(callbackErr).finally(collbackFinal)
```

```
event1 → (成功) callback1 : event2 ┬ → (成功）callback2 :┬→ event3: collbackFinal
      └---------------------------└→(失敗) callbackErr : -┘      
```

#### promise の非同期処理の並行実行

promiseは，非同期処理の並行実行にも利用できる．
all() メソッドは，並行実行された非同期処理のコールバック関数を配列にして登録し，そのすべての完了を待つ．
一つでも失敗すると失敗のコールバック関数が実行される．

```
Promise.all([callback1,collback2,callback3,...]).then(callbackn).chatch(callbackErr)
```

```
callback1: event1 → (成功) ┐
callback2: event2 → (成功) │
callback2: event3 → (成功) │
...                 (成功) ┘-> callbackn    
      └------------→(失敗) --> callbackErr
```

#### promise の並行処理の最初の応答を受け取る

並行実行する非同期処理の最初の成功／失敗を受け取ると完了する．

```
Promise.race([callback1,collback2,callback3,...]).then(callbackn).chatch(callbackErr)
```


```
callback1: event1 → 
callback2: event2 → (最初の成功) -> callbackn  
callback2: event3 → 
...                   
      └------------→(最初の失敗) --> callbackErr
```


### await/ async

Promise の使用法を劇的に簡略化する構文で，Promiseを事実上隠蔽します．

#### await

Promiseオブジェクトを受け取り，その完了を待ち，その返り値やエラーに変換します．

#### 非同期関数 fetch

http を使ってデータを取得するPromise オブジェクトの非同期処理です．
以下は，await を使ってURLの応答を得て，そのボディを得る例です

```js
> const fetch = require('node-fetch')
> let url = 'https://www.kindai.ac.jp'
> let resp = await fetch(url)
> let body = await resp.text()
> body

'<!DOCTYPE html>\n' +
  '<html lang="ja">\n' +
  '<head>\n' +
  '  <meta charset="UTF-8">\n' +
  '<meta http-equiv="X-UA-Compatible" content="IE=edge">\n' +
  '<meta name="viewport" content="width=device-width,initial-scale=1.0">\n' +
  '  <title>近畿大学</title>\n' +

...
```

Promiseオブジェクトなので，then() メソッドを使うことができます．

```js
> body = await fetch(url).then(resp=>resp.text())
> body

'<!DOCTYPE html>\n' +
  '<html lang="ja">\n' +
  '<head>\n' +
  '  <meta charset="UTF-8">\n' +
  '<meta http-equiv="X-UA-Compatible" content="IE=edge">\n' +
  '<meta name="viewport" content="width=device-width,initial-scale=1.0">\n' +
  '  <title>近畿大学</title>\n' +

...
```

### async関数

async は非同期関数を宣言するキーワードです．

非同期な処理を含む関数は非同期になります．
awaitが指定されたコードを含む関数は，async 関数でなければなりません．

```js
> const fetchbody = async url=>{
    let resp = await fetch(url)
    let body = await resp.text()
    console.log(body)
}

> fetchbody('https://www.kindai.ac.jp')
```

#### 複数のPromise を並行実行する場合

```js
> await Promise.all([fetchbody('https://www.kindai.ac.jp'),fetchbody('https://www.google.com')])

```