# HTLCの作成
2021/11/20


# Alice からBobに送金する


![](./htlc1.png)

# Alice からBob経由で Calolに送金する

![](./htlc2.png)

## HTCLの典型例



### redeem script (一般化したロックスクリプト)

注意!:　(`OP_CSV`は，初期のビットコインスクリプトの仕様ではリザーブされていたオペコードを使用しているので，旧仕様のノードにリジェクトされないように `OP_DROP`でスタックから取り除いています)

```
OP_IF
    OP_HASH160 <Sのハッシュ値> OP_EQUALVERIFY 
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
    OP_HASH160 <Sのハッシュ値> OP_EQUALVERIFY 
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
3. `<S>` の`OP_HASH160` の結果と `<Sのハッシュ値>` が`OP_EQUALVERIFY` で比較されます
4. 等しければ，Bobの公開鍵による Bobの署名が検証され，成功すればロックが解除されます


## AliceとBobのアドレスを生成する

AliceとBobは異なるワレットで操作するとよいでしょう

```
bitcoin-core.cli getnewaddress Alice
tb1q92v4dxsz47zxs5rdu42q7nl4xsdlncmvswxr5f
```

```
bitcoin-core.cli getnewaddress Bob
tb1qy0e6mr95dwcsk4uf4365vjnfl58nc9duzdkhe4
```

## AliceとBobの秘密鍵

```
bitcoin-core.cli dumpprivkey tb1q92v4dxsz47zxs5rdu42q7nl4xsdlncmvswxr5f
cVkvmDQ2TvRg1L1eLCfoAhYZpWAiaJyesyF2owMA9kUJj6mczJQP
```

```
bitcoin-core.cli dumpprivkey tb1qy0e6mr95dwcsk4uf4365vjnfl58nc9duzdkhe4
cW2sF8NzcUyXW6gmz2Utarz5xSiFCFsXwWhNPN2wVP5RwscdsQxB
```

## AliceのUTXOの確認

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

# bitcoinrb で実装する

```ruby
require 'bitcoin'
Bitcoin.chain_params = :signet

addrAlice="tb1q92v4dxsz47zxs5rdu42q7nl4xsdlncmvswxr5f"
addrBob="tb1qy0e6mr95dwcsk4uf4365vjnfl58nc9duzdkhe4"

# 秘密鍵
privAlice='cVkvmDQ2TvRg1L1eLCfoAhYZpWAiaJyesyF2owMA9kUJj6mczJQP'
privBob='cW2sF8NzcUyXW6gmz2Utarz5xSiFCFsXwWhNPN2wVP5RwscdsQxB'

# 鍵情オブジェクト
keyAlice=Bitcoin::Key.new(priv_key: privAlice)
keyBob=Bitcoin::Key.new(priv_key: privBob)

# 公開鍵 (bitcoin core の getaddressinfo で得る）
pubkeyAlice= "02f51aea0586248f9528b96d13fd155d06c394fb6dc5d790568537be68c75eaff7"
pubkeyBob= "03353ec9fc21ebf6235250c0eace51b5f3d8fc44d7fdfbf1d49c099cbbba48d359"

# 使用するUTXOを持つトランザクション

prev_tx=Bitcoin::Tx.parse_from_payload('02000000000102e2963966f127b78006fb67a5a8b9d1662b3086c3b916c52ec9e141cb6cf94adc0100000000feffffffa1565e6097789c42843cf4b0eafaa6480542828daa34d8218bad6311250307ca0100000000feffffff02d2ab680000000000160014661fe0da3bea105d7f934ca978437b8a190bfeb280969800000000001600142a99569a02af8468506de5540f4ff5341bf9e36c0247304402200a42ca1de97830a203e2da163a08f04fab83d4c982789d32f3cb85115532b26e022005d42b3fe00ae0dec31d35c200d55df23cd91997503a2237b2cabe89794a1bf4012103760b5a28bb264ca5b310d766ad3a095c9f866fe9a0b38b8079ca350c6a3e334702473044022019298d970a2414ed153ea132f903119f11e5a473a3a444ee21d330018bc5186b022069f3db1f33d566425b7549c056a87a8c23dbf419ce6e25d01b8dbe6a6b6f03db012103760b5a28bb264ca5b310d766ad3a095c9f866fe9a0b38b8079ca350c6a3e334700000000'.htb)

# UTXO のscript pubkey
prev_script_pubkey=prev_tx.out[1].script_pubkey

prev_script_pubkey.to_h
=> 
{:asm=>"0 2a99569a02af8468506de5540f4ff5341bf9e36c",
 :hex=>"00142a99569a02af8468506de5540f4ff5341bf9e36c",
 :type=>"witness_v0_keyhash",
 :req_sigs=>1,
 :addresses=>["tb1q92v4dxsz47zxs5rdu42q7nl4xsdlncmvswxr5f"]}
