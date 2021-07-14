# bitcoinrb

bitcoin core API をRuby から操作する rubygems

## インストール方法

### Ruby言語のインストール

[Ruby インストール](https://github.com/ShigeichiroYamasaki/yamalabo/blob/master/ruby.md)

### ubuntuでのbitcoinrbのインストール

```bash
sudo apt install -y libleveldb-dev
gem install bitcoinrb
```


### MacOSXでのbitcoinrbのインストール

```
brew install --build-from-source leveldb
gem install bitcoinrb
```

## bitcoin core を起動する

bitcoind でも. bitcoin-qt でもsnap のbitocoin-core でもよい

接続するネットワークを意識する（以下ではsignetを想定）

###  irbで確認

```bash
irb
```

## bitcoin core RPC API の利用

```ruby
require 'bitcoin'
require 'net/http'
require 'json'

Bitcoin.chain_params = :signet  # mainnet の場合は :mainnet


HOST="localhost"
PORT=38332          # mainnetの場合は 8332
RPCUSER="hoge"      # bitcoin core RPCユーザ名
RPCPASSWORD="hoge"  # bitcoin core パスワード

# bitcoin core RPC を利用するメソッド

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

# helpメッセージが出てくれば成功
```

### bitcoin core RPC への基本操作の例

#### 鍵生成　aliceというラベルでアドレス生成

```ruby
wallet=bitcoinRPC('loadwallet', ['alice'])
```

```ruby
addr_alice=bitcoinRPC('getnewaddress', ['alice'])
addr_bob=bitcoinRPC('getnewaddress', ['alice'])
```

```ruby
balance=bitcoinRPC('getbalance', [])
```

### 暗号鍵の生成

```ruby
# 鍵ペア生成
key=Bitcoin::Key.generate

# 秘密鍵
priv=key.priv_key
# 公開鍵
pub=key.pubkey
```

### ワレット

```ruby
# マスターキー生成
master=Bitcoin::Wallet::MasterKey.generate

# マスターキーのニーモニックコード

 master.mnemonic
=> ["canyon", "space", "snack", "unlock", "fitness", "basic", "frequent", "license", "slab", "brisk", "can", "violin", "race", "way", "magic", "weapon", "sentence", "frequent", "shy", "valid", "toe", "reveal", "essence", "unfair"]

```

### bitcoin スクリプト

```ruby
script="2 4 OP_ADD 6 OP_EQUAL"
s=Bitcoin::Script.from_string(script)
s.run
=> true
```


