#  chai と mocha による JavaScript プログラムのテスト駆動開発

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
cd ~
npm install -g chai
```

npm で mocha をインストール

```bash
cd ~
npm install -g mocha
```

### テスト用ディレクトリの作成

```bash
mkdir test
```
### mocha 設定ファイル mocha.config.js

```bash
cd ~
nano mocha.config.js
```


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

## chai の使い方

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

```js
const expect = require('chai').expect;
foo = 'bar';
beverages = { tea: [ 'chai', 'matcha', 'oolong' ] };

expect(foo).to.be.a('string');
expect(foo).to.equal('bar');
expect(foo).to.have.lengthOf(3);
expect(beverages).to.have.property('tea').with.lengthOf(3);
```

### アサーションメソッド

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


## chai によるテストの作成

### ユークリッドの互除法による最大公約数の例

関数 gcd の仕様
```
b==0 のとき、最大公約数は a （と定義する）
そうでないとき gcd(a,b) = gcd(b,a%b)
p,q,r が素数で
a=p*q, b=p*r なら、最大公約数は p
a,b が互いに素のとき、最大公約数は 1

```

#### chai による仕様の定義

node.js で実行

```bash
node
Welcome to Node.js v20.10.0.
Type ".help" for more information.
> 
```

以下は node.js のプロンプトから実行

```js
const assert = require('chai').assert

// 関数 gcd のアサーションを定義する
//   b==0 のとき、最大公約数は a （定義する）
p=11;
q=13;
r=17;
assert.equal(gcd(p*q*r,0) ,p*q*r);
assert.equal(gcd(p*q,p*r) ,p);
assert.equal(gcd(p,q) ,1);

//実行すると関数 gcd が未定義というエラーが出る
Uncaught ReferenceError: gcd is not defined
```

関数 gcd の定義

```js
const gcd=(a,b)=>{
    if (b==0) {
      return a;
    } else {
      return gcd(b,a%b);
    }
}
```

テストの実行

```js
p=11;
q=13;
r=17;
assert.equal(gcd(p*q*r,0) ,p*q*r);
assert.equal(gcd(p*q,p*r) ,p);
assert.equal(gcd(p,q) ,1);
```
エラーが起きないで終了

## mocha 

### テスト対象のモジュール

#### ディレクトリ

~/src/

とする

```bash
mkdir ~/src
cd ~/src

nano math.js
```

ファイル名 math.js

モジュール機構を使って gcd 関数をエクスポートします

```js
const gcd=(a,b)=>{
    if (b==0) {
      return a;
    } else {
      return gcd(b,a%b);
    }
}

module.exports = {gcd};
```

### テストファイルの作成

```bash
cd ~/test
nano gcdtest.js
```

require 関数でソース・ファイルから gcd 関数をインポートします

```js
const assert = require('assert');
const math = require('../src/math.js');

describe('gcd関数のテスト', function () {
  describe('#b==0', function () {
        let p=11;
        let q=13;
        let r=17;
        it('b==0 のとき、最大公約数は a （と定義する)', function () {
                assert.equal(math.gcd(p*q*r,0) ,p*q*r);
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


```bash
cd ~
npx mocha

  gcd関数のテスト
    #b==0
      ✔ b==0 のとき、最大公約数は a （と定義する)
      ✔ p*q と p*r の最大公約数は p
      ✔ a b が互いに素のとき最大公約数は 1


  3 passing (2ms)


```

