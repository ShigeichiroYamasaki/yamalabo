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


### sshのインストール

```bash
sudo apt install ssh -y
```

### nano のインストール

```bash
sudo apt install nano -y
```

### expect のインストール

```bash
sudo apt  install expect -y
```

### Rubyのインストール

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
...(既存の設定)

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
mainnet=1
txindex=1
server=1
rest=1
rpcuser='yamalabo'
rpcpassword='yozoranomukou'
rpcport=8332
EOF

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
sleep 300
sudo apt update
sudo apt upgrade -y
sleep 2
sudo apt-get install -y bitcoind
bitcoind &
```


## 自動起動設定

cron を使って設定

* ベアボーンではユーザは yamalabo
* raspberry pi ではユーザは ubuntu


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
@reboot /usr/bin/bitcoind &
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



--
## スクリプトの処理内容の説明

### apt でインストール

```bash
sudo apt-add-repository ppa:bitcoin/bitcoin

# enterキーを押す

sudo apt-get update
sudo apt-get install -y bitcoind
```

### bitcoind の一時起動と環境ファイルの作成

```bash
bitcoind &
```

### bitcoind の停止

bitcoin-cli を使って停止させる

```bash
bitcoin-cli stop
```

これで ~/.bitcoin の下に起動環境が作成される

## 設定ファイルbitcoin.confの作成

~/.bitcoin の下に bitcoin.conf を作成する

```bash
nano ~/.bitcoin/bitcoin.conf
```

### mainnetの場合

```
mainnet=1 
txindex=1  
server=1   
rest=1      
rpcuser= "ユーザ名"
rpcpassword= "パスワード"
rpcport=8332 
```

testnet

```
testnet=3
txindex=1  
server=1   
rest=1      
rpcuser= "ユーザ名"
rpcpassword= "パスワード"
rpcport=18332 
```

### bitcoind の起動確認

```bash
bitcoind &
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

