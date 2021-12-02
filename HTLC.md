# HTLCの作成
2021/11/20


## Alice からBobに送金する


![](./htlc1.png)

#E Alice からBob経由で Calolに送金する

![](./htlc2.png)

## HTCLの典型例



### redeem script (一般化したロックスクリプト)

注意!:　(`OP_CSV`は，初期のビットコインスクリプトの仕様ではリザーブされていたオペコードを使用しているので，旧仕様のノードにリジェクトされないように `OP_DROP`でスタックから取り除いています)

```
OP_IF
    OP_SHA256 <Sのハッシュ値> OP_EQUALVERIFY 
    <Bobの公開鍵>
OP_ELSE
    <ロックするブロック数> OP_CSV 
    OP_DROP  
    <Aliceの公開鍵>
OP_ENDIF
OP_CHECKSIG
```

### unlocking script (Bobによるアンロックの場合）

```
<Bobの署名> 
<S> 
OP_1
```

### unlocking script と redeem scriptの連接結果 (witness)

```
<Bobの署名> 
<S> 
OP_1
------------連接--------------
OP_IF
    OP_SHA256 <Sのハッシュ値> OP_EQUALVERIFY 
    <Bobの公開鍵>
OP_ELSE
    <ロックするブロック数> OP_CSV 
    OP_DROP  
    <Aliceの公開鍵>
OP_ENDIF
OP_CHECKSIG
```

1. スタックに， `<Bobの署名>` ，`<S>`，`OP_1`　が順に積まれます
2. `OP_1`(true) が適用され，`OP_IF` 側の処理が実行されます
3. `<S>` の`OP_SHA256` の結果と `<Sのハッシュ値>` が`OP_EQUALVERIFY` で比較されます
4. 等しければ，Bobの公開鍵による Bobの署名が検証され，成功すればロックが解除されます


### AliceとBobのアドレスを生成する

AliceとBobは異なるワレットで操作するとよいでしょう

```
bitcoin-core.cli getnewaddress Alice
tb1q92v4dxsz47zxs5rdu42q7nl4xsdlncmvswxr5f
```

```
bitcoin-core.cli getnewaddress Bob
tb1q9vml26m9vgm5nk3fk9v7cfkad7tlfgsgnahkfu
```

### AliceとBobの秘密鍵

```
bitcoin-core.cli dumpprivkey tb1q92v4dxsz47zxs5rdu42q7nl4xsdlncmvswxr5f
cVkvmDQ2TvRg1L1eLCfoAhYZpWAiaJyesyF2owMA9kUJj6mczJQP
```

```
bitcoin-core.cli dumpprivkey tb1q9vml26m9vgm5nk3fk9v7cfkad7tlfgsgnahkfu
cUpy2Z19AC22MnGLNBfrNMZqrbz7v7rtL9UByzMoxqGC4v9SKtFf
```

### AliceのUTXOの確認

Alice に 0.1 btc 送金する

```bash
bitcoin-core.cli send '{"tb1q92v4dxsz47zxs5rdu42q7nl4xsdlncmvswxr5f": 0.1}'

{
  "txid": "243f497b5452d0810522cdc53d36b9f1f99bbe4ff07c5d6d12bb5ff1760e8817",
  "complete": true
}

bitcoin-core.cli getrawtransaction 243f497b5452d0810522cdc53d36b9f1f99bbe4ff07c5d6d12bb5ff1760e8817

02000000000102e2963966f127b78006fb67a5a8b9d1662b3086c3b916c52ec9e141cb6cf94adc0100000000feffffffa1565e6097789c42843cf4b0eafaa6480542828daa34d8218bad6311250307ca0100000000feffffff02d2ab680000000000160014661fe0da3bea105d7f934ca978437b8a190bfeb280969800000000001600142a99569a02af8468506de5540f4ff5341bf9e36c0247304402200a42ca1de97830a203e2da163a08f04fab83d4c982789d32f3cb85115532b26e022005d42b3fe00ae0dec31d35c200d55df23cd91997503a2237b2cabe89794a1bf4012103760b5a28bb264ca5b310d766ad3a095c9f866fe9a0b38b8079ca350c6a3e334702473044022019298d970a2414ed153ea132f903119f11e5a473a3a444ee21d330018bc5186b022069f3db1f33d566425b7549c056a87a8c23dbf419ce6e25d01b8dbe6a6b6f03db012103760b5a28bb264ca5b310d766ad3a095c9f866fe9a0b38b8079ca350c6a3e334700000000

```

