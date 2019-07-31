# 山崎研究室実験ネットワーク

##ハード

### ベアボーン：　512Gb 23 台 (meinnet)
	
* (M+SM+EM+FS)+P  12 台
* (M+SM+EM+FS)+L  11台

### ベアボーン：　256Gb   2台 (testnet) lnd

* (T+ST+ET+FS)+L　２台
	
### ラズベリーパイ　：40台 (testnet)

* (T+ST+ET)+P　40台

## 稼働させるサービス

* bitcoind (mainnet) (M)
* bitcoind (testnet) (T)
* ethereum (mainnet) (EM)
* ethereum (tesnet) (ET)
* IPFS (FS)
* bitcoinサイドチェーン (elements mainnet) (SM)
* bitcoinサイドチェーン (elements testnet) (ST)
* ightning network (ptermigan mainnet) 
* lightning network (ptermigan testnet)
* lightning network (lnd mainnet) 
* lightning network (lnd testnet)

## ノードの種類

### mainnet

* (M+SM+EM+FS)+P
* (M+SM+EM+FS)+L

### testnet

* (T+ST+ET+FS)+P
* (T+ST+ET+FS)+L


## 利用するポート番号一覧

### bitcoind (mainnet)

* TCP 8333 : ノード接続待ち受け
* TCP 8332 : RPC待ち受け

### bitcoind (testnet)

* TCP 18333 : ノード接続待ち受け
* TCP 18332 : RPC待ち受け

### lightning network (ptermigan mainnet)

* TCP 6000 	: ifpaytt
* TCP 9735 	: lightningd (mainnetもtestnetも)
* TCP 9736 	: lightningd (mainnetもtestnetも)

### lightning network (lnd mainnet)

* TCP 9735 	: lightningd (mainnetもtestnetも)
* TCP 9736 	: lightningd (mainnetもtestnetも)
* 
### lightning network (ptermigan testnet)

* bitcoinサイドチェーン (elements)
* ethereum (mainnet)
* ethereum (tesnet)

### IPFS

* TCP 4001 IPFSゲートウェイ
* TCP 8080 IPFSゲートウェイ

## インストール方法

### bitcoin core

```bash

```

#### bitcoin.conf

mainnet

```
mainnet=1 
txindex=1  
server=1   
rest=1      
rpcuser= "yamalabo"
rpcpassword= "yozoranomukou"
rpcport=8332 
```

testnet

```
testnet=3
txindex=1  
server=1   
rest=1      
rpcuser= "yamalabo"
rpcpassword= "yozoranomukou"
rpcport=18332 
```
