# SegWit (Segregated Witness)
2021/11/19


## トランザクションの構造と署名対象

|フィールド名 | サイズ| 内容|
| :---: | :---: | :---: |
|nVersion|4|リトルエンディアン|
|marker|1||
|flag|1||
|txin_count|1+||
|txins|41+|outpoint <br> (script_sig) <br> sequence|
|txout_count|1+||
|txouts|9+|script_pubkey <br> value|
|witnesses|1+||
|lock_time|4||

* witnesses の数は，txin_count と同じ
* witnessの順序はtxinの順序と同じ

## script_pubkey の witness program

* script_pubkey が次のパターンのとき，SegWitを判断する
* 現在のバージョンは0 なので，先頭にバージョンを意味する 0 がプッシュされる
* バージョンの後にプッシュされるデータを witness program と呼ぶ

### P2WPKHの場合

```
0 <20バイトのデータ>
```

この20バイトは，公開鍵のHASH160の値

### P2WSHの場合

```
0 <32バイトのデータ>
```

この32バイトは，redeem script のSHA256ハッシュ

## P2WPKHのUTXOを使用する場合

* script_sigは空
* txinwitness

```
<署名>, <公開鍵>
```

## P2WSHのUTXOを使用する場合

* script_sigは空
* txinwitness

```
<redeem scriptのアンロック条件>, <redeem script>
```

## SegWitトランザクションへの署名

トランザクションへの署名方法

1. トランザクションデータのシリアライズ
2. シリアライズデータへの SHA256ダブルハッシュ (SignatureHash)
3. SignatureHash への署名



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