```

## 秘密情報はCarolが生成するものとします。

`<S>` ：秘密情報　

この説明では　"htlctest"　とします。

```ruby
secret='htlctest'
```

### redeem script

```ruby
# <Sの OP_HASH160 ハッシュ値>
secret_hash=Bitcoin.hash160(secret)

# scriptのテスト
test_script="#{secret} OP_HASH160 #{secret_hash} OP_EQUAL"
ts=Bitcoin::Script.from_string(test_script)
ts.run

=> true
```

```

# <ロックするブロック数>`：
locktime = 6*24*10

redeem_script ="OP_IF OP_HASH160 #{secret_hash} OP_EQUALVERIFY #{pubkeyBob} OP_ELSE #{locktime} OP_CSV OP_DROP #{pubkeyAlice} OP_ENDIF OP_CHECKSIG"

=> "OP_IF OP_HASH160 914cb6e51042e1a98def39ecaa52962ebb4aad81 OP_EQUALVERIFY 03353ec9fc21ebf6235250c0eace51b5f3d8fc44d7fdfbf1d49c099cbbba48d359 OP_ELSE 1440 OP_CSV OP_DROP 02f51aea0586248f9528b96d13fd155d06c394fb6dc5d790568537be68c75eaff7 OP_ENDIF OP_CHECKSIG"
```

### bitcoinrbで  redeem script をエンコードする

```ruby
enc_redeem_script = Bitcoin::Script.from_string(redeem_script)

enc_redeem_script.to_h

WARNING: Bitcoin::Script#addresses is deprecated. Use Bitcoin::Script#to_addr instead.
=> 
{:asm=>
  "OP_IF OP_HASH160 914cb6e51042e1a98def39ecaa52962ebb4aad81 OP_EQUALVERIFY 03353ec9fc21ebf6235250c0eace51b5f3d8fc44d7fdfbf1d49c099cbbba48d359 OP_ELSE 1440 OP_CSV OP_DROP 02f51aea0586248f9528b96d13fd155d06c394fb6dc5d790568537be68c75eaff7 OP_ENDIF OP_CHECKSIG",
 :hex=>
  "63a914914cb6e51042e1a98def39ecaa52962ebb4aad81882103353ec9fc21ebf6235250c0eace51b5f3d8fc44d7fdfbf1d49c099cbbba48d3596702a005b2752102f51aea0586248f9528b96d13fd155d06c394fb6dc5d790568537be68c75eaff768ac",
 :type=>"nonstandard"}
```

エンコードされた redeem script は，:hex=>　の値

## HTLCロックトランザクションの scriptPubKey

P2WSHのscriptPubKeyは，以下の形式になる

```
0 <redeem scriptの SHA256ハッシュ>
```

Bitcoin::Script オブジェクトの to_payload メソッドでバイナリを得ることができるので，そのSHA256ハッシュをとる

```ruby
script_pubkey = Bitcoin::Script.from_string("0 #{Bitcoin.sha256(enc_redeem_script.to_payload)}")

script_pubkey.to_h

=> 
{:asm=>"0 99483e5a4ea9bc743c0782832226e4ada31f54f8f075ed995c7a5d24edb30419",
 :hex=>"002099483e5a4ea9bc743c0782832226e4ada31f54f8f075ed995c7a5d24edb30419",
 :type=>"witness_v0_scripthash",
 :req_sigs=>1,
 :addresses=>["tb1qn9yrukjw4x78g0q8s2pjyfhy4k33748c7p67mx2u0fwjfmdnqsvsmhsv4f"]}

```

## P2WSH アドレスの生成

`script_pubkey.to_h` の addresses　の内容がP2WSHアドレス

```ruby p2wshaddr=script_pubkey.addresses[0]

=> "tb1qn9yrukjw4x78g0q8s2pjyfhy4k33748c7p67mx2u0fwjfmdnqsvsmhsv4f"
```

## AliceがHTLCロックトランザクションを作成

### AliceはBobからBobの公開鍵を得る

```ruby
pubkeyBob
=> "03353ec9fc21ebf6235250c0eace51b5f3d8fc44d7fdfbf1d49c099cbbba48d359"
```

### AliceはCarolから秘密情報 `<S>` の<Sの OP_HASH160 ハッシュ値>　を得る

```ruby
secret_hash
=> "914cb6e51042e1a98def39ecaa52962ebb4aad81"
```

### HTLCロック・トランザクションの作成

使用するAliceのUTXO

* txid: 243f497b5452d0810522cdc53d36b9f1f99bbe4ff07c5d6d12bb5ff1760e8817
* vout: 1
* amount: 0.1000000


output (送金額：0.01,手数料 0.00002)


```ruby
utxo_txid="243f497b5452d0810522cdc53d36b9f1f99bbe4ff07c5d6d12bb5ff1760e8817"
utxo_vout=1
utxo_amount=0.1000000

fee=0.00002
amount=0.01
change=utxo_amount-amount-fee

tx = Bitcoin::Tx.new
tx.in << Bitcoin::TxIn.new(out_point: Bitcoin::OutPoint.from_txid(utxo_txid, utxo_vout))

