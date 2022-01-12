# Ethereum プライベートネットワーク 構築

## ethereum

### インストールスクリプト


```
nano install-ethereum.sh
```


```bash
#!/bin/bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y apt-file
sudo apt-file update

sudo apt-get update
sudo apt-get install git vim -y
sudo apt-get install software-properties-common
sudo add-apt-repository -y ppa:ethereum/ethereum
sudo apt-get update
sudo apt-get install -y ethereum
```


## プライベートネットワークへの接続

### Genesisファイルを作成する

```bash
mkdir ~/eth_private_net
cd eth_private_net/
```

```
nano myGenesis.json
```

```json
{
  "config": {
    "chainId": 15,
    "homesteadBlock": 0,
    "eip150Block": 0,
    "eip155Block": 0,
    "eip158Block": 0,
    "byzantiumBlock": 0,
    "constantinopleBlock": 0,
    "petersburgBlock": 0,
    "istanbulBlock": 0,
    "berlinBlock": 0
  },
  "nonce": "0x0000000000000042",
  "timestamp": "0x0",
  "parentHash": "0x0000000000000000000000000000000000000000000000000000000000000000",
  "extraData": "",
  "gasLimit": "0x8000000",
  "difficulty": "0x4000",
  "mixhash": "0x0000000000000000000000000000000000000000000000000000000000000000",
  "coinbase": "0x3333333333333333333333333333333333333333",
  "alloc": {}
}
```

### genesisブロックの初期化

```bash
geth --datadir ~/eth_private_net init ~/eth_private_net/myGenesis.json
```

### gethの起動

ネットワークIDを15とする

```bash
geth --networkid "15" --nodiscover --datadir "~/eth_private_net" console 2>> ~/eth_private_net/geth_err.log
```

この結果以下のようなプロンプトが返ってくる

```
Welcome to the Geth JavaScript console!

instance: Geth/v1.10.15-stable-8be800ff/linux-amd64/go1.17.5
coinbase: 0x1341eb5292c14bc092ad5f565bf690aecdeeb9e2
at block: 0 (Thu Jan 01 1970 09:00:00 GMT+0900 (JST))
 datadir: /home/yamasaki/eth_private_net
 modules: admin:1.0 debug:1.0 eth:1.0 ethash:1.0 miner:1.0 net:1.0 personal:1.0 rpc:1.0 txpool:1.0 web3:1.0

To exit, press ctrl-d or type exit
> 
```


### Gethの操作

ブロック番号 0 のブロック情報を表示

```
> eth.getBlock(0)

{
  difficulty: 16384,
  extraData: "0x",
  gasLimit: 134217728,
  gasUsed: 0,
  hash: "0x7b2e8be699df0d329cc74a99271ff7720e2875cd2c4dd0b419ec60d1fe7e0432",
  logsBloom: "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
  miner: "0x3333333333333333333333333333333333333333",
  mixHash: "0x0000000000000000000000000000000000000000000000000000000000000000",
  nonce: "0x0000000000000042",
  number: 0,
  parentHash: "0x0000000000000000000000000000000000000000000000000000000000000000",
  receiptsRoot: "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421",
  sha3Uncles: "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347",
  size: 507,
  stateRoot: "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421",
  timestamp: 0,
  totalDifficulty: 16384,
  transactions: [],
  transactionsRoot: "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421",
  uncles: []
}

```

## 基本操作


* eth：ブロックチェーンの操作
* net：p2pネットワークステータス
* admin：ノードの管理
* miner：マイニング
* txpool：トランザクションメモリプール
* web3：単位変換など

### 一般的なコマンド：

* personal.newAccount(パスワード)：アカウントを作成
* personal.unlockAccount()：アカウントのロックを解除
* eth.accounts：システムのアカウントを列挙
* eth.getBalance() ：アカウントの残高
* eth.blockNumber：ブロックの総数
* eth.getTransaction() ：トランザクションを取得
* eth.getBlock()：ブロックを取得
* miner.start()：マイニングを開始
* miner.stop()：マイニングを停止
* web3.fromWei()：WeiをEtherに変換
* web3.toWei()：EtherをWeiに変換
* txpool.status：トランザクションプールのステータス。
* admin.addPeer()：他のノードに接続

