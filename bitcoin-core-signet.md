# Bitcoin core Signet ノードの構築

## install 方法

### MacOSX

```bash
xcode-select --install
```

homebrew をインストールする

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install.sh)"
```

brew でインストール

```bash
brew install autoconf automake berkeley-db4 libtool boost miniupnpc openssl pkg-config protobuf qt5 libevent

brew install librsvg
```

```bash
git clone https://github.com/bitcoin/bitcoin
cd bitcoin

./autogen.sh
```

ワレットなしでインストール

```bash
./configure --disable-wallet
make
```
Berkeley DB 4.8



### ubuntu 

```bash
nano install-bitcoincore-signet.sh
```

```bash
#!/bin/bash
sudo apt update
sudo apt-get install build-essential libtool autotools-dev automake pkg-config bsdmainutils python3 libssl-dev libevent-dev libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-test-dev libboost-thread-dev libminiupnpc-dev libzmq3-dev libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler git ccache

sudo apt install lcov sqlite3
sudo apt install libsqlite3-dev

git clone https://github.com/bitcoin/bitcoin
cd bitcoin

./contrib/install_db4.sh `pwd`

./autogen.sh
export BDB_PREFIX='<PATH-TO>/db4'

./configure --disable-wallet

make 
```

cat << EOF > ~snap/
signet=1
txindex=1
daemon=1
server=1
rest=1
rpcuser=hoge
rpcpassword=hoge
rpcport=38332
port=38333
fallbackfee=0.0002
EOF

cd ~
cat << EOF > ~/.bashrc
alias bitcoin-cli="bitcoin-core.cli"
alias bitcoind="bitcoin-core.daemon"
alias bitcoin-qt="bitcoin-core.qt"
EOF
source ~/.bashrc
```

### インストール実行

```bash
chmod 766 install-bitcoincore-signet.sh

./install-bitcoincore-signet.sh
```


## bitcoind の起動



```bash
bitcoind &
```


## 確認

```bash
bitcoin-cli help
```

## テスト用コインの入手

[https://signet.bc-2.jp/](https://signet.bc-2.jp/)



