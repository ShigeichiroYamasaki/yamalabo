# JavaScript言語入門

Node.jsを中心にした解説です．

2024/05/16
Shigeichiro Yamasaki

## 目次

* [nvm](#nvm)
* [Node.js のインストール](#nodejsinstall)
* [Node.jsのインタープリタの起動と終了](#nodejsss)
* [JavaScriptの文法](#grammar)
* [制御構造](#controle)
* [関数](#function)
* [文字列処理](#string)
* [非同期処理](#async)
* [Node.js のパッケージ](#package)
* [npm](#npm)
* [JavaScriptのモジュール](#module)

## <a id="nvm"> </a>nvm

nvm（Node Version Manager）は、Node.js のバージョン管理ツールであり、複数の Node.js バージョンを簡単にインストール、切り替え、管理するために使用されます

### nvm (ubuntu)のインストール

```bash
sudo apt install curl 
curl https://raw.githubusercontent.com/creationix/nvm/master/install.sh | bash
source ~/.profile
```

### nvm (macOSX)のインストール

Homebrew がインストールされていることが前提です

```bash
source $(brew --prefix nvm)/nvm.sh
echo 'source $(brew --prefix nvm)/nvm.sh' >> ~/.zprofile
source ~/.zprofile
```

### nvm(Windows) のインストール

[nvm winsdows インストーラのページ](https://github.com/coreybutler/nvm-windows/releases)

## <a id="nodejsinstall"> </a>Node.jsのインストール

最新の LTS バージョンをインストール
 
```bash
nvm install --lts
```

## <a id="nodejsss"> </a>Node.jsのインタープリタの起動と終了

起動

```bash
$ node

Type ".help" for more information.
> 
```

終了

(コントロールｄ）


## <a id="grammar"> </a>JavaScriptの文法

### 式

JavaScript の式はセミコロンで終わるのが基本です．

セミコロンは改行で代替できます．

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
ここまでが複数行のコメント 
*/
```

### 予約語

以下は予約語なので変数名などの識別子として利用することができない

```
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

### 型

#### 基本型

イミュータブル（生成後，変更することができないオブジェクト）

* 数値
* 文字列
* 論理値
* null, undefined
* Symbol

#### オブジェクト型

プロパティ（名前と値を持ち，値として基本型やオブジェクトを保持できる）の集合体のこと

ミュータブル（生成後，変更することが可能）

* 配列 (Array)
* Set （集合 同じ要素は同一になる）
* Map （キーと値の対の集まり）
* RegExp (正規表現 パターマッチング)
* Date （日付と時間）
* 関数
* クラス

### リテラル

#### 数値

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

#### 数値演算

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

#### 数学的演算

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

#### BigInt （任意精度整数）

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

#### 文字リテラル

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

\ （バクスラッシュ）に続けて文字を記述することで ' や " など文字列内に記述できない文字を記述できる

```js
> "hello \"hello\" hello"
'hello "hello" hello'

// \n 改行記号
// \t タブ
// \r 復帰記号
> "hello \n hello"
```

##### 文字リテラルへの操作

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

### 論理値

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

JavaScript 言語処理系は，自動的にオブジェクトの型変換を行います

* == による比較は自動型変換の結果で比較される．
* === による比較は自動型変換を行わない．

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

### 代入

#### 変数へのバインディング

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


#### 分割代入

```js
> {k1: x, k2: y}={k1: 1, k2: 3, k3: 5}
{ k1: 1, k2: 3, k3: 5 }
> x
1
> y
3

> [a,b,c]=[1,2,3,4,5]
[ 1, 2, 3, 4, 5 ]
> a
1
> b
2
> c
3

```

#### var 

古い仕様である var による変数は複数回宣言できますが，望ましくありません
トップレベルで var で宣言されば変数のスコープはグローバルスコープになります

```js
> var y=1
> var y=2
```

#### 定数定義

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

#### 変数名の文法

* アルファベットの大文字でも小文字でも使えます
* 数字から始まってはいけません
* $ と _ は使えますが、それ以外の句読点や特殊文字は使えません
* 予約語は使えません


### 標準出力への出力

print文にあたるもの

```js
> console.log('hello')
hello
undefined

```

### 配列

#### 配列リテラルによる配列の生成

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

#### Array() コンストラクタによる配列の生成

```js
> let arr3=new Array()
undefined
> arr3
[]
> let arr4=new Array(5)
> arr4
[ <5 empty items> ]
```

#### インデックスによる配列要素へのアクセス

```js
> arr[0]
1
> arr[1]
2
> arr[3]
4
```

#### 配列への操作

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

#### スプレッド演算子

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

### オブジェクト

#### オブジェクトの生成

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

> let arr5= new Array(5)

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

JavaScriptの配列は，インデックスをキーとするオブジェクト

```
> let arr5= new Array(5)
undefined
> arr5
[ <5 empty items> ]

> [...Array(5).keys()]
[ 0, 1, 2, 3, 4 ]
```

#### オブジェクトのプロパティへのアクセス

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

#### Map オブジェクト

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

#### プロトタイプ・オブジェクトから新しいオブジェクトを生成

JavaScript では，プロトタイプを使って新しいオブジェクトを生成できる．

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

#### プロパティのテスト

オブジェクトがそのプロパティを持っているかテストできる

```js
> p1
{ x: 0 }
> 'y' in p1
true
```

#### オブジェクトの型

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


## <a id="controle"> </a>制御構造

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

オブジェクトには，配列や文字列など，それ自身が繰り返し構造を持つものがあります．

イテレータとは，ループによる繰り返し制御ではなく，対象となるオブジェクト自体が持つ繰り返し構造を利用した繰り返し制御です．

* forEach

    繰り返し要素ごとに処理を行う
* map

    繰り返し要素ごとに処理を行い，その処理結果も繰り返し構造を持つオブジェクトになる
* filter

    繰り返し要素ごとに条件判断を行い，true となるものだけの繰り返し構造オブジェクトを出力する（条件適合要素の選択など）
* reduce

    繰り返し要素ごとに処理を行い，それを集約した結果を出力する（要素の合計など）

```js
> arr = [1,2,3,4,5,6]

// forEach
// それぞれを出力する
> arr.forEach(x=>console.log(3*x))
3
6
9
12
15
18

// map
// それぞれを３倍する
> arr.map(x=>3*x)
[ 3, 6, 9, 12, 15, 18 ]

// filter
// 3よりも大きい要素を選択する
> arr.filter(x=>x>3)
[ 4, 5, 6 ]

// redude
// 要素の和を求める（和の初期値は 0)
> arr.reduce((s,x)=>s+x,0)
21

// 要素の積を求める(積の初期値は 1)
> arr.reduce((p,x)=>p*x,1)
720
```

インデックス付きイテレータ（第2引数は配列のインデックスになる）

```js
// map
> arr.map((x,i)=>[x,i])
[ [ 1, 0 ], [ 2, 1 ], [ 3, 2 ], [ 4, 3 ], [ 5, 4 ], [ 6, 5 ] ]

// filter
// インデックスが偶数のものだけを選択する
> arr.filter((x,i)=>i%2==0)
[ 1, 3, 5 ]
```

#### 文字列に対するイテレータ

文字列オブジェクトも繰り返し構造を持つオブジェクト

```js
let str="abcdefghijklmnopqrstu"

// 同じ文字を連接した配列を出力する
Array.from(str).map((x)=>{return x+x;})
[
  'aa', 'bb', 'cc', 'dd',
  'ee', 'ff', 'gg', 'hh',
  'ii', 'jj', 'kk', 'll',
  'mm', 'nn', 'oo', 'pp',
  'qq', 'rr', 'ss', 'tt',
  'uu'
]

// 同じ文字の連接の連接(文字列の初期値は ""（空文字列）)
> Array.from(str).reduce((s,x)=>s+x+x,"")
'aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuu'
```

### イテレータオブジェクトの next() メソッド

イテレータの対象となるオブジェクトは，配列，Set，Map などそれ自身が繰り返し構造を持つオブジェクトです.これらを反復可能オブジェクトと呼びます．

反復可能オブジェクトには，Symbol.iterator というメソッドが定義されています．

```js
> let list=[1,2,3,4,5]
> let it=list[Symbol.iterator]()
Object [Array Iterator] {}
```
このメソッドが返すオブジェクトには，その繰り返しを実施する next() というメソッドが定義されています．

next() メソッドが返すオブジェクトは，反復結果オブジェクトと呼ばれますが，value と done というプロパティがあります．done は繰り返しの終了状態を意味するプロパティで，doneの値が true になると繰り返しが終了します．

```js
> it.next()
{ value: 1, done: false }
> it.next()
{ value: 2, done: false }
> it.next()
{ value: 3, done: false }
> it.next()
{ value: 4, done: false }
> it.next()
{ value: 5, done: false }
> it.next()
{ value: undefined, done: true }
```

## <a id="function"> </a>関数

### 関数定義

関数はオブジェクトです

#### function文による関数定義（定義文）

```js
// reduce による配列の合計
> function sumR(arr) {
    return arr.reduce((s,x)=>s+x,0);
}
> let arr10=[...Array(10+1).keys()]
[
   0, 1, 2, 3, 4,
   5, 6, 7, 8, 9,
  10
]

> sumR(arr10)
55

// ループ制御による 1からn の合計
> function sumN(n) {
    let s=0;
    do {
        s+=n;
        n-=1;
    } while (n>0)
    return s;
}

> sumN(100)
5050

> typeof sumR
'function'
> typeof sumN
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

> typeof fact
'function'
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

JavaScriptには，pythonやRubyの範囲オブジェクトに対応するものが無いので，次のようにして配列を生成する関数として作成することができる

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
// 再帰による階乗関数
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

// 100の階乗
> factMemo(100n,memo)
93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000n

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
> factMemo(10000n,memo)
> factMemo(15000n,memo)
> factMemo(20000n,memo)
```

任意の関数をメモ化する関数

```js
> let memorize = f=>{
    let m=new Map()
}
```


### コールバック関数と高階関数

関数の引数として渡される関数オブジェクト
コールバック関数を引数にとる関数（関数を引数にとる関数）を高階関数といいます．

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

クロージャは，関数が呼ばれたときの変数の束縛環境(変数とそれにバインディングされている値)を保持します．

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

イテラブルなオブジェクトを生成する処理過程の「定義」です．

無限集合を生成する処理過程などを定義することができます．

関数定義の function のあとに * をつけるとジェネレータ（オブジェクト）になります．

★ アロー関数形式ではジェネレータを定義することはできません

ジェネレータの生成処理は基本的に停止しており，ジェネレータに対して next() メソッドを適用すると，次のステップの生成処理が行われます．

毎回の生成処理は，現在の位置から yield まで．
yield 文は関数の return 文のようにジェネレータのアウトプットを指定します．



```js
// フィボナッチ数列を無限に生成するジェネレータ
// 論理的にはフィボナッチ数列という無限集合を意味しているとみなせます．
//  もしこれが関数なら，for(;;) という無限ループになります．

> function* fib() {
    let x=0, y=1;
    for(;;){
        yield y;
        [x,y]=[y,x+y]
    }
}

// ジェネレータオブジェクトの生成
> f=fib()
Object [Generator] {}

// ジェネレータに next() メソッドを適用してみる
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

### take で n 個取り出す

```js
> const take=(n, generator)=> {
    let range = ((from,to) =>{return [...Array(to-from+1)].map((_,i)=>i+from)})
    return range(1,n).map(()=>generator.next()['value'])
}

// フィボナッチ数列の集合を 20 個取り出す
> f=fib()
> take(20,f)
[
    1,    1,    2,    3,    5,
    8,   13,   21,   34,   55,
   89,  144,  233,  377,  610,
  987, 1597, 2584, 4181, 6765
]

// さらに 20個取り出す
> take(20,f)
[
     10946,     17711,    28657,
     46368,     75025,   121393,
    196418,    317811,   514229,
    832040,   1346269,  2178309,
   3524578,   5702887,  9227465,
  14930352,  24157817, 39088169,
  63245986, 102334155
]
```

## <a id="string"> </a>文字列処理

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

## <a id="async"> </a>非同期処理

ユーザの操作やデータの到着などのイベントが発生するまで処理の実行を保留する制御を非同期処理という

JavaScript言語では、非同期処理は通常コールバック関数として登録される

コールバック関数には、イベントに関連した詳細情報が引数として渡されることがあります

```
    イベント  →  コールバック関数（イベント情報）
```
### イベント駆動型プログラムの例

#### ブラウザ上のユーザ操作とイベントハンドラー

ブラウザ上のユーザのキーボード操作やマウスクリックなどのイベントに反応する処理をコールバック関数として登録するプログラミング．

webブラウザではこのようなコールバック関数をイベントリスナーという．

イベントハンドラーのコールバック関数には，イベントに関する情報（マウスポインタの座標や時刻など）が渡される．

```
// イベントリスナー
    event1 → callback1
    event2 → callback2
    ...
```

#### ネットワークイベントのコールバック関数

ネットワークに対してリクエストを送ったときに、
その応答をイベントとして起動されるコールバック関数．

セッションを openし， send によってリクエストを送り，onload で応答を得る．

ネットワークイベントは成功するとは限らないので，異常事態に対するコールバック関数も必要になる．

```
    event1→ (成功) callback1
             └→ （失敗）callback2
    ...
```
#### ファイル入出力イベントのコールバック関数

ネットワークイベントと同様に，ファイルの読み込みや書き込みに対する結果をイベントとして実行されるコールバック関数．

異常事態に対するコールバック関数も必要になる．

### コールバックチェーン

コールバック関数の実行によって次のイベントとそれに対する新たなコールバック関数が定義されることがある

さらにそれが深く連鎖することがあります．

しかも、それぞれのイベントは失敗することもあるので，それを捕捉するコールバック関数も必要

```
    event1→ (成功) callback11 : event2 → （成功）callback21：event3 → ...
             └→ （失敗）callback12          └→ （失敗）callback22
```


### タイマーによる非同期処理

一定時間が経過したというイベントの後に関数を実行するタイマー関数

`setTimeout(コールバック関数, delay)`

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

// 10秒待つ
> 処理3
処理2
処理1
```

一定時間が経過したというイベントの後に関数を実行するタイマー関数

`setInterval()`

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

webページを取得する場合，リクエストとレスポンスの２段階のイベントとコールバック関数の非同期処理が実行されます

ここでは，リクエストに対する非同期処理のみを記述します

Node.js では，イベントハンドラのコールバック関数を登録するメソッドは on() 

```js
> const https = require("https")
> let url = "https://www.kindai.ac.jp"

// HTTP GET メソッドでリクエストを送る
> let req = https.get(url)
// HTTP レスポンスイベントを処理するコールバック関数
> const responce = resp=>{
    let body=""
    // ボディ部が読み終わった後のイベントハンドラ
    resp.on("data", chunk=>{body+=chunk;})
    // レスポンス処理が完了したときのイベントハンドラ
    resp.on("end", ()=>console.log(body))
}
> req.on("response",responce)

// 以下省略

```

### Promiseによる非同期処理

非同期処理は，処理がどのような順番で結果が返ってくるかわかりません．

これを同期処理のようにイベントの完了に沿って、順番どおりにコールバック関数を順次実行するしくみがpromiseです．

特に複雑化しやすいエラー処理を含むコールバック関数の連鎖（コールバックチェーン）を簡潔に記述するためのもので、JavaScript コアのオブジェクトに備わっています．

#### Promise オブジェクトのメソッド

* then()    : イベント成功時のコールバック関数の登録
* catch()   ：イベント失敗時のコールバック関数の登録
* finally() ：イベントが成功しても失敗しても最終的に行われるコールバック関数の登録
* all()     ：並行実行される複数のイベントがすべて完了したときのコールバック関数の登録
* race()    ：並行実行される複数のイベントのうち最初のものが完了したときのコールバック関数の登録

#### Promise チェーン

then() メソッドで登録されたコールバック関数の戻り値は再び Promise オブジェクトです。

この Promise オブジェクトにはコールバック関数が登録されているので、コールバック関数の入れ子が構成されます。
この戻り値の Promise オブジェクトの then()メソッドの連鎖によって非同期処理を順番どおりに実行させることができます．

これがコールバックチェーンで Promiseチェーンと呼ばれます。

* Promise チェーン

Promise チェーンは、下のようにメソッドチェーンで記述します

```
promise
    .then(callback1)
    .then(callback2)
    .catch(callbackErr)
    .finally(collbackFinal)
```

このチェーンのどこかでエラーが発生した場合は，エラーをを chach() メソッドで捕捉できます．
finally() メソッドで登録されたコールバック関数は，promiseチェーンが成功でも失敗でも最終的に実行される．

* エラーの補足

```
event1 → (成功) callback1 : event2 ┬ → (成功）callback2 :┬→ event3: collbackFinal
      └---------------------------└→(失敗) callbackErr : -┘      
```

#### promise の非同期処理の並行実行

promiseは，非同期処理の並行実行にも利用できる．

all() メソッドは，並行実行された非同期処理のコールバック関数を配列にして登録し，
そのすべての完了を待つ．

```js
Promise
    .all([callback1,collback2,callback3,...])
    .then(callbackn)
    .chatch(callbackErr)
```

一つでも失敗すると失敗のコールバック関数が実行される．

```
callback1: event1 → (成功) ┐
callback2: event2 → (成功) │
callback2: event3 → (成功) │
...                 (成功) ┘-> callbackn    
      └------------→(失敗) --> callbackErr
```

#### promise の並行処理の最初の応答を受け取る

race() メソッドは、並行実行する非同期処理の最初の成功／失敗を受け取ると完了する．

```js
Promise
    .race([callback1,collback2,callback3,...])
    .then(callbackn)
    .chatch(callbackErr)
```

```
callback1: event1 → 
callback2: event2 → (最初の成功) -> callbackn  
callback2: event3 → 
...                   
      └------------→(最初の失敗) --> callbackErr
```

### await / async

Promise の使用法を劇的に簡略化する構文で，Promise の複雑性を事実上隠蔽します．

### await

await はPromiseオブジェクトを受け取り，その完了を待ち，その返り値やエラーに変換します．

`await p` は Promise オブジェクト p の完了を待つという意味になります。

その返り値が Promise オブジェクトなら、それを await することで逐次的に処理を行うことができます。

* 非同期関数 fetch は、http を使って url のデータを取得する Promise オブジェクトです．

以下の例は、２つの非同期処理を await によって逐次処理のようにしたものです。

1. await を使ってURLへの GET 応答を得るまで完了を待ちます。

2. resp が返った後に、その返り値のPromiseオブジェクトに対して https でHTMLのbody部を得ます

```js
> let url = 'https://www.kindai.ac.jp/'
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

上記の resp はPromiseオブジェクトなので，then() メソッドを使って以下のようにコールバックチェーンで処理をつなげることと同じ意味です。

then メソッドの中のアロー関数の引数の resp にはfetch の返り値の Promise オブジェクトが束縛されることになります

```js
> let url = 'https://www.kindai.ac.jp/'
> let body = await fetch(url).then(resp=>resp.text())
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

### async 関数

async は非同期関数を宣言するキーワードです．

async で定義された関数を async 関数と言います。

async 関数の返り値は Promise オブジェクトになります。

非同期処理を含む関数は非同期関数として宣言しなければなりません。

await が指定されたコードを含む関数は，async 関数として定義しなければなりません．

```js
> // 非同期関数 fetchbody の宣言
> const fetchbody = async url=>{
    let resp = await fetch(url)
    let body = await resp.text()
    return body
}

> await fetchbody('https://www.kindai.ac.jp/')
```

then()を使った場合

```js
> const fetchbodyC = async url=>{
    let body = await fetch(url).then(resp=>resp.text())
    return body
}

> await fetchbodyC('https://www.kindai.ac.jp/')
```

#### 複数のPromise を並行実行する場合

awaitは非同期処理を逐次化してしまいます．

効率化などのために非同期処理を並行実行したいときは，Promise.all() を利用します．


```js
> await Promise.all([fetchbodyC('https://www.kindai.ac.jp/'),fetchbodyC('https://www.google.com/')])

```

## <a id="package"> </a>Node.js のパッケージ

Node.js では，一つの Node.js アプリケーションシステムをパッケージと呼んでいる．
Node.js のパッケージは，他のシステムにおける「プロジェクト」と考えてよい．

### パッケージのディレクトリとモジュール

パッケージは一つのディレクトリで管理します．
Node.js のパッケージは，モジュールの集合体です．

## <a id="npm"> </a>npm （Node Package Manager）

Node.js のパッケージ管理には、主に npm コマンドが使用されます。npm は、ソフトウェアレジストリであり、Node.js のライブラリやツールが公開されています．

### npm の基本的なコマンド

* npm init：プロジェクトの初期化
* npm install（または npm i）：パッケージのインストール
* npm update：パッケージを最新バージョンに更新します。
* npm uninstall：パッケージをアンインストールします。

####  npm init：npm による新しいプロジェクトの作成

まずプロジェクトのディレクトリを作成し，そこで初期化を行います．

package.json ファイルが作成されます．

```bash
$ mkdir <プロジェクトディレクトリ＞
$ cd  <プロジェクトディレクトリ＞

$ npm init

This utility will walk you through creating a package.json file.
It only covers the most common items, and tries to guess sensible defaults.

See `npm help init` for definitive documentation on these fields
and exactly what they do.

Use `npm install <pkg>` afterwards to install a package and
save it as a dependency in the package.json file.

Press ^C at any time to quit.
package name: (mynodeproj) mynodeproj
version: (1.0.0) 
description: 
entry point: (index.js) 
test command: 
git repository: 
keywords: 
author: 
license: (ISC) 
About to write to /Users/shigeichiroyamasaki/git/yamalabo/myNodeProj/package.json:

{
  "name": "mynodeproj",
  "version": "1.0.0",
  "main": "index.js",
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1"
  },
  "author": "",
  "license": "ISC",
  "description": ""
}


Is this OK? (yes) 

npm notice
npm notice New minor version of npm available! 10.7.0 -> 10.8.0
npm notice Changelog: https://github.com/npm/cli/releases/tag/v10.8.0
npm notice To update run: npm install -g npm@10.8.0
npm notice

```

#### npm install:パッケージのインストール

    * --save：依存関係として package.json に追加します（デフォルトで npm install で追加されます）。
    * --save-dev：開発依存関係として追加します（例：npm install --save-dev mocha）
    * -g : グローバルインストール.システム全体で使用するパッケージのインストール


#### package.json

Node.js プロジェクトの中心的な設定ファイルであり、以下のような情報を含みます

* プロジェクトの名前、バージョン、説明
* 依存関係のリスト（dependencies、devDependencies）
* スクリプト（npm scripts）：プロジェクトのビルド、テスト、デプロイなどの自動化に使用します。
例："scripts": { "start": "node app.js", "test": "mocha" }

#### パッケージのバージョン管理

package.json には、インストールされている各パッケージのバージョンが記載されます

#### 他のパッケージマネージャー

npm 以外に Yarn という Facebook が開発したパッケージマネージャーがあります．



## <a id="module"> </a>JavaScript のモジュール

既存のコードのライブラリなど外部のソースコードを利用するためにモジュール機構は必要です．
大規模開発になると複数の様々な開発主体が作成したモジュールを利用することになります．

大規模開発では，クラス名，関数名，変数や定数の名前などの名前空間の管理が重要になります．

外部モジュールの間も含めて，グローバルな名前空間が衝突などの汚染が発生しないような仕組みが必要です．

### エクスポートとインポート

外部のファイルなどに定義された JavaScript のソースコードをモジュールとして他のプログラムから利用できるようにするとき，そのモジュールの外部に公開するクラス名や関数名や定数などのオブジェクト名を定義することを「エクスポート」といいます．

また外部モジュールを自分のプログラムから利用するために，モジュールのファイルを指定して，そのモジュールのクラスや関数や定数などのオブジェクトを利用できるようにすることを「インポート」といいます．

### Node.js のモジュール

クラス，ブロックスコープ，クロージャはそのようなモジュール化の仕組みとして利用できます．
Node.js のモジュールは基本的にこれです．

JavaScriptにモジュール機構が正式に組み込まれる以前に Node.js のモジュール機構は作成されました．

 ES6 (ES2015) でJavaScriptに正式に組み込まれたモジュール機構では，Node.js とはインポート方法が異なっていることに注意が必要です．

#### Node.js モジュールのエクスポート

まずNode.js の場合について説明します．

この例では，ファイル名を stat.js とします．

```js
const sum =(a,b)=> a+b;
const square = a=>a**2;
// エクスポートする関数
exports.mean = arr=> arr.reduce(sum)/arr.length;
exports.stddev = arr=>{
    let m = exports.mean(arr);
    return (Math.sqrt(arr.map(x=>square(x-m)).reduce(sum)))/(arr.length-1);
};
```

関数を一つずつエクスポートする代わりに，１個のオブジェクトだけをエクスポートする方法

```js
const sum =(a,b)=> a+b;
const square = a=>a**2;
const mean = arr=> arr.reduce(sum)/arr.length;
const stddev = arr=>{
    let m = mean(arr);
    return (Math.sqrt(arr.map(x=>square(x-m)).reduce(sum)))/(arr.length-1);
};
// エクスポートする関数，定数，クラスのリスト
module.exports = {mean, stddev};
```

### Node.js モジュールのインポート

Node.js ではインポートに require() 関数を利用します．

ここでは，同じディレクトリに stat.js ファイルがあるものとします．

#### require文によるインポート

```js
> const stat = require('./stat.js')
{ mean: [Function: mean], stddev: [Function: stddev] }

// この例では，stat という定数にモジュールをインポートしました
// したがって，この後は stat という定数の名前空間でインポートした関数を利用できます

> stat.mean([1,2,3,4])
2.5
> stat.stddev([1,2,3,4])
0.7453559924999299
```

### ES6 のモジュール

ES6 から JavaScriptに import と export というキーワードが追加され JavaScript言語処理系のコア機能としてモジュールがサポートされました．

Node.js で ES6 モジュールを使用するためには、以下のいずれかの方法でプロジェクトを設定する必要があります：

* package.json に "type": "module" を追加する。
* ファイル拡張子を .mjs にする。


package.json の例

```json
{
  "name": "your-project-name",
  "version": "1.0.0",
  "main": "index.js",
  "type": "module",
  "scripts": {
    "start": "node index.js"
  }
}
```

#### export キーワード によるエクスポート

ファイル名を stat.mjs とします．

```js
const sum =(a,b)=> a+b;
const square = a=>a**2;
// エクスポートする関数
export const mean = arr=> arr.reduce(sum)/arr.length;
export const stddev = arr=>{
    let m = mean(arr);
    return (Math.sqrt(arr.map(x=>square(x-m)).reduce(sum)))/(arr.length-1);
};
```


#### import キーワードによるインポート

```js
> import {mean, stddev} from "./test/stat.js";
```

