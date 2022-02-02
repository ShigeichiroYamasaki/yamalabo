# Ethereum Ropsten ノード　構築


## ethereum (geth)

### インストールスクリプト

新規マシンにインストール

```
nano install-ethereum.sh
```

```bash
#!/bin/bash
sudo apt update
sudo apt upgrade -y
sudo apt install git -y
sudo apt install -y libsnappy-dev wget curl build-essential cmake gcc sqlite3
sudo apt install software-properties-common
sudo add-apt-repository -y ppa:ethereum/ethereum
sudo apt update
sudo apt install -y ethereum
```

```bash
chmod a+x install-ethereum.sh
./install-ethereum.sh
```

## Ropstenネットワークへの接続(高速モードで同期）

```
cd ~
mkdir Ethereum
cd Ethereum
mkdir ropsten
geth --ropsten --syncmode "snap" --datadir "./ropsten" console 2>> ./ropsten/geth_err.log
```

## geth 基本操作

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
* eth.coinbase : コインベースのアカウントの確認
* miner.setEtherbase(eth.accounts[index]) : コインベースアカウントの変更
* eth.getBalance() ：アカウントの残高
* eth.blockNumber：ブロックの総数
* eth.getTransaction() ：トランザクションを取得
* eth.getBlock()：ブロックを取得
* eth.sendTransaction({from: eth.accounts[source_index], to: eth.accounts[destination_index], value: web3.toWei(amount, "ether")}) : 送金
* eth.getTransactionReceipt("transaction_address") : トランザクションの実行結果（レシート）の確認
* eth.mining : マイニング中か確認
* miner.start()：マイニングを開始
* miner.stop()：マイニングを停止
* miner.hashrate : マイニングHashrateの確認
* web3.fromWei()：WeiをEtherに変換
* web3.toWei()：EtherをWeiに変換
* admin.addPeer()：他のノードに接続
* net.listening : 疎通確認
* net.peerCount : 接続されているノード数
* admin.nodeInfo : ノードの情報
* admin.peers : 接続されているノード情報


## Gethの操作

```
> eth.blockNumber
111871
```

## EOAの作成

簡単のためパスワードを名前にしておく

```
> var alice=personal.newAccount("alice")
"0xf5e1a80090966c6c8a9b6b6c19e3e74a35da44b6"
> var bob=personal.newAccount("bob")
"0xe9da4c344c57b179590f7eb78eca44816cc1536e"
```

### 所持金の確認

```
> eth.getBalance(eth.accounts[0])
0
> eth.getBalance(alice)
0
> eth.getBalance(eth.accounts[1])
0
> eth.getBalance(bob)
0
```

## MetaMask を Chrome ブラウザの拡張機能にインストールしアカウントとワレットを作成

[MetaMask(メタマスク)とは？Chromeへのインストール方法や初期設定、使い方を完全解説(2021最新版)](https://nft-now.jp/what-is-metamask/)


### MetaMask でRopsten ネットワークに接続

### Ropstenネットワークの Faucet でテスト用 Etherを入手する


### アカウントの所持金の確認

```
> eth.getBalance(alice)
300000000000000000

> eth.getBalance(bob)
400000000000000000

```

# 接続先

```
To exit, press ctrl-d or type exit
> admin.peers
[{
    caps: ["eth/66"],
    enode: "enode://f60710578e6e364ea003cd6a0d6f0aecf23fc592c45fc761d1f4a01872cebc8adee220260dde64634b39d440bcf78b383e59a80fe05506a0959aa210b5136e1b@34.81.33.87:30303",
    enr: "enr:-Je4QHSLBLi6ZvDDTCX4dLo4VwAxDSbEFh_9JQkm2xMV3SnFS_U5G6rUYxklNaSKq7eA_ejKzgwPo3XD1z2eH7n4IPAHg2V0aMfGhHEZtrOAgmlkgnY0gmlwhCJRIVeJc2VjcDI1NmsxoQP2BxBXjm42TqADzWoNbwrs8j_FksRfx2HR9KAYcs68ioN0Y3CCdl-DdWRwgnZf",
    id: "393db698665fe2466892b6262cf67707da5ff33c4b54fe020aa3eab2a6612ba5",
    name: "erigon/v2021.12.3-beta-47c3b9df/linux-amd64/go1.17.5",
    network: {
      inbound: false,
      localAddress: "192.168.0.254:49842",
      remoteAddress: "34.81.33.87:30303",
      static: false,
      trusted: false
    },
    protocols: {
      eth: {
        difficulty: 39372544544231100,
        head: "0x4974392269413df7a0c469775267942f2076a0c5eed4cae44f2f68a5844ecea6",
        version: 66
      }
    }
}]
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