## アカウント (EOA) の作成

### 確認

```
> eth.accounts
[]
```

### EOAの作成

パスワードを入力して作成する

```
personal.newAccount("passwd")
```

```
> personal.newAccount("alice")
"0xc980370ab1ecd29f7501edd2c03ff1791917bfde"
> personal.newAccount("bob")
"0x0d24994bac5aee7bb8f4ceaede35ef7b7ca82085"
```

### etherbase

採掘を行う際にその報酬を紐づけるEOAのアドレス

```
> eth.coinbase
"0x1341eb5292c14bc092ad5f565bf690aecdeeb9e2"
```

## etherの採掘

```
> miner.start()
null

> eth.blockNumber
10357
```

### ブロックの確認

1000番のブロック

parentHash: （直前のブロックのハッシュ値）を確認する

```
> eth.getBlock(1000)
{
  difficulty: 212811,
  extraData: "0xd883010a0f846765746888676f312e31372e35856c696e7578",
  gasLimit: 50523715,
  gasUsed: 0,
  hash: "0x613ed17a392ef0ae6054c84e625f7406922597c96cfe94db5e837939d78826d4",
  logsBloom: "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
  miner: "0x1341eb5292c14bc092ad5f565bf690aecdeeb9e2",
  mixHash: "0x034795aea43f2cb7dbfb95eb1b1c5a3bdf5b8dfdbcb1f5106305197b60382147",
  nonce: "0x3661a4f61bc67715",
  number: 1000,
  parentHash: "0x7f76d51999d2cb12a81ec0e9430dca77e95a4d40c90fc326a5b90a8575611ec5",
  receiptsRoot: "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421",
  sha3Uncles: "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347",
  size: 539,
  stateRoot: "0xb4da00bf06a5eb3638c092ade03e15aa29787a1e51d88540f660b5792a4ed912",
  timestamp: 1641571274,
  totalDifficulty: 168654309,
  transactions: [],
  transactionsRoot: "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421",
  uncles: []
}
```

999番のブロック

hash: が，1000番のブロックの parentHash: と一致することを確認する

```
> eth.getBlock(999)
{
  difficulty: 212708,
  extraData: "0xd883010a0f846765746888676f312e31372e35856c696e7578",
  gasLimit: 50573101,
  gasUsed: 0,
  hash: "0x7f76d51999d2cb12a81ec0e9430dca77e95a4d40c90fc326a5b90a8575611ec5",
  logsBloom: "0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
  miner: "0x1341eb5292c14bc092ad5f565bf690aecdeeb9e2",
  mixHash: "0x8b85fb0909c7d0ab9346929307822bb25aeec8e97cbcfd16ef0bf6ca2a7f506e",
  nonce: "0x2cf7130d9ade908e",
  number: 999,
  parentHash: "0x3c3652aeca25458287890ce275c64cc3d8217b547c45837488d99707d7051de5",
  receiptsRoot: "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421",
  sha3Uncles: "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347",
  size: 539,
  stateRoot: "0x375ddcb16d84a6de5b558b3f7a362ce85713ba34816df6a2902dd08134704233",
  timestamp: 1641571273,
  totalDifficulty: 168441498,
  transactions: [],
  transactionsRoot: "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421",
  uncles: []
}
```

### マイナーの所持金

```
> eth.getBalance(eth.accounts[0])
2.0824e+22
```

Wei -> ether 変換

```
> web3.fromWei(eth.getBalance(eth.accounts[0]),"ether")
20856
> 
```


## etherの送金

EOAの確認

```
> eth.accounts

["0x2ede3e42f1353f0d08796e6c2565577ef61e4bdd", "0x7de4840627b962591fd28695a96c038cb0bff3eb"]
```

### アカウントの所持金の確認

```
> eth.getBalance(eth.accounts[0])
524000000000000000000
> eth.getBalance(eth.accounts[1])
0
> 
```

### アカウントのアンロック

アカウントのパスワードでアンロックする

