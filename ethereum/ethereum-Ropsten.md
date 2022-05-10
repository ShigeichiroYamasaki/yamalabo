# Ethereum Ropsten ノード　構築

2022/05/09 更新 Shigeichiro Yamasaki


## Ethrerumテストネットワークの種類

* Ropsten: Proof-of-work test network
* Rinkeby: Proof-of-authority test network
* Görli: Proof-of-authority test network

## ethereum (geth) インストール手順 ubuntu 20.04 LTS

ubuntu 22.04 LTSでのインストールは 2022年５月現在おすすめしません


```bash
sudo add-apt-repository -y ppa:ethereum/ethereum
sudo apt-get update
sudo apt-get install ethereum
```

## ethereum (geth) インストール手順 MacOSX

HOmebrew がインストールされている前提です

[Homebrew インストール](https://brew.sh/index_ja)


```bash
brew tap ethereum/ethereum
brew install ethereum
```

## Ethereumネットワークの同期モード

* Full: ブロックチェーンの全ブロックをダウンロードし、全てのブロックでの状態更新を確認して同期する
* Snap: (Default): ブロックチェーンの全ブロックをダウンロードし、最近ブロックでの状態更新を確認して同期する
* Light: 最近のブロックヘッダーのみダウンロードし、オンデマンドで必要なデータをダウンロードする

## Ropstenネットワークへの接続(高速モード 'snap' で同期）


```bash
cd ~
mkdir Ethereum
cd Ethereum
mkdir ropsten

geth --ropsten --syncmode "snap" --datadir "./ropsten" --http --http.addr "localhost" --http.port "8545" --http.api "eth,net,web3,admin,miner,txpool,personal" --allow-insecure-unlock -- console 2>> ./ropsten/geth_err.log
```

しばらくして以下のプロンプトが現れると成功

```
To exit, press ctrl-d or type exit
> 
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

```js
> admin.nodeInfo 
{
  enode: "enode://b79c3d2c53e632499c00fc80e2edc15ece8ff680869b84f8f59606886eabf41eaae62b9057c44b6cdfd73a4b510a9f062c9a419e454154411953b1f4b4d0d9e1@106.157.214.199:30303",
  enr: "enr:-KO4QKb9jQvyVwjuMWVo6GiN_9A7zprDasaXhA-uRHVIgvogKIWwADQvO7SEuoHFdd7fRYrGa5dfXtH-JEW-jYOuDLmGAYBp5UiTg2V0aMfGhHEZtrOAgmlkgnY0gmlwhGqd1seJc2VjcDI1NmsxoQO3nD0sU-YySZwA_IDi7cFezo_2gIabhPj1lgaIbqv0HoRzbmFwwIN0Y3CCdl-DdWRwgnZf",
  id: "cd9e2c4b1f050f9170a14673a5b6f579950351d6d6305f9ef454e28c01c7ffc7",
  ip: "106.157.214.199",
  listenAddr: "[::]:30303",
  name: "Geth/v1.10.17-stable-25c9b49f/linux-amd64/go1.18",
  ports: {
    discovery: 30303,
    listener: 30303
  },
  protocols: {
    eth: {
      config: {
        berlinBlock: 9812189,
        byzantiumBlock: 1700000,
        chainId: 3,
        constantinopleBlock: 4230000,
        daoForkSupport: true,
        eip150Block: 0,
        eip150Hash: "0x41941023680923e0fe4d74a34bdac8141f2540e3ae90623718e47d66d1ca4a2d",
        eip155Block: 10,
        eip158Block: 10,
        ethash: {},
        homesteadBlock: 0,
        istanbulBlock: 6485846,
        londonBlock: 10499401,
        muirGlacierBlock: 7117117,
        petersburgBlock: 4939394
      },
      difficulty: 1048576,
      genesis: "0x41941023680923e0fe4d74a34bdac8141f2540e3ae90623718e47d66d1ca4a2d",
      head: "0x41941023680923e0fe4d74a34bdac8141f2540e3ae90623718e47d66d1ca4a2d",
      network: 3
    },
    snap: {}
  }
}


> admin.peers
[{
    caps: ["eth/66"],
    enode: "enode://8bb0c33e2552a013e6d842b4817fa31f9f58e46fe68b9622c22b1177ac811c80178ee40b3c9e7737c63f4858bf15d2135ffd4dd9d5a84018bb06941a9d5e0aab@51.81.244.144:30307",
    enr: "enr:-Ji4QA2KcniCYB-yKD95v9lDgb7PXge3l3lV-HYSnfx-77wycn_LG84Z81rtxT4Ds-8WBGPbw9PBLO2O2Ba_9KqhWyOB0INldGjHxoRxGbazgIJpZIJ2NIJpcIQzUfSQiXNlY3AyNTZrMaEDi7DDPiVSoBPm2EK0gX-jH59Y5G_mi5YiwisRd6yBHICDdGNwgnZjg3VkcIJ2Yw",
    id: "170198a4eba5ead883b135e2eaebc7d09a08c126f079424b6ad76744cb8a17aa",
    name: "erigon/v2022.02.3-beta-227bb736/linux-amd64/go1.17.7",
    network: {
      inbound: false,
      localAddress: "192.168.0.247:41874",
      remoteAddress: "51.81.244.144:30307",
      static: false,
      trusted: false
    },
    protocols: {
      eth: {
        difficulty: 41365115962874410,
        head: "0x60680e9a3b3da1e9a490eea0054f8ca63bd375081cd5cdb5493a3f85d51da3ee",
        version: 66
      }
    }
}, {
    caps: ["eth/66", "snap/1"],
    enode: "enode://93321232a52d8a0214be1bc0c372679534f729a7e74f049c970fa6e943027a4802f7a4a015d2a5a965fc58a12124ec647d8d97dcf77493fa3ccdf9d8dd2b60d8@91.239.56.4:37255",
    id: "177dd9af0ceaf714bbcbc0084211002fe581b23d0ba5307d9ea8b4910330f471",
    name: "Geth/v1.10.17-stable-25c9b49f/linux-amd64/go1.18",
    network: {
      inbound: true,
      localAddress: "192.168.0.247:30303",
      remoteAddress: "91.239.56.4:37255",
      static: false,
      trusted: false
    },
    protocols: {
      eth: {
        difficulty: 41365159478115544,
        head: "0x5fcae4b5172535c1089e9c8a3118837847c28e6e03f7a7638f80e6c1a850b418",
        version: 66
      },
      snap: {
        version: 1
      }
    }
}, {
    caps: ["eth/66"],
    enode: "enode://f60710578e6e364ea003cd6a0d6f0aecf23fc592c45fc761d1f4a01872cebc8adee220260dde64634b39d440bcf78b383e59a80fe05506a0959aa210b5136e1b@34.81.33.87:30303",
    enr: "enr:-Je4QFhuFE1LrD-ttvK_oReIXkKGVRM9hJIS9s8tLu_kBW4eXw3ACAdpOQ4olzH9lomafFRIyCP4Mdo6D3-4pstCv6YOg2V0aMfGhHEZtrOAgmlkgnY0gmlwhCJRIVeJc2VjcDI1NmsxoQP2BxBXjm42TqADzWoNbwrs8j_FksRfx2HR9KAYcs68ioN0Y3CCdl-DdWRwgnZf",
    id: "393db698665fe2466892b6262cf67707da5ff33c4b54fe020aa3eab2a6612ba5",
    name: "erigon/v2021.12.3-beta-47c3b9df/linux-amd64/go1.17.5",
    network: {
      inbound: false,
      localAddress: "192.168.0.247:60070",
      remoteAddress: "34.81.33.87:30303",
      static: false,
      trusted: false
    },
    protocols: {
      eth: {
        difficulty: 41365159478115544,
        head: "0x5fcae4b5172535c1089e9c8a3118837847c28e6e03f7a7638f80e6c1a850b418",
        version: 66
      }
    }
}]


> eth.blockNumber
11931513
```

## EOAの作成

簡単のためパスワードを名前にしておく

```
> eth.accounts
[]

> const alice=personal.newAccount("alice")
"0xf2243cfefaa468704176eea0012f8e186bd44c2f"
> const bob=personal.newAccount("bob")
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

[Ropstenのフォセット](https://faucet.egorfine.com)

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
