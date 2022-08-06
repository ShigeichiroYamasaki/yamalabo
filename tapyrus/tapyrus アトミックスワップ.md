# Tapyrus と Bitcoin の間でアトミックスワップ

最終更新 2022/08/05 Shigeichiro Yamasaki

* Tapyrus側のトークンは、TPCとします
* ストーリーは、Alice が所持している （デポジットしている）TPCを Bobが bitcoin で購入するというものです。
* 秘密情報は Bob (bitcoinでTPCを購入する人）が生成し、そのハッシュ値をAliceに伝えます

--

1. Bobは、そのハッシュ値で bitcoin をロックした HTLCロックトランザクションを作成、ブロードキャストします。（bitcoinのがロック状態になります）
2. Aliceも、そのハッシュ値で TPC をロックした HTLCロックトランザクションを作成し、ブロードキャストします。（TPCがロック状態になります）
3. Bobは、ロック状態のTPCをアンロックするために、秘密情報（ハッシュ値の原像）を埋め込んだ HTLCアンロックトランザクションを作成し、ブロードキャストします。これによってTPCを入手します。
4. Aliceは、Bobによって TapyrusネットワークにブロードキャストされたHTLCアンロックトランザクションから、秘密情報（ハッシュ値の原像）を入手します。
5. Aliceは、秘密情報（ハッシュ値の原像）を使って bitcoin の HTLCアンロックトランザクションを作成し、ブロードキャストします。これによって bitcoin を入手します。

![](./images/tapyrus-swap01.png)

### 主体ごとに4つのターミナルで作業

AliceもBobも、Tapyrus と bitcoin　の双方に主体を持つ必要があります。
それぞれ次のように名付けます

| | Tapyrus | bitcoin|
| :--|:--|:--|
|Alice| AliceTP|AliceBC|
|Bob|  BobTP|BobBC|


それぞれ別マシンで実行することが望ましい。
同じマシンでも別ターミナルで実施するので、４つのターミナルが必要


## 1.1 AliceTP の準備(Tapyrus)

AliceTP用ターミナルで実行

```ruby
require 'tapyrus'
require 'json'
include Tapyrus
include Tapyrus::Opcodes

Tapyrus.chain_params = :prod

# tapyrus-cli コマンドのフルパス
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

# Aliceのアドレス生成
addrAliceTP = tapyrusRPC("getnewaddress", [])

# Aliceの秘密鍵
privAliceTP = tapyrusRPC("dumpprivkey", [addrAliceTP])

# Aliceの鍵オブジェクト(WIF形式の秘密鍵から生成）
keyAliceTP=Tapyrus::Key.from_wif(privAliceTP)

# Aliceの公開鍵
pubkeyAliceTP = keyAliceTP.pubkey

# Aliceに送金してUTXOを用意する
tapyrusRPC('sendtoaddress',[addrAliceTP, 0.0003])
tapyrusRPC('sendtoaddress',[addrAliceTP, 0.0004])
tapyrusRPC('sendtoaddress',[addrAliceTP, 0.0005])
tapyrusRPC('sendtoaddress',[addrAliceTP, 0.0006])
tapyrusRPC('sendtoaddress',[addrAliceTP, 0.0007])

# 10分後（マイニングされるのを待つ）
```

## 1.2 BobTP の準備(Tapyrus)

AliceTP用ターミナルで実行

```ruby
require 'tapyrus'
require 'json'
include Tapyrus
include Tapyrus::Opcodes

Tapyrus.chain_params = :prod

# tapyrus-cli コマンドのフルパス
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

# Bobのアドレス生成
addrBobTP = tapyrusRPC("getnewaddress", [])

# Bobの秘密鍵
privBobTP = tapyrusRPC("dumpprivkey", [addrBobTP])

# Bobの鍵オブジェクト(WIF形式の秘密鍵から生成）
keyBobTP=Tapyrus::Key.from_wif(privBobTP)

# Bobの公開鍵
pubkeyBobTP = keyBobTP.pubkey

# Bobに送金してUTXOを用意する
tapyrusRPC('sendtoaddress',[addrBobTP, 0.0003])
tapyrusRPC('sendtoaddress',[addrBobTP, 0.0004])
tapyrusRPC('sendtoaddress',[addrBobTP, 0.0005])
tapyrusRPC('sendtoaddress',[addrBobTP, 0.0006])
tapyrusRPC('sendtoaddress',[addrBobTP, 0.0007])

# 10分後（マイニングされるのを待つ）
```

## 2.1 AliceBCの準備(bitcoin)

Bob用ターミナルで実行

```ruby
require 'bitcoin'
require 'net/http'
require 'json'
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

# Bobのアドレス生成
addrAliceBC = bitcoinRPC("getnewaddress", [])

# Bobの秘密鍵
privAliceBC = bitcoinRPC("dumpprivkey", [addrAliceBC])

# Bobの鍵オブジェクト(WIF形式の秘密鍵から生成）
keyAliceBC = Bitcoin::Key.from_wif(privAliceBC)

# Bobの公開鍵
pubkeyAliceBC = keyAliceBC.pubkey


# Bobに送金しておく (0.0002)のUTXOを4個
bitcoinRPC('sendtoaddress',[addrAliceBC, 0.0003])
bitcoinRPC('sendtoaddress',[addrAliceBC, 0.0004])
bitcoinRPC('sendtoaddress',[addrAliceBC, 0.0005])
bitcoinRPC('sendtoaddress',[addrAliceBC, 0.0006])
bitcoinRPC('sendtoaddress',[addrAliceBC, 0.0007])

# 10分後（マイニングされるのを待つ）
```


## 2.2 BobBCの準備(bitcoin)

Bob用ターミナルで実行

```ruby
require 'bitcoin'
require 'net/http'
require 'json'
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

# Bobのアドレス生成
addrBobBC = bitcoinRPC("getnewaddress", [])

# Bobの秘密鍵
privBobBC = bitcoinRPC("dumpprivkey", [addrBobBC])

# Bobの鍵オブジェクト(WIF形式の秘密鍵から生成）
keyBobBC = Bitcoin::Key.from_wif(privBobBC)

# Bobの公開鍵
pubkeyBobBC = keyBobBC.pubkey


# Bobに送金しておく (0.0002)のUTXOを4個
bitcoinRPC('sendtoaddress',[addrBobBC, 0.0003])
bitcoinRPC('sendtoaddress',[addrBobBC, 0.0004])
bitcoinRPC('sendtoaddress',[addrBobBC, 0.0005])
bitcoinRPC('sendtoaddress',[addrBobBC, 0.0006])
bitcoinRPC('sendtoaddress',[addrBobBC, 0.0007])

# 10分後（マイニングされるのを待つ）
```

## 3. BobがAliceに秘密情報のハッシュ値をおしえる (bitcoin) BobBC

* Bobが秘密情報を生成する →　ここでは　'DAO24hChallenge'　とします
* Bobが秘密情報のハッシュ値を生成して、Aliceに伝える
* Bobの公開鍵もAliceに伝える
* AkuceはBobにAliceのアドレスを伝える
* AliceはBobにAliceの公開鍵を伝える

```ruby
# 秘密情報
secret = 'DAO24hChallenge'
# 秘密情報のハッシュ値
secret_hash=Bitcoin.sha256(secret).bth
=> "23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15"
```

### BobからAliceへ (bitcoin -> Tapyrus)

BobBC

```ruby
# Bobの公開鍵
pubkeyBobBC
=> "02bc8b4919cb8e75996a2e7c9bb7c953d48caa7c7b4c3cb70ad04310c30f504754"

#設定
pubkeyAliceBC = "029a0d485aad639366dae75ba75e4c9aefe6611060fe4da9a605e32e468c1908b1"
```

BobTP

```ruby
# Bobの公開鍵
pubkeyBobTP
=> "03a19529f0c86628b5b4f027b2e0430fd13540595a90cbebf9797dad933cd6dc86"
```

AliceTP

```ruby
# Bobの公開鍵
pubkeyBobTP = "03a19529f0c86628b5b4f027b2e0430fd13540595a90cbebf9797dad933cd6dc86"
# 秘密情報のハッシュ値
secret_hash = "23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15"
```

AliceBC

```ruby
# AliceBCの公開鍵
pubkeyAliceBC
=> "029a0d485aad639366dae75ba75e4c9aefe6611060fe4da9a605e32e468c1908b1"

# Bobの公開鍵
pubkeyBobBC = "02bc8b4919cb8e75996a2e7c9bb7c953d48caa7c7b4c3cb70ad04310c30f504754"
# 秘密情報のハッシュ値
secret_hash = "23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15"
```

BobBC

```ruby
#設定
pubkeyAliceBC = "029a0d485aad639366dae75ba75e4c9aefe6611060fe4da9a605e32e468c1908b1"
```

### Alice から Bobへ (Tapyrus -> bitcoin)

Tapyrus側

```ruby
# Aliceの公開鍵
pubkeyAlice
=> "035e3e797153be07b3997e2bf97fc9db9fe1f15cd968e7921f50f25c01d569a3ae"
```
bitcoin側

```ruby
pubkeyAlice = "035e3e797153be07b3997e2bf97fc9db9fe1f15cd968e7921f50f25c01d569a3ae" 
```

## 4. Bob は Aliceへ bitcoin を送金しようとする (bitcoin) BobBC

* Bobがbitcoin のHTLC ロックトランザクションを作成
* ブロードキャストまで行う


```ruby
# key　鍵オブジェクト key=keyBobBC
# secret_hash 秘密情報のハッシュ値 secret_hash = "23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15"
# pubkey 送金先の公開鍵
# deposit HTLCでロックする資金の金額
# fee　手数料
# lockDays ロック時間

def create_HTCL_lock_transaction(key, secret_hash, pubkey, deposit, fee, lockDays)
    balance = bitcoinRPC('getbalance',[])
    utxos=bitcoinRPC('listunspent',[]).select{|x| x["address"]==key.to_p2wpkh}
    unless (deposit > balance) or (utxos == []) then
        # AliceのUTXOと残高を確認（とりあえず最初の Aliceのアドレス宛のUTXOを利用することにする）
        utxoAmount = utxos[0]["amount"]
        utxoVout = utxos[0]["vout"]
        utxoTxid = utxos[0]["txid"]
        utxoScriptPubKey = utxos[0]["scriptPubKey"]

        # <ロックするブロック数> 10日間のブロック数（リトルエンディアン）
        locktime = (6*24*lockDays).to_bn.to_s(2).reverse.bth
        # redeem script
        redeem_script = Bitcoin::Script.new << OP_IF << OP_SHA256 << secret_hash << OP_EQUALVERIFY << pubkey.htb << OP_ELSE << locktime << OP_CSV << OP_DROP << key.pubkey.htb << OP_ENDIF << OP_CHECKSIG
        # HTLCロックトランザクションの scriptPubKey
        scriptPubKey_p2wsh = Bitcoin::Script.from_string("0 #{redeem_script.to_sha256}")
        # P2WSH アドレスの生成
        p2wshaddr = scriptPubKey_p2wsh.to_addr
        # お釣り
        change= utxoAmount-deposit-fee
        # それぞれの金額をSatoshiに変換
        utxoAmount_satoshi = (utxoAmount * (10**8)).to_i
        deposit_satoshi = (deposit *  (10**8)).to_i
        change_satoshi = (change *  (10**8)).to_i
        # トランザクションテンプレートの生成
        tx = Bitcoin::Tx.new
        # inputの作成
        tx.in << Bitcoin::TxIn.new(out_point: Bitcoin::OutPoint.from_txid(utxoTxid, utxoVout))
        # デポジット用　P2WSH outputの作成
        tx.out << Bitcoin::TxOut.new(value: deposit_satoshi, script_pubkey:  Bitcoin::Script.parse_from_addr(p2wshaddr))
        # おつり用のP2WPKH outputの作成
        tx.out << Bitcoin::TxOut.new(value: change_satoshi , script_pubkey:  Bitcoin::Script.parse_from_addr(key.to_p2wpkh))
        # UTXOのロックを解除するために、UTXOのScript Public key を取得
        utxo_scriptPubKey = Bitcoin::Script.parse_from_payload(utxoScriptPubKey.htb)
        # sighashを作成
        sighash = tx.sighash_for_input(0, utxo_scriptPubKey, sig_version: :witness_v0, amount: utxoAmount_satoshi)
        # Aliceの秘密鍵でHTLCロックトランザクションの署名を作成する
        signature = key.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
        # witness 領域にBobのsighash へ署名をプッシュする
        tx.in[0].script_witness.stack << signature
        # witness 領域にBobの公開鍵（バイナリ形式）をプッシュする
        tx.in[0].script_witness.stack <<  key.pubkey.htb
        return [tx, redeem_script]
    else
        puts "you don't have enough funds for deposit"
    end
end
```

### 4.1 HTLC ロックトランザクションの生成とredeem_scriptの生成 (bitcoin) BobBC


```ruby
lockTx, redeem_script  = create_HTCL_lock_transaction(keyBobBC, secret_hash, pubkeyAliceBC, 0.0005, 0.00002, 10)

redeem_script.to_h[:asm]
irb(main):147:0> redeem_script.to_h[:asm]

=> "OP_IF OP_SHA256 23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15 OP_EQUALVERIFY 029a0d485aad639366dae75ba75e4c9aefe6611060fe4da9a605e32e468c1908b1 OP_ELSE 1440 OP_CSV OP_DROP 02bc8b4919cb8e75996a2e7c9bb7c953d48caa7c7b4c3cb70ad04310c30f504754 OP_ENDIF OP_CHECKSIG"             
```

### 4.2 HTLC ロックトランザクションのブロードキャスト (bitcoin) BobBC

```ruby
lockTx_txid = bitcoinRPC('sendrawtransaction',[lockTx.to_hex])

# HTLCロックトランザクションのトランザクションID（Bobにわたす）
htcl_lockTx_txid 
=> 
"8425ee07a9053bc3fc796f9cf92bb21b5fbea4648d49890f4bf96dc8b88f9f2e"
```

### 4.3 BobはAlice に redeem_scriptを渡す (bitcoin -> Tapyrus)

Alice側の処理 AliceBC

```ruby
redeem_script_BobBC_asm = "OP_IF OP_SHA256 23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15 OP_EQUALVERIFY 029a0d485aad639366dae75ba75e4c9aefe6611060fe4da9a605e32e468c1908b1 OP_ELSE 1440 OP_CSV OP_DROP 02bc8b4919cb8e75996a2e7c9bb7c953d48caa7c7b4c3cb70ad04310c30f504754 OP_ENDIF OP_CHECKSIG" 
```

## 5. Alice からBobに TPC を送金しようとする (Tapyrus)

* Aliceが TPCのHTLC ロックトランザクションを作成
* ブロードキャストまで行う


```ruby
# key　鍵オブジェクト
# secret_hash 秘密情報のハッシュ値
# pubkey 送金先の公開鍵
# deposit HTLCでロックする資金の金額
# fee　手数料
# lockDays ロック時間
# お釣り用アドレス

def create_HTCL_lock_transaction(key, secret_hash, pubkey, deposit, fee, lockDays, addr)
    balance = tapyrusRPC('getbalance',[])
    utxos= tapyrusRPC('listunspent',[]).select{|x| x["address"]==addr}
    unless (deposit > balance) or (utxos == []) then
        # AliceのUTXOと残高を確認（とりあえず最初の Aliceのアドレス宛のUTXOを利用することにする）
        utxoAmount = utxos[0]["amount"]
        utxoVout = utxos[0]["vout"]
        utxoTxid = utxos[0]["txid"]
        utxoScriptPubKey = utxos[0]["scriptPubKey"]
        # <ロックするブロック数> 10日間のブロック数（リトルエンディアン）
        locktime = (6*24*lockDays).to_bn.to_s(2).reverse.bth
        # redeem script
        redeem_script = Tapyrus::Script.new << OP_IF << OP_SHA256 << secret_hash << OP_EQUALVERIFY << pubkey.htb << OP_ELSE << locktime << OP_CSV << OP_DROP << key.pubkey.htb << OP_ENDIF << OP_CHECKSIG
        # HTLCロックトランザクションの scriptPubKey
        scriptPubKey_p2sh=redeem_script.to_p2sh
        # P2SH アドレスの生成
        p2shaddr = scriptPubKey_p2sh.addresses[0]
        # お釣り
        change= utxoAmount-deposit-fee
        # それぞれの金額をSatoshiに変換
        utxoAmount_satoshi = (utxoAmount * (10**8)).to_i
        deposit_satoshi = (deposit *  (10**8)).to_i
        change_satoshi = (change *  (10**8)).to_i
        # トランザクションテンプレートの生成
        tx = Tapyrus::Tx.new
        # inputの作成
        tx.in << Tapyrus::TxIn.new(out_point: Tapyrus::OutPoint.from_txid(utxoTxid, utxoVout))
        # デポジット用　P2SH outputの作成
        tx.out << Tapyrus::TxOut.new(value: deposit_satoshi, script_pubkey:  Tapyrus::Script.parse_from_addr(p2shaddr))
        # おつり用のP2PKH outputの作成
        tx.out << Tapyrus::TxOut.new(value: change_satoshi , script_pubkey:  Tapyrus::Script.parse_from_addr(addr))
        # UTXOのロックを解除するために、UTXOのScript Public key を取得
        utxo_scriptPubKey = Tapyrus::Script.parse_from_payload(utxoScriptPubKey.htb)
        # sighashを作成
        sighash = tx.sighash_for_input(0, utxo_scriptPubKey, amount: utxoAmount_satoshi)
        # Aliceの秘密鍵でHTLCロックトランザクションの署名を作成する
        signature = key.sign(sighash) + [Tapyrus::SIGHASH_TYPE[:all]].pack('C')
        # script_sig 領域にAliceのsighash へ署名をプッシュする
        tx.in[0].script_sig << signature
        # script_sig 領域にAliceの公開鍵（バイナリ形式）をプッシュする
        tx.in[0].script_sig <<  key.pubkey.htb
        # Aliceによる、完成したHTLCロックトランザクションのブロードキャスト
        return [tx, redeem_script]
    else
        puts "you don't have enough funds for deposit"
    end
end
```

### 5.1 HTLC ロックトランザクションとredeem_scriptの生成 (tapyrus)

```ruby
lockTx, redeem_script  = create_HTCL_lock_transaction(keyAlice, secret_hash, pubkeyBob, 0.0005, 0.00002, 10, addrAlice)

redeem_script.to_h[:asm]
WARNING: Bitcoin::Script#addresses is deprecated. Use Bitcoin::Script#to_addr instead.
=> "OP_IF OP_SHA256 23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15 OP_EQUALVERIFY 0223e1fe76cf8c0ef4c4531a71d14d7e4d3010d34324ba1ad934122ebe35c2e985 OP_ELSE 1440 OP_CSV OP_DROP 027b14d72f48077413ca77ace53c900fa2c4fbc4465f8025e244044cd1ebb72dc1 OP_ENDIF OP_CHECKSIG"                
```

### 5.2 HTLC ロックトランザクションのブロードキャスト (tapyrus)

```ruby
 lockTx_txid = tapyrusRPC('sendrawtransaction',[lockTx.to_hex])
=> "15bf6dd6b33ee44f2011c73c2fa90e8fa0e5c1fa83a5f783ddef059b8efd80c6"
```


## 6. BobがCarolから秘密情報を使って AliceのHTLCロックトランザクションをアンロックする (bitcoin)

* HTLCアンロックトランザクションを作成する
* HTLCアンロックトランザクションをブロードキャストする


* Bobは、秘密情報を知っている
* Aliceから redeem script をもらう
* Aliceから　HTLCロックトランザクションの トランザクションIDをもらう（ブロックチェーンで検出してもよい）

```ruby
# 秘密情報
secret

# redeem script
redeem_script = "63a82023ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c158821037344b60d92625fb2b1f903c959eae0c5ac07a98f4f5d89c00fb2cfafe36d51d36702a005b275210223e1fe76cf8c0ef4c4531a71d14d7e4d3010d34324ba1ad934122ebe35c2e98568ac"

# HTLCロックトランザクションの トランザクションID
lockTx_txid = "15bf6dd6b33ee44f2011c73c2fa90e8fa0e5c1fa83a5f783ddef059b8efd80c6"
```

```ruby
def unlock_HTLC_transaction(secret, redeem_script_asm, htcl_lockTx_txid, keyBob, fee)
    # redeem scriptの復元
    redeem_script = Bitcoin::Script.from_string(redeem_script_asm)
    # HTLCロックトランザクションを得る
    htlc_tx = bitcoinRPC('decoderawtransaction',[ bitcoinRPC('getrawtransaction',[htcl_lockTx_txid])])
    # HTLCロックトランザクションの scriptPubKey
    scriptPubKey_p2wsh = Bitcoin::Script.from_string("0 #{redeem_script.to_sha256}")
    # HTLCロックトランザクションのP2WSHアドレス
    p2wshaddr = scriptPubKey_p2wsh.to_addr
    # アンロックの対象となるUTXO のvout
    htcl_lockTx_vout=0
    # アンロックの対象となるUTXO のデポジット金額
    deposit=htlc_tx["vout"][0]["value"]
    # 報酬金額
    reward = deposit-fee
    # satoshi 変換
    deposit_satoshi = (deposit * (10**8)).to_i
    reward_satoshi = (reward* (10**8)).to_i
    # トランザクションテンプレートの生成
    tx = Bitcoin::Tx.new
    # inputの作成
    tx.in << Bitcoin::TxIn.new(out_point: Bitcoin::OutPoint.from_txid(htcl_lockTx_txid, htcl_lockTx_vout))
    # 報酬用のP2WPKH outputの作成
    tx.out << Bitcoin::TxOut.new(value: reward_satoshi, script_pubkey: Bitcoin::Script.parse_from_addr(keyBob.to_p2wpkh))
    # sighashを作成
    sighash = tx.sighash_for_input(0, redeem_script, sig_version: :witness_v0, amount: deposit_satoshi, hash_type: Bitcoin::SIGHASH_TYPE[:all])
    # SHIGHASH_TYPE ALLでBobが署名
    sigBob = keyBob.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
    tx.in[0].script_witness.stack << sigBob
    # witness scriptの追加
    tx.in[0].script_witness.stack << secret
    tx.in[0].script_witness.stack << [1].pack("C")
    tx.in[0].script_witness.stack << redeem_script.to_payload
    return tx
end

```

### HTLC アンロックトランザクションの生成 (bitcoin)

```ruby
unlock_tx = unlock_HTLC_transaction(secret, redeem_script_asm, htcl_lockTx_txid, keyBob, 0.00002)
```

### HTLC アンロックトランザクションのブロードキャスト (bitcoin)

```ruby
htcl_unlockTx_txid = bitcoinRPC('sendrawtransaction',[unlock_tx.to_hex])
htcl_unlockTx_txid
=> 
"4df0f17d418c1e913369e5cdf83c6f047a4156274a2336ffc282837187a30fd2"
```






---















## 7. Alice が秘密情報を見つける

* Aliceは、ブロックチェーンをモニタする
* 秘密情報のハッシュ値を含むHTLCアンロックトランザクションを見つけると
* トランザクションから秘密情報を得る


```ruby
```


## 8. 秘密情報を得たAliceがHTLCアンロックトランザクションを作成する

* Alice がHTLCアンロックトランザクションをブロードキャストしてBobからのbitcoinの資金を得る


* Carolから、秘密情報をもらう
* Aliceから redeem script をもらう
* Aliceから　HTLCロックトランザクションの トランザクションIDをもらう

```ruby
# 秘密情報
secret='HTLC_test'
```

```ruby
# redeem script
redeem_script_asm = "OP_IF OP_SHA256 996bf59473947d9906275f427ecb318371514db2ffb8e9d8517b5e45cb65e357 OP_EQUALVERIFY 037344b60d92625fb2b1f903c959eae0c5ac07a98f4f5d89c00fb2cfafe36d51d3 OP_ELSE 1440 OP_CSV OP_DROP 021cbe42a46b34eded4242666648275f866c836257888607d611bce7eb85b15238 OP_ENDIF OP_CHECKSIG"

 # HTLCロックトランザクションの トランザクションID
htcl_lockTx_txid = "1501924bac50fc8f66367179f4f669921e9e2eb273b0a642b43c7e9fc026fe7a"
```

```ruby
def unlock_HTLC_transaction(secret, redeem_script_asm, htcl_lockTx_txid, keyBob, fee)
    # redeem scriptの復元
    redeem_script = Bitcoin::Script.from_string(redeem_script_asm)
    # HTLCロックトランザクションを得る
    htlc_tx = bitcoinRPC('decoderawtransaction',[ bitcoinRPC('getrawtransaction',[htcl_lockTx_txid])])
    # HTLCロックトランザクションの scriptPubKey
    scriptPubKey_p2wsh = Bitcoin::Script.from_string("0 #{redeem_script.to_sha256}")
    # HTLCロックトランザクションのP2WSHアドレス
    p2wshaddr = scriptPubKey_p2wsh.to_addr
    # アンロックの対象となるUTXO のvout
    htcl_lockTx_vout=0
    # アンロックの対象となるUTXO のデポジット金額
    deposit=htlc_tx["vout"][0]["value"]
    # 報酬金額
    reward = deposit-fee
    # satoshi 変換
    deposit_satoshi = (deposit * (10**8)).to_i
    reward_satoshi = (reward* (10**8)).to_i
    # トランザクションテンプレートの生成
    tx = Bitcoin::Tx.new
    # inputの作成
    tx.in << Bitcoin::TxIn.new(out_point: Bitcoin::OutPoint.from_txid(htcl_lockTx_txid, htcl_lockTx_vout))
    # 報酬用のP2WPKH outputの作成
    tx.out << Bitcoin::TxOut.new(value: reward_satoshi, script_pubkey: Bitcoin::Script.parse_from_addr(keyBob.to_p2wpkh))
    # sighashを作成
    sighash = tx.sighash_for_input(0, redeem_script, sig_version: :witness_v0, amount: deposit_satoshi, hash_type: Bitcoin::SIGHASH_TYPE[:all])
    # SHIGHASH_TYPE ALLでBobが署名
    sigBob = keyBob.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
    tx.in[0].script_witness.stack << sigBob
    # witness scriptの追加
    tx.in[0].script_witness.stack << secret
    tx.in[0].script_witness.stack << [1].pack("C")
    tx.in[0].script_witness.stack << redeem_script.to_payload
    return tx
end

```

### HTLC アンロックトランザクションの生成

```ruby
unlock_tx = unlock_HTLC_transaction(secret, redeem_script_asm, htcl_lockTx_txid, keyBob, 0.00002)
```

### HTLC アンロックトランザクションのブロードキャスト

```ruby
htcl_unlockTx_txid = bitcoinRPC('sendrawtransaction',[unlock_tx.to_hex])
htcl_unlockTx_txid
=> 
"4df0f17d418c1e913369e5cdf83c6f047a4156274a2336ffc282837187a30fd2"
```












































