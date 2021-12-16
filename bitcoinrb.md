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

```bash
brew install --build-from-source leveldb
gem install bitcoinrb
```

## bitcoin core を起動しておく

bitcoin core をAPI を介して利用する。

bitcoind でも. bitcoin-qt でもsnap のbitocoin-core でもよい

接続するネットワークを意識してポート番号などを設定する（以下ではsignetを想定）


###  irbで確認

```bash
irb
```

## bitcoin core RPC API の利用

```ruby
require 'bitcoin'
require 'net/http'
require 'json'
include Bitcoin
include Bitcoin::Opcodes

Bitcoin.chain_params = :signet

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
```

### アドレス

```ruby
# AliceとBobのアドレスの生成
addrAlice =  bitcoinRPC('getnewaddress', [])
addrBob =  bitcoinRPC('getnewaddress', [])
  
# 秘密鍵（WIF形式）
privAlice = bitcoinRPC('dumpprivkey', [addrAlice])
privBob = bitcoinRPC('dumpprivkey', [addrBob])

# 鍵オブジェクト(WIF形式の秘密鍵から生成）
keyAlice=Bitcoin::Key.from_wif(privAlice)
keyBob=Bitcoin::Key.from_wif(privBob)
```

### UTXOの確認

```ruby
# AliceのUTXOと残高を確認（とりあえず最初の Alice宛のUTXOを利用することにする）
utxos = bitcoinRPC('listunspent',[])
utxoAmount = utxos[0]["amount"]
utxoVout = utxos[0]["vout"]
utxoTxid = utxos[0]["txid"]
utxoScriptPubKey = utxos[0]["scriptPubKey"]

# 送金可能なUTXO
spendable_utxos = utxos.select{|t|t["spendable"]==true}

# 送金可能なUTXOの金額の合計
balance = spendable_utxos.reduce(0){|s,t|s+=t["amount"]}
# bitcoin core APIによる送金可能金額の確認
bitcoinRPC('getbalance', [])

```


# bitcoin core の基本操作の bitcoinrb による実装

## 送金のために使用するUTXOを集める（指定金額以上のUTXO）

```ruby
# 送金金額によって送金で消費するUTXOの選定する
def consuming_utxos(amount)
  utxos=bitcoinRPC('listunspent', [])
  spendable_utxos = utxos.select{|t|t["spendable"]==true}
  sorted_utxos = spendable_utxos.sort_by{|x|x["amount"]}
  r=[]
  begin
      r<< sorted_utxos.shift
      balance = r.reduce(0){|s,t|s+=t["amount"]}
  end until balance >= amount
  return r
end

# test
amount = 0.02
utxos = consuming_utxos(amount)
# 送金金額の総和の確認
utxos.reduce(0){|s,t|s+=t["amount"]}

amount_satoshi = (amount*(10**8)).to_i
```

### 未署名トランザクションの作成 (P2WPKH)

```ruby
tx = Bitcoin::Tx.new
utxos.each{|utxo|
    tx.in << Bitcoin::TxIn.new(out_point: Bitcoin::OutPoint.from_txid(utxo["txid"], utxo["vout"],))
}
tx.out << Bitcoin::TxOut.new(value: amount_satoshi, script_pubkey: Bitcoin::Script.parse_from_addr(addrBob))

# トランザクションの構造の確認
bitcoinRPC('decoderawtransaction', [tx.to_hex])
```

### トランザクションのハッシュ値の計算とトランザクションへの署名

```ruby
utxos.each.with_index{|utxo,index|
    script_pubkey = Bitcoin::Script.parse_from_payload(utxo["scriptPubKey"].htb)
    satoshi = (utxo["amount"]*(10**8)).to_i
    sighash = tx.sighash_for_input(index, script_pubkey, sig_version: :witness_v0, amount: satoshi)
    sig = keyAlice.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
    # witness scriptの追加
    tx.in[index].script_witness.stack << sig
}
```

### トランザクションへの署名

```ruby
# トランザクションの構造の確認
bitcoinRPC('decoderawtransaction', [tx.to_hex])
```

### トランザクションの送金

```ruby
txid = bitcoinRPC('sendrawtransaction', [tx.to_hex])
```

### P2WPKH による送金

```ruby
# 送金先アドレスと送金金額を指定して送金

def send(addr, amount)
    utxos = consuming_utxos(amount)
    
end
```

### bitcoin スクリプト

```ruby
include Bitcoin::Opcodes

script="2 4 OP_ADD 6 OP_EQUAL"
s=Bitcoin::Script.from_string(script)
s.run
=> true

# こちらの記述方法の方が安全
s=Bitcoin::Script.new << 2 << 4 << OP_ADD << 6 << OP_EQUAL
s.run
=> true
```


----



### 暗号鍵の生成

```ruby
# 鍵ペア生成
key=Bitcoin::Key.generate

# 秘密鍵
priv=key.priv_key
# 公開鍵
pub=key.pubkey
```

### ワレット(bitcoin coreのワレット機能を利用せずに作成）

```ruby
# マスターキー生成
master=Bitcoin::Wallet::MasterKey.generate

# マスターキーのニーモニックコード
master.mnemonic
=> ["canyon", "space", "snack", "unlock", "fitness", "basic", "frequent", "license", "slab", "brisk", "can", "violin", "race", "way", "magic", "weapon", "sentence", "frequent", "shy", "valid", "toe", "reveal", "essence", "unfair"]

master.derive(47, harden=true).derive(0, harden=true).derive(0, harden=true)
```