tx.out << Bitcoin::TxOut.new(value: (amount*100000000).to_i, script_pubkey: Bitcoin::Script.parse_from_addr(p2wshaddr))
tx.out << Bitcoin::TxOut.new(value: (change*100000000).to_i, script_pubkey: Bitcoin::Script.parse_from_addr(addrAlice))
```

```
tx.to_hex

=> "010000000117880e76f15fbb126d5d7cf04fbe9bf9f1b9363dc5cd220581d052547b493f240100000000ffffffff0240420f000000000022002099483e5a4ea9bc743c0782832226e4ada31f54f8f075ed995c7a5d24edb30419704c8900000000001600142a99569a02af8468506de5540f4ff5341bf9e36c00000000"

```



### Alice がトランザクションへのデジタル署名

```bash
bitcoin-core.cli signrawtransactionwithwallet 010000000117880e76f15fbb126d5d7cf04fbe9bf9f1b9363dc5cd220581d052547b493f240100000000ffffffff0240420f000000000022002099483e5a4ea9bc743c0782832226e4ada31f54f8f075ed995c7a5d24edb30419704c8900000000001600142a99569a02af8468506de5540f4ff5341bf9e36c00000000
{
  "hex": "0100000000010117880e76f15fbb126d5d7cf04fbe9bf9f1b9363dc5cd220581d052547b493f240100000000ffffffff0240420f000000000022002099483e5a4ea9bc743c0782832226e4ada31f54f8f075ed995c7a5d24edb30419704c8900000000001600142a99569a02af8468506de5540f4ff5341bf9e36c02473044022025894832af31d2f1f97f6e2be47af4fc01fa47f57b59d12c69e21cd30236a0c4022069fb99d7a881dd49466de7faaffb7a9be2467c3aab8967bcaf67fee0d8aef9a8012102f51aea0586248f9528b96d13fd155d06c394fb6dc5d790568537be68c75eaff700000000",
  "complete": true
}

```

## AliceによるHTLCロックトランザクションのブロードキャスト

```bash
bitcoin-core.cli sendrawtransaction 0100000000010117880e76f15fbb126d5d7cf04fbe9bf9f1b9363dc5cd220581d052547b493f240100000000ffffffff0240420f000000000022002099483e5a4ea9bc743c0782832226e4ada31f54f8f075ed995c7a5d24edb30419704c8900000000001600142a99569a02af8468506de5540f4ff5341bf9e36c02473044022025894832af31d2f1f97f6e2be47af4fc01fa47f57b59d12c69e21cd30236a0c4022069fb99d7a881dd49466de7faaffb7a9be2467c3aab8967bcaf67fee0d8aef9a8012102f51aea0586248f9528b96d13fd155d06c394fb6dc5d790568537be68c75eaff700000000


94c26200cbed99967bf8899245bb9d812f21d54ccedea3d234e6770126712a9c
```

### HTLCロックトランザクションのトランザクションID

94c26200cbed99967bf8899245bb9d812f21d54ccedea3d234e6770126712a9c


===================== 2021/11/22 ===============


# bitcoinrb での実装

## address

addrAlice="tb1qm5gqhe7e46jhyy2ca00x66slmrlljwa3m476sr"
addrBob="tb1qm5gqhe7e46jhyy2ca00x66slmrlljwa3m476sr"


## AliceとBobの鍵

```ruby
require 'bitcoin'

privAlice='cNz434p392FXbmm2ZH2p9eaVDg5YfvANuFM1hHqf8bmRUZ3SSSAp'
privBob='cUpy2Z19AC22MnGLNBfrNMZqrbz7v7rtL9UByzMoxqGC4v9SKtFf'

keyAlice=Bitcoin::Key.new(priv_key: privAlice)
keyBob=Bitcoin::Key.new(priv_key: privBob)
```

## UTXO (P2WSH)

```ruby
txid='27a0ed782436dc23cf9be696e1c4f6b708ed1321ac41c491ec213596cc95105b'
vout=0
amount=0.01000000
value=0.01
fee=0.00002
val_out = ((value-fee)*100000000).to_i
val_in = (value*100000000).to_i
```

## script_pubkey (BobへのP2WPKH)

```ruby
script_pubkey = Bitcoin::Script.parse_from_addr(addrBob)
```

## トランザクションの作成

```ruby
tx = Bitcoin::Tx.new
tx.in << Bitcoin::TxIn.new(out_point: Bitcoin::OutPoint.from_txid(txid, vout))
tx.out << Bitcoin::TxOut.new(value: val_in, script_pubkey: script_pubkey)
```

## 署名対象のsighashを計算


sighash_for_input(input_index, output_script, hash_type: SIGHASH_TYPE[:all], sig_version: :base, amount: nil, skip_separator_index: 0) ⇒ Object

```ruby
prevouts = [Bitcoin::TxOut.new(value: val_out, script_pubkey: script_pubkey)]
sighash = tx.sighash_for_input(0, script_pubkey, hash_type: SIGHASH_TYPE[:all], sig_version: :base,  amount: val_in)
```

## 署名を作成

```ruby
sig = keyBob.sign(sighash)
```

## インプットに署名をセット

```ruby
tx.in[0].script_witness.stack << sig
```

## Txペイロード

```ruby
tx.to_hex
010000000001015b1095cc963521ec91c441ac2113ed08b7f6c4e196e69bcf23dc362478eda0270000000000ffffffff0140420f0000000000160014dd100be7d9aea5721158ebde6d6a1fd8fff93bb10146304402200d1f1d93ee3f7f68119d17becb4b3e2d5ba9dbc98fff743e745c0b478d2d36310220365a354e16f4a7144a29841141e7e26de0fffbf6ea99e7b9aaad4e0299c72d9700000000


