# 山崎研究室実験ネットワーク

## 確認

### bitcoin mainnet

### web


## IPアドレス申請

### 以前の申請分は全て取り消し

* A.X.Y.130 ~ 220: 8333
    
### IPアドレス新規申請対象

* Etherum: A.X.Y.1~20     (20台)
* IPFS: A.X.Y.41~50          (10台)
* web : A.X.Y.51~54           (4台)
* bitcoin mainnet (Lightning network): A.X.Y.66~126      (61台)
* bitcoin regtest (Elements): A.X.Y.129~138             (10台)
* bitcoin testnet (Lightning network): A.X.Y.139~254   (116台)

## 稼働させるサービス

* bitcoin (bitcoin core)
* lightning network (ptarmigan, Lnd)
* sidechain (elements)
* Ethereum (parity)
* IPFS
* tapyrus (testnet)
* web (apache ssl) web wallet用


## 利用するポート番号一覧

###tapyrus (mainnet)

* ★TCP: 2357 : ノード接続
* 
### bitcoind (mainnet)

* ★TCP: 8333 : ノード接続
* (TCP: 8332 : 内部RPC)

### bitcoind (testnet)

* ★TCP 18333 : ノード接続
* (TCP 18332 : 内部RPC)

### lightning network (ptarmigan mainnet/testnet)

* ★TCP: 6000 	: ifpaytt (If Pay Then That)
* ★TCP: 9735 	: ノード接続
* (TCP: 9736 	: 内部RPC)

### lightning network (Lnd mainnet/testnet)

* ★TCP: 9735: ノード接続
* (TCP: 9736 : 内部RPC)

### bitcoinサイドチェーン　elements

* 特になし

### Ethereum (mainnet)

* ★TCP: 8545  ethereum (mainnet)
* ★TCP: 30303
* ★UDP: 30303

### Ethereum (testnet)

* ★UDP: 50303
* ★TCP: 50303

### IPFS

* ★TCP: 4001 IPFSゲートウェイ
* ★TCP: 8080 IPFSゲートウェイ
* ★TCP: 443
* ★TCP: 80

### web サーバ

* ★TCP: 443 (SSL, HTTP/2)
* ★TCP: 80 (http, http/2)

## 当面の機器構成（2019/10/03現在）


### bitcoin testnet (Raspberry pi 3)

* 76台
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


---
