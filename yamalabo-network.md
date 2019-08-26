# 山崎研究室実験ネットワーク

## IPアドレス申請

IPアドレスとポート番号の申請では実際よりも多く申請

* main net：157.13.61.130~220 (90台)
* test net   ：157.13.61.66~126   (60台)
* application : 157.13.61.221~25    (5台)

## ハードの準備状況

### ベアボーン：　512Gb 23 台 

ubuntu 18.04.3 LTS (B512)

### ベアボーン：　256Gb   2台 

ubuntu 18.04.3 LTS (B256)

### ラズベリーパイ　：256Gb 40台 

Ubuntu 18.04.2 LTS (R256)

## 稼働させるサービス

* bitcoin (bitcoin core)
* lightning network (ptermigan, Ind)
* sidechain (elements)
* Ethereum (parity)
* IPFS
* Libra
* web (apache ssl) web wallet用

## main net サービス

ベアボーン(512Gb) 23台

* bitcoind (BC)　            （20台）
	* ightning network (ptermigan)   （10/20台）
	* ightning network (Ind) 　　　　（10/20台）
	* Elements 　　　　　　  (20/20台)
* Ethereum (parity)　　　     (2台)
* IPFS(IP) 　　　　　　　　 (1台)	

## test net サービス

ベアボーン(256Gb) 3台
ラズベリーパイ(256Gb) 40台

* bitcoind (BC)　（ラズベリーパイ 40台）
	* ightning network (ptermigan) (BC+LNP)　（20/40台）
	* ightning network (Ind) (BC+LNI)　（20/40台）
	* Elements (BC+EL) （40/40台）
* Ethereum (parity)(ET) (ベアボーン(256Gb) 1台)
* Libra (ベアボーン(256Gb) 1台)

ベアボーン(256Gb)
* アプリケーションサーバ (1台)

## 利用するポート番号一覧

### bitcoind (mainnet)

* ★TCP 8333 : ノード接続
* TCP 8332 : RPC

### bitcoind (testnet)

* ★TCP 18333 : ノード接続
* TCP 18332 : RPC

### lightning network (ptermigan mainnet)

* ★TCP 6000 	: ifpaytt (If Pay Then That)
* ★TCP 9735 	: ノード接続
* TCP 9736 	: RPC

### lightning network (lnd mainnet)

* ★TCP 9735 	: ノード接続
* TCP 9736 	: RPC

### lightning network (ptermigan testnet)

* bitcoinサイドチェーン (elements)

### Ethereum

* TCP: 8545  ethereum (mainnet)
* 
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

[./ethereum.md](./ethereum.md)

### ptermigan

