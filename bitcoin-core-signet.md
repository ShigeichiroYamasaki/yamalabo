# Bitcoin core Signet ノードの構築

## install 方法

nano install-signet.sh

```bash
#!/bin/bash
cd ~
sudo add-apt-repository ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get install -y libdb4.8-dev libdb4.8++-dev

sudo apt install expect -y
sudo apt install -y curl
sudo apt install -y apt-transport-https ca-certificates software-properties-common
sudo apt install -y build-essential 
sudo apt install -y clang
sudo apt install -y cmake
sudo apt install -y golang
sudo apt install -y libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev
sudo apt install -y libevent-dev
sudo apt install -y libtool
sudo apt install -y autoconf
sudo apt install -y git
sudo apt install -y libevent-dev
sudo apt autoremove -y

cd ~
git init
rm -fr bitcoin
git clone https://github.com/kallewoof/bitcoin.git signet
cd signet
git checkout signet
./autogen.sh
./configure
make -j5

cd src
mkdir signet
echo "signet=1
daemon=1" > signet/bitcoin.conf
sudo make install
cd ~

cat << EOF > ~/.bitcoin/bitcoin.conf
signet=1
txindex=1  
server=1   
rest=1      
rpcuser=yamasaki
rpcpassword=kindai
rpcport=38332 
EOF

/use/local/bin/bitcoind &

```

## bitcoind の起動

```bash
bitcoind -datadir=signet
```

## 確認

```bash
bitcoin-cli -datadir=signet getblockcount
```


