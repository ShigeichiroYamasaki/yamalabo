# Ethereum Solidityによるスマートコントラクト

## Solidityの文法

### 文（Statement）にはセミコロン

### コメント

* // 単一行のコメント
* /* ～ */ 複数行のコメント

### Version Pragma

```
pragma solidity >=0.4.16 <0.9.0;
```

コンパイラ命令
指定されたバージョン以外のコンパイラではコンパイルされない事を保証します。

上の例では、「0.4.16以上、0.9.0 未満のコンパイラ」という意味です。

`^` はマイナーリビジョンの意味で、小数点2桁目以上のバージョンは許容するという意味で、小数点1桁目のバージョンアップは許容しない


### データ型

|基本型|記述内容|
| :-- | :-- |
|整数| int, uint (符号なし整数)|
|固定小数点型| fixed, ufixed  例：ufixed32x2(整数部32ビット小数部2ビット）|
|アドレス|address (20バイトのEthereumアドレス)| 
|ブーリアン型| true, false, !(否定）,&&(論理積)、`||`(論理和), ==(等価)、!=(非等価)|

|参照型|記述内容|
|:-- |:--|
|固定バイト配列型|bytes1 ~ bytes32|
|動的バイト配列型|bytes, string|
|列挙型|enum NAME {LABEL1, LABEL2,...}|
|配列型|uint32[][5] は符号なし整数の5つの配列の配列|
|Struct型| struct NAME {TYPE1 VARIABLE1; TYPE2 VARIABLE2; ...}|
|Mapping型|mapping (KEY_TYPE => VALUE_TYPE) NAME (キー=>値ペアのハッシュテーブルの作成)|

#### 変数宣言の例

```
int x;
```

#### Ether通貨単位

基本単位は wei = 10^(-18) ether

```
require(withdraw_amount <= 100000000000000000);
```

は、以下のように記述可能

```
require(withdraw_amount <= 0.1 ether);
```

### グローバル変数

block, msg, tx, address などのオブジェクトがスマートコントラクトからアクセス可能
意味は想像可能だが、正確な内容は仕様書で確認してください

#### masg

* msg.sender
* msg.value
* msg.gas
* msg.data
* msg.sig

### tx

* tx.gasprice
* tx.origin

#### block

* block.blockhash(BLOCKNUMBER)
* block.coinbase
* block.difficulty
* block.gaslimit
* block.number
* block.timestamp

#### address

* address.balance
* address.transfer(AMOUNT)
* address.send(AMOUNT)
* address.call(PAYLOAD)
* address.callcode(PAYLOAD)
* address.delegatecall()

#### 組み込み関数

* addmod ：剰余和
* mulmod ：剰余積
* keccak256
* shar256
* sha3
* ripemd160
* ecrecover：署名からアドレスを復元
* selfdestruct(RECIPIENT_ADDRESS)：現在のコントラクトを削除してアカウントに残っているEtherを受信者アドレスに送付


## コントラクトの定義

Solidityのトップレベルのオブジェクトの型が　contract　

つまり、Solidityプログラムとは、コントラクトオブジェクト（データとメソッド）を定義すること

ただし、コントラクトに類似した、interface, library というオブジェクトも存在する。



### 関数

EOAトランザクションや別のコントラクトから呼び出すことが可能な関数の定義


```
  function say(string text) public returns (string) {
    return text;
  }
```

* 関数名（無名関数も可能）
* 引数
* visibility (public|external|internal（コントラクト内からのみ呼び出し可能）|private（他のコントラクトから呼び出し不可）)
* 状態変更 (constant|view|pure|payable)
* returns (戻り値の型)


★（注意）internal や private な関数定義も、ブロックチェーンのデータとしては公開されていて秘匿化はされていない。

### Ethereumのコントラクトのアトミック性

Ethereumのコントラクトを実行するトランザクションは、成功するか失敗するかの２つ状態しかないことが保証されます。
エラーなどでコントラクトの処理が途中で以上終了すると、変数や残高などは元の状態に戻されます。
但し、エラー終了の場合でも処理に要したガスは消費されます。

### assert関数、require 関数

ゲート条件（それが trueにならないと残りの処理を停止する関数）を定義する関数


### コンストラクタとディストラクタ

* コンストラクタ

コントラクトを生成するときに１度だけ評価される関数がコンストラクタです
コントラクトの状態を初期化します。

```
contract MyContract {
    address owner;
    constractor () {
        owner = msg.sender;
  }
}
```

* ディストラクタ

デプロイされたスマートコントラクトは、ブロックチェーンが存在し続けるかぎり動作しつずけます。

コントラクトが、意図したライフサイクルが終了した時点で削除可能にするためには、予めコントラクトの中に `selfdestruct()` 関数によるデストラクタを組み込んでおく必要があります。

```
function destroy() public {
    require(msg.sender==owner);
    selfdestruct(owner);
}
```


### コントラクトの継承

コントラクトの間に継承を定義するときは、contract定義で `is` というキーワードを利用します。

継承によって子コントラクトは親コントラクトの関数や変数を利用できるようになります。

```
contract Child is Parent1, Parent2 {
}
```

### アドレス型の利用例

アドレス型はEOAやContractの20バイトの長さのアドレスを格納する

* balance属性

    アドレスが保有するetherの量が取得可能

```
address a = 0xa; //アドレス型変数aに0xaのアドレスを格納。
uint b = a.balance; //アドレス"0xa"の持つetherの量をbに格納。
```

* send()関数

指定したアドレスにx weiのetherを送金する

```
contract Test {
    function sendTest(){
        address a = 0xf2057b8aefb9093331faf48f30c1ebeab4ff961d; //送信先のアドレスの指定
        a.send(5); //コントラクト・アドレスが保有するetherから指定のアドレス"a"へ5wei送金
    }
}
```


### Import

対象のファイルを取り込む

```
import "some.sol";
```
some.solの中身をsomeに格納

```
import * as some from "some.sol";
```

someの中からsymbol1と、symbol2をaliasという名前でインポート

``` 
import { symbol1, symbol2 as alias } from "some.sol";
```



## Silidity コンパイラ（solc)のインストール

```bash
sudo add-apt-repository ppa:ethereum/ethereum
sudo apt update
sudo apt install solc -y

solc --version
solc, the solidity compiler commandline interface
Version: 0.8.11+commit.d7f03943.Linux.g++
```

## スマートコントラクトの例

複数のユーザーで１つの整数値を登録・更新するスマートコントラクト

```
nano SimpleStorage.sol
```

```
// SPDX-License-Identifier: MIT
pragma solidity >=0.4.16 <0.9.0;

contract SimpleStorage {
    uint storedData;

    function set(uint x) public {
        storedData = x;
    }

    function get() public view returns (uint) {
        return storedData;
    }
}
```

* contract　宣言：スマートコントラクトの名前
* 変数宣言：
* 関数定義 get, set

### solc でコンパイル

```bash
solc --abi --bin SimpleStorage.sol


======= SimpleStorage.sol:SimpleStorage =======
Binary:
608060405234801561001057600080fd5b50610150806100206000396000f3fe608060405234801561001057600080fd5b50600436106100365760003560e01c806360fe47b11461003b5780636d4ce63c14610057575b600080fd5b610055600480360381019061005091906100c3565b610075565b005b61005f61007f565b60405161006c91906100ff565b60405180910390f35b8060008190555050565b60008054905090565b600080fd5b6000819050919050565b6100a08161008d565b81146100ab57600080fd5b50565b6000813590506100bd81610097565b92915050565b6000602082840312156100d9576100d8610088565b5b60006100e7848285016100ae565b91505092915050565b6100f98161008d565b82525050565b600060208201905061011460008301846100f0565b9291505056fea2646970667358221220e81c002fe6366e43e58ea57d3733ae74f9adde8185f4f2a4717525676161aa2164736f6c634300080b0033
Contract JSON ABI
[{"inputs":[],"name":"get","outputs":[{"internalType":"uint256","name":"","type":"uint256"}],"stateMutability":"view","type":"function"},{"inputs":[{"internalType":"uint256","name":"x","type":"uint256"}],"name":"set","outputs":[],"stateMutability":"nonpayable","type":"function"}]
```

## コントラクトアカウントの生成

geth上のプロンプトでコンパイル結果を変数に格納する

16進数のバイナリは先頭に '0x' を追加する

```json
> var bin ="0x"+"608060405234801561001057600080fd5b50610150806100206000396000f3fe608060405234801561001057600080fd5b50600436106100365760003560e01c806360fe47b11461003b5780636d4ce63c14610057575b600080fd5b610055600480360381019061005091906100c3565b610075565b005b61005f61007f565b60405161006c91906100ff565b60405180910390f35b8060008190555050565b60008054905090565b600080fd5b6000819050919050565b6100a08161008d565b81146100ab57600080fd5b50565b6000813590506100bd81610097565b92915050565b6000602082840312156100d9576100d8610088565b5b60006100e7848285016100ae565b91505092915050565b6100f98161008d565b82525050565b600060208201905061011460008301846100f0565b9291505056fea2646970667358221220e81c002fe6366e43e58ea57d3733ae74f9adde8185f4f2a4717525676161aa2164736f6c634300080b0033"
```

Contract JSON ABI

```json
> var abi=[{"inputs":[],"name":"get","outputs":[{"internalType":"uint256","name":"","type":"uint256"}],"stateMutability":"view","type":"function"},{"inputs":[{"internalType":"uint256","name":"x","type":"uint256"}],"name":"set","outputs":[],"stateMutability":"nonpayable","type":"function"}]

```

### コントラクトオブジェクトの生成

```
> var contract = eth.contract(abi)
```

aliceのEOAアカウントをアンロック

```
> personal.unlockAccount(alice)
Passphrase: 
true
```

```
> var myContract = contract.new({ from: eth.accounts[0], data: bin})

> myContract
{
  abi: [{
      inputs: [],
      name: "get",
      outputs: [{...}],
      stateMutability: "view",
      type: "function"
  }, {
      inputs: [{...}],
      name: "set",
      outputs: [],
      stateMutability: "nonpayable",
      type: "function"
  }],
  address: undefined,
  transactionHash: "0x418301a52299263924c32a9b04cabc302475bd943e4fc76815dcd12e0d7b4905"
}
```

### コントラクトの確認

15秒以上して確認（コントラクト・アドレスが設定されている）

```json
> myContract
{
  abi: [{
      inputs: [],
      name: "get",
      outputs: [{...}],
      stateMutability: "view",
      type: "function"
  }, {
      inputs: [{...}],
      name: "set",
      outputs: [],
      stateMutability: "nonpayable",
      type: "function"
  }],
  address: "0x92f2d2dada37788f1e22a53525509f24ea45cd55",
  transactionHash: "0x418301a52299263924c32a9b04cabc302475bd943e4fc76815dcd12e0d7b4905",
  allEvents: function bound(),
  get: function bound(),
  set: function bound()
}
```

#### コントラクトアドレスの確認

```
> myContract.address
"0x92f2d2dada37788f1e22a53525509f24ea45cd55"
```
  
  
### ABI (Application Binary Interface) 

Contractの定義

```json
> myContract.abi
[{
    inputs: [],
    name: "get",
    outputs: [{
        internalType: "uint256",
        name: "",
        type: "uint256"
    }],
    stateMutability: "view",
    type: "function"
}, {
    inputs: [{
        internalType: "uint256",
        name: "x",
        type: "uint256"
    }],
    name: "set",
    outputs: [],
    stateMutability: "nonpayable",
    type: "function"
}]
```

## スマートコントラクトへのアクセス方法

```
eth.contract(ABI).at(コントラクトアドレス);
```

```
> var cnt = eth.contract(myContract.abi).at(myContract.address);
```


```
> personal.unlockAccount(alice)
Passphrase: 
true

> cnt.set.sendTransaction(100,{from: alice})
 "0xfbe00d6afb05ff3f0a5b9a42eda27ff832ad77dbb023623a984d6461f267d21b"
```

```
>  personal.unlockAccount(alice)
Unlock account 0xf2243cfefaa468704176eea0012f8e186bd44c2f
Passphrase: 
true
> cnt.get.sendTransaction({from: alice})
"0x47f040aa432b18d4208aa603be0dfdc37e4b8add0a6fa311f783f2b58a0391a1"
```

```
>  personal.unlockAccount(bob)
Unlock account 0x05c9748414b496cefa62ef383ae73d22a96574f8
Passphrase: 
true
> cnt.get.sendTransaction({from: bob})
"0xb985376f209f621975140ac423106dcc855051294770e2d0aeea404ac6859e21"

```

## Faucet

```
nano Faucet.sol
```

```
// SPDX-License-Identifier: CC-BY-SA-4.0

// Version of Solidity compiler this program was written for
pragma solidity >=0.4.16 <0.9.0;

// Our first contract is a faucet!
contract Faucet {
    // Accept any incoming amount
    receive() external payable {}

    // Give out ether to anyone who asks
    function withdraw(uint withdraw_amount) public {
        // Limit withdrawal amount
        require(withdraw_amount <= 100000000000000000);

        // Send the amount to the address that requested it
        payable(msg.sender).transfer(withdraw_amount);
    }
}

```

```
 solc --abi --bin Faucet.sol

======= Faucet.sol:Faucet =======
Binary:
608060405234801561001057600080fd5b50610154806100206000396000f3fe6080604052600436106100225760003560e01c80632e1a7d4d1461002e57610029565b3661002957005b600080fd5b34801561003a57600080fd5b50610055600480360381019061005091906100f1565b610057565b005b67016345785d8a000081111561006c57600080fd5b3373ffffffffffffffffffffffffffffffffffffffff166108fc829081150290604051600060405180830381858888f193505050501580156100b2573d6000803e3d6000fd5b5050565b600080fd5b6000819050919050565b6100ce816100bb565b81146100d957600080fd5b50565b6000813590506100eb816100c5565b92915050565b600060208284031215610107576101066100b6565b5b6000610115848285016100dc565b9150509291505056fea264697066735822122050c9ba595b0c895ca65b5167c85d42e05776befdc95d831bf994dbda568331bf64736f6c634300080b0033

Contract JSON ABI

[{"inputs":[{"internalType":"uint256","name":"withdraw_amount","type":"uint256"}],"name":"withdraw","outputs":[],"stateMutability":"nonpayable","type":"function"},{"stateMutability":"payable","type":"receive"}]

```

### コントラクトアカウントの生成

geth上のプロンプトでコンパイル結果を変数に格納する

16進数のバイナリは先頭に '0x' を追加する

```json
> var bin2 ="0x"+"608060405234801561001057600080fd5b50610154806100206000396000f3fe6080604052600436106100225760003560e01c80632e1a7d4d1461002e57610029565b3661002957005b600080fd5b34801561003a57600080fd5b50610055600480360381019061005091906100f1565b610057565b005b67016345785d8a000081111561006c57600080fd5b3373ffffffffffffffffffffffffffffffffffffffff166108fc829081150290604051600060405180830381858888f193505050501580156100b2573d6000803e3d6000fd5b5050565b600080fd5b6000819050919050565b6100ce816100bb565b81146100d957600080fd5b50565b6000813590506100eb816100c5565b92915050565b600060208284031215610107576101066100b6565b5b6000610115848285016100dc565b9150509291505056fea264697066735822122050c9ba595b0c895ca65b5167c85d42e05776befdc95d831bf994dbda568331bf64736f6c634300080b0033"
```

Contract JSON ABI

```json
> var abi2 =[{"inputs":[{"internalType":"uint256","name":"withdraw_amount","type":"uint256"}],"name":"withdraw","outputs":[],"stateMutability":"nonpayable","type":"function"},{"stateMutability":"payable","type":"receive"}]
```


### コントラクトオブジェクトの生成

```
> var contract2 = eth.contract(abi2)
```

EOAアカウントをアンロック

```
> personal.unlockAccount(eth.accounts[0])
Passphrase: 
true
```

```
> var myContract2 = contract2.new({ from: eth.accounts[0], data: bin2})
```

```
>  myContract2.abi
[{
    inputs: [{
        internalType: "uint256",
        name: "withdraw_amount",
        type: "uint256"
    }],
    name: "withdraw",
    outputs: [],
    stateMutability: "nonpayable",
    type: "function"
}, {
    stateMutability: "payable",
    type: "receive"
}]
```

#### コントラクトアドレス

```
> myContract2.address
"0x650a04b9ac45f4eee9f781a9251ef69a5f941b39"
```

### コントラクトへのアクセス

```
var cnt2 = eth.contract(myContract2.abi).at(myContract2.address);
```

```
> personal.unlockAccount(eth.accounts[0])
Passphrase: 
true

> cnt2.set.sendTransaction(1,{from:eth.accounts[1]})
 "0xde3a75f63ddf0fd25b75d35651fefa07a8e5be0771960ade37c771761bf43859"
```

## http経由でのアクセス

curlコマンドをローカルに利用

```
curl -s -X POST --data '{"jsonrpc":"2.0","id":67867379,"method":"eth_getBalance","params":[alice,"latest"]}' http://localhost:8545
```