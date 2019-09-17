# 山崎研究室実験ネットワーク

## IPアドレス申請対象

* main net：157.13.61.130~220  (90台)
* test net   ：157.13.61.1~126   (126台)
* application : 157.13.61.221~225    (5台)

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
* lightning network (ptarmigan, Lnd)
* sidechain (elements)
* Ethereum (parity)
* IPFS
* Libra (testnet)
* web (apache ssl) web wallet用

## mainnet サービス

ベアボーン(512Gb) 23台

* bitcoind (BC)　                （20台）
	* ightning network (ptarmigan)   （10/20台）
	* ightning network (Lnd) 　　　　（10/20台）
	* Elements 　　　　　　  (20/20台)
* Ethereum (parity)　　　     (2台)
* IPFS(IP) 　　　　　　　　 (1台)	

## testnet サービス

ベアボーン(256Gb) 6台
ラズベリーパイ(256Gb) 40台

* bitcoind　（ラズベリーパイ 40台）
	* ightning network (ptarmigan)（20/40台）
	* ightning network (Lnd) 　（20/40台）
	* Elements （40/40台）
* Ethereum (parity) (ベアボーン(256Gb) 2台)
* Libra (ベアボーン(256Gb) 2台)
* アプリケーションサーバ (ベアボーン(256Gb)2台)

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

### lightning network (ptarmigan testnet)

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

#### expect のインストール

```bash
sudo apt  install expect -y
```

#### Rubyのインストール

```bash
sudo apt  install ruby -y
```

### sudoerの設定

パスワード入力なしでsudo が実行できるようにする

自動実行の場合、あらためてパスワードを入れることに意味がないため

```bash
sudo visudo
```

```
...

yamalabo ALL=(ALL) NOPASSWD:ALL
```
## raspberry pi の初期設定

```bash
# ロケール設定
sudo locale-gen ja_JP.UTF-8
sudo dpkg-reconfigure -f noninteractive locales
echo "LANG=ja_JP.UTF-8" | sudo tee /etc/default/locale
sudo timedatectl set-timezone Asia/Tokyo
```

#### スワップファイルの作成

```bash
sudo fallocate -l 1g /swapfile
sudo chmod 600 /swapfile
sudo mkswap /swapfile 
echo '/swapfile swap swap defaults 0 0' | sudo tee -a /etc/fstab
sudo swapon -a
```

そしてrebootする

### bitcoin core のインストール

[./bitcoin-core.md](./bitcoin-core.md)

### lightning network (ptarmigan)のインストール

[./ptarmigan.md](./ptarmigan.md)

### lightning network (Lnd)のインストール

[./Lnd.md](./Lnd.md)

### サイドチェーン Elements のインストール

[./elements.md](./elements.md)

### etherum (parity) のインストール

[./ethereum.md](./ethereum.md)

### IPFS のインストール

[./IPFS.md](./IPFS.md)




