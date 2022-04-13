# JavaScript言語入門

node.jsをインストールしておく

ubuntu

```bash
sudo apt install nodejs
```

MacOSX

```bash
brew install nodejs
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

## 基本操作

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
> 1==2
false
> 1<2
true
> 1<=2
true
> 1>2
false
> Math.PI
3.141592653589793
> Math.E
2.718281828459045
> Math.sin(Math.PI/2)
1
> Math.log(Math.E)
1
> Math.sqrt(2)
1.4142135623730951
> (Math.sqrt(2))**2
2.0000000000000004
> (2+5)*7
49
> 1/0
Infinity
> Infinity+1
Infinity
> "hello"
'hello'
```

### 標準出力への出力

print文にあたるもの

```js
> console.log('hello')
hello
undefined

```

### オブジェクトの型

数値

```js
> 23
> 9.24
> 2.99e8 # 2.99 * 10^8 = 299000000
```

文字列

```js
> "hello"+"world"
'helloworld'
> 'hello'+'world'
'helloworld'
> "I am ${10*6} years old"
'I am ${10*6} years old'
> `I am ${10*6} years old`
'I am 60 years old'
> "hello 'hello' hello"
"hello 'hello' hello"
> "hello \"hello\" hello"
'hello "hello" hello'
```

ブーリアン

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

Nullとundefined

```js
> null
null
> undefined
undefined
```

オブジェクトの型を確認するメソッド

```js
> typeof 1
'number'
> typeof 1.1
'number'
> typeof 'aaa'
'string'
> typeof true
'boolean'
> typeof null
'object'
> typeof Infinity
'number'
```

### 演算子

論理演算子

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

### 式

セミコロンで終わるのが基本
省略できることもあるが付けておいた方がよい

```js
> 1+2;
3
```

### 代入（変数へのバインディング）

```js
> let x=2;
undefined
> x
2
> x*10
20
```

変数が未定義の場合はエラーになります

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

定数定義

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

変数名の文法

* アルファベットの大文字でも小文字でも使えます
* 数字から始まってはいけません
* $ と _ は使えますが、それ以外の句読点や特殊文字は使えません
* 予約後が使えません


### 制御構造

if文

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

while文

```js
> let n=1;
> let s=0; > while (n<=10) {
 s=s+n;
 n=n+1;
 }
 
11
> s;
55
```

do while文

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

for文

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


### 関数

関数を実行することを「評価する」引数がある場合は「適用する」といいます。

関数を評価した結果、戻り値が返ってきます。

```js
> const square=function(x) {
    return x*x;
}

> square(3)

9

> const fizzbuzz=function(n){
    if(n%3==0 && n%5==0) {
        return "FizzBuzz";
    } else if (n%3==0) {
        return "Fizz";
    } else if (n%5==0) {
        return "Buzz";
    }
}

> fizzbuzz(3)
'Fizz'
> fizzbuzz(5)
'Buzz'
> fizzbuzz(15)
'FizzBuzz'
> fizzbuzz(150)
'FizzBuzz'
> fizzbuzz(16)
undefined
```

=> による関数定義

```js
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


function による関数定義

```js
> function sum(n) {
    if(n==0) {
        return 0;
    } else {
        return n+sum(n-1);
    }
}

> sum(10)
55
> sum(100)
5050
```

無名関数

```js
> ((x) => x*x)(10)
100

> ((a,b)=> a+b)(2,3)
5
```

クロージャ

```js
> function env(x) {
     let a=x;
     return (y)=>a*y;
 }

> let f1=env(2);
> let f2=env(3);

> f1(5)
10
> f2(5)
15
```

### 文字列処理

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

### 配列

```js
> let arr=[1,2,3,4,5,6];
undefined
> arr[0]
1
> arr[1]
2
> arr[3]
4
> arr.length;
6
```

### イテレータによる繰り返し

ループ以外に、配列のようにオブジェクト自体が持つ繰り返し構造を利用した繰り返し制御が可能です

配列に対する map, filter, reduce, for of

```js
> arr.map(function(x){return x;})
[ 1, 2, 3, 4, 5, 6 ]

> arr.filter(function(x){return x>3;})
[ 4, 5, 6 ]

> arr.reduce(function(s,x){return s+x;})
21

//無名関数の場合

> arr.map(x=>x)
[ 1, 2, 3, 4, 5, 6 ]

> arr.filter(x=>x>3)
[ 4, 5, 6 ]

> arr.reduce((s,x)=>s+x)
21

> let ss=0
> for(n of arr){ss+=n}
21

```

文字列に対するイテレータ

```js
let str="abcdefghijklmnopqrstu"

Array.from(str).map(function(x){return x+x;})
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


### 非同期処理

通常のJavascriptの処理は、次々に逐次的に実行される。
しかし、ある処理が完了するまで、実行を遅延や保留しておきたい場合がある。
そのような処理を非同期処理という

#### setTimeout() による非同期処理

一定時間経過後に関数を実行する

`setTimeout(function, delay);`

delayは、ミリ秒

```js
> let t1=setTimeout(() => {console.log("event1")}, 5000);
> let t2=setTimeout(() => {console.log("event2")}, 3000);
> let t3=setTimeout(() => {console.log("event3")}, 1000);
 
event3
event2
event1
```

#### Promiseによる非同期処理

ある処理が完了するまでの遅延処理を「プロミス」という

JavaScriptの場合、プロミスは、Promiseクラスのインスタンスオブジェクトを使って実現される。

Promiseのインスタンスオブジェクトは状態を持ち、非同期処理の結果に応じて状態が決定されて値がバインドされます。


 Promiseの状態

* 待機 (pending): 初期状態。成功も失敗もしていません。
* 履行 (fulfilled): 処理が成功して完了したことを意味します。
* 拒否 (rejected): 処理が失敗したことを意味します。


![](https://developer.mozilla.org/ja/docs/Web/JavaScript/Reference/Global_Objects/Promise/promises.png)

```js
new Promise(function(resolve, reject) {resolve('成功');});
new Promise(function(resolve, reject) {reject('失敗');});
```

thenを使って完了後に実行する処理を実行する

```js
console.log("event1");
const promise=new Promise((resolve) => {
  setTimeout(() => {console.log("event2");resolve();}, 5000);
}).then(() => {console.log("event3");});
```

これを実行すると以下のようにevent2 が完了した後に event3 が実行される

```js
event1
event2
event3
```

Promiseの書き方

```js
const promise = new Promise((resolve, reject) => {});
```