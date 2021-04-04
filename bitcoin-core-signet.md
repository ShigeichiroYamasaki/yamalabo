# Bitcoin core Signet ノードの構築

## install 方法

### MacOSX

#### Bitcoin core ダウンロードサイトから dmg 形式のインストーラをダウンロードする

```
https://bitcoin.org/ja/download
```

#### インストーラで bitcoin core をインストールしてアプリを起動

* ドラックアンドドロップでアプリフォルダーにインストール
* アプリを起動

#### 設定ファイルを編集（作成）

* ファイルメニューの preferences.. 
* 「設定ファイルを開く」ボタンをクリック
* 設定ファイルを以下のように作成して保存

```
signet=1
txindex=1
daemon=1
server=1
rest=1
[signet]
rpcuser=hoge
rpcpassword=hoge
rpcport=38332
port=38333
fallbackfee=0.0002
```

* 「OK」をクリック

#### bitcoin core を一旦終了して再度起動

アイコンが薄い黄緑になっていれば成功




----

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



make 
```

cat << EOF > ~snap/
signet=1
txindex=1
daemon=1
server=1
rest=1
[signet]
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



