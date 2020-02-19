# bitcoin testnet 構築

## bitcoin core インストール

### インストールスクリプト



★ユーザ名／パスワードの指定

### testnet

>> ユーザ名、パスワードの修正が必要

install-bitcoincore-testnet.sh


```bash
#!/bin/bash
sudo apt update
sudo apt upgrade -y
sudo apt install ssh -y
sudo apt  install expect -y
sudo apt  install ruby -y
sudo apt install -y apt-transport-https  ca-certificates  curl  software-properties-common
sudo apt install -y chkrootkit
sudo apt install -y build-essential 
sudo apt install -y clang
sudo apt install -y cmake
sudo apt install -y golang
sudo apt install -y apt-file
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

/use/local/bin/bitcoind &
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

/use/local/bin/bitcoind &
```


## 自動起動設定

cron を使って設定

* ユーザは yamalabo


### raspberry pi

```bash
crontab -u yamalabo -e

# 1 nano エディタを選ぶ
```

### crontab の編集

以下を最後に追加

```
# ...

@reboot /usr/local/bin/bitcoind &
```

^(コントロール)o ^(コントロール)x でnanoエディタを保存終了

### 再起動で確認

```bash
sudo reboot
```

再起動後

bitcoind のhelpが出力されれば成功

```bash
bitcoin-cli help
```



## Rail のインストール

```bash
gem install sqlite3
gem install rails
```

## MacOSX

```bash
brew update
brew upgrade

brew install automake berkeley-db4 libtool boost miniupnpc openssl pkg-config protobuf python qt libevent qrencode
```
