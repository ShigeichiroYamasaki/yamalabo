#  chai と mocha による JavaScript プログラムのテスト駆動開発

last update 2024/09/23
Shigeichiro Yamasaki

## 目次

* [概要と前提条件](#overview)
* [プロジェクトの作成](#project)
* [chai](#chai)
* [mocha](#mocha)
* [テストの記述方法](#testdescription)
* [mochaによるテストの実行](#execution)

##  <a id="overview"> </a>概要と前提条件

JavaScriptプログラムのテストを行うためのモジュールとして次の2つを利用することにします．

* chai: JavaScript のアサーションツール
  テストの内容を定義します

  chai のサイト <https://www.chaijs.com/>

* mocha: JavaScriptのテストフレームワーク

  テストの実行環境

  mocha のサイト <https://mochajs.org/>


* 前提条件

[Node.js と npm はインストール済とします](https://github.com/ShigeichiroYamasaki/yamalabo/blob/master/JavaScript/JavaScript.md)

##  <a id="project"> </a>プロジェクトの作成

開発するアプリケーションプロジェクトとそのテスト環境を作成します

このプロジェクトの中に chai と mocha のモジュールをインストールします

### プロジェクトルート・ディレクトリの作成

新しいNode.js プロジェクト（パッケージ）のディレクトリを作成します．

ここでは，math-for-crypto というプロジェクトを作成するものとします．

```bash
$ mkdir math-for-crypto
$ cd math-for-crypto
```

このディレクトリをプロジェクトルート・ディレクトリと呼ぶことにします．

プロジェクトルートに移動する操作を以下のように表記します

```bash
$ cd <プロジェクトルート>
```

### npm プロジェクトの初期化

Node.js のパッケージとして初期化します．

その結果 package.json ファイルが作られます．

★ test コマンドは mocha にします

```bash
$ cd <プロジェクトルート>
$ npm init

...

package name: (math-for-crypto) 
version: (1.0.0) 
description: 暗号用の数学的関数
entry point: (math.js) 
test command: mocha
git repository: 
keywords: 
author: Shigeichiro Yamasaki
license: (ISC) 
About to write to /Users/shigeichiroyamasaki/git/yamalabo/JavaScript/math-for-crypto/package.json:

{
  "name": "math-for-crypto",
  "version": "1.0.0",
  "description": "暗号用の数学的関数",
  "main": "math.js",
  "directories": {
    "test": "test"
  },
  "scripts": {
    "test": "mocha"
  },
  "author": "Shigeichiro Yamasaki",
  "license": "ISC"
}


Is this OK? (yes) 
```

### mocha と chai のインストール

```bash
$ cd <プロジェクトルート>
$ npm install mocha chai --save-dev
```
##  <a id="chai"> </a>chai

以下の３つのテストの宣言方法がある

* assert 
* should
* expect

assert  は宣言的な方法で TDD 型開発用

should と expect は BDD 型開発用

### chai のモジュール形式

Node.js は，デフォルトではモジュールを歴史的に使われてきた CommonJS 形式のものとして扱います．

chai のモジュールは CommonJS 形式もありますが，基本的に現在の標準である ES6 のESモジュールとして作られています．

#### モジュールのインポート方法の違い

* CommonJSモジュール の場合：
  `require(モジュール名)` でインポートする
* ES モジュールの場合：複数の方法がある
  `import {名前} from パス` でインポートする（package.jsonの修正やファイル名の修正が必要）

  動的インポートを利用する

  `await import()` でインポートできる

  ただし，await は async 関数の中でのみ利用できることに注意


### assertの例

assert APIの一般構造
```
assert(式, エラーメッセージ)
```

### chai によるテストの例

#### chai による仕様の定義

node.js を起動して実行

```bash
$ node

> 
```

#### ユークリッドの互除法による最大公約数を求める gcd関数のテスト

関数gcd の仕様を次のように定義します

```
2つの整数 a b に対して

a%b==0 のとき、最大公約数は b

そうでないとき gcd(a,b) = gcd(b,a%b)

p,q,r が素数のとき
a=p*q, b=p*r なら、最大公約数は p

a,b が互いに素のとき、最大公約数は 1

```


以下は node.js のプロンプトからインタラクティブに実行

```js
> const chai = async ()=>{return await import('chai')};
> let c = await chai();
> let assert=c.assert

// 関数 gcd のアサーションを定義する
//   b==0 のとき、最大公約数は a （定義する）
> let p=11;
> let q=13;
> let r=17;
>  assert(gcd(p*q,q) == q);
Uncaught ReferenceError: gcd is not defined
> assert(gcd(p*q,p*r) == p);
Uncaught ReferenceError: gcd is not defined
>  assert(gcd(p,q) == 1);
Uncaught ReferenceError: gcd is not defined

//実行すると関数 gcd が未定義というエラーが出る
```

関数 gcd の定義

```js
> const gcd=(a,b)=>{
    if (a%b==0) {
      return b;
    } else {
      return gcd(b,a%b);
    }
}
```

テストの再実行

```js
> assert(gcd(p*q,q) == q);
undefined
> assert(gcd(p*q,p*r) == p);
undefined
> assert(gcd(p,q) == 1);
undefined
```
エラーが起きないで終了

##  <a id="mocha"> </a>mocha 

### テストプログラム用のディレクトリの作成

プロジェクトルートの下に test というディレクトリを作成します

```bash
$ cd <プロジェクトルート>
$ mkdir test
$ cd test
```
### mocha の設定ファイル

プロジェクトルートディレクトリに `mocha.config.js` ファイルを作成します

```bash
$ cd <プロジェクトルート>
$ nano mocha.config.js
```

mocha.config.js

```js
module.exports={
  // テストのディレクトリを test にする
  spec: 'test/**/*.spec.js',
  // テストスイートのタイムアウトは5秒
  timeout: 5000,
  // 使用するテストインターフェース TDD/BDD
  ui: 'tdd',
  // レポーターの指定
  reporter: 'spec'
}
```

### npx コマンドによる mocha テストの実行

```bash
$ cd <プロジェクトルート>
$ npx mocha
```


### package.json ファイルの設定内容の確認

プロジェクトルート・ディレクトリに移動

Node.js プロジェクトの設定を行う package.json ファイルを確認する

```bash
$ cd <プロジェクトルート>
$ nano package.json
```

```json
  "name": "math-for-crypto",
  "version": "1.0.0",
  "description": "暗号用の数学的関数",
  "main": "math.js",
  "directories": {
    "test": "test"
  },
  "scripts": {
    "test": "mocha"
  },
  "author": "Shigeichiro Yamasaki",
  "license": "ISC",
  "devDependencies": {
    "chai": "^5.1.1",
    "mocha": "^10.4.0"
  }
}

```

##  <a id="testdescription"> </a>mocha によるテストの記述方法

mocha によるテスト記述の文法を説明します

### Describe ブロック

関連する複数のテストをグループ化するためのものです．

一般的に機能的なまとまりやコンポーネントを１つのDescribe ブロックにします．

#### 階層化 describe ブロック

describe ブロックは階層化させることができます．

### it ブロック

descripbe ブロックの中で各テストケースを定義するためのものです．

1つのテストケースごとに１つの it ブロックを作成します．

### アサーション

仕様を意味する挙動を定義することです．

典型的には実行の結果が満たすべき等式や不等式や型などとして定義されます．

assert, expect, should などの定義方法があります．

assertは，引数の式が`真` であることを期待します．

また，結果が `真` でないときにテストは失敗します．
失敗時のエラーメッセージで原因調査に有用な情報を上げることができます．

### テスト対象プログラムの作成

プロジェクトルート・ディレクトリ

テスト対象プログラムのファイルを math.js とします．

```bash
$ cd <プロジェクトルート>
$ nano math.js
```

```js
const gcd=(a,b)=>{
    if (a%b==0) {
      return b;
    } else {
      return gcd(b,a%b);
    }
}

module.exports = {gcd};

```

### テストファイルの作成

```bash
$ cd <プロジェクトルート>
$ cd test
$ nano mathtest.js
```

#### mathtest.js の内容

* テストは test ディレクトリをカレントディレクトリとして実行されるので、テスト対象モジュールのファイルはここからの相対パスになります
* require 関数でソース・ファイルから gcd 関数をインポートします
* chai モジュールは await で動的インポートするので，それを行う非同期関数を定義します．
* さらに assert 関数を利用するときもテスト項目の it ごとに非同期関数を使って assert関数をインポートします．



```js
const chai = async ()=>{return await import('chai')};
const math = require('../math.js');

// ユークリッド互除法
describe('gcd関数のテスト', function () {
  describe('#dcg', function () {
        let p=11;
        let q=13;
        let r=17;
        it('a%b==0 のとき、最大公約数は b', async function () {
                let c = await chai();
                let assert = c.assert;
                assert(math.gcd(p*q,q) == q);
        });
        it('p*q と p*r の最大公約数は p', async function () {
                let c = await chai();
                let assert = c.assert;
                assert(math.gcd(p*q,p*r) == p);
        });
        it('a b が互いに素のとき最大公約数は 1', async function () {
                let c = await chai();
                let assert = c.assert;
                assert(math.gcd(p,q) == 1);
        });

  });
});
```

## <a id="execution">mochaによるテストの実行


```bash
$ npx mocha


  gcd関数のテスト
    #dcg
      ✔ a%b==0 のとき、最大公約数は b
      ✔ p*q と p*r の最大公約数は p
      ✔ a b が互いに素のとき最大公約数は 1


  3 passing (7ms)

```
