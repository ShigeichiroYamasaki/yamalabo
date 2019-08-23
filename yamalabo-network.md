# 山崎研究室実験ネットワーク

##ハードの準備状況

実際の申請では、10台ずつ増加させておく

### ベアボーン：　512Gb 23 台 

ubuntu 18.04.2 LTS (B512)

### ベアボーン：　256Gb   2台 

ubuntu 18.04.2 LTS (B256)

### ラズベリーパイ　：256Gb 40台 

Ubuntu 18.04.2 LTS (R256)

## 稼働させるサービス

### bitcoin (bitcoin core)
#### lightning network (ptermigan, Ind)
#### sidechain (elements)
### Ethereum (parity)
### IPFS
### Libra

## main net サービス

ベアボーン(512Gb) 23台

* bitcoind (BC)　（20台）
	* ightning network (ptermigan) (BC+LNP)　（10/20台）
	* ightning network (Ind) (BC+LNI)　（10/20台）
	* Elements (BC+EL) (10/20台)
* Ethereum (parity)(ET) (2台)
* IPFS(IP) (1台)	

## test net サービス

ベアボーン(256Gb) 2台
ラズベリーパイ(256Gb) 40台

* bitcoind (BC)　（ラズベリーパイ 40台）
	* ightning network (ptermigan) (BC+LNP)　（20/40台）
	* ightning network (Ind) (BC+LNI)　（20/40台）
	* Elements (BC+EL) （40/40台）
* Ethereum (parity)(ET) (ベアボーン(256Gb) 1台)
* Libra (ベアボーン(256Gb) 1台)


## 利用するポート番号一覧

### bitcoind (mainnet)

* ★TCP 8333 : ノード接続待ち受け
* TCP 8332 : RPC待ち受け

### bitcoind (testnet)

* ★TCP 18333 : ノード接続待ち受け
* TCP 18332 : RPC待ち受け

### lightning network (ptermigan mainnet)

* ★TCP 6000 	: ifpaytt (If Pay Then That)
* ★TCP 9735 	: lightningd (mainnetもtestnetも)
* ★TCP 9736 	: lightningd (mainnetもtestnetも)

### lightning network (lnd mainnet)

* ★TCP 9735 	: lightningd (mainnetもtestnetも)
* ★TCP 9736 	: lightningd (mainnetもtestnetも)

### lightning network (ptermigan testnet)

* bitcoinサイドチェーン (elements)
* ethereum (mainnet)
* ethereum (tesnet)

* UDP 30303 
* TCP 30303

### IPFS

* TCP 4001 IPFSゲートウェイ
* TCP 8080 IPFSゲートウェイ

## インストール方法

### bitcoin core

[./bitcoin-core.md](./bitcoin-core.md)

### etherum (parity) 

[]()