bitcoin-core.cli decoderawtransaction 010000000001015b1095cc963521ec91c441ac2113ed08b7f6c4e196e69bcf23dc362478eda0270000000000ffffffff0140420f0000000000160014dd100be7d9aea5721158ebde6d6a1fd8fff93bb10146304402200d1f1d93ee3f7f68119d17becb4b3e2d5ba9dbc98fff743e745c0b478d2d36310220365a354e16f4a7144a29841141e7e26de0fffbf6ea99e7b9aaad4e0299c72d9700000000
{
  "txid": "f96e8e344db62fa88592d5d36bfa7d4bc0bb4895a0f25645e0acb75c263e8849",
  "hash": "81192eb6fc38e971e16cf6ef61dca53351a3a5480d9cf4c4ae1cd1101375f2cb",
  "version": 1,
  "size": 156,
  "vsize": 101,
  "weight": 402,
  "locktime": 0,
  "vin": [
    {
      "txid": "27a0ed782436dc23cf9be696e1c4f6b708ed1321ac41c491ec213596cc95105b",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "txinwitness": [
        "304402200d1f1d93ee3f7f68119d17becb4b3e2d5ba9dbc98fff743e745c0b478d2d36310220365a354e16f4a7144a29841141e7e26de0fffbf6ea99e7b9aaad4e0299c72d97"
      ],
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.01000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 dd100be7d9aea5721158ebde6d6a1fd8fff93bb1",
        "hex": "0014dd100be7d9aea5721158ebde6d6a1fd8fff93bb1",
        "address": "tb1qm5gqhe7e46jhyy2ca00x66slmrlljwa3m476sr",
        "type": "witness_v0_keyhash"
      }
    }
  ]
}
```




================================================================


## Bob によるHTCLアンロックトランザクションの作成

HTLCロックトランザクションがブロックに入った時間経過後，トランザクションIDでHTCLロックトランザクションを検索する

```bash
bitcoin-core.cli getrawtransaction 27a0ed782436dc23cf9be696e1c4f6b708ed1321ac41c491ec213596cc95105b

0200000000010185a4babb90ab2b4d50f3bd828b90ad3d6bb61066e35617eed49168a87e12ae140100000000ffffffff0240420f0000000000220020b643cda12f82884ba4dd8c834a638f11e5afbef415b885808686cbea2fdc5c49403d780000000000160014ea9e8244bd85408c66348a7280571ff420a962000247304402203721bf416d277df5d8c32fc69881227008f2b508856f4fb602719558ccd19ca802201442d87efca83397067b1d698fbce5b89f4f39e26b0bff7d435f6e055fe84f84012103760b5a28bb264ca5b310d766ad3a095c9f866fe9a0b38b8079ca350c6a3e334700000000
```

#### HTLCトランザクションのUTXO

トランザクションID:　27a0ed782436dc23cf9be696e1c4f6b708ed1321ac41c491ec213596cc95105b
vout: 0
 "value": 0.01000000
 
```ruby
tx=Bitcoin::Tx.new
txin=Bitcoin::TxIn.new
txout=Bitcoin::TxOut.new
```
 
### HTLCアンロックトランザクションの input
 
```json
'[{"txid":"27a0ed782436dc23cf9be696e1c4f6b708ed1321ac41c491ec213596cc95105b","vout":0}]' 
```
 
```ruby


tx.
```

###  HTLCアンロックトランザクションのoutput (Bobへの送金)

```json
'[{"tb1q9vml26m9vgm5nk3fk9v7cfkad7tlfgsgnahkfu":0.0098}]'
```

### redeem script（HTLCロックトランザクションの）

```
{:asm=>
  "OP_IF OP_HASH160 d5c91f1deccd50ae0a384b9b1ec44140af96a972ffc1ab8863a1e607b838885a OP_EQUALVERIFY 03d66199f0dd6bbd161cd4a854cd238a4dbebf2d0cf1133180797e1270dac3e528 OP_ELSE 10 OP_CSV OP_DROP 03760b5a28bb264ca5b310d766ad3a095c9f866fe9a0b38b8079ca350c6a3e3347 OP_ENDIF OP_CHECKSIG",
 :hex=>
  "63aa20d5c91f1deccd50ae0a384b9b1ec44140af96a972ffc1ab8863a1e607b838885a882103d66199f0dd6bbd161cd4a854cd238a4dbebf2d0cf1133180797e1270dac3e528675ab2752103760b5a28bb264ca5b310d766ad3a095c9f866fe9a0b38b8079ca350c6a3e334768ac",
 :type=>"nonstandard"}
