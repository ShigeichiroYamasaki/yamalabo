# ラズベリーパイノードのインストール方法

* bitcoin core testnet
* ligntning network (@tarmigan)

### ubuntu 18.04.3 をインストール



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
xz -d ubuntu-18.04.3-preinstalled-server-arm64+raspi3.img.xz
```

## sdカードを挿す

挿したSDカードをアンマウントする

```bash
diskutil list

diskutil umountdisk /dev/disk(n)
```
## ddコマンド


* raspberry pi (SDカード)

```bash
sudo dd if=./ubuntu-18.04.3-preinstalled-server-arm64+raspi3.img of=/dev/rdisk(n) bs=1m
```

---

## SDカードでubuntu を起動


省略


### yamalaboユーザを作成

```bash
sudo usrradd yamalabo

sudo moduser yamalabo -g sudo

su yamalabo
```
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

### curlのインストール

```bash
sudo apt install -y curl
```

### Dockerのインストール

```bash
sudo apt install -y \
     apt-transport-https \
     ca-certificates \
     curl \
     software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

sudo add-apt-repository \
     "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
     $(lsb_release -cs) \
     stable"
sudo apt update
```

### ルートキット検出ツール

```bash
sudo apt install -y chkrootkit
```
### そのほかコンパイラなど

```bash
sudo apt install -y build-essential 
sudo apt install -y clang
sudo apt install -y cmake
sudo apt install -y golang
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



---

## bitcoin core インストール

### インストールスクリプト


★ユーザ名／パスワードの指定

### testnet

>> ユーザ名、パスワードの修正が必要

install-bitcoincore-testnet.sh


```bash
#!/bin/bash
sudo apt update
sleep 2
sudo apt upgrade -y
sleep 2
sudo apt-get install -y apt-file
sudo apt-file update

expect -c "
  set timetout -1
  spawn sudo apt-add-repository ppa:bitcoin/bitcoin
  expect {
    \"ENTER\" { send \"\\n\"}
  }
  interact
"
sleep 2
sudo apt-get update
sudo apt-get install -y bitcoind
sleep 2
bitcoind &
sleep 10
bitcoin-cli stop
sleep 30
cat << EOF > ~/.bitcoin/bitcoin.conf
testnet=3
txindex=1
server=1
rest=1
rpcuser=ユーザ名
rpcpassword=パスワード
rpcport=18332
EOF

bitcoind &
```


### 実行

```bash
chmod a+x install-bitcoincore-testnet.sh

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
sleep 2

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

bitcoind &
```

```bash
chmod a+x update-bitcoincore.sh
```



# lightning network (testnet)

# ptarmiganのインストール

## ptarmigan インストールスクリプト

install-ptarmigan.sh


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
echo "export PATH=\$PATH:~/ptarmigan/install/" >> ~/.bashrc
source ~/.bashrc
~/ptarmigan/install/ptarmd --network=testnet &
```



```bash
chmod a+x install-ptarmigan.sh
```

```bash
./install-ptarmigan.sh
```

## PATHを通す

```bash
nano ~.profile
```

以下を追加する

```
...
export PATH=$PATH:~/ptarmigan/install/

```

## 自動起動設定

cron を使って設定

* raspberry pi ではユーザは yamalabo


### raspberry pi

```bash
crontab -u yamalabo -e

# 1 nano エディタを選ぶ
```

### crontab の編集

以下を最後に追加

毎月、１日の０時０分にbitcoind をアップデートする

```
# ...
0 0 1 * * /home/yamalabo/update-bitcoincore.sh &
@reboot /usr/bin/bitcoind -deamon &
@reboot /home/yamalabo/ptarmigan/install/ptarmd --network=testnet &
```



