#  chai と mocha による JavaScript プログラムのテスト駆動開発

last update 2024/05/09
Shigeichiro Yamasaki

* chai: javascript のアサーションツール
  テストの内容を定義します

  chai のサイト <https://www.chaijs.com/>

* mocha: javascriptのテストフレームワーク

  テストの実行環境

  mocha のサイト <https://mochajs.org/>


## 前提条件

node.js と npm はインストール済とします

## インストール

npm で chai をインストール

```bash
$ cd ~
$ npm install -g chai
```

npm で mocha をインストール

```bash
$ cd ~
$ npm install -g mocha
```

### テスト用ディレクトリの作成

```bash
$ mkdir test
```
### mocha 設定ファイル mocha.config.js

```bash
$ cd ~
$ nano mocha.config.js
```

mocha.config.js の内容

```js
module.exports={
  // テストのディレクトリ
	spec:'test/**/*.spec.js',
	timeout: 5000,
  // テスト用インターフェース TDD/BDD
	ui: 'tdd',
	reporter: 'spec'
};

```

## chai

以下の３つのテストの宣言方法がある

* assert 
* should
* expect

assert  は宣言的な方法で TDD 型開発用

should と expect は BDD 型開発用

### assertの例

assert APIの一般構造
```
assert(式, エラーメッセージ)
```

javaScript のテストコード

```js
const assert = require('chai').assert
foo = 'bar';
beverages = { tea: [ 'chai', 'matcha', 'oolong' ] };

assert.typeOf(foo, 'string'); // エラーメッセージなし
assert.typeOf(foo, 'string', 'fooは string型'); // エラーメッセージ付き
assert.equal(foo, 'bar', 'foo equal `bar`');
assert.lengthOf(foo, 3, 'foo`の値の長さは 3');
assert.lengthOf(beverages.tea, 3, 'beverages has 3 types of tea');
```

### shouldの例

javaScript のテストコード

```js
const should = require('chai').should();
foo = 'bar';
beverages = { tea: [ 'chai', 'matcha', 'oolong' ] };

foo.should.be.a('string');
foo.should.equal('bar');
foo.should.have.lengthOf(3);
tea.should.have.property('flavors')
  .with.lengthOf(3);
```

### expectの例

javaScript のテストコード

```js
const expect = require('chai').expect;
foo = 'bar';
beverages = { tea: [ 'chai', 'matcha', 'oolong' ] };

expect(foo).to.be.a('string');
expect(foo).to.equal('bar');
expect(foo).to.have.lengthOf(3);
expect(beverages).to.have.property('tea').with.lengthOf(3);
```

### assert スタイルのアサーションメソッド一覧

assertスタイルを利用する場合、以下をおぼえておけばよい

```
isOk()              // true
deepEqual()         // 同値
isAbove()           // >
isAtLeast()         // >=
isBelow()           // <
isAtMost()          // <=
isTrue()            // == true
isFalse()           // == false
isNull()            // == null
isNaN()             // isNan()
isUndefined()       // === undefined
isFunction()        // Function型
isObject()          // Object型
isArray()           // Array型
isString()          // String型
isNumber()          // Number型
isBoolean()         // Boolean型
typeOf()            // 型チェック
instanceOf()        // 型のインスタンスである
include()           // ex. 'foobar' includes 'foo'
lengthOf()          // サイズ
match()             // RE
```


### chai によるテストの例

### ユークリッドの互除法による最大公約数を求める gcd関数のテスト

関数gcd の仕様

```
a%b==0 のとき、最大公約数は b
そうでないとき gcd(a,b) = gcd(b,a%b)
p,q,r が素数で
a=p*q, b=p*r なら、最大公約数は p
a,b が互いに素のとき、最大公約数は 1

```

#### chai による仕様の定義

node.js を起動して実行

```bash
$ node

Welcome to Node.js v20.10.0.
Type ".help" for more information.
> 
```

以下は node.js のプロンプトから実行

```js
> const assert = require('chai').assert

// 関数 gcd のアサーションを定義する
//   b==0 のとき、最大公約数は a （定義する）
> let p=11;
> let q=13;
> let r=17;
> assert.equal(gcd(p*q,q) ,q);
> assert.equal(gcd(p*q,p*r) ,p);
> assert.equal(gcd(p,q) ,1);

//実行すると関数 gcd が未定義というエラーが出る
Uncaught ReferenceError: gcd is not defined
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
> assert.equal(gcd(p*q,q) ,q);
> assert.equal(gcd(p*q,p*r),p);
> assert.equal(gcd(p,q) ,1);
```
エラーが起きないで終了

## mocha 

### テスト対象のモジュールの作成

#### テスト対象モジュールのディレクトリ

```
~/src/
```

とする

```bash
$ mkdir ~/src
$ cd ~/src

$ nano math.js
```

#### テスト対象モジュール

ファイル名 math.js

javascriptのモジュール機構を使って gcd 関数をエクスポートします

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
$ cd ~/test
$ nano mathtest.js
```

#### mathtest.js の内容

* テストは ~/test をカレントディレクトリとして実行されるので、テスト対象モジュールのファイルはここからの相対パスになります
* require 関数でソース・ファイルから gcd 関数をインポートします


```js
const assert = require('assert');
const math = require('../src/math.js');

describe('gcd関数のテスト', function () {
  describe('#gcd', function () {
        let p=11;
        let q=13;
        let r=17;
        it('a%b==0 のとき、最大公約数は b', function () {
                assert.equal(math.gcd(p*q,q) ,q);
        });
        it('p*q と p*r の最大公約数は p', function () {
                assert.equal(math.gcd(p*q,p*r) ,p);
        });
        it('a b が互いに素のとき最大公約数は 1', function () {
                assert.equal(math.gcd(p,q) ,1);
        });

  });
});
```

### npx を使ったmocha のテスト実行

シェルから実行

```bash
$ cd ~
$ npx mocha

  gcd関数のテスト
    #gcd
      ✔ a%b==0 のとき、最大公約数は b
      ✔ p*q と p*r の最大公約数は p
      ✔ a b が互いに素のとき最大公約数は 1


  3 passing (2ms)


```