```

### bitcoinrbで  redeem script をエンコードする

```ruby
require 'bitcoin'

redeem_script = Bitcoin::Script.from_string('OP_IF OP_HASH160 60ef882961ca4e3686bdec1d20173f3fac4b51c059fadf2422c65f1268a17ca1 OP_EQUALVERIFY 03d66199f0dd6bbd161cd4a854cd238a4dbebf2d0cf1133180797e1270dac3e528 OP_ELSE 10 OP_CSV OP_DROP 03760b5a28bb264ca5b310d766ad3a095c9f866fe9a0b38b8079ca350c6a3e3347 OP_ENDIF OP_CHECKSIG')

redeem_script.to_h
=> 
{:asm=>
  "OP_IF OP_HASH160 60ef882961ca4e3686bdec1d20173f3fac4b51c059fadf2422c65f1268a17ca1 OP_EQUALVERIFY 03d66199f0dd6bbd161cd4a854cd238a4dbebf2d0cf1133180797e1270dac3e528 OP_ELSE 10 OP_CSV OP_DROP 03760b5a28bb264ca5b310d766ad3a095c9f866fe9a0b38b8079ca350c6a3e3347 OP_ENDIF OP_CHECKSIG",
 :hex=>
  "63aa2060ef882961ca4e3686bdec1d20173f3fac4b51c059fadf2422c65f1268a17ca1882103d66199f0dd6bbd161cd4a854cd238a4dbebf2d0cf1133180797e1270dac3e528675ab2752103760b5a28bb264ca5b310d766ad3a095c9f866fe9a0b38b8079ca350c6a3e334768ac",
 :type=>"nonstandard"}
```

エンコードされた redeem script は，:hex=>　の値

### sigHash  (BIP-143の仕様でSIGHASHを生成)

```Ruby
tx=Bitcoin::Tx.new

sighash_for_input(
```

### scriptSig

空

### scriptPubKey (HTLCロックトランザクションの）

```json
scriptPubKey": {
        "asm": "0 b643cda12f82884ba4dd8c834a638f11e5afbef415b885808686cbea2fdc5c49",
        "hex": "0020b643cda12f82884ba4dd8c834a638f11e5afbef415b885808686cbea2fdc5c49",
        "address": "tb1qkepumgf0s2yyhfxa3jp55cu0z8j6l0h5zkugtqyxsm975t7ut3ysymx70l",
        "type": "witness_v0_scripthash"
      }
```

### witness

```
```


### 未署名のトランザクションの作成

```bash
bitcoin-core.cli createrawtransaction  '[{"txid":"27a0ed782436dc23cf9be696e1c4f6b708ed1321ac41c491ec213596cc95105b","vout":0}]'  '[{"tb1q9vml26m9vgm5nk3fk9v7cfkad7tlfgsgnahkfu":0.0098}]'
￼
02000000015b1095cc963521ec91c441ac2113ed08b7f6c4e196e69bcf23dc362478eda0270000000000ffffffff0120f40e00000000001600142b37f56b65623749da29b159ec26dd6f97f4a20800000000
```


### 秘密鍵

```bash
dumpprivkey tb1q9vml26m9vgm5nk3fk9v7cfkad7tlfgsgnahkfu
￼
cUpy2Z19AC22MnGLNBfrNMZqrbz7v7rtL9UByzMoxqGC4v9SKtFf
```

```
signrawtransactionwithkey 02000000015b1095cc963521ec91c441ac2113ed08b7f6c4e196e69bcf23dc362478eda0270000000000ffffffff0120f40e00000000001600142b37f56b65623749da29b159ec26dd6f97f4a20800000000 '["cUpy2Z19AC22MnGLNBfrNMZqrbz7v7rtL9UByzMoxqGC4v9SKtFf"]'

```



確認

```bash
bitcoin-core.cli decoderawtransaction 02000000015b1095cc963521ec91c441ac2113ed08b7f6c4e196e69bcf23dc362478eda0270000000000ffffffff0120f40e00000000001600142b37f56b65623749da29b159ec26dd6f97f4a20800000000