## bitcoinrb で実装する

```ruby
require 'bitcoin'
require 'net/http'
require 'json'

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

# AliceとBobのアドレス
addrAlice="tb1q92v4dxsz47zxs5rdu42q7nl4xsdlncmvswxr5f"
addrBob="tb1q9vml26m9vgm5nk3fk9v7cfkad7tlfgsgnahkfu"

# 秘密鍵（WIF形式）
privAlice='cVkvmDQ2TvRg1L1eLCfoAhYZpWAiaJyesyF2owMA9kUJj6mczJQP'
privBob='cUpy2Z19AC22MnGLNBfrNMZqrbz7v7rtL9UByzMoxqGC4v9SKtFf'

# 鍵オブジェクト(WIF形式の秘密鍵から生成）
keyAlice=Bitcoin::Key.from_wif(privAlice)
keyBob=Bitcoin::Key.from_wif(privBob)

# AliceのUTXOと残高を確認（とりあえず最初の Alice宛のUTXOを利用することにする）
utxos=bitcoinRPC('listunspent',[]).select{|x| x["address"]==addrAlice}
utxoAmount = utxos[0]["amount"]
utxoVout = utxos[0]["vout"]
utxoTxid = utxos[0]["txid"]
utxoScriptPubKey = utxos[0]["scriptPubKey"]
```

OP_SHA256
### 秘密情報はCarolが生成するものとします。

`<S>` ：秘密情報　

この説明では　"HTLC_test"　とします。

```ruby
secret='HTLC_test'
```

#### script処理のテスト(OP_SHA256の検証)

```ruby
# <Sの OP_SHA256 ハッシュ値>
secret='HTLC_test'
secret_hash=Bitcoin.sha256(secret)

# scriptのテスト
test_script="#{secret} OP_SHA256 #{secret_hash} OP_EQUAL"
ts=Bitcoin::Script.from_string(test_script)
ts.run

=> true
```

#### redeem script

```ruby
# <ロックするブロック数> 10日間
locktime = 6*24*10

# テキスト形式の redeem script
redeem_script_text ="OP_IF OP_SHA256 #{secret_hash} OP_EQUALVERIFY #{keyBob.pubkey} OP_ELSE #{locktime} OP_CSV OP_DROP #{keyAlice.pubkey} OP_ENDIF OP_CHECKSIG"

# redeem scriptオブジェクト
redeem_script = Bitcoin::Script.from_string(redeem_script_text)

```

redeem scriptの内容を確認する

```ruby
# redeem script
redeem_script.to_h

=> 
{:asm=>
  "OP_IF OP_SHA256 996bf59473947d9906275f427ecb318371514db2ffb8e9d8517b5e45cb65e357 OP_EQUALVERIFY 303364363631393966306464366262643136316364346138353463643233386134646265626632643063663131333331383037393765313237306461633365353238 OP_ELSE 1440 OP_CSV OP_DROP 303266353161656130353836323438663935323862393664313366643135356430366333393466623664633564373930353638353337626536386337356561666637 OP_ENDIF OP_CHECKSIG",
 :hex=>
  "63a820996bf59473947d9906275f427ecb318371514db2ffb8e9d8517b5e45cb65e35788423033643636313939663064643662626431363163643461383534636432333861346462656266326430636631313333313830373937653132373064616333653532386702a005b2754230326635316165613035383632343866393532386239366431336664313535643036633339346662366463356437393035363835333762653638633735656166663768ac",
 :type=>"nonstandard"}
 
# 秘密情報 Sのハッシュ値(16進数形式)
secret_hash.bth

=> "996bf59473947d9906275f427ecb318371514db2ffb8e9d8517b5e45cb65e357"

# Bobの公開鍵
keyBob.pubkey

=> "03d66199f0dd6bbd161cd4a854cd238a4dbebf2d0cf1133180797e1270dac3e528"

#ロックタイム
locktime

=> 1440

# Aliceの公開鍵
keyAlice.pubkey

=> "02f51aea0586248f9528b96d13fd155d06c394fb6dc5d790568537be68c75eaff7"
 
```

