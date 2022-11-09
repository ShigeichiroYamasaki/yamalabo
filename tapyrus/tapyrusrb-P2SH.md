# tapyrusrbでHTLC

最終更新 2022/09/04 Shigeichiro Yamasaki

tapyrusrb を使ってP2SH の送金と受領を行うトランザクションを作成する

スクリプトとしては、HTLCを使う


### HTLCのunlocking script

```
        [HTLC の scriptSig]
<Bobの署名> 
<Secret> 
OP_TRUE
------------連接--------------
       [HTLCの redeem script] 
OP_IF
    OP_SHA256 <Secretのハッシュ値> OP_EQUALVERIFY 
    <Bobの公開鍵>
OP_ELSE
    <ロックするブロック数> OP_CSV 
    OP_DROP  
    <Aliceの公開鍵>
OP_ENDIF
OP_CHECKSIG
```

### Tapyrus RPC

```ruby
require 'tapyrus'
include Tapyrus::Opcodes
Tapyrus.chain_params = :prod

Tapyrus_cli ='~/tapyrus-core-0.5.1/bin/tapyrus-cli'
# RPC
def tapyrusRPC(method,params)
    r=`#{Tapyrus_cli} #{method} #{params.join(' ')}`.chomp
    begin
        return JSON.parse(r)
    rescue JSON::ParserError
        return r
    end
end
```

### Alice

```ruby
# Aliceのアドレス
addrAliceTP = tapyrusRPC("getnewaddress", [])
# Aliceの秘密鍵
privAliceTP = tapyrusRPC("dumpprivkey", [addrAliceTP])
# Aliceの鍵オブジェクト(WIF形式の秘密鍵から生成）
keyAliceTP=Tapyrus::Key.from_wif(privAliceTP)
# Aliceの公開鍵
pubkeyAliceTP= keyAliceTP.pubkey
```

### Bob

```ruby
# Bobのアドレス
addrBobTP = tapyrusRPC("getnewaddress", [])
# Bobの秘密鍵
privBobTP = tapyrusRPC("dumpprivkey", [addrBobTP])
# Bobの鍵オブジェクト(WIF形式の秘密鍵から生成）
keyBobTP=Tapyrus::Key.from_wif(privBobTP)
# Bobの公開鍵
pubkeyBobTP= keyBobTP.pubkey
```


### 秘密情報

`<Secret>` ：秘密情報
`<Secretのハッシュ値> `: 秘密情報のハッシュ値

この説明では　"HTLC_test"　とします。

#### Bob

```ruby
secret='HTLC_test'
secret_hash=Tapyrus.sha256(secret)
```

#### script処理のテスト(OP_SHA256の検証)

★注意！　secret.bth　のように .bth メソッドで 16進数化しないといけません

```ruby
# scriptのテスト
ts=Tapyrus::Script.new << secret.bth << OP_SHA256 << secret_hash << OP_EQUAL
ts.run
# => true
# 失敗するケース
ts2=Tapyrus::Script.new << secret << OP_SHA256 << secret_hash << OP_EQUAL
ts2.run
# => false
```

### AliceとBobの情報交換

BobからAliceへ

#### Bob
```ruby
# Bobの公開鍵 => Alice
puts "pubkeyBobTP='#{pubkeyBobTP}'"
# 秘密情報のハッシュ値=> Bob
puts "secret_hash ='#{secret_hash.bth}'"
```

Alice からBobへ

#### Alice

```ruby
# Aliceの公開鍵 => Bob
puts "pubkeyAliceTP ='#{pubkeyAliceTP}'"
```

### HTLC のredeem script(Alice)

Alice => Bob の場合

```
OP_IF OP_SHA256 <Secretのハッシュ値> OP_EQUALVERIFY <Bobの公開鍵> OP_ELSE <locktime> OP_CSV OP_DROP <Aliceの公開鍵> OP_ENDIF OP_CHECKSIG
```

### HTLC Lock トランザクション構築と送金メソッド

```ruby
# secret_hash 秘密情報のハッシュ値
# pubkeySND 送金者の公開鍵
# addrSND 送金者のアドレス
# pubkeyRSV 受領者の公開鍵
# lockDays　ロックする日数
def send_HTLC_lock_txTP(secret_hash, pubkeySND, addrSND, pubkeyRSV, deposit, fee, lockDays)
    # 未署名のHTLC Lock トランザクションの生成
    unsigned_HTCL_lock_tx, redeem_script, p2shaddr  = unsigned_HTCL_lock_transactionTP(secret_hash, pubkeySND, addrSND, pubkeyRSV, deposit, fee, lockDays)
    # HTLC Lock トランザクションへの署名
    signed_HTCL_lock_txTP = tapyrusRPC('signrawtransactionwithwallet',[unsigned_HTCL_lock_tx.to_hex])
    # HTLC Lock トランザクションのブロードキャスト
    htlc_lock_txidTP = tapyrusRPC('sendrawtransaction',[signed_HTCL_lock_txTP["hex"]])
    return [htlc_lock_txidTP, redeem_script, p2shaddr]
