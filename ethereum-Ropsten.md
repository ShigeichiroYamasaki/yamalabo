# Ethereum Ropsten ノード　構築

2022/02/17 Shigeichiro Yamasaki



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

geth --ropsten --syncmode "snap" --datadir "./ropsten" --http --http.addr "localhost" --http.port "8545" --http.api "eth,net,web3,admin,miner,txpool,personal" --allow-insecure-unlock -- console 2>> ./ropsten/geth_err.log
```

## geth 基本操作

* eth：ブロックチェーンの操作
* net：p2pネットワークステータス
* admin：ノードの管理
* miner：マイニング
* txpool：トランザクションメモリプール
* web3：単位変換など
* personal : アカウント管理

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
* net.listening :　導通確認
* net.peerCount : 接続されているノード数
* admin.nodeInfo : ノードの情報
* admin.peers : 接続されているノード情報

## Gethの操作

```
> eth.blockNumber
11931513
```

## EOAの作成

簡単のためパスワードを名前にしておく

```
> var alice=personal.newAccount("alice")
"0xf2243cfefaa468704176eea0012f8e186bd44c2f"
> var bob=personal.newAccount("bob")
"0x05c9748414b496cefa62ef383ae73d22a96574f8"

> alice
"0xf2243cfefaa468704176eea0012f8e186bd44c2f"
> bob
"0x05c9748414b496cefa62ef383ae73d22a96574f8"
> 
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

### Ropstenネットワークの Faucet でテスト用 Etherを入手する

[Ropstenのフォセット](https://faucet.ropsten.be)

alice と bob のそれぞれに Etherを送金する

### アカウントの所持金の確認

```
> eth.getBalance(alice)
300000000000000000

> eth.getBalance(bob)
400000000000000000
```

## 送金

```
> personal.unlockAccount(eth.accounts[0])
> 
> eth.sendTransaction({from: eth.accounts[0], to: eth.accounts[1], value: web3.toWei(0.001, "ether")}) 
```

## 接続先

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