### HTLCロックトランザクションの scriptPubKey

P2WSHのscriptPubKeyは，以下の形式になる

```
0 <redeem scriptの SHA256ハッシュ>
```

Bitcoin::Script オブジェクトの to_payload メソッドでバイナリを得ることができるので，そのSHA256ハッシュをとる

```ruby
scriptPubKey_p2wsh = Bitcoin::Script.from_string("0 #{Bitcoin.sha256(redeem_script.to_payload)}")

scriptPubKey_p2wsh.to_h

=> 
{:asm=>"0 5ce9fba086d4473abbb3ec6e30711c004a8a763d2bf82327c385889bc36ec1fd",
 :hex=>"00205ce9fba086d4473abbb3ec6e30711c004a8a763d2bf82327c385889bc36ec1fd",
 :type=>"witness_v0_scripthash",
 :req_sigs=>1,
 :addresses=>["tb1qtn5lhgyx63rn4wana3hrquguqp9g5a3a90uzxf7rskyfhsmwc87srs227z"]}
```

### P2WSH アドレスの生成

```ruby
p2wshaddr = scriptPubKey_p2wsh.to_addr

=> "tb1qtn5lhgyx63rn4wana3hrquguqp9g5a3a90uzxf7rskyfhsmwc87srs227z"
```

## AliceがHTLCロックトランザクションを作成

### AliceはBobからBobの公開鍵を得る

```ruby
pubkeyBob = keyBob.pubkey

=> "03d66199f0dd6bbd161cd4a854cd238a4dbebf2d0cf1133180797e1270dac3e528"
```

### AliceはCarolから秘密情報 `<S>` の OP_SHA256 ハッシュ値を得る

```ruby
secret_hash

=> "\x99k\xF5\x94s\x94}\x99\x06'_B~\xCB1\x83qQM\xB2\xFF\xB8\xE9\xD8Q{^E\xCBe\xE3W"
```

### HTLCロック・トランザクションの資金の確認

output (デポジット金額：0.01,手数料 0.0002)

```ruby
# 使用するUTXOの金額
utxoAmount
# HTLCでデポジットする金額
deposit =0.01
# 手数料
fee=0.0002
# お釣り
change= utxoAmount-deposit-fee

# それぞれの金額をSatoshiに変換
utxoAmount_satoshi = (utxoAmount * 100000000).to_i
deposit_satoshi = (deposit * 100000000).to_i
change_satoshi = (change * 100000000).to_i
```

### 未署名のHTLCロック・トランザクションの作成

```ruby
# トランザクションテンプレートの生成
tx = Bitcoin::Tx.new
# inputの作成
tx.in << Bitcoin::TxIn.new(out_point: Bitcoin::OutPoint.from_txid(utxoTxid, utxoVout))
# デポジット用　P2WSH outputの作成
tx.out << Bitcoin::TxOut.new(value: deposit_satoshi, script_pubkey:  Bitcoin::Script.parse_from_addr(p2wshaddr))
# おつり用のP2WPKH outputの作成
tx.out << Bitcoin::TxOut.new(value: change_satoshi , script_pubkey:  Bitcoin::Script.parse_from_addr(addrAlice))
```


### Alice によるHTLCロックトランザクションへのデジタル署名


#### 署名対象のsighashを計算

bitcoinrb のStringクラスには，htb (hex to binary) や bth(binary to hex) というメソッドが追加されている

```ruby
# UTXOのロックを解除するために、UTXOのScript Public key を取得
utxo_scriptPubKey = Bitcoin::Script.parse_from_payload(utxoScriptPubKey.htb)

# sighashを作成
sighash = tx.sighash_for_input(0, utxo_scriptPubKey, sig_version: :witness_v0, amount: utxoAmount_satoshi)

```

