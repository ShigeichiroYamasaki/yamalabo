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
sleep 2
sudo apt upgrade -y
sleep 2
sudo apt install ssh -y
sudo apt  install expect -y
sudo apt  install ruby -y
sudo apt install -y curl

sudo apt install -y \
     apt-transport-https \
     ca-certificates \
     curl \
     software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

sudo apt install -y chkrootkit
sudo apt install -y build-essential 
sudo apt install -y clang
sudo apt install -y cmake
sudo apt install -y golang
sudo apt-get install -y apt-file
sudo apt-file update

sudo apt install -y libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev
sudo apt install -y  libevent-dev
sudo apt install -y libtool
sudo apt install -y autoconf
sudo apt install -y git
sudo apt autoremove -y

cd ~
git init
rm -fr bitcoin
git clone https://github.com/bitcoin/bitcoin.git
cd bitcoin
./autogen.sh
./configure --enable-upnp-default --disable-wallet
make -j2 
sudo make install

sleep 10
bitcoin-cli stop
sleep 30
cat << EOF > ~/.bitcoin/bitcoin.conf
mainnet=1
txindex=1
server=1
rest=1
rpcuser='ユーザ名'
rpcpassword='パスワード'
rpcport=8332
EOF

sleep 10
bitcoind &
```

### 実行

```bash
chmod a+x install-bitcoincore-mainnet.sh
```

## bitcoin core アップデートスクリプト

update-bitcoincore.sh

### mainnet/ testnet 共通

```bash
#!/bin/bash
bitcoin-cli stop
sleep 30
sudo apt update
sudo apt upgrade -y
sudo apt install -y libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev
sudo apt install -y  libevent-dev

cd ~
rm -fr bitcoin
git clone https://github.com/bitcoin/bitcoin.git
cd bitcoin
./autogen.sh
 ./configure --enable-upnp-default --disable-wallet
make -j2 
sudo make install
sleep 2

/usr/local/bin/bitcoind &
```


## 自動起動設定

cron を使って設定

### ベアボーン

```bash
crontab -u yamalabo -e

# 1 nano エディタを選ぶ
```

### crontab の編集

以下を最後に追加

```
# ...
15 1 * * 5 /home/yamalabo/update-bitcoincore.sh &
@reboot /usr/local/bin/bitcoind &
```

^(コントロール) o ^(コントロール) x でnanoエディタを保存終了

### 再起動で確認

```bash
sudo reboot
```

再起動後

bitcoind のhelpが出力されれば成功

```bash
bitcoin-cli help
```


### bitcoind の停止

bitcoin-cli を使って停止させる

```bash
bitcoin-cli stop
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

## crontab の編集

* ベアボーン

```bash
crontab -u yamalabo -e
```

```
...

@reboot ~/ptarmigan/install/ptarmd --network=mainnet
```



---

