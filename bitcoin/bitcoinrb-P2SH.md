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
    redeem_script = Bitcoin::Script.to_multisig_script(m,pubkeys)
    # P2WSHトランザクションの構成
    tx,utxos = p2wsh_tx(amount, redeem_script)
    # トランザクションへの署名
    tx_h = bitcoinRPC('signrawtransactionwithwallet',[tx.to_hex])
    # トランザクションのデプロイ
    txid = bitcoinRPC('sendrawtransaction',[tx_h["hex"]])
    # P2WSHトランザクションのアンロックに必要な情報の出力
    return tx, txid, redeem_script
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
    # P2WSHアドレス
    p2wsh_addr = Bitcoin::Script.to_p2wsh(redeem_script).to_addr
    scriptPubKey0 = Bitcoin::Script.parse_from_addr(p2wsh_addr)
    tx.out << Bitcoin::TxOut.new(value: amount_satoshi, script_pubkey: scriptPubKey0)
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
tx, locked_txid, redeem_script = send_multisig(amount,m,pubkeys)

# P2WSH でロックされたトランザクションのtxid
locked_txid
```

## P2WSH タイプのUTXOの使用（アンロック）


### アンロックに必要な情報

* 送金者から、送金したP2WSHトランザクションID (locked_txid) と redeem scriptを入手する必要がある
* m of n マルチシグにおける m 人以上の署名鍵が必要

```ruby
# P2WSH のUTXOの使用

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

# P2WSH でロックされたトランザクションのtxid
locked_txid

# redeem script の16進形式
redeem_script_hex = "52210307f83f230d23c2f706f4108ecf6b8330b57369c0529b735e38f9c69fb9d826bf21034d18816344093cbe309beaf4fb1e86c1057c391ea7c85e1c050cb67f0d4d7e2a210260ba91b61f1a47a35f72b4a62880da13c101a2cf2fcd8e7a83fff4e3a0bda5b153ae"
# redeem script の復元
redeem_script = Bitcoin::Script.parse_from_payload(redeem_script_hex.htb)

# アンロック対象トランザクションとUTXO
locked_tx = Bitcoin::Tx.parse_from_payload(bitcoinRPC('getrawtransaction',[p2wsh_txid]).htb)
p2wsh_utxo = locked_tx.out
utxo_vout = 0
utxo_value = p2wsh_utxo[utxo_vout].value    # satoshi

# アンロックトランザクションの構成（送金先はaliceとする）
p2wsh_tx = Bitcoin::Tx.new
# input
outpoint = Bitcoin::OutPoint.from_txid(p2wsh_txid, utxo_vout)
p2wsh_tx.in <<  Bitcoin::TxIn.new(out_point: outpoint)
#output (P2WPKH)
script_pubkey = Bitcoin::Script.parse_from_addr(alice)
p2wsh_tx.out << Bitcoin::TxOut.new(value: utxo_value-(FEE*(10**8)).to_i, script_pubkey: script_pubkey)

# アンロックトランザクションの署名対象のハッシュ値 sighash
sighash = p2wsh_tx.sighash_for_input(0, redeem_script, sig_version: :witness_v0, amount: utxo_value, hash_type: Bitcoin::SIGHASH_TYPE[:all])
# aliceとbobのsighasuへの署名
sigAlice = keyAlice.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
sigBob   = keyBob.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
# witness scriptの追加
p2wsh_tx.in[0].script_witness.stack << ""   # CHECKMULTISIGのバグ対応　NULLDUMMY　を入れる
p2wsh_tx.in[0].script_witness.stack << sigAlice
p2wsh_tx.in[0].script_witness.stack << sigBob
p2wsh_tx.in[0].script_witness.stack << redeem_script.to_payload
# 署名したトランザクションをブロードキャストする
p2wsh_txid = bitcoinRPC('sendrawtransaction', [p2wsh_tx.to_hex])

```

