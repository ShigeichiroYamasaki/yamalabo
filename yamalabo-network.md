# 山崎研究室実験ネットワーク

## IPアドレス申請対象

IPアドレスとポート番号の申請では実際よりも多く申請

* main net：157.13.61.130~220 (90台)
* test net   ：157.13.61.66~126   (60台)
* application : 157.13.61.221~25    (5台)

## ハードの準備状況

### ベアボーン：　512Gb 23 台 

ubuntu 18.04.3 LTS (B512)

### ベアボーン：　256Gb   6台 

ubuntu 18.04.3 LTS (B256)

### raspberry pi 3：256Gb 40台 

Ubuntu 18.04.2 LTS (R256)

### (計画）raspberry pi 4：512Gb 10台 )

## 稼働させるサービス

* bitcoin (bitcoin core)
* lightning network (ptermigan, Lnd)
* sidechain (elements)
* Ethereum (parity)
* IPFS
* Libra (testnet)
* web (apache ssl) web wallet用

## main net サービス

ベアボーン(512Gb) 23台

* bitcoLnd (BC)　                （20台）
	* ightning network (ptermigan)   （10/20台）
	* ightning network (Lnd) 　　　　（10/20台）
	* Elements 　　　　　　  (20/20台)
* Ethereum (parity)　　　     (2台)
* IPFS(IP) 　　　　　　　　 (1台)	

## test net サービス

ベアボーン(256Gb) 6台
ラズベリーパイ(256Gb) 40台

* bitcoLnd　（ラズベリーパイ 40台）
	* ightning network (ptermigan)（20/40台）
	* ightning network (Lnd) 　（20/40台）
	* Elements （40/40台）
* Ethereum (parity) (ベアボーン(256Gb) 2台)
* Libra (ベアボーン(256Gb) 2台)
* アプリケーションサーバ (ベアボーン(256Gb)2台)

## 利用するポート番号一覧

### bitcoLnd (mainnet)

* ★TCP 8333 : ノード接続
* TCP 8332 : RPC

### bitcoLnd (testnet)

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

* ★TCP: 8545  ethereum (mainnet)
* ethereum (tesnet)

* ★UDP 30303 
* ★TCP 30303

### IPFS

* ★TCP 4001 IPFSゲートウェイ
* ★TCP 8080 IPFSゲートウェイ

## インストール方法

### ubuntu 18.04.3 をインストール

* ベアボーン
[https://ubuntu.com/download/desktop/thank-you?country=JP&version=18.04.3&architecture=amd64](https://ubuntu.com/download/desktop/thank-you?country=JP&version=18.04.3&architecture=amd64)

ubuntu-18.04.3-desktop-amd64.iso

* raspberry pi

[http://cdimage.ubuntu.com/releases/bionic/release/ubuntu-18.04.3-preinstalled-server-arm64+raspi3.img.xz](http://cdimage.ubuntu.com/releases/bionic/release/ubuntu-18.04.3-preinstalled-server-arm64+raspi3.img.xz)

ubuntu-18.04.3-preinstalled-server-arm64+raspi3.img.xz

★ダウンロードしたファルをホームディレクトリに移動させておく

#### xz 圧縮ファイルの解凍方法

macOSX の場合

xz  のインストール

```bash
brew install xz
```

解凍（結構時間がかかります）

```bash
xz -d sudo ubuntu-18.04.3-preinstalled-server-arm64+raspi3.img.xz
```

## ddコマンド

* ベアボーン　（USBメモリ）

```bash
# USBメモリを挿す前のストレージデバイスの確認
diskutil list
# USBメモリを挿したあとのストレージデバイスの確認
diskutil list
# USBメモリのアンマウント
diskutil umoountdisk /dev/disk(n)
```


```bash
cd ~

# /dev/disk(n) ではなく /dev/rdisk(n) にする
sudo dd if=./ubuntu-18.04.3-desktop-amd64.iso of=/dev/rdisk(n) bs=1m
```

* raspberry pi

```bash
sudo dd if=./ubuntu-18.04.3-preinstalled-server-arm64+raspi3.img.xz of=/dev/rdisk(n) bs=1m

### 事前準備

```bash
sudo apt update
sudo apt upgrade -y
```

#### sshのインストール

```bash
sudo apt install ssh -y
```

#### nano のインストール

```bash
sudo apt install nano -y
```

### bitcoin core のインストール

[./bitcoin-core.md](./bitcoin-core.md)

### lightning network (ptermigan)のインストール

[./ptermigan.md](./ptermigan.md)

### lightning network (Lnd)のインストール

[./Lnd.md](./Lnd.md)

### etherum (parity) のインストール

[./ethereum.md](./ethereum.md)

### IPFS のインストール

[./IPFS.md](./IPFS.md)




