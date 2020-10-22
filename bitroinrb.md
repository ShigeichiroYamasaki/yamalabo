# bitroinrb の使い方

## インストール方法

### ubuntu

```bash
sudo apt install -y git
sudo apt install -y gcc

sudo apt-get install -y libleveldb-dev
sudo apt-get install -y rbenv
```


### MacOSX

```bash
brew install gcc
brew install git
brew install leveldb
brew install rbenv
```

### Ruby

```bash
cd ~
git clone https://github.com/rbenv/rbenv.git ~/.rbenv
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build


cat << EOF >> .bashrc
export PATH="$HOME/.rbenv/bin:$PATH"
eval "$(rbenv init -)"
EOF

source .bashrc

rbenv install -l

# 最新バージョンをインストールする 2.7.2 の場合
rbenv install 2.7.2
rbenv global 2.7.2


gem install bitcoinrb
```

## irbで確認

```bash
irb
```

```ruby
require 'bitcoin'

Bitcoin.chain_params = :regtest
```

### 鍵

```ruby
# 鍵生成
key=Bitcoin::Key.generate

# 秘密鍵
priv=key.priv_key
# 公開鍵生成
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

```

### スクリプトの基本

* lock_script を unlock_scriptで解除する

```ruby
lock_script=""
unlock_script=""
```

## RPC （http 経由でbitcoin core を利用する）

```ruby
require 'bitcoin'
require 'net/http'
require 'json'
RPCUSER="hoge"
RPCPASSWORD="hoge"
HOST="localhost"
PORT=18332
 
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

### 基本操作

#### 鍵生成　aliceというラベルでアドレス生成

```ruby
addr_alice=bitcoinRPC('getnewaddress', ['alice'])
```

#### aliceのアドレスマイニングをする (ハッシュ値を50個作成する）

```ruby
addr_alice=bitcoinRPC('generatetoaddress', [10, addr_alice])
```

#### 10分以上後　残高が増えていることを確認する

```ruby
addr_alice=bitcoinRPC('getbalance', [])
```

#### bobというラベルでアドレスを生成

```ruby
addr_bob=bitcoinRPC('getnewaddress', ['bob'])
```

#### 送金 alice がbobへ送金する

```ruby
addr_bob=bitcoinRPC('sendtoaddress', ['bob'])

```


## 基本

```ruby
require 'bitcoin'
Bitcoin.chain_params = :regtest

# code:TX

tx = Bitcoin::Tx.new

# code:TxIn

tx_in = Bitcoin::TxIn.new
out_point = Bitcoin::OutPoint.from_txid("txid", "index")
script_sig = Bitcoin::Script.new
script_witness = ScriptWitness.new
sequence = SEQUENCE_FINAL

# code:TxOut

tx_out = Bitcoin::TxOut.new
 	value = "value satoshi"
 	script_pubkey = Bitcoin::Script.parse_from_addr("address")

# code:署名の準備
# if BitcoinCore secret_key(52byte)

secret_key = "secret_key"
core_key = Bitcoin::Key.from_wif("secret_key")
 
key = Bitcoin::Key.new(priv_key: "core_key.priv_key", key_type: Bitcoin::Key::TYPES[:p2pkh] or [:p2wpkh_p2sh])


# code:署名-P2PKH 

script_pubkey = Bitcoin::Script.parse_from_payload('script_pubkey'.htb)
sighash = tx.sighash_for_input(0, prev_script_pubkey)
sig = key.sign("sighash") + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
tx.in[0].script_sig = Bitcoin::Script.new << sig << key.pubkey.htb

# code:署名-P2SH
#  送金されたトランザクションからredeem_scriptを取ってくる

  redeem_script = Bitcoin::Script.parse_from_payload("redeem_script".htb)
  tx.inputs[0].script_sig = Bitcoin::Script.parse_from_payload(Bitcoin::Script.pack_pushdata("redeem_script".to_payload))
  
  key = Bitcoin::Key.new(priv_key: key.priv_key, key_type: Bitcoin::Key::TYPES[:p2wpkh_p2sh])
  sig_hash = tx.sighash_for_input(0, redeem_script, amount: "amount", sig_version: :witness_v0)
  sig = key.sign("sig_hash", false) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
  tx.inputs[0].script_witness.stack << sig
  tx.inputs[0].script_witness.stack << key.pubkey.htb
  
  tx.verify_input_sig(0, "prev_script_pubkey", amount: "amount")

# code:P2WPKH

 amount = (satoshi)
 priv_script_pubkey = Bitcoin::Script.parse_from_payload('prev_script_pubkey'.htb)
 sighash = tx.sighash_for_input(index, script_pubkey, sig_version: :witness_v0, amount: amount)
 sig = key.sign("sighash") + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
 tx.in[0].script_witness.stack << sig
 tx.in[0].script_witness.stack << key.pubkey.htb

# code:P2WPKH

 require 'bitcoin'
 Bitcoin.chain_params = :testnet
 
 tx = Bitcoin::Tx.new
 
 tx_in = Bitcoin::TxIn.new(out_point: out_point)
  out_point = Bitcoin::OutPoint.from_txid("txid", "index")
 
 tx_out = Bitcoin::TxOut.new(value: value, script_pubkey: script_pubkey)
  amount = "amount(satoshi)"
  value = amount - "fee"
  script_pubkey = Bitcoin::Script.parse_from_addr("address")
  
 key = Bitcoin::Key.from_wif("secret_key")
 
 prev_script_pubkey = Bitcoin::Script.parse_from_payload('prev_script_pubkey'.htb)
 sighash = tx.sighash_for_input(index, prev_script_pubkey, sig_version: :witness_v0, amount: amount)
 sig = key.sign("sighash") + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
 
 tx.in[0].script_witness.stack << sig
 tx.in[0].script_witness.stack << key.pubkey.htb
 
 tx.verify_input_sig(index, prev_script_pubkey, amount: amount)

```