```
personal.unlockAccount(eth.accounts[0])
Unlock account 0xc980370ab1ecd29f7501edd2c03ff1791917bfde
Passphrase: 
true
```

### 送金トランザクションの作成

```
> eth.sendTransaction({from: eth.accounts[0], to: eth.accounts[1], value: web3.toWei(1, "ether")})
"0x5a674eba5dd9e8d178840abcb49ad13d60dfbd5c8e1ca69baae646c6d90421c4"
```

### 送金トランザクションの確認


```
> eth.getTransaction('0x5a674eba5dd9e8d178840abcb49ad13d60dfbd5c8e1ca69baae646c6d90421c4')
{
  blockHash: "0x027ab30301d5f04cdb6b1e3f7200c0c4a9c6acf2bb5fa4d36db9bb74533025e7",
  blockNumber: 10669,
  from: "0xc980370ab1ecd29f7501edd2c03ff1791917bfde",
  gas: 21000,
  gasPrice: 1000000000,
  hash: "0x5a674eba5dd9e8d178840abcb49ad13d60dfbd5c8e1ca69baae646c6d90421c4",
  input: "0x",
  nonce: 0,
  r: "0x6dae13b76bb47d0cc98892e87629a646d45d61e835b10b76ecbd67223e21013f",
  s: "0x554c212d0fbb77c9f9c267d094a000e40f5b52d2ef376a731502b8100ef02d45",
  to: "0x0d24994bac5aee7bb8f4ceaede35ef7b7ca82085",
  transactionIndex: 0,
  type: "0x0",
  v: "0x42",
  value: 1000000000000000000
}
```

### アカウントの所持金の確認

```
> eth.getBalance(eth.accounts[0])
523000000000000000000
> eth.getBalance(eth.accounts[1])
1000000000000000000
> 
```


# スマートコントラクト

アカウント：　EOA（Externally Owned Account）とContract


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
> var bin ="0x608060405234801561001057600080fd5b50610150806100206000396000f3fe608060405234801561001057600080fd5b50600436106100365760003560e01c806360fe47b11461003b5780636d4ce63c14610057575b600080fd5b610055600480360381019061005091906100c3565b610075565b005b61005f61007f565b60405161006c91906100ff565b60405180910390f35b8060008190555050565b60008054905090565b600080fd5b6000819050919050565b6100a08161008d565b81146100ab57600080fd5b50565b6000813590506100bd81610097565b92915050565b6000602082840312156100d9576100d8610088565b5b60006100e7848285016100ae565b91505092915050565b6100f98161008d565b82525050565b600060208201905061011460008301846100f0565b9291505056fea2646970667358221220e81c002fe6366e43e58ea57d3733ae74f9adde8185f4f2a4717525676161aa2164736f6c634300080b0033"
```

Contract JSON ABI

```json
> var abi=[{"inputs":[],"name":"get","outputs":[{"internalType":"uint256","name":"","type":"uint256"}],"stateMutability":"view","type":"function"},{"inputs":[{"internalType":"uint256","name":"x","type":"uint256"}],"name":"set","outputs":[],"stateMutability":"nonpayable","type":"function"}]

```

### コントラクトオブジェクトの生成

```
> var contract = eth.contract(abi)
```

EOAアカウントをアンロック

```
> personal.unlockAccount(eth.accounts[0])
Passphrase: 
true
```

```
> var myContract = contract.new({ from: eth.accounts[0], data: bin})
```

### コントラクトの確認

15秒以上して確認

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
  address: "0x1e344e85ea621bccb84f5c7af4e1ac4c3989e861",
  transactionHash: "0x9db64ee2204bf3ee754dbe4c3eb9f5e343ac02f8620792777732c88b70735567",
  allEvents: function bound(),
  get: function bound(),
  set: function bound()
}
```

#### コントラクトアドレス

```
> myContract.address
"0x1e344e85ea621bccb84f5c7af4e1ac4c3989e861"
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
> personal.unlockAccount(eth.accounts[0])
Passphrase: 
true

> cnt.set.sendTransaction(100,{from:eth.accounts[0]})
 "0xde3a75f63ddf0fd25b75d35651fefa07a8e5be0771960ade37c771761bf43859"

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