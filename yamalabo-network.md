# 山崎研究室実験ネットワーク

## ネットワークセグメント構成


```
---------+-------------------- 157.X.Z.0/24
         |
         62
+----+---+------------------- 157.X.Y.0/26 (Libra,Etherum,web,IPFS)
60   61    
|    |    
|   129
|    +----------------------- 157.X.Y.128/25 (bitcoin testnet,lightning network)
|
65
+---------------------------- 157.X.Y.64/26 (bitcoin mainnet,lightning network)

```

## IPアドレス申請

### 以前の申請分は全て取り消し

* 157.X.Y.130 ~ 220: 8333
    
## IPアドレス新規申請対象

* Etherum: 157.X.Y.1~11      (10台)
* Libra:157.13.Y.12~22      (10台)
* IPFS: 157.X.Y.23~33        (10台)
* web : 157.X.Y.34~37         (3台)
* bitcoin mainnet (Lightning network): 157.X.Y.66~106     (40台)
* bitcoin regtest (Elements): 157.X.Y.107~126      (19台)
* bitcoin testnet (Lightning network): 157.X.Y.130~254   (124台)




## 当面の機器構成（2019/10/03）


### bitcoin testnet (Raspberry pi 3)

* 60台
* Lightninge network (ptarmigan) ノードにする

[raspberrypi testnetインストール方法](./raspberrypi.md)

### bitcoin testnet + webアプリ (ベアボーン 256Gb)

* 1台
* Lightninge network (ptarmigan) ノードにする

[bitcoin testnetインストール方法](./bitcoin-core-testnet.md)

### bitcoin regtest  (ベアボーン 256Gb)

* 2台
* Elements

[bitcoin regtest Elementsインストール方法](./bitcoin-core-regtest-elements.md)


### bitcoin mainnet (ベアボーン 512G)

* 10台
* Lightning network (ptarmigan) ノードにする

[bitcoin mainnetインストール方法](./bitcoin-core-mainnet.md)


### Etherum mainnet (ベアボーン1T)

* 1台

[Etherum mainnetインストール方法](./etherum-main.md)

### IPFS (ベアボーン 512G)

* 1台

[IPFSインストール方法](./IPFS.md)


### Ethereum testnet (ベアボーン 512G)

* 3台

[ethereumインストール方法](./ethereum-testnet.md)


### Libra testnet (ベアボーン 512G)

* 6台

[Libraインストール方法](./libra.md)

---



## ハードの準備状況

### ベアボーン：　512Gb 21台
### ベアボーン：  1T  　1台)

ubuntu 18.04.3 LTS (B512)

### ベアボーン：　256Gb   3台

ubuntu 18.04.3 LTS (B256)

### raspberry pi 3：256Gb 76台

Ubuntu 18.04.3 LTS (R256)

12*4 =48
8*3=24
4*1
76


### (今年度計画）raspberry pi4：512Gb 10台 )


## 稼働させるサービス

* bitcoin (bitcoin core)
* lightning network (ptarmigan, Lnd)
* sidechain (elements)
* Ethereum (parity)
* IPFS
* Libra (testnet)
* web (apache ssl) web wallet用


## 利用するポート番号一覧

### bitcoind (mainnet)

* ★TCP 8333 : ノード接続
* TCP 8332 : RPC

### bitcoind (testnet)

* ★TCP 18333 : ノード接続
* TCP 18332 : RPC

### lightning network (ptarmigan mainnet)

* ★TCP 6000 	: ifpaytt (If Pay Then That)
* ★TCP 9735 	: ノード接続
* TCP 9736 	: RPC

### lightning network (Lnd mainnet)

* ★TCP 9735 	: ノード接続
* TCP 9736 	: RPC

### elements

* bitcoinサイドチェーン (elements)

### Ethereum

* ★TCP: 8545  ethereum (mainnet)
* ethereum (tesnet)

* ★UDP 30303 
* ★TCP 30303

### Libra validator

* TCP 30307
* TCP 8000
* TCP 443, 80 (HTTP/2)

### IPFS

* ★TCP 4001 IPFSゲートウェイ
* ★TCP 8080 IPFSゲートウェイ

### web アプリケーションサーバ

* TCP 443, 80 (http, SSL)

## OSのインストール方法

### ベアボーンノードのOSインストール方法

[./barebone.md](./barebone.md)

### ラズベリーパイノードのOSインストール方法

[./raspberrypi.md](./raspberrypi.md)

