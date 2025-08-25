# TypeScript言語入門

2025/08/25  Shigeichiro Yamasaki

事前に[javaScript言語入門](./JavaScript.md)を学んでいることを前提にします


## TypeScript言語の概要

javaScript言語は，非常に多くのシステムで利用されていますが，初期の言語仕様は洗練されたものではなくいろいろな問題をかかえていました．そのため，直接 javaScript言語で開発するのではなく，AltJS と総称される javaScript言語に変換できるプログラミング言語が開発言語として人気を集めるようになりました．

TypeScript言語は，そのような AltJS の一つです．開発者たちは JavaScript言語で直接書かれたコードを「バニラ」と呼びます．これはフレーバーの無いアイスクリームという意味です．これに対して TypeScript言語は，型システムというフレーバーを javaScript言語にトッピングした AltJSです．

TypeScript言語は，実際には javaScript のコードにコンパイルされて javaSCript言語処理系で実行されます．つまりTypeScript 言語は javaSCript言語を超えるものではありません．

## 型 (Type) とは

型 (Type) とは論理学および哲学の用語です．

「対象＝モノ」と「操作＝対象への作用や属性」はものごとを定義するときに「どちらがより根源的な概念か？」ということは言えません．これを「対象と操作の2元論」と呼ぶことがあります．

対象を直接集めたものが「集合」です．
一方で操作の視点から対象を見たものが「型」です．

例えば「整数」という対象を直接集めると「整数の集合」ができます．
「整数型」とは，整数に対する，足し算，引き算，掛け算，割り算，大小比較，整列など，操作の視点から見た「整数」です．

こういう型の区別は計算機システムとしても必須のものです．
実際に，浮動小数の数と整数では，同じ「足し算」という操作でもCPUの中で処理する回路が異なります．

関数にも型があります．引数の型と返り値の型の組み合わせが関数の型です．
関数は，「操作」のように思えるでしょうが「対象」としての側面もあります．
関数は，プログラムの中で関数型の対象（オブジェクト）として扱うことがあります．
関数を引数にとる関数（高階関数）や関数を返り値として出力する関数も存在します．

### 型理論と型推論

対象をその操作の視点で定義する論理学を型理論といいます．
型理論には型推論と呼ばれる計算があります．

例えば，引数と返り値の型を持つ関数にその引数の対象を適用した結果の型は返り値の型になります．

この例は単純ですが，関数を引数にしたり関数を返り値とする関数の型を考えたりすることを考えるとわかるように，型理論は自体は極めて抽象的で複雑な理論であることに注意が必要です．

例えば，1.1 + 1 のように浮動小数点数と整数が混在した演算の処理方法など実際の型推論が非常に複雑なことは想像できるでしょう．

理論的には，プログラムの仕様とは型を定義することであるということも可能です．
この考え方を厳密に実施することも可能で，実際に型理論を背景に持つプログラミング言語も存在します．

しかし，JavaScript言語や TypeScript言語は，そのような厳密に型理論を利用する言語ではありません．
TypeScript言語は，基本的には手続き型言語と呼ばれる処理の実行手順を抽象化しない言語です．
TypeScript は，型理論を利用していますが，その内容は単純なものに限定されています．

### プログラミング言語に型を導入する利点

プログラミングに型理論を導入すると，型推論を利用して対象（変数や関数など）に対する操作の矛盾をコンパイルの段階で事前に検出することができます．
これは，特に大規模な開発やスマートコントラクトのように些細なバグが巨額の経済的被害を引き起こす開発にとっては大きな利点となります．

