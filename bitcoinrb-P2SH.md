# bitcoinrbでP2WSHを実装

2022/02/25 Shigeichiro Yamasaki

bitcoinでスマートコントラクトを記述するときの基本は P2WSH です

P2WSHの代表例はマルチシグですが、それ以外のケースも重要です。

## bitcoinrb WiKi

開発者の安土さんによる Wikiです。
基本的な使用法は、ここでわかります。

[https://github.com/chaintope/bitcoinrb/wiki](https://github.com/chaintope/bitcoinrb/wiki)

## bitcoinrb のクラスとメソッドの一覧

[https://www.rubydoc.info/gems/bitcoinrb/0.1.5](https://www.rubydoc.info/gems/bitcoinrb/0.1.5)


## P2WSHマルチシグ送金プログラム


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
FEE = 0.00002       # 手数料

# bitcoin core RPC を利用するメソッド
def bitcoinRPC(method, params)
    http = Net::HTTP.new(HOST, PORT)
    request = Net::HTTP::Post.new('/')
    request.basic_auth(RPCUSER, RPCPASSWORD)
    request.content_type = 'application/json'
    request.body = { method: method, params: params, id: 'jsonrpc' }.to_json
    JSON.parse(http.request(request).body)["result"]
end

###  m of n マルチシグトランザクションのP2WSH送金 
#    mはアンロックに必要な公開鍵の数
#    amount は、P2WSH で送金する金額
#    pubkeys は、n個の公開鍵の配列

def send_multisig(amount, m, pubkeys)
    # redeem scriptの生成
    redeem_script = redeeming_info(m,pubkeys)
    # P2WSHトランザクションの構成
    tx,utxos = p2wsh_tx(amount, redeem_script)
    # トランザクションへの署名
    tx_h = bitcoinRPC('signrawtransactionwithwallet',[tx.to_hex])
    # トランザクションのデプロイ
    txid = bitcoinRPC('sendrawtransaction',[tx_h["hex"]])
    # P2WSHトランザクションのアンロックに必要な情報の出力
    return tx, txid, redeem_script
end

# マルチシグトランザクションの償還に必要な情報
    # redeem script （アンロックに必要なスクリプト）
    # scriptPubKey （ロックスクリプト）
    # P2WSH アドレス

# redeem scriptの生成
def redeeming_info(m,pubkeys)
    op_n = [OP_0,OP_1,OP_2,OP_3,OP_4,OP_5,OP_6,OP_7,OP_8,OP_9,OP_10,OP_11,OP_12,OP_13,OP_14,OP_15,OP_16]
    redeem_script = "#{op_n[m]} " + pubkeys.join(' ') + " #{op_n[pubkeys.size]} OP_CHECKMULTISIG"
    return Bitcoin::Script.from_string(redeem_script)
end

# P2WSHトランザクションの構成
def p2wsh_tx(amount, redeem_script)
    balance = bitcoinRPC('getbalance', [])
    if balance < (amount+FEE) then
        puts "error (残高不足)"
    else
        tx = Bitcoin::Tx.new
        # 送金金額＋手数料をぎりぎり上回るUTXOリストの作成
        utxos = consuming_utxos(amount+FEE)
        # トランザクションのinputの構成
        tx = make_inputs(tx,utxos)
        # おつり = UTXOの総額 - 送金金額 - 手数料
        change = (utxos.map{|utxo|utxo["amount"]}.sum)-amount-FEE
        # おつり用アドレス（UTXOの０番目を受け取ったアドレスを利用する）
        change_addr = utxos[0]["address"]       
        # トランザクションのoutputの構成
        tx = make_outputs(tx, amount, redeem_script, change, change_addr)
        return tx,utxos
    end
end

# トランザクションのinputの構成
def make_inputs(tx, utxos)
    utxos.each{|utxo|
        outpoint = Bitcoin::OutPoint.from_txid(utxo["txid"], utxo["vout"])
        tx.in << Bitcoin::TxIn.new(out_point: outpoint)
    }
    return tx
end

# トランザクションのoutputの構成
def make_outputs(tx, amount, redeem_script, change, change_addr)
    change_satoshi = (change*(10**8)).to_i
    amount_satoshi = (amount*(10**8)).to_i
    scriptPubKey0 = Bitcoin::Script.from_string("OP_0 #{redeem_script.to_sha256}")
    tx.out << Bitcoin::TxOut.new(value: amount_satoshi , script_pubkey: scriptPubKey0)
    # おつり用outputの構成(P2PKH)
    scriptPubKey1 = Bitcoin::Script.parse_from_addr(change_addr)
    tx.out << Bitcoin::TxOut.new(value: change_satoshi, script_pubkey: scriptPubKey1)
    return tx
end


# 送金金額＋手数料をぎりぎり上回るUTXOリストの作成
def consuming_utxos(amount)
    # ワレットの未使用のUTXOの一覧を得る
    unspent = bitcoinRPC('listunspent', [])
    # 消費可能状態のUTXOの選定
    spendable_utxos = unspent.select{|t|t["spendable"]==true}
    # UTXOを金額で昇順にソートする
    sorted_utxos = spendable_utxos.sort_by{|x|x["amount"]}
    # 少額のUTXOから集めて，指定金額を上回るぎりぎりのUTXOのリストを作成する
    utxos=[]
    begin
        utxos << sorted_utxos.shift
        balance = utxos.reduce(0){|s,t|s+=t["amount"]}
    end until balance >= amount
    return utxos
end
```

#### 送金のテスト

```ruby
# テスト用の秘密鍵（WIF形式）
priv_alice = "cTzEHTNYQQ2PGLJWFkXdpxHm3B8kuU7yN3c5b5ZkSWzUY6j3ZW5B"
priv_bob   = "cUTymPpf51Y1Q8hddD93kSqVahkdFQJXWCuMgY8ctHJqbermrXPK"
priv_carol = "cRaww9WmBUogesd6vEBBi4A24yZdxSMQT75FJ9ZQTqWtpRJSTG2f"
priv_david = "cSHFCDprZSsp423BG8brHReK7FG4CNifJYZvgcZ6N6QchcxEgyDp"

# 鍵オブジェクトの復元(WIF形式の秘密鍵から生成）
keyAlice = Bitcoin::Key.from_wif(priv_alice)
keyBob   = Bitcoin::Key.from_wif(priv_bob)
keyCarol = Bitcoin::Key.from_wif(priv_carol)
keyDavid = Bitcoin::Key.from_wif(priv_david)

## アドレス
alice = keyAlice.to_p2wpkh
bob   = keyBob.to_p2wpkh
carol = keyCarol.to_p2wpkh
david = keyDavid.to_p2wpkh

# 公開鍵
pub_alice = keyAlice.pubkey
pub_bob   = keyBob.pubkey
pub_carol = keyCarol.pubkey
pub_david = keyDavid.pubkey

# 送金金額、署名閾値、公開鍵リスト
amount = 0.001
m = 2
pubkeys = [pub_alice, pub_bob, pub_carol]

# 送金
tx, txid, redeem_script = send_multisig(amount,m,pubkeys)

# トランザクション
tx.to_h

=> 
{:txid=>"d2d4c64cb1a66c00978c6ac856710f0326170432364162201ca14248001b7f6e",
 :hash=>"d2d4c64cb1a66c00978c6ac856710f0326170432364162201ca14248001b7f6e",
 :version=>1,
 :size=>125,
 :vsize=>125,
 :locktime=>0,
 :vin=>[{:txid=>"2eac8a8e303df242ff5c797c4d9322623996af068fedd892f0f33e639949c411", :vout=>1, :script_sig=>{:asm=>"", :hex=>""}, :sequence=>4294967295}],
 :vout=>
  [{:value=>0.001,
    :script_pubkey=>
     {:asm=>"0 c2ca3ee12442603be1ef35e41e84a48b45853df8811820c48916b95b842fd320",
      :hex=>"0020c2ca3ee12442603be1ef35e41e84a48b45853df8811820c48916b95b842fd320",
      :type=>"witness_v0_scripthash",
      :req_sigs=>1,
      :addresses=>["tb1qct9racfygfsrhc00xhjpap9y3dzc200csyvzp3yfz6u4hpp06vsqx3jamu"]},
    :n=>0},
   {:value=>0.00128839,
    :script_pubkey=>
     {:asm=>"0 6355781f432b1948a063961430b93b2b17c399a3",
      :hex=>"00146355781f432b1948a063961430b93b2b17c399a3",
      :type=>"witness_v0_keyhash",
      :req_sigs=>1,
      :addresses=>["tb1qvd2hs86r9vv53grrjc2rpwfm9vtu8xdrwsxpga"]},
    :n=>1}]}

# txid
=> "d2d4c64cb1a66c00978c6ac856710f0326170432364162201ca14248001b7f6e"

# redeem script
redeem_script.to_h
=> 
{:asm=>
  "2 03514c503bf86f1be5db9c5aa0de0e4ef21e2d305917e3ce467eac4e0120a03fd3 03397cb18a98e031beab4581ff0ba6964552844f41334d1f669c30629c02441d1c 02e165e95b34ffb98414a1f1d7649d888919b419182dd7ff0953da42995b9014bf 3 OP_CHECKMULTISIG",
 :hex=>
  "522103514c503bf86f1be5db9c5aa0de0e4ef21e2d305917e3ce467eac4e0120a03fd32103397cb18a98e031beab4581ff0ba6964552844f41334d1f669c30629c02441d1c2102e165e95b34ffb98414a1f1d7649d888919b419182dd7ff0953da42995b9014bf53ae",
 :type=>"multisig",
 :req_sigs=>2,
 :addresses=>["mgaMA7mRNLCdKAA8XuzPaCTgUy38nyf7Z6", "mqe56tG5LMsVSc7Uf8ohcNQvsmqo7NkHAt", "mqWSBCnFLJxxtMknK6QJ7AWs8AWprpwUzd"]}

# redeem scriptの16進数形式
redeem_script.to_hex
=> "522103514c503bf86f1be5db9c5aa0de0e4ef21e2d305917e3ce467eac4e0120a03fd32103397cb18a98e031beab4581ff0ba6964552844f41334d1f669c30629c02441d1c2102e165e95b34ffb98414a1f1d7649d888919b419182dd7ff0953da42995b9014bf53ae"

```

## P2WSH タイプのUTXOの使用（アンロック）

#### bitcoin core RPC

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
FEE = 0.00002       # 手数料

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


### アンロックに必要な情報

* 送金者から、送金したP2WSHトランザクションID と redeem scriptを入手する必要がある
* m of n マルチシグにおける m 人以上の署名鍵が必要

#### マルチシグP2WSH のトランザクションID

```ruby
txid="d2d4c64cb1a66c00978c6ac856710f0326170432364162201ca14248001b7f6e"
```

#### redeem script（16進形式）

```ruby
redeem_script_hex = "522103514c503bf86f1be5db9c5aa0de0e4ef21e2d305917e3ce467eac4e0120a03fd32103397cb18a98e031beab4581ff0ba6964552844f41334d1f669c30629c02441d1c2102e165e95b34ffb98414a1f1d7649d888919b419182dd7ff0953da42995b9014bf53ae"
```

#### 受領者の秘密鍵、アドレス、公開鍵

```ruby
# テスト用の秘密鍵（WIF形式）
priv_alice = "cTzEHTNYQQ2PGLJWFkXdpxHm3B8kuU7yN3c5b5ZkSWzUY6j3ZW5B"
priv_bob   = "cUTymPpf51Y1Q8hddD93kSqVahkdFQJXWCuMgY8ctHJqbermrXPK"
priv_carol = "cRaww9WmBUogesd6vEBBi4A24yZdxSMQT75FJ9ZQTqWtpRJSTG2f"
priv_david = "cSHFCDprZSsp423BG8brHReK7FG4CNifJYZvgcZ6N6QchcxEgyDp"

# 鍵オブジェクトの復元(WIF形式の秘密鍵から生成）
keyAlice = Bitcoin::Key.from_wif(priv_alice)
keyBob   = Bitcoin::Key.from_wif(priv_bob)
keyCarol = Bitcoin::Key.from_wif(priv_carol)
keyDavid = Bitcoin::Key.from_wif(priv_david)
```

#### redeem scriptの復元

```ruby
redeem_script = Bitcoin::Script.parse_from_payload(redeem_script_hex.htb)
```

#### scriptPubKey

```ruby
# (P2WSH マルチシグ)のscriptPubKey生成
scriptPubKey = Bitcoin::Script.from_string("OP_0 #{redeem_script.to_sha256}")

```

#### アンロック対象のP2WSHトランザクションの確認

```ruby
locked_tx = Bitcoin::Tx.parse_from_payload(bitcoinRPC('getrawtransaction',[txid]).htb)
```

#### アンロック対象UTXOの確認

```ruby
utxo = locked_tx.outputs[0]

utxo.to_h

=> 
{:value=>0.001,
 :script_pubkey=>
  {:asm=>"0 c2ca3ee12442603be1ef35e41e84a48b45853df8811820c48916b95b842fd320",
   :hex=>"0020c2ca3ee12442603be1ef35e41e84a48b45853df8811820c48916b95b842fd320",
   :type=>"witness_v0_scripthash",
   :req_sigs=>1,
   :addresses=>["tb1qct9racfygfsrhc00xhjpap9y3dzc200csyvzp3yfz6u4hpp06vsqx3jamu"]}}
```

#### P2WSHでデポジットされている金額(satoshi)

```ruby
deposit = utxo.value
```

#### デポジット金額から手数料を引いた報酬金額 (satoshi)

```ruby
reward = deposit - (FEE*(10**8)).to_i
```

#### 報酬の送付先のP2WPKHアドレス

ここでは Davidにします

```ruby
david = keyDavid.to_p2wpkh
=> "tb1qysvevlktvn07tmt79e8j2rzxdgxphzt2xr9e9v"
```

#### P2WSHアンロックトランザクションの構成（未署名）

```ruby
# トランザクションテンプレートの生成
tx = Bitcoin::Tx.new
# inputの作成
tx.in << Bitcoin::TxIn.new(out_point: Bitcoin::OutPoint.from_txid(txid, 0))
# 報酬用のP2WPKH outputの作成
tx.out << Bitcoin::TxOut.new(value: reward, script_pubkey: Bitcoin::Script.parse_from_addr(david))
```

#### 未署名のP2WSHアンロックトランザクションの sighash

```ruby
sighash = tx.sighash_for_input(0, redeem_script, sig_version: :witness_v0, amount: deposit_satoshi, hash_type: Bitcoin::SIGHASH_TYPE[:all])
```

#### sighash への署名

Aliceの秘密鍵でsighashに署名

```ruby
sigAlice = keyAlice.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
```

Bobの秘密鍵でsighashに署名

```ruby
sigBob = keyBob.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
```

#### 未署名のP2WSHアンロックトランザクションのwitness scriptの構成

sighash に対して、Aliceの署名→Bobの署名の順番で署名を追加（順番を守る）

witness program は、

`OP_0 <sig A> <sig B> <redeem script>` になります

```ruby
tx.in[0].script_witness.stack << sigAlice
tx.in[0].script_witness.stack << sigBob
tx.in[0].script_witness.stack << redeem_script.to_payload
```

#### 完成した2WSHアンロックトランザクション

```ruby
tx.to_h

=> 
{:txid=>"928bdc8a0fb859ca28bcb9001b91ffd35a0526aecef3c2c3960e5a9fb54ae059",
 :hash=>"3b79470646765692f92cef4f5762aeee97164a34859ea78361efcd5121dadd49",
 :version=>1,
 :size=>335,
 :vsize=>146,
 :locktime=>0,
 :vin=>
  [{:txid=>"d2d4c64cb1a66c00978c6ac856710f0326170432364162201ca14248001b7f6e",
    :vout=>0,
    :script_sig=>{:asm=>"", :hex=>""},
    :txinwitness=>
     ["30440220792e6c8cc5ac9471691255f19c1ebed7754a29963289c850a8a153482e1404ae022052fe2362818c5a5df75b234cb1ff382531408c88b5bff1a7ba44d1a82a7af73d01",
      "3044022029162672645650c20d8c573e16a32721be7d8184a41ec03eb645dea5addc125b02200e6ebcaccb23850c7c6696023073201603879d270a61342cd062773c652a459401",
      "522103514c503bf86f1be5db9c5aa0de0e4ef21e2d305917e3ce467eac4e0120a03fd32103397cb18a98e031beab4581ff0ba6964552844f41334d1f669c30629c02441d1c2102e165e95b34ffb98414a1f1d7649d888919b419182dd7ff0953da42995b9014bf53ae"],
    :sequence=>4294967295}],
 :vout=>
  [{:value=>0.00098,
    :script_pubkey=>
     {:asm=>"0 2419967ecb64dfe5ed7e2e4f250c466a0c1b896a",
      :hex=>"00142419967ecb64dfe5ed7e2e4f250c466a0c1b896a",
      :type=>"witness_v0_keyhash",
      :req_sigs=>1,
      :addresses=>["tb1qysvevlktvn07tmt79e8j2rzxdgxphzt2xr9e9v"]},
    :n=>0}]}

```

#### P2WSHトランザクションのデプロイ

```ruby
txid = bitcoinRPC('sendrawtransaction', [tx.to_hex])
```

## P2WSH アンロックトランザクション

上記の総合

```ruby
```