end
# HTLC redeem script作成メソッド
def redeem_script_HTLC_TP(pubkeySND, pubkeyRSV, secret_hash, lockDays)
    # <ロックするブロック数> 10日間のブロック数（リトルエンディアン）
    locktime = (6*24*lockDays).to_bn.to_s(2).reverse.bth
    # redeem script
    redeem_script = Tapyrus::Script.new << OP_IF << OP_SHA256 << secret_hash << OP_EQUALVERIFY << pubkeyRSV << OP_ELSE << locktime << OP_CSV << OP_DROP << pubkeySND << OP_ENDIF << OP_CHECKSIG
    # redeem scriptの内容の確認
    return redeem_script
end
# 送金金額＋手数料をぎりぎり上回るUTXOリストの作成とその総額
def consuming_utxosTP(amount)
    # ワレットの未使用のUTXOの一覧を得る
    unspent = tapyrusRPC('listunspent', [])
    # 消費可能状態のUTXOの選定
    spendable_utxos = unspent.select{|t|t["spendable"]}
    # UTXOを金額で昇順にソートする
    sorted_utxos = spendable_utxos.sort_by{|x|x["amount"]}
    # 少額のUTXOから集めて，指定金額を上回るぎりぎりのUTXOのリストを作成する
    utxos=[]
    balance = 0
    begin
        utxos << sorted_utxos.shift
        balance = utxos.reduce(0){|s,t|s+=t["amount"]}
    end until balance >= amount
    return utxos
end
#未署名のHTLC Lock トランザクションの生成
def unsigned_HTCL_lock_transactionTP(secret_hash, pubkeySND, addrSND, pubkeyRSV, deposit, fee, lockDays)
    if tapyrusRPC('getbalance',[]) < (deposit+fee) then
        puts "you don't have enough funds for deposit"
    else
        # 送金金額＋手数料をぎりぎり上回るUTXOリストの作成とその総額
        utxos = consuming_utxosTP(deposit + fee)
        balance = utxos.map {|tx|tx[ "amount"]}.sum
        # HTLC のredeem scriptの生成
        redeem_script = redeem_script_HTLC_TP(pubkeySND, pubkeyRSV, secret_hash, lockDays)
        # HTLCロックトランザクションの scriptPubKey
        scriptPubKey_p2sh = redeem_script.to_p2sh
        # P2SH アドレスの生成
        p2shaddr = scriptPubKey_p2sh.to_addr
        # お釣り
        change= balance-deposit-fee
        # それぞれの金額をSatoshiに変換
        deposit_satoshi = (deposit *  (10**8)).to_i
        change_satoshi = (change *  (10**8)).to_i
        # トランザクションテンプレートの生成
        tx = Tapyrus::Tx.new
        # inputの作成
        tx = make_inputsTP(tx, utxos)
        # デポジット用　P2SH outputの作成
        tx.out << Tapyrus::TxOut.new(value: deposit_satoshi, script_pubkey:  Tapyrus::Script.parse_from_addr(p2shaddr))
        # おつり用のP2PKH outputの作成
        tx.out << Tapyrus::TxOut.new(value: change_satoshi , script_pubkey:  Tapyrus::Script.parse_from_addr(addrSND))
        return [tx, redeem_script.to_h[:asm], p2shaddr]
    end
end
# トランザクションのinputの構成
def make_inputsTP(tx, utxos)
    utxos.each{|utxo|
        # UTXOをinputから参照するための txid と vout としてエンコードする
        outpoint = Tapyrus::OutPoint.from_txid(utxo["txid"], utxo["vout"])
        # エンコードした参照をトランザクションのinputに埋め込む
        tx.in << Tapyrus::TxIn.new(out_point: outpoint)
    }
    return tx
