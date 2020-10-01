# bitcoin testnet 構築

## bitcoin core インストール

### testnet インストールスクリプト

>> ユーザ名、パスワードの修正が必要

nano install-bitcoincore-testnet.sh


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
rpcuser=yamasaki
rpcpassword=kindai
rpcport=18332 
EOF

/use/local/bin/bitcoind &
```


### インストール実行

```bash
chmod a+x install-bitcoincore-testnet.sh

```

### デーモン起動


```bash
bitcoind &
```

