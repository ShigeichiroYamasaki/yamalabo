# Bitcoin core Signet ノードの構築

## install 方法

### MacOSX

#### Bitcoin core ダウンロードサイトから dmg 形式のインストーラをダウンロードする

```
https://bitcoin.org/ja/download
```

#### インストーラで bitcoin core をインストールしてアプリを起動

* ドラックアンドドロップでアプリフォルダーにインストール

#### アプリ起動

アプリケーションフォルダのアプリで起動

### ubuntu 20.04LTS

#### snap でインストールする

```bash
sudo snap install bitcoin-core
```

#### bitcoin-qt アプリを起動

```
bitcoin-core.qt
```

#### snap で bitcoin core をインストールしたときのコマンドインターフェース

* bitcoin-core.daemon : デーモン起動 (bitcoind)
* bitcoin-core.cli : クライアント (bitcoin-cli)


## bitcoind 設定ファイルを編集（作成）

### bitcoin-qt (GUI) から設定ファイル (bitcoin.conf) を編集

* MacOSX :「ファイル」メニューの 「preferences」.. 
* ubuntu: 「設定」メニューの「オプション」
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

## bitcoin core を一旦終了して再度起動

アイコンが薄い黄緑になっていれば成功

* bitcoin-qt で自分のワレットの作成しておく

## テスト用コインの入手

[https://signet.bc-2.jp/](https://signet.bc-2.jp/)



## Ruby からのAPIの利用

### MacOSX

```
brew install leveldb 

gem install bitcoinrb
```

### bitcoinrb のRPC (signet)

```
require 'bitcoin'
require 'net/http'
require 'json'
RPCUSER="hoge"
RPCPASSWORD="hoge"
HOST="localhost"
PORT=38332
 
def bitcoinRPC(method, params)
 	http = Net::HTTP.new(HOST, PORT)
 	request = Net::HTTP::Post.new('/')
 	request.basic_auth(RPCUSER, RPCPASSWORD)
 	request.content_type = 'application/json'
 	request.body = { method: method, params: params, id: 'jsonrpc' }.to_json
 	JSON.parse(http.request(request).body)["result"]
end

# テスト
bitcoinRPC('help', [])
```

### 鍵生成

```
# 鍵生成
key=Bitcoin::Key.generate

# 秘密鍵
priv=key.priv_key
# 公開鍵生成
pub=key.pubkey
```
### ワレット

```
# マスターキー生成
master=Bitcoin::Wallet::MasterKey.generate

# マスターキーのニーモニックコード

 master.mnemonic
=> ["canyon", "space", "snack", "unlock", "fitness", "basic", "frequent", "license", "slab", "brisk", "can", "violin", "race", "way", "magic", "weapon", "sentence", "frequent", "shy", "valid", "toe", "reveal", "essence", "unfair"]

```


### bitcoin スクリプト

```
script="2 4 OP_ADD 6 OP_EQUAL"
s=Bitcoin::Script.from_string(script)
s.run
```

## 基本操作

### 鍵生成　aliceというラベルでアドレス生成

```
addr_alice=bitcoinRPC('getnewaddress', ['alice'])
```

### 残高を確認

```
balance=bitcoinRPC('getbalance', [])
```

### bobというラベルでアドレスを生成

```
addr_bob=bitcoinRPC('getnewaddress', ['bob'])
```

### 送金 alice がbobへ送金する

```
txid=bitcoinRPC('sendtoaddress', [addr_bob, 0.001])
```


""