{
  "txid": "1d24bf63ae5663947f24a6f6951f7c41c7fccd77ca65733f328dd9209d520a95",
  "hash": "1d24bf63ae5663947f24a6f6951f7c41c7fccd77ca65733f328dd9209d520a95",
  "version": 2,
  "size": 82,
  "vsize": 82,
  "weight": 328,
  "locktime": 0,
  "vin": [
    {
      "txid": "27a0ed782436dc23cf9be696e1c4f6b708ed1321ac41c491ec213596cc95105b",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "sequence": 4294967295
    }
  ],
  "vout": [
    {
      "value": 0.00980000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 2b37f56b65623749da29b159ec26dd6f97f4a208",
        "hex": "00142b37f56b65623749da29b159ec26dd6f97f4a208",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1q9vml26m9vgm5nk3fk9v7cfkad7tlfgsgnahkfu"
        ]
      }
    }
  ]
}

```


### bitcoin core API signrawtransactionwithwallet の仕様

```
Argument #1 - hexstring
Type: string, required

The transaction hex string
```

```
Argument #2 - 直前のトランザクション情報
Type: json array, optional

The previous dependent transaction outputs

[
  {                            (json object)
    "txid": "hex",             (string, required) The transaction id
    "vout": n,                 (numeric, required) The output number
    "scriptPubKey": "hex",     (string, required) script key
    "redeemScript": "hex",     (string) (required for P2SH) redeem script
    "witnessScript": "hex",    (string) (required for P2WSH or P2SH-P2WSH) witness script
    "amount": amount,          (numeric or string) (required for Segwit inputs) the amount spent
  },
  ...
]
```

```
Argument #3 - sighashtype
Type: string, optional, default=ALL

The signature hash type. Must be one of
“ALL” “NONE” “SINGLE” “ALL|ANYONECANPAY” “NONE|ANYONECANPAY” “SINGLE|ANYONECANPAY”
```

### Carol  からの秘密情報 `<S>` のハッシュ値を得て，HTLCのredeem script を作成する（Aliceと同じものになる）

63aa20d5c91f1deccd50ae0a384b9b1ec44140af96a972ffc1ab8863a1e607b838885a882103d66199f0dd6bbd161cd4a854cd238a4dbebf2d0cf1133180797e1270dac3e528675ab2752103760b5a28bb264ca5b310d766ad3a095c9f866fe9a0b38b8079ca350c6a3e334768ac

### Carol  からの秘密情報 `<S>` を得る

` <S>`：babe0000beef0000cafe 

### P2WSHアドレス生成のための decodescriptの結果をもう一度確認する

```json
 bitcoin-core.cli decodescript 63aa20d5c91f1deccd50ae0a384b9b1ec44140af96a972ffc1ab8863a1e607b838885a882103d66199f0dd6bbd161cd4a854cd238a4dbebf2d0cf1133180797e1270dac3e528675ab2752103760b5a28bb264ca5b310d766ad3a095c9f866fe9a0b38b8079ca350c6a3e334768ac
{
  "asm": "OP_IF OP_HASH160 d5c91f1deccd50ae0a384b9b1ec44140af96a972ffc1ab8863a1e607b838885a OP_EQUALVERIFY 03d66199f0dd6bbd161cd4a854cd238a4dbebf2d0cf1133180797e1270dac3e528 OP_ELSE 10 OP_CHECKSEQUENCEVERIFY OP_DROP 03760b5a28bb264ca5b310d766ad3a095c9f866fe9a0b38b8079ca350c6a3e3347 OP_ENDIF OP_CHECKSIG",
  "type": "nonstandard",
  "p2sh": "2N6JDe6QMzk2eMMrFrDvzbpbL2z5nU9vsPy",
  "segwit": {
    "asm": "0 b643cda12f82884ba4dd8c834a638f11e5afbef415b885808686cbea2fdc5c49",
    "hex": "0020b643cda12f82884ba4dd8c834a638f11e5afbef415b885808686cbea2fdc5c49",
    "address": "tb1qkepumgf0s2yyhfxa3jp55cu0z8j6l0h5zkugtqyxsm975t7ut3ysymx70l",
    "type": "witness_v0_scripthash",
    "p2sh-segwit": "2N3jAsJhZXBheumeUWric3RjziGND292BnC"
  }
}

```

### unlocking script　(witness)

```ruby
require 'bitcoin'

script=Bitcoin::Script.from_string("babe0000beef0000cafe OP_1")

script.to_h
=> {:asm=>"babe0000beef0000cafe 1", :hex=>"0ababe0000beef0000cafe51", :type=>"nonstandard"}
```

### HTLC アンロックトランザクションへの署名

第２引数のJSON形式

```json
'[{"txid": "27a0ed782436dc23cf9be696e1c4f6b708ed1321ac41c491ec213596cc95105b",
    "vout": 0,
    "scriptPubKey": "",
    "redeemScript": "63aa20d5c91f1deccd50ae0a384b9b1ec44140af96a972ffc1ab8863a1e607b838885a882103d66199f0dd6bbd161cd4a854cd238a4dbebf2d0cf1133180797e1270dac3e528675ab2752103760b5a28bb264ca5b310d766ad3a095c9f866fe9a0b38b8079ca350c6a3e334768ac",
    "witnessScript": "0020b643cda12f82884ba4dd8c834a638f11e5afbef415b885808686cbea2fdc5c49",
    "amount": 0.001
  }]'