####  Aliceの秘密鍵でHTLCロックトランザクションの署名を作成する

 最後に　SHIGHASH_TYPE を追加して指定することを忘れないようにする。SHIGHASH_TYPE はALL
 
```ruby
signature = keyAlice.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')

signature.bth

=> "304402200b600aba52781d5923c350d64e335c98c395036fc56ac09b3fd41ba413815236022024023a32409c8d520d9aa99671fc7eacb4e29a0f9bd5d521fb649f871b89053101"
```

#### witness script の追加

witness script はスタックとして追加していく
`
<Aliceの署名>
<Aliceの公開鍵>
`

```ruby
# witness にAliceのsighash へ署名をプッシュする
tx.in[0].script_witness.stack << signature

# witness にAliceの公開鍵（バイナリ形式）をプッシュする
tx.in[0].script_witness.stack <<  keyAlice.pubkey.htb
```

#### 完成したHTLCロックトランザクション

```ruby
bitcoinRPC('decoderawtransaction',[tx.to_payload.bth])

=> 
{"txid"=>"9a63c0aca52a71d2999ca82716f7b2890cb999fc5b82026e0e65444258012c04",
 "hash"=>"abf5b73d20ea01f354be3ac0e5fec00226cc9b3cd55de6584a3c28c10c60fe6c",
 "version"=>1,
 "size"=>234,
 "vsize"=>153,
 "weight"=>609,
 "locktime"=>0,
 "vin"=>
  [{"txid"=>"4c31b60f174b734f0bb085dd16f458a85776b48ae499757ee03c93c5c8dc1d1d",
    "vout"=>1,
    "scriptSig"=>{"asm"=>"", "hex"=>""},
    "txinwitness"=>
     ["304402200b600aba52781d5923c350d64e335c98c395036fc56ac09b3fd41ba413815236022024023a32409c8d520d9aa99671fc7eacb4e29a0f9bd5d521fb649f871b89053101",
      "02f51aea0586248f9528b96d13fd155d06c394fb6dc5d790568537be68c75eaff7"],
    "sequence"=>4294967295}],
 "vout"=>
  [{"value"=>0.01,
    "n"=>0,
    "scriptPubKey"=>
     {"asm"=>"0 5ce9fba086d4473abbb3ec6e30711c004a8a763d2bf82327c385889bc36ec1fd",
      "hex"=>"00205ce9fba086d4473abbb3ec6e30711c004a8a763d2bf82327c385889bc36ec1fd",
      "address"=>"tb1qtn5lhgyx63rn4wana3hrquguqp9g5a3a90uzxf7rskyfhsmwc87srs227z",
      "type"=>"witness_v0_scripthash"}},
   {"value"=>0.05938,
    "n"=>1,
    "scriptPubKey"=>
     {"asm"=>"0 2a99569a02af8468506de5540f4ff5341bf9e36c",
      "hex"=>"00142a99569a02af8468506de5540f4ff5341bf9e36c",
      "address"=>"tb1q92v4dxsz47zxs5rdu42q7nl4xsdlncmvswxr5f",
      "type"=>"witness_v0_keyhash"}}]}
```


#### AliceによるHTLCロックトランザクションのブロードキャスト

```ruby
htcl_lockTx_txid = bitcoinRPC('sendrawtransaction',[tx.to_payload.bth])


htcl_lockTx_txid

=> "70c906bb6e4473bd0d86c9df6530db810c44f0999cbdef8db5a9c1363537dc73"
```

---------------------------------

## Bob によるHTCLアンロックトランザクションの作成

### Bob側のマシンの環境セットアップ