javaScript言語も進化を続けており，現在では [ES2025](https://tc39.es/ecma262/2025/) という洗練された言語仕様を備えています．

TypeScriptの利点をより活かすには，できるだけ手続き型のコーディングスタイルから関数型のスタイルを利用する方が良いでしょう．

## インストール

このチュートリアルでは，TypeScript を Node.js でサーバーサイドで利用することに特化します．Node.jsで TypeScript を実行する場合，Node.js の 23.6.0 以降が必要になります．

* Node.js のインストール

```bash
cd ~
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.3/install.sh | bash

\. "$HOME/.nvm/nvm.sh"

nvm install node    
```

確認

```bash
node -v
```

* プロジェクトディレクトリの作成

ここでは TypeScriptという名前のディレクトリを利用することにします

```bash
mkdir TypeScript
cd TypeScript
```

* プロジェクトの初期化

package.json というプロジェクトの設定ファイルが生成されます

```bash
npm init -y
```

* tsc コマンドのインストール

tsc コマンドは，TypeScriptのコンパイラです．

```bash
npm install typescript @types/node -D
```

### tscコマンド

TypeScript のプログラム（ファイル名の拡張子は .ts） をコンパイルして javaScriptプログラム (ファイル名の拡張子 .js) を出力します

以下では，VScode エディタで実行することを想定していますが，ターミナルなどからの実行も同様です

* tsc のバージョンの確認
 
```
tsc -v
Version 5.9.2
```

* 実行環境のセットアップ
 
```bash
tsc --init
```

* 生成されたファイルの確認

tsconfig.json という設定ファイルが生成されています

```bash
ls
=>
tsconfig.json
```

## TypeScript 設定ファイル （tsconfig.json）

tsconfig.json  は，TypeScript 言語のコンパイルを行う tsc コマンドの設定ファイルです

* target
  * コンパイル先の javaScript のバージョンの指定
* module
  * javaScript が利用するモジュールの指定


## tsc コマンドによるコンパイル

TypeScriptコード → （tsc によるコンパイル） → javaScriptコード


* テスト用実行ファイル (index.ts)
  
  TypeScript のソースコードは，.ts という拡張子をつけます

```ts
console.log('こんにちは');
```

* TypeScriptコードのコンパイルと実行

```ts
tsc index.ts
```


* コンパイル結果の確認

index.js という javaScriptのファイルが生成されています

```bash
ls
index.js                index.ts                node_modules            package-lock.json       package.json            tsconfig.json
```

## Node.js によるコンパイルと実行

Node.js は，自動的に TypeScript プログラムののコンパイルとコンパイル結果の  javaScript コードの実行を行ってくれます

```bash
node index.ts
=>
こんにちは
```

## 変数への型アノーテーション

':' によって，変数に型を宣言することができます

javaSCriptにコンパイルされると，型アノーテーションは削除されてしまいます．

```ts
let 変数: 型
const 定数: 型
```

```ts
let name: string
const version: number
```

* 変数が初期値を持つ場合

型アノテーションをつけてもよいが， TypeScript 言語処理系は初期値によって型はわかるので冗長なので省略する方がよい．

```ts
let name: string = "Kindai"
```
と書いてもよいが，以下のように型アノーテーションを省略してよい

```ts
let name = "Kindai"
```

## TypeScript の基本型

基本型は javaScriptと同じです

### イミュータブルなオブジェクト 

（生成後，変更することができないオブジェクト）

* 数値  number
  * TypeScript では，javaScript と同様に整数と浮動小数点数を自動判別します．TypeScriptの型は基本的に人間のための型です
* 文字列  string
* 論理値  boolean
* bigint
  * 2^53 より大きな整数
* null
* undefined
  * 初期値を持たない変数は，当初 undefined 型になります
* Symbol

### 型チェックの例

* テスト用実行ファイル (index.ts)

```ts
let user: string
let age: number

user = "山崎"
age = 68
console.log(user)
console.log(age)
```

* コンパイルと実行

```bash
node index.ts
=>
山崎
68
```

* 型検査によるエラーが生じるケース

```ts
let user: string
let age: number

user = "山崎"
age = 68
console.log(user*2)
console.log(age*2)
```

string 型のオブジェクトには *2 という操作は定義されていないのでエラーになる

```bash
node index.ts
=>
NaN
136 
```

## リテラル型

特定の値を型とするもの

```ts
let inf: 'infinity'
inf = 'infinity'
```

## ユニオン型

複数の型の和を意味します

```ts
let data: number|string
data = "yamasaki"
data = 68
```

```ts
let num: number|'infinity'|'-infinity'
num = 100
num = 'infinity'
num = '-infinity'
```

## 型エイリアス

ユニオン型などの複雑な型アノーテーションを再利用したい場合，以下のように type キーワードを使って名前をつけることができます．

```ts
type 型エイリアス名 = 型アノーテーション
```

```ts
type compact_number = number|'infinity'|'-infinity'
let num: compact_number
num = 100
num = 'infinity'
num = '-infinity'
```

## 配列

### 配列の型指定

配列の型には [] 表記を使います

### 配列リテラルによる配列の生成

```ts
let arr: number[] = [1,2,3,4,5,6]
console.log(arr)
```

```bash
node index.ts
[ 1, 2, 3, 4, 5, 6 ]
```

```ts
let cities: string[] = ['鹿児島','福岡','大阪']
console.log(cities)
```

### Array() コンストラクタによる配列の生成

```ts
let arr3: number[] = new Array()
arr3.push(3)
arr3.push(4)
arr3.push(5)
console.log(arr3)
```

### 多次元配列

```ts
let arr4: number[][]
arr4 = [
  [1,2,3],
  [4,5,6]
]
console.log(arr4)
```

### ユニオン型配列

```ts
let arr5: (string|number|undefined)[]
arr5 = ['Yamasaki',68,,]
console.log(arr5)
```


### スプレッド演算子

... は配列リテラルの展開を意味する
配列リテラルの中に配列要素を入れることに利用できる

```ts
let a: number[]
let b: number[]
a = [1,2,3]
b=[0, ...a,4]
console.log(b)
// [ 0, 1, 2, 3, 4 ]

// 多重代入での利用
let car: number
let cdr: number[]
[car, ...cdr]=[1,2,3,4,5,6]
console.log(car)
//1 
console.log(cdr)
// [ 2, 3, 4, 5, 6 ]
```

## オブジェクト

オブジェクトのキーに対して，その値の型を定義できます

```ts
{ キー1: 型1；...; キーn: 型n}
```

### オブジェクトの生成

```ts
// オブジェクトリテラルによるオブジェクトの生成
let point: {x: number; y: number}
point = {x: 12, y: 333}

console.log(point)
// { x: 12, y: 333 }
```

型エイリアスを利用する方が再利用性が高くスマートです

```ts
type dimention2 = {x: number; y: number}
let point: dimention2
point = {x: 12, y: 333}

console.log(point)
// { x: 12, y: 333 }
```
## タプル型

固定サイズの配列のことです

```ts
let nameNumber: [string, number]
nameNumber = ['Jenny', 8675309];
```

## any型

anyは型システムのすべての型と互換性があります。any型の変数には何でも代入できます

```ts
let power: any;

power = '123';
power = 123;
```

## void

関数に返り値が無い場合などに使います

```ts
function log(message): void {
    console.log(message);
}
```


## Date型

Date型は javaScriptの型ですが，TypeScriptでも利用可能です

```ts
let date1: Date
date1 = new Date()
console.log(date1)
// 2025-08-25T02:29:16.071Z
```

## 関数定義

関数の型の基本は，関数の引数の型と返り値の型です．

* 必須引数の型
  * 関数には，必須の引数とオプションの引数があります
  * TypeScript の関数では必須引数への型アノーテーションによる型指定が必要です．

* 返り値の型
  * 一方で，返り値の型は推論可能です．
  初期化された変数と同様に型アノーテーションの指定は冗長なので省略するのが普通です

### function文による関数定義（定義文）

```ts
function(引数1: 型1, ..., 引数n: 型n): 返り値の型 { ... }
```


```ts
// reduce による配列の合計
function sumR(arr: number[]) {
    return arr.reduce((s,x)=>s+x,0);
}
let arr10 = [1,2,3,4,5,6,7,8,9,10]

console.log(sumR(arr10))
// 55

```

### アロー演算子 => による関数定義（定義式）

```ts
// 生成された関数を fact という定数に代入する
const fact = (x: number)=>{
    if(x==0) {
        return 1;
    } else {
        return x*fact(x-1);
    }
}
console.log(fact(20))
// 2432902008176640000
```

#### BigIntの場合

TypeScript で bigint を使うためには es2020 以上のライブラリが必要

また，数値の表記も `123n` ではなく，`BigInt(123)` のように表記します．

```ts
const factn = (x: bigint)=>{
    if(x==BigInt(0)) {
        return BigInt(1);
    } else {
        return x*factn(x-BigInt(1));
    }
}

console.log(factn(BigInt(100)))
// 93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000n
```

#### 無名関数

```ts
console.log(((x: number) => x*x)(10))
// 100
console.log(((a: number, b: number)=> a+b)(2,3))
// 5
```

### 範囲オブジェクトに対応する配列の生成の例

JavaScriptには，pythonやRubyの範囲オブジェクトに対応するものが無いので，次のようにして配列を生成する関数として作成することができる

```ts
// 0から n-1 の範囲
const range0 = ((n: number) => {return [...Array(n).keys()]})
console.log(range0(3))
// [ 0, 1, 2 ]

// 開始と終了の範囲
const range = ((from: number,to: number) =>{return [...Array(to-from+1)].map((_,i)=>i+from)})
console.log(range(7,20))
// [7,  8,  9, 10, 11, 12,  13, 14, 15, 16, 17, 18,  19, 20]
```

### 関数オブジェクトの型

関数はオブジェクトなので，変数に代入したり，関数の引数の値として入力したりすることができます．特にこのような引数はコールバック関数として多用されます．

このような関数オブジェクトの型は次のように記述します．

```ts
(引数1: 型1,...,引数n: 型n) => 返り値の型
```

### コールバック関数と高階関数

関数の引数として渡される関数オブジェクト
コールバック関数を引数にとる関数（関数を引数にとる関数）を高階関数といいます．

forEach, map, filter, reduce などのイテレータは高階関数で，コールバック関数を引数にとっていました．

```ts
let arr = [1,2,3,4,5,6]
// map で各要素を3倍した値の配列を出力する例
console.log(arr.map(x=>3*x))
// [ 3, 6, 9, 12, 15, 18 ]

// コールバック関数（要素を3倍する関数）の定数化
const m3 = x=>3*x
console.log(arr.map(m3))
// [ 3, 6, 9, 12, 15, 18 ]
```

### クロージャ

関数の変数環境（変数名とその値の辞書）は関数オブジェクトの一部として管理され，クロージャと呼ばれます．

クロージャは，関数が呼ばれたときの変数環境を保持します．

```ts
// 関数を出力する関数（yを引数にして x の y乗を行う関数が出力される）

const powN = (x: number) => {
     return (y: number) => x**y
 }

// pow2 は 2のべき乗関数
let pow2=powN(2)
console.log(pow2(3))
// 8

// pow3 は 3のべき乗関数
let pow3=powN(3);
console.log(pow3(3))
// 27

// カウンター （変数環境にカウンターの値が保存される）
const counter=()=>{
    let c=0;
    return ()=>{return c+=1}
}

let count=counter()

console.log(count())
// 1
console.log(count())
// 2
console.log(count())
// 3
console.log(count())
// 4
console.log(count())
// 5
```

### ジェネレータ

イテラブルなオブジェクトを生成する処理過程の「定義」です．

無限集合を生成する処理過程などを定義することができます．

関数定義の function のあとに * をつけるとジェネレータ（オブジェクト）になります．

★ アロー関数形式ではジェネレータを定義することはできません

ジェネレータの生成処理は基本的に停止しており，ジェネレータに対して next() メソッドを適用すると，次のステップの生成処理が行われます．

毎回の生成処理は，現在の位置から yield まで．
yield 文は関数の return 文のようにジェネレータのアウトプットを指定します．



```ts
// フィボナッチ数列を無限に生成するジェネレータ
// 論理的にはフィボナッチ数列という無限集合を意味しているとみなせます．
//  もしこれが関数なら，for(;;) という無限ループになります．

function* fib() {
    let x=0, y=1;
    for(;;){
        yield y;
        [x,y]=[y,x+y]
    }
}

let f=fib()

// ジェネレータオブジェクトの生成

// Object [Generator] {}

// ジェネレータに next() メソッドを適用してみる
console.log(f.next()['value'])
// 1
console.log(f.next()['value'])
// 1
console.log(f.next()['value'])
// 2
console.log(f.next()['value'])
// 3
console.log(f.next()['value'])
// 5
console.log(f.next()['value'])
// 8
```

### take で n 個取り出す

```ts
const take=(n: number, generator)=> {
    let range = ((from,to) =>{return [...Array(to-from+1)].map((_,i)=>i+from)})
    return range(1,n).map(()=>generator.next()['value'])
}

// フィボナッチ数列の集合を 5個ずつ取り出す
f=fib()

console.log(take(5,f))
[ 1, 1, 2, 3, 5 ]
console.log(take(5,f))
[ 8, 13, 21, 34, 55 ]
console.log(take(5,f))
[ 89, 144, 233, 377, 610 ]
console.log(take(5,f))
[ 987, 1597, 2584, 4181, 6765 ]
```

## ユニオン型とインターセクション型

* ユニオン型は，複数の型のいずれかに適合すればよい
* インターセクション型は，複数の型の全てに適合する必要がある

```ts
type Id = {
  id: number
  name: string
}

type User = {
  name: string
  email: string
}
// ユニオン型
type Id_or_User = Id|User
// インターセクション型
type Id_and_User = Id & User

const p1: Id_or_User = {
  id: 1,
  name: "yamasaki"
}
const p2: Id_or_User = {
  name: "taro",
  email: "taro@ee.aa"
}

const user: Id_and_User ={
  id: 3,
  name: "ichiro",
  email: "ichiro@aa.bb"
}

```

## ジェネリック型

型を抽象化したい場合があります．

例えば，javaScript の Map オブジェクトは，キーと値のペアからなるオブジェクトですが，キーと値の型は様々なものが利用できます．

しかし，キーと値の型が一度決まってしまうとそれを混在させることはできません．

* 型パラメータとジェネリック
  * このように型をパラメータ化して制約条件を与えことをジェネリックと呼びます
* 型引数
  * 型パラメータに代入される型を型引数といいます
* ジェネリック関数
  * 関数パラメータの引数の丸括弧の前に型パラメータを `<>` で囲んで 指定した関数です

```ts
ジェネリック関数<型パラメータ>(引数1,...,引数m)
```

### Map オブジェクトの例

Map オブジェクトのキーと値の型は次のようにして定義します

```ts
Map<キーの型, 値の型>()
```

```ts
const map1=new Map<string, number>()

// set
map1.set('a',1)
map1.set('b',1)
map1.set('c',2)

// get
console.log(map1.get('b'))
// 1
console.log(map1.get('c'))
// 2
console.log(map1.keys())
// [Map Iterator] { 'a', 'b', 'c' }
```

### Que クラスの例

FIFO のデータ構造で，格納するデータが数値の場合と文字列の場合

```ts
class Queue<T> {
  private data = [];
  push(item: T) { this.data.push(item); }
  pop(): T | undefined { return this.data.shift(); }
}

const queue = new Queue<number>()
queue.push(0)
queue.push(3)
queue.push(100)
console.log(queue.pop())

const queueS = new Queue<string>()
queueS.push("a")
queueS.push("b")
queueS.push("c")
console.log(queueS.pop())
```

## 非同期処理

ユーザの操作やデータの到着などのイベントが発生するまで処理の実行を保留する制御を非同期処理という

JavaScript言語では、非同期処理は通常コールバック関数として登録される

コールバック関数には、イベントに関連した詳細情報が引数として渡されることがあります

```
    イベント  →  コールバック関数（イベント情報）
```
### イベント駆動型プログラムの例

#### web ブラウザ上のユーザ操作とイベントリスナー

* webブラウザ上のイベント駆動型プログラミング

ブラウザ上のユーザのキーボード操作やマウスクリックなどのイベントに反応する処理をコールバック関数として登録するプログラミング．

* イベントリスナー

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
その応答を受け取ったというイベントの後に起動されるコールバック関数．

セッションを openし， send によってリクエストを送り，onload で応答を得る．

ネットワークイベントは成功するとは限らないので，異常事態に対するコールバック関数も必要になる．

```
    event1→ (成功) callback1
         └→ （失敗）callback2
    ...
```
#### ファイル入出力イベントのコールバック関数

ファイル入出力も OS にリクエストを送り，その応答をイベントとして受け取る．
ファイルの読み込みや書き込みに対する結果をイベントとして受け取った後に実行されるコールバック関数．

* 失敗時のコールバック関数

ファイル入出力も成功するとは限らないので，処理の失敗時に対応するコールバック関数も必要になる．

### コールバックチェーン

コールバック関数の実行によって，さらに次のイベントが発生させることができる．

そのイベントの完了に対する新たなコールバック関数を定義できる．

またさらに，そのイベントとコールバック関数が深く連鎖することがあります．これをコールバックチェーンと言います．

#### コールバックチェーンのエラーの補足

しかも、それぞれのイベントは失敗することもあるので，このイベントとコールバックの連鎖の中でエラーを捕捉したときに実行されるコールバック関数も必要になる．

```
    event1→ (成功) callback11 : event2 → （成功）callback21：event3 → ...
             └→ （失敗）callback12          └→ （失敗）callback22
```


### タイマーによる非同期処理

一定時間が経過したというイベントの後に関数を実行するタイマー関数は，非同期処理の典型です

`setTimeout(コールバック関数, delay)`

delayは、ミリ秒

```ts
//  コールバック関数
const f1=()=>console.log("処理1")
const f2=()=>console.log("処理2")
const f3=()=>console.log("処理3")

// タイムアウト・イベントの登録
let t1=setTimeout(f1, 3000);
let t2=setTimeout(f2, 2000);
let t3=setTimeout(f3, 1000);
```

一定時間が経過したというイベントの後に関数を実行するタイマー関数

`setInterval()`

delayは、ミリ秒

```ts
//  コールバック関数
const f4=()=>console.log("5秒経過")

// タイムアウト・イベントの登録
let t4=setInterval(f4, 5000);

// ^C で停止
```


### Promiseによる非同期処理 （★ 重要）

非同期処理は，処理がどのような順番で結果が返ってくるかわかりません．

これを同期処理のようにイベントの完了に沿って、順番どおりにコールバック関数を逐次化して実行するしくみが promise です．

特に複雑化しやすいエラー処理を含むコールバックチェーンを簡潔に記述するためのものです

Promise は JavaScript コアのオブジェクトに備わっています．

#### Promise オブジェクトのメソッド

* then()    : イベント成功時のコールバック関数の登録
* catch()   ：イベント失敗時のコールバック関数の登録
* finally() ：イベントの成功／失敗にかかわらず最終的に行われるコールバック関数の登録
* all()     ：並行実行される複数のイベントがすべて完了したときに実行されるコールバック関数の登録
* race()    ：並行実行される複数のイベントのうち最初のものが完了したときに実行されるコールバック関数の登録

#### Promise チェーン

then() メソッドで登録されたコールバック関数の戻り値は再び Promise オブジェクトになります．

この Promise オブジェクトにもコールバック関数が登録されるので、コールバック関数の入れ子が構成できます。

そしてこの then()メソッドの連鎖によって非同期処理を順番どおりに実行させることができます．

この逐次処理化したコールバックチェーンを Promiseチェーンと呼びます．

Promise チェーンは、下のように記述します

```ts
promise
    .then(callback1)
    .then(callback2)
    ...
    .then(callbackn)
    .catch(callbackErr)
    .finally(collbackFinal)
```

このチェーンのどこかでエラーが発生した場合は，エラーを chach() メソッドで捕捉できます．
finally() メソッドで登録されたコールバック関数は，promiseチェーンが成功でも失敗でも最終的に実行される．

* エラーの補足

```ts
event1 → (成功) callback1 : event2 ┬ → (成功）callback2 : ... ┬→ eventn: collbackFinal
      └---------------------------└→(失敗) callbackErr : ----┘      
```

#### Promise の非同期処理の並行実行

Promise は，非同期処理の並行実行にも利用できます．

all() メソッドは，並行実行するコールバック関数を配列で登録し，
そのすべての完了を待ちます．

```ts
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

#### Promise の並行処理の最初の応答を受け取る

race() メソッドは、並行実行するコールバック関数の最初の結果を受け取ると完了する．

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

### await / async （★重要）

Promise チェーンも複雑化するとデバッグが困難になります．

await / async による記法は，Promise の使用法を劇的に簡略化する構文で，Promise の複雑性を事実上隠蔽します．


#### async 関数

* async は非同期関数を宣言するキーワードです．
* async で定義された関数を async 関数と言います。
* async 関数の返り値は Promise オブジェクトになります。
* 非同期処理を内部に含む関数は，非同期関数として宣言しなければなりません。


#### await

* await は Promise オブジェクトを受け取り，その完了を待ち，その返り値やエラーに変換します．
* await が指定されたコードを含む関数は非同期関数なので， async 関数として定義しなければなりません．
* async 関数の実行結果を得るためには，関数呼び出しに await を指定しておく必要があります．

* `await p` 
  *  Promise オブジェクト p の完了を待つという意味になります。
  * その返り値が Promise オブジェクトなら、さらにそれを await することで逐次的に処理を行うことができます。

* 例
  
下記の例の非同期関数 fetch は、http を使って url のデータを GET する Promise オブジェクトです．

この例は、２つの非同期処理を await によって逐次処理化するものです。

1. await を使ってURLへの http GET の応答を得るまで完了を待ち，その結果を変数 resp に代入します。
2. response オブジェクトの json() メソッドの非同期処理によって，JSON データを取り出します．それが完了すると 変数 data に結果を代入します．

```ts
// 気象庁の福岡県の天気予報の概要を取得するURL
const url =
  "https://www.jma.go.jp/bosai/forecast/data/overview_forecast/400000.json";

async function httpget() {
  // URLからデータを非同期で取得
  const response = await fetch(url);
  // 取得したレスポンスをJSON形式に変換
  const data = await response.json();
  // 取得したJSONデータを整形してコンソールに出力
  console.log(JSON.stringify(data, null, 2));
}

httpget();
```


```bash
node index.ts
=>
{
  "publishingOffice": "福岡管区気象台",
  "reportDatetime": "2025-08-25T16:34:00+09:00",
  "targetArea": "福岡県",
  "headlineText": "",
  "text": "　福岡県は、高気圧に覆われておおむね晴れとなっていますが、暖かく湿った空気や強い日射の影響により曇りで、雨が降っている所があります。\n\n　２５日は、高気圧に覆われておおむね晴れとなりますが、暖かく湿った空気や強い日射の影響により曇りで、雷を伴った激しい雨が降る所があるでしょう。\n\n　２６日は、高気圧に覆われておおむね晴れとなりますが、暖かく湿った空気や強い日射の影響により曇りで、雷を伴った激しい雨が降る所があるでしょう。\n\n＜天気変化等の留意点＞\n　福岡地方では、２５日１８時から２６日１８時までに予想される降水量は、多い所で、１時間に２０ミリ、２４時間に３０ミリの見込みです。また、２６日は、気温が著しく高くなることにより、熱中症による人の健康に係る被害が生ずるおそれがあります。熱中症予防のための行動をとってください。"
}
```


上記のプログラムで  await をつけないと，以下のように 変数 data は fetch() による http GETの完了を待たない状態のオブジェクトが代入されるので，内容は空にです．

```js
const url =
  "https://www.jma.go.jp/bosai/forecast/data/overview_forecast/400000.json";

async function httpget() {
  const response = await fetch(url);
  // await をつけない場合
  const data = response.json();
  console.log(JSON.stringify(data, null, 2));
}

httpget();
```

```bash
node index.ts
=>
{}
```

上記の response はPromiseオブジェクトなので，then() メソッドを使って以下のようにコールバックチェーンで処理をつなげることと同じ意味です。

then メソッドの中のアロー関数の引数の response にはfetch の返り値の Promise オブジェクトが束縛されることになります

```js
const url =
  "https://www.jma.go.jp/bosai/forecast/data/overview_forecast/400000.json";

async function httpget() {
  const data = await fetch(url).then(resp=>resp.text())
  console.log(JSON.stringify(data, null, 2));
}

httpget();
```