```

```bash
bitcoin-core.cli signrawtransactionwithwallet 02000000015b1095cc963521ec91c441ac2113ed08b7f6c4e196e69bcf23dc362478eda0270000000000ffffffff0120f40e00000000001600142b37f56b65623749da29b159ec26dd6f97f4a20800000000 

```





## P2WSH トランザクション

|フィールド	|内容|サイズ|
| ---- | ---- | ---- |
| バージョン| int32| 4|
|marker| 0x00| 1|
| flag| 0x01| 1|
|inputの数|1|1+|
|inputリスト|TxIns|41+|
|outputの数|1|1+|
|outputリスト||9+|
|witness|witness structure|0+|
|nLocktime|int32|4|


### TxIn

|フィールド	|内容|サイズ|
| ---- | ---- | ---- |
| previous tranzaction hash| char[32]|32 |
|txout index| int32|4|
|scriptSig bytes|無いときは 0x00|1+|
|scriptSig||?|
|nSequence|0xffffffff |4|

### TxOut

|フィールド	|内容|サイズ|
| ---- | ---- | ---- |
|value|int64|8|
|scriptPubKey bytes||1+|
|scriptPubKey||?|

### P2WSH トランザクションの例（マルチシグ）

* 署名済トランザクション

```
0200000000010183472c297bdbe8e36106657a61303d54e6b6166ea40b3c2dcae1063ec42a35b20400000000ffffffff0160361e00000000002200203cd019a6a8a88b4325acf2b7ebf578393a99f4ed060236d030c7b99ac31ee0870247304402200b2689214e8ae74af9db450069e80ea5136fb7dc9cd06399a546c45f3dcaa1ad022032250fea21e9f46eed4541a81ab260c3085e91798b2dff2b491403bd2bd624a8012103d40d729a618dfd1fd41c633e08eada6d245d9234cf345efb0ab67baf820860b000000000
```

* redeem script

```
52210346711e7845d77b5dba283743228f5c6162e626445ae694fbd9962c60137759582103d798a0fc210729dab75473393296ed1c9ed8ec4ed85f97bb3273c9cb7b8144762103e0e27add506965861763916b8daa3744d6136b2e4b6a1aeb9c4274deeb48e59553ae
```

* redeem script hash (OP_SHA256)

```
c19f74e59e40783b87cfcba6abb03eb94fbdc960bb91686b25a24c2e1b4bfea0
```

* redeem script をdecode してみる

```bash
 bitcoin-core.cli decodescript 52210346711e7845d77b5dba283743228f5c6162e626445ae694fbd9962c60137759582103d798a0fc210729dab75473393296ed1c9ed8ec4ed85f97bb3273c9cb7b8144762103e0e27add506965861763916b8daa3744d6136b2e4b6a1aeb9c4274deeb48e59553ae
 