end
```
 
HTLCトランザクションの構築と送金の実行
 
```ruby
htlc_lock_txidTP, redeem_scriptTP, p2shaddrTP = send_HTLC_lock_txTP(secret, pubkeyAliceTP, addrAliceTP, pubkeyBobTP, 0.0003, 0.00002, 10)

# HTLCロックトランザクションのトランザクションID
htlc_lock_txidTP
=> "9990ac521b697b520eb8d452d0677a385836394eb5acc637958a447c75088431"

# redeem スクリプト
redeem_scriptTP
=> "OP_IF OP_SHA256 1d5cfdecd0 OP_EQUALVERIFY 028888d55bd3ad9cee9367b05af1603f34d3350163283374ae13d6c5a05ff13155 OP_ELSE 1440 OP_CSV OP_DROP 023ba7c325ab141a2575d8e18130e94ded9057dd2dc4c0e4acc4423451cf52f6c8 OP_ENDIF OP_CHECKSIG"

# P2SHアドレス
p2shaddrTP
=> "3HZ45GVSncJFU8zHQLJxJUCWFpXS3Wk73L"
```

## HTLC アンロックトランザクションの構成


### アンロックのためにBobが知っている（べき）情報

* secret   : 秘密情報 <Secret> (Aliceからもらう）
* redeem_scriptTP  : redeem script (Aliceからもらう）
* `htlc_lock_txidTP`  : HTLCロックトランザクションの トランザクションID (Aliceからもらう）
* fee=0.00002

### HTLC unlock トランザクション生成メソッド

```ruby
def htlc_unlockTP(htlc_lock_txidTP, secret, redeem_scriptTP , addrRSV, keyRSV, fee)
    # HTLC ロックトランザクション本体を得る
    htlc_lock_txTP = Tapyrus::Tx.parse_from_payload(tapyrusRPC('getrawtransaction',[htlc_lock_txidTP]).htb)
    deposit_satoshi = htlc_lock_txTP.outputs[0].value
    # アンロックによる報酬金額
    reward_satoshi = deposit_satoshi - (fee * (10**8)).to_i
    tx = Tapyrus::Tx.new
    # inputの作成
    htlc_utxo = Tapyrus::OutPoint.from_txid(htlc_lock_txidTP,0)
    tx.in << Tapyrus::TxIn.new(out_point: htlc_utxo)
    # 報酬用のP2PKH outputの作成
    script_pubkey_reward = Tapyrus::Script.parse_from_addr(addrRSV)
    tx.out << Tapyrus::TxOut.new(value: reward_satoshi, script_pubkey: script_pubkey_reward)
    # redeem_scriptの復元
    redeem_script = Tapyrus::Script.from_string(redeem_scriptTP)
    # アンロック対象UTXOのscriptPubKey
    scriptPubKey = htlc_lock_txTP.outputs[0].script_pubkey
    # 署名対象のsighash
    sighash = tx.sighash_for_input(0, scriptPubKey)
    # Bobの秘密鍵で署名
    signature = keyRSV.sign(sighash) + [Tapyrus::SIGHASH_TYPE[:all]].pack('C')
    tx.in[0].script_sig << signature
    tx.in[0].script_sig << secret.htb
    tx.in[0].script_sig << Tapyrus::Opcodes::OP_1
    tx.in[0].script_sig << redeem_script
    # スクリプトデバッグ用
    scriptSig=Tapyrus::Script.new
    scriptSig << signature 
    scriptSig << secret.htb 
    scriptSig << Tapyrus::Opcodes::OP_1 
    scriptSig << redeem_script
    return [tx, scriptSig, scriptPubKey, redeem_script,signature]
end
# 実行
tx, scriptSig, scriptPubKey, redeem_script,signature = htlc_unlockTP(htlc_lock_txidTP, secret, redeem_scriptTP , addrBobTP, keyBobTP, 0.00002)
```

### スクリプトのチェック

```ruby

script_pubkey = Tapyrus::Script.to_p2sh(htlc_lock_txidTP)



tx_checker = Tapyrus::TxChecker.new(tx: tx, input_index: 0, amount: 0.0003)
interpreter = Tapyrus::ScriptInterpreter.new(flags: Tapyrus::STANDARD_SCRIPT_VERIFY_FLAGS, checker: tx_checker)
interpreter.verify_script(script_sig, script_pubkey)


ts.run
=> true
```

### ブロードキャスト

```ruby
txid = tapyrusRPC('sendrawtransaction', [tx.to_hex])

error code: -26
error message:                                                      
bad-txns-nonstandard-inputs (code 64)                               
=> ""           
```