```ruby
require 'bitcoin'
require 'net/http'
require 'json'

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

# AliceとBobのアドレス
addrAlice="tb1q92v4dxsz47zxs5rdu42q7nl4xsdlncmvswxr5f"
addrBob="tb1q9vml26m9vgm5nk3fk9v7cfkad7tlfgsgnahkfu"

# 秘密鍵（WIF形式）
privAlice='cVkvmDQ2TvRg1L1eLCfoAhYZpWAiaJyesyF2owMA9kUJj6mczJQP'
privBob='cUpy2Z19AC22MnGLNBfrNMZqrbz7v7rtL9UByzMoxqGC4v9SKtFf'

# 鍵オブジェクト(WIF形式の秘密鍵から生成）
keyAlice=Bitcoin::Key.from_wif(privAlice)
keyBob=Bitcoin::Key.from_wif(privBob)

```


### アンロックのためにBobが知っている（べき）情報

* Aliceの公開鍵
* Bobの公開鍵
* 秘密情報 `<S>` (Carolから開示される）
* redeem script (Aliceからもらう）
* HTLCロックトランザクションの トランザクションID (Aliceからもらう）
    * HTLCロックトランザクションの scriptPubKey
    * HTLCロックトランザクションのP2WSHアドレス
    * アンロックの対象となるUTXO のvout
    * アンロックの対象となるUTXO の金額



```ruby

# Aliceの公開鍵
pubkeyAlice = keyAlice.pubkey
# Bobの公開鍵
pubkeyBob = keyBob.pubkey
# 秘密情報
secret='HTLC_test'

# redeem script
    # <ロックするブロック数> 10日間
locktime = 6*24*10
    # 秘密情報のハッシュ値
secret_hash=Bitcoin.sha256(secret)
# テキスト形式の redeem script
redeem_script_text ="OP_IF OP_SHA256 #{secret_hash} OP_EQUALVERIFY #{pubkeyBob} OP_ELSE #{locktime} OP_CSV OP_DROP #{pubkeyAlice} OP_ENDIF OP_CHECKSIG"
# redeem scriptオブジェクト
redeem_script = Bitcoin::Script.from_string(redeem_script_text)

# HTLCロックトランザクションの トランザクションID
htcl_lockTx_txid = "7ccab3206daf6e37c641a969505e98530228caee5dc2fa02a826eeb495b4b54b"
# HTLCロックトランザクション
htlc_tx = bitcoinRPC('decoderawtransaction',[ bitcoinRPC('getrawtransaction',[htcl_lockTx_txid])])

# HTLCロックトランザクションの scriptPubKey
scriptPubKey_p2wsh = Bitcoin::Script.from_string("0 #{Bitcoin.sha256(redeem_script.to_payload)}")
# HTLCロックトランザクションのP2WSHアドレス
p2wshaddr = scriptPubKey_p2wsh.to_addr
 
# アンロックの対象となるUTXO のvout
htcl_lockTx_vout=0
# アンロックの対象となるUTXO へのデポジット金額
deposit=0.01
# 手数料
fee=0.0002
# 報酬金額
reward = deposit-fee
# satoshi 変換
deposit_satoshi = (deposit * 100000000).to_i
reward_satoshi = (reward* 100000000).to_i
```

### redeem script と unlocking script の基本構造（再掲）

```
<Bobの署名> 
<S> 
OP_1

        (unlocking script)
------------連接--------------
        (redeem script)
        
OP_IF
    OP_SHA256 <Sのハッシュ値> OP_EQUALVERIFY 
    <Bobの公開鍵>
OP_ELSE
    <ロックするブロック数> OP_CSV 
    OP_DROP  
    <Aliceの公開鍵>
OP_ENDIF
OP_CHECKSIG
```


### HTLCをアンロックする未署名トランザクションの作成

```ruby
# トランザクションテンプレートの生成
tx = Bitcoin::Tx.new
# inputの作成
tx.in << Bitcoin::TxIn.new(out_point: Bitcoin::OutPoint.from_txid(htcl_lockTx_txid, htcl_lockTx_vout))
# 報酬用のP2WPKH outputの作成
tx.out << Bitcoin::TxOut.new(value: reward_satoshi, script_pubkey: Bitcoin::Script.parse_from_addr(addrBob))
```

### HTLCロックトランザクションの scriptPubKey

```ruby
scriptPubKey_p2wsh = Bitcoin::Script.parse_from_addr(p2wshaddr)
```

### 署名対象のsighashを計算

```ruby
# sighashを作成
sighash = tx.sighash_for_input(0, scriptPubKey_p2wsh, sig_version: :witness_v0, amount: deposit_satoshi)
```

### Bobの秘密鍵でHTLCロックトランザクションをアンロックするための署名を作成する

```ruby
rs="63a820996bf59473947d9906275f427ecb318371514db2ffb8e9d8517b5e45cb65e35788423033643636313939663064643662626431363163643461383534636432333861346462656266326430636631313333313830373937653132373064616333653532386702a005b2754230326635316165613035383632343866393532386239366431336664313535643036633339346662366463356437393035363835333762653638633735656166663768ac"

# SHIGHASH_TYPE ALL
signature = keyBob.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
```

### script 処理のテスト

このsignature で P2WSHがアンロックできることを確認する

```ruby
# P2WSH のscriptPubKeyの確認
scriptPubKey_p2wsh.to_hex
=> "00205ce9fba086d4473abbb3ec6e30711c004a8a763d2bf82327c385889bc36ec1fd"

unlock_script =""
unlock_script << signature.bth
unlock_script << " "
unlock_script << secret.bth
unlock_script << " "
unlock_script << Bitcoin::Script.from_string("OP_1").to_payload.bth
script = Bitcoin::Script.from_string(unlock_script + " " + redeem_script_text.bth)
script.run

```

### witness scriptの追加


```ruby
# witness に "" プッシュする
tx.in[0].script_witness.stack << ""
# witness に Bob のsighash 署名をプッシュする
tx.in[0].script_witness.stack << signature
# 秘密情報をプッシュ
tx.in[0].script_witness.stack <<  secret
# OP_1をプッシュ
tx.in[0].script_witness.stack <<  Bitcoin::Script.from_string("OP_1").to_payload
# redeem scriptをプッシュ
tx.in[0].script_witness.stack <<  Bitcoin::Script.from_string(redeem_script_text).to_payload
```

### 完成したトランザクションの確認

```ruby
bitcoinRPC('decoderawtransaction',[tx.to_payload.bth])

=> 
{"txid"=>"349bffb75a0022bf55774b0d91fb6edecba3c2a72abd7d1063f4797571e6f991",
 "hash"=>"289b0ddb1ff44b00b1ba62775d80962e03d82bf2fdd99a40435109f14287451e",
 "version"=>1,
 "size"=>348,
 "vsize"=>149,
 "weight"=>594,
 "locktime"=>0,
 "vin"=>
  [{"txid"=>"7ccab3206daf6e37c641a969505e98530228caee5dc2fa02a826eeb495b4b54b",
    "vout"=>0,
    "scriptSig"=>{"asm"=>"", "hex"=>""},
    "txinwitness"=>
     ["30440220311b7756b08237bf47524852e49fdea2d1074c26952874a6d026bd17198bbaa30220203dd2bab9ae8fdf0f2c6d376d39df81621e43e7447ab0c57ae2832346a2ec3d01",
      "48544c435f74657374",
      "51",
      "63a820996bf59473947d9906275f427ecb318371514db2ffb8e9d8517b5e45cb65e35788423033643636313939663064643662626431363163643461383534636432333861346462656266326430636631313333313830373937653132373064616333653532386702a005b2754230326635316165613035383632343866393532386239366431336664313535643036633339346662366463356437393035363835333762653638633735656166663768ac"],
    "sequence"=>4294967295}],
 "vout"=>
  [{"value"=>0.0098,
    "n"=>0,
    "scriptPubKey"=>
     {"asm"=>"0 2b37f56b65623749da29b159ec26dd6f97f4a208",
      "hex"=>"00142b37f56b65623749da29b159ec26dd6f97f4a208",
      "address"=>"tb1q9vml26m9vgm5nk3fk9v7cfkad7tlfgsgnahkfu",
      "type"=>"witness_v0_keyhash"}}]}
```

## HTLC アンロックトランザクションのブロードキャスト

```ruby
htcl_unlockTx_txid = bitcoinRPC('sendrawtransaction',[tx.to_payload.bth])


htcl_unlockTx_txid
```