{
  "asm": "2 0346711e7845d77b5dba283743228f5c6162e626445ae694fbd9962c6013775958 03d798a0fc210729dab75473393296ed1c9ed8ec4ed85f97bb3273c9cb7b814476 03e0e27add506965861763916b8daa3744d6136b2e4b6a1aeb9c4274deeb48e595 3 OP_CHECKMULTISIG",
  "type": "multisig",
  "p2sh": "2MuhiFJwtMYZHXjyvLXunQAHgqcYUEQZH5m",
  "segwit": {
    "asm": "0 3cd019a6a8a88b4325acf2b7ebf578393a99f4ed060236d030c7b99ac31ee087",
    "hex": "00203cd019a6a8a88b4325acf2b7ebf578393a99f4ed060236d030c7b99ac31ee087",
    "address": "tb1q8ngpnf4g4z95xfdv72m7hatc8yafna8dqcprd5psc7ue4sc7uzrs4avaje",
    "type": "witness_v0_scripthash",
    "p2sh-segwit": "2Muc15m2GwwZEq7vAw3vVLXUFoBdnNHQamj"
  }
}
```

-------
分解して見る

```
02000000 (バージョン）
00（marker）
01（flag）
01（inputの数）

# TxIn

83472c297bdbe8e36106657a61303d54e6b6166ea40b3c2dcae1063ec42a35b2
（previous tranzaction hash　リトルエンディアンに変換）

04000000
（txout index　リトルエンディアンに変換）

00　(scriptSig bytes)

ffffffff (nSequence)


01 (outputの数)

# TxOut

60361e0000000000 
(value リトルエンディアン，16進数）

22 (scriptPubKey bytes)

0020 3
cd019a6a8a88b4325acf2b7ebf578393a99f4ed060236d030c7b99ac31ee087　
(scriptPubKey)

# witness

0247304402200b2689214e8ae74af9db450069e80ea5136fb7dc9cd06399a546c45f3dcaa1ad022032250fea21e9f46eed4541a81ab260c3085e91798b2dff2b491403bd2bd624a8012103d40d729a618dfd1fd41c633e08eada6d245d9234cf345efb0ab67baf820860b0

# nLocktime
00000000
```

```bash
bitcoin-core.cli decodescript 0247304402200b2689214e8ae74af9db450069e80ea5136fb7dc9cd06399a546c45f3dcaa1ad022032250fea21e9f46eed4541a81ab260c3085e91798b2dff2b491403bd2bd624a8012103d40d729a618dfd1fd41c633e08eada6d245d9234cf345efb0ab67baf820860b0


{
  "asm": "12359 02200b2689214e8ae74af9db450069e80ea5136fb7dc9cd06399a546c45f3dcaa1ad022032250fea21e9f46eed4541a81ab260c3085e91798b2dff2b491403bd2bd624a8 33 7474644 OP_BOOLAND OP_NOP OP_2MUL OP_UNKNOWN [error]",
  "type": "nonstandard",
  "p2sh": "2Mva4MuBsR9bzjgfb3mRdUGw9eurQiuUuVa",
  "segwit": {
    "asm": "0 173e3356696063e693be048d6d8f0ae8fb9abe4832aa0fcb2f809b30c3642ca3",
    "hex": "0020173e3356696063e693be048d6d8f0ae8fb9abe4832aa0fcb2f809b30c3642ca3",
    "address": "tb1qzulrx4nfvp37dya7qjxkmrc2arae40jgx24qlje0szdnpsmy9j3sxsgtyp",
    "type": "witness_v0_scripthash",
    "p2sh-segwit": "2MvLJN1TYbUBuDuc2y8ripz5JZd5bMfvxmt"
  }
}

```

### P2WSH のUTXO を使用するトランザクション

```json
{
    "txid": "95ccd50a75ab3bb3767df0b7669f3472965f8d1598feab7cb424f1862857ca8f",
    "vout": 0,
    "address": "tb1q8ngpnf4g4z95xfdv72m7hatc8yafna8dqcprd5psc7ue4sc7uzrs4avaje",
    "label": "",
    "witnessScript": "52210346711e7845d77b5dba283743228f5c6162e626445ae694fbd9962c60137759582103d798a0fc210729dab75473393296ed1c9ed8ec4ed85f97bb3273c9cb7b8144762103e0e27add506965861763916b8daa3744d6136b2e4b6a1aeb9c4274deeb48e59553ae",
    "scriptPubKey": "00203cd019a6a8a88b4325acf2b7ebf578393a99f4ed060236d030c7b99ac31ee087",
    "amount": 0.01980000,
    "confirmations": 476,
    "spendable": false,
    "solvable": true,
    "desc": "wsh(multi(2,[036ce8f5]0346711e7845d77b5dba283743228f5c6162e626445ae694fbd9962c6013775958,[1ac1df2e]03d798a0fc210729dab75473393296ed1c9ed8ec4ed85f97bb3273c9cb7b814476,[3f0ddcf2]03e0e27add506965861763916b8daa3744d6136b2e4b6a1aeb9c4274deeb48e595))#7c5ke0ec",
    "safe": true
```

#### witnessScriptをデコードしてみる

```bash
￼bitcoin-core.cli decodescript 52210346711e7845d77b5dba283743228f5c6162e626445ae694fbd9962c60137759582103d798a0fc210729dab75473393296ed1c9ed8ec4ed85f97bb3273c9cb7b8144762103e0e27add506965861763916b8daa3744d6136b2e4b6a1aeb9c4274deeb48e59553ae


{
  "asm": "2 0346711e7845d77b5dba283743228f5c6162e626445ae694fbd9962c6013775958 03d798a0fc210729dab75473393296ed1c9ed8ec4ed85f97bb3273c9cb7b814476 03e0e27add506965861763916b8daa3744d6136b2e4b6a1aeb9c4274deeb48e595 3 OP_CHECKMULTISIG",
  "reqSigs": 2,
  "type": "multisig",
  "addresses": [
    "mfq4nq3W3Gr4GBP2cxnKjgLif5T3k5X93C",
    "mhxS7VPtgkNYFT88wXifRB4YMUTXSZRwSQ",
    "mmGMUN1VpYnEP3HKkMBhgJL5DH5gxyZvog"
  ],
  "p2sh": "2MuhiFJwtMYZHXjyvLXunQAHgqcYUEQZH5m",
  "segwit": {
    "asm": "0 3cd019a6a8a88b4325acf2b7ebf578393a99f4ed060236d030c7b99ac31ee087",
    "hex": "00203cd019a6a8a88b4325acf2b7ebf578393a99f4ed060236d030c7b99ac31ee087",
    "reqSigs": 1,
    "type": "witness_v0_scripthash",
    "addresses": [
      "tb1q8ngpnf4g4z95xfdv72m7hatc8yafna8dqcprd5psc7ue4sc7uzrs4avaje"
    ],
    "p2sh-segwit": "2Muc15m2GwwZEq7vAw3vVLXUFoBdnNHQamj"
  }
}

```