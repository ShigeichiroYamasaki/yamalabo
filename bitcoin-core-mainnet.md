# ベアボーンノードのインストール方法

* bitcoin core mainnet
* ligntning network (ptarmigan)


## ubuntu 18.04.3 をインストールUSBの作成


### インストール元 ubuntu-18.04.3-desktop-amd64.iso

[https://ubuntu.com/download/desktop/thank-you?country=JP&version=18.04.3&architecture=amd64](https://ubuntu.com/download/desktop/thank-you?country=JP&version=18.04.3&architecture=amd64)


ダウンロードしたファイルはxzで圧縮されている

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

### ddコマンドでインストールUSBを作成する


```bash
# USBメモリを挿す前のストレージデバイスの確認
diskutil list
```

* /dev/disk(n) のリストを確認

* 空のUSBを挿す

```bash
# USBメモリを挿したあとのストレージデバイスの確認
diskutil list
```

挿したUSBの/dev/disk(n) を確認

```bash
# USBメモリのアンマウント
diskutil umountdisk /dev/disk(n)
```


```bash
cd ~

# /dev/disk(n) ではなく /dev/rdisk(n) にする
sudo dd if=./ubuntu-18.04.3-desktop-amd64.iso of=/dev/rdisk(n) bs=1m
```

## USBでubuntu をインストール

省略

## ubuntu インストール後のセットアップ

### sudoerの設定

パスワード入力なしでsudo が実行できるようにする

自動実行の場合、あらためてパスワードを入れることに意味がないため

```bash
sudo visudo
```

```
yamalabo ALL=(ALL) NOPASSWD:ALL
```


# bitcoin core インストール

## インストールスクリプト


★ユーザ名／パスワードの指定


### mainnet

install-bitcoincore-mainnet.sh

```bash
#!/bin/bash
sudo apt update
sudo apt upgrade -y
sudo apt install snapd
sudo snap install bitcoin-core
cat << EOF > ~/.bitcoin/bitcoin.conf
mainnet=1
txindex=1
server=1
rest=1
rpcuser="user"
rpcpassword="password"
rpcport=8332
EOF
```

### 実行

```bash
chmod a+x install-bitcoincore-mainnet.sh
```

### デーモン起動


```bash
bitcoin-core.daemon -mainnet
```

### クライアント接続

```bash
bitcoin-core.cli -mainnet help
```
---

# lightning network 

## ptarmiganのインストール

### ptarmigan インストールスクリプト

```bash
nano install-ptarmigan.sh
```

```bash
#!/bin/bash
~/ptarmigan/install/ptarmcli --stop
sudo apt install -y git autoconf pkg-config build-essential libtool python3 wget jq bc
sleep 2
git clone https://github.com/nayutaco/ptarmigan.git
cd ptarmigan
make full
sleep 2
cd install
sleep 2
~/ptarmigan/install/new_nodedir.sh ptarmigan-yamalabo
cd ~/ptarmigan/install/ptarmigan-yamalabo
sleep 2
echo "export PATH=\$PATH:~\/ptarmigan\/install" >> ~/.bashrc
source ~/.bashrc
~/ptarmigan/install/ptarmd --network=mainnet &
```


```bash
chmod a+x install-ptarmigan.sh
```

```bash
./install-ptarmigan.sh
```

