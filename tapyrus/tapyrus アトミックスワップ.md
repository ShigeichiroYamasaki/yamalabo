# Tapyrus と Bitcoin の間でアトミックスワップ

最終更新 2022/08/07 Shigeichiro Yamasaki

* Tapyrus側のトークンは、TPCとします（ほかのトークンでもOK)
* Alice の TPCを Bobが bitcoin で購入する。
* 秘密情報は Bob (bitcoinでTPCを購入する人）が生成し、そのハッシュ値をAliceに伝えます

--

1. Bobは、秘密情報のハッシュ値で bitcoin をロックした HTLCロックトランザクションを作成、ブロードキャストします。（bitcoinがロック状態になります）
2. Aliceも、そのハッシュ値で TPC をロックした HTLCロックトランザクションを作成し、ブロードキャストします。（TPCがロック状態になります）
3. Bobは、ロック状態のTPCをアンロックするために、秘密情報（ハッシュ値の原像）を埋め込んだ HTLCアンロックトランザクションを作成し、ブロードキャストします。これによってBobは Alice のTPCを入手します。
4. Aliceは、Bobによって TapyrusネットワークにブロードキャストされたHTLCアンロックトランザクションから、秘密情報（ハッシュ値の原像）を入手します。
5. Aliceは、秘密情報（ハッシュ値の原像）を使って bitcoin の HTLCアンロックトランザクションを作成し、ブロードキャストします。これによって Aliceは代金の bitcoin を入手します。

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

BobBC

```ruby
# 秘密情報
secret = 'DAO24hChallenge'
# 秘密情報のハッシュ値
secret_hash=Bitcoin.sha256(secret).bth
=> "23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15"
```

BobTP

```ruby
secret_hash = "23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15"
```

AliceBC

```ruby
secret_hash = "23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15"
```

AliceTP

```ruby
secret_hash = "23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15"
```

## 4. Bobの公開鍵をAliceに伝え、Aliceの公開鍵をBobに伝える

(bitcoin -> bitcoin)

BobBC

```ruby
# Bobの公開鍵
pubkeyBobBC
=> "03ba74b5ee733f39e22c84f1b9c07eb11ad68ab73480012de950b40510b7625272"
```

AliceBC

```ruby
pubkeyBobBC = "03ba74b5ee733f39e22c84f1b9c07eb11ad68ab73480012de950b40510b7625272"

# Aliceの公開鍵
pubkeyAliceBC
=> "03dfce2e185286fad66da7d6426777f26e6a3d805ae668f8e6eb76cc93dfad4480"
```

BobBC

```ruby
pubkeyAliceBC = "03dfce2e185286fad66da7d6426777f26e6a3d805ae668f8e6eb76cc93dfad4480"
```

(Tapyrus -> Tapyrus)

BobTP

```ruby
# Bobの公開鍵
pubkeyBobTP
=> "03efdd1fee448d2dff78fd1eff959c28c36138c4eb2cfe3291d9a0fd2c104b9a73"
```

AliceTP

```ruby
pubkeyBobTP = "03efdd1fee448d2dff78fd1eff959c28c36138c4eb2cfe3291d9a0fd2c104b9a73"

# Aliceの公開鍵
pubkeyAliceTP
=> "02fc0056c8e097547608eab61e2effc3264f4444f80e2e9e6d834f3f8ac631736b"
```

BobTP

```ruby
pubkeyAliceTP = "02fc0056c8e097547608eab61e2effc3264f4444f80e2e9e6d834f3f8ac631736b"
```

公開鍵の確認

```ruby
pubkeyBobBC = "03ba74b5ee733f39e22c84f1b9c07eb11ad68ab73480012de950b40510b7625272"
pubkeyBobTP = "03efdd1fee448d2dff78fd1eff959c28c36138c4eb2cfe3291d9a0fd2c104b9a73"
pubkeyAliceTP = "02fc0056c8e097547608eab61e2effc3264f4444f80e2e9e6d834f3f8ac631736b"
pubkeyAliceBC = "03dfce2e185286fad66da7d6426777f26e6a3d805ae668f8e6eb76cc93dfad4480"
```

## 5. Bob は Aliceへ bitcoin を送金しようとする (bitcoin) BobBC

### 5.1  HTLC ロックトランザクション作成メソッド　(bitcoin)

BobBC

```ruby
# key　鍵オブジェクト 
# secret_hash 秘密情報のハッシュ値 
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

### 5.2 HTLC ロックトランザクションの生成とredeem_scriptの生成 (bitcoin) 

BobBC

```ruby
# HTLC ロックトランザクションの生成
lockTx, redeem_scriptBC  = create_HTCL_lock_transaction(keyBobBC, secret_hash, pubkeyAliceBC, 0.0003, 0.00002, 10)

# redeem_scriptの確認
redeem_scriptBC.to_h[:asm]
=> "OP_IF OP_SHA256 23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15 OP_EQUALVERIFY 03dfce2e185286fad66da7d6426777f26e6a3d805ae668f8e6eb76cc93dfad4480 OP_ELSE 1440 OP_CSV OP_DROP 03ba74b5ee733f39e22c84f1b9c07eb11ad68ab73480012de950b40510b7625272 OP_ENDIF OP_CHECKSIG"            
```

### 5.3 HTLC ロックトランザクションのブロードキャスト (bitcoin) 

BobBC

```ruby
# HTLC ロックトランザクションのブロードキャスト 
lockTx_txidBC = bitcoinRPC('sendrawtransaction',[lockTx.to_hex])

# HTLCロックトランザクションのトランザクションID
lockTx_txidBC 
=> 
"bb5ac459a1a4157b08ff32fa27927eeacdbd2d5f816bbb4dffd7e3a8f6f94902"
```


### 5.4 BobはAlice に redeem_scriptとトランザクションIDを渡す (bitcoin)

AliceBC

```ruby
# redeem_scriptのasm文字列
redeem_scriptBC_asm = "OP_IF OP_SHA256 23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15 OP_EQUALVERIFY 029a0d485aad639366dae75ba75e4c9aefe6611060fe4da9a605e32e468c1908b1 OP_ELSE 1440 OP_CSV OP_DROP 02bc8b4919cb8e75996a2e7c9bb7c953d48caa7c7b4c3cb70ad04310c30f504754 OP_ENDIF OP_CHECKSIG"
# redeem_scriptの復元
redeem_scriptBC = Bitcoin::Script.from_string(redeem_scriptBC_asm)

# HTLCロックトランザクションのトランザクションID
lockTx_txidBC = "bb5ac459a1a4157b08ff32fa27927eeacdbd2d5f816bbb4dffd7e3a8f6f94902"
```

## 6. Alice からBobに TPC を送金しようとする (Tapyrus) 

### 6.1  HTLC ロックトランザクション作成メソッド (Tapyrus)

AliceTP

```ruby
# key　鍵オブジェクト
# secret_hash 秘密情報のハッシュ値
# pubkey 送金先の公開鍵
# deposit HTLCでロックする資金の金額
# fee　手数料
# lockDays ロック時間
# お釣り用アドレス

def create_HTCL_lock_transaction(key, secret_hash, pubkey, deposit, fee, lockDays)
    balance = tapyrusRPC('getbalance',[])
    utxos= tapyrusRPC('listunspent',[]).select{|x| x["address"]==key.to_p2pkh}
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
        tx.out << Tapyrus::TxOut.new(value: change_satoshi , script_pubkey:  Tapyrus::Script.parse_from_addr(key.to_p2pkh))
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

### 6.2 HTLC ロックトランザクションの生成とredeem_scriptの生成 (tapyrus)

AliceTP

```ruby
# HTLC ロックトランザクションの生成
lockTx, redeem_scriptTP  = create_HTCL_lock_transaction(keyAliceTP, secret_hash, pubkeyBobTP, 0.0005, 0.00002, 10)

# redeem_script
redeem_scriptTP.to_h[:asm]
=> "OP_IF OP_SHA256 23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15 OP_EQUALVERIFY 03efdd1fee448d2dff78fd1eff959c28c36138c4eb2cfe3291d9a0fd2c104b9a73 OP_ELSE 1440 OP_CSV OP_DROP 02fc0056c8e097547608eab61e2effc3264f4444f80e2e9e6d834f3f8ac631736b OP_ENDIF OP_CHECKSIG"           
```

### 6.3 HTLC ロックトランザクションのブロードキャスト (tapyrus)

AliceTP

```ruby
# HTLC ロックトランザクションのブロードキャスト
lockTx_txidTP = tapyrusRPC('sendrawtransaction',[lockTx.to_hex])
=> "15bf6dd6b33ee44f2011c73c2fa90e8fa0e5c1fa83a5f783ddef059b8efd80c6"
```

### 6.4 Alice はBob に redeem_scriptとトランザクションIDを渡す (Tapyrus)

BobTP

```ruby
# redeem_scriptのasm文字列
redeem_scriptTP_asm = "OP_IF OP_SHA256 23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15 OP_EQUALVERIFY 03efdd1fee448d2dff78fd1eff959c28c36138c4eb2cfe3291d9a0fd2c104b9a73 OP_ELSE 1440 OP_CSV OP_DROP 02fc0056c8e097547608eab61e2effc3264f4444f80e2e9e6d834f3f8ac631736b OP_ENDIF OP_CHECKSIG"

# redeem_scriptの復元
redeem_scriptTP = Bitcoin::Script.from_string(redeem_scriptTP_asm)

# HTLCロックトランザクションのトランザクションID
lockTx_txidTP = "15bf6dd6b33ee44f2011c73c2fa90e8fa0e5c1fa83a5f783ddef059b8efd80c6"
```


## 7. Bobが秘密情報を使って AliceのHTLCロックトランザクションをアンロックする (Tapyrus)

### 7.1 HTLCアンロックトランザクション作成メソッド(Tapyrus)

BobTP

```ruby
# 秘密情報（Bobは知っている）
secret

# redeem_script
redeem_scriptTP

# HTLCロックトランザクションの トランザクションID
lockTx_txidTP
```

```ruby
def unlock_HTLC_transaction(secret, redeem_scriptTP, lockTx_txidTP, key, fee)
    # HTLCロックトランザクションを得る
    htlc_tx = bitcoinRPC('decoderawtransaction',[ bitcoinRPC('getrawtransaction',[lockTx_txidTP])])
    # HTLCロックトランザクションの scriptPubKey
    scriptPubKey_p2wsh = Bitcoin::Script.from_string("0 #{redeem_scriptTP.to_sha256}")
    # HTLCロックトランザクションのP2WSHアドレス
    p2wshaddr = scriptPubKey_p2wsh.to_addr
    # アンロックの対象となるUTXO のvout
    lockTx_vout=0
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
    tx.in << Bitcoin::TxIn.new(out_point: Bitcoin::OutPoint.from_txid(lockTx_txid, lockTx_vout))
    # 報酬用のP2WPKH outputの作成
    tx.out << Bitcoin::TxOut.new(value: reward_satoshi, script_pubkey: Bitcoin::Script.parse_from_addr(key.to_p2wpkh))
    # sighashを作成
    sighash = tx.sighash_for_input(0, redeem_script, sig_version: :witness_v0, amount: deposit_satoshi, hash_type: Bitcoin::SIGHASH_TYPE[:all])
    # SHIGHASH_TYPE ALLでBobが署名
    signature = key.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
    tx.in[0].script_sig << signature
    tx.in[0].script_sig << secret
    tx.in[0].script_sig << [1].pack("C")
    tx.in[0].script_sig << redeem_script.to_payload
    return tx
end
```

### 7.2 BobがHTLC アンロックトランザクションの生成 (Tapyrus)

BobTP

```ruby
unlock_tx = unlock_HTLC_transaction(secret,redeem_scriptTP, lockTx_txid, keyBobTP, 0.00002)
```

### 7.3 BobがHTLC アンロックトランザクションをブロードキャスト (Tapyrus)

BobTP

```ruby
# HTLC アンロックトランザクションのブロードキャスト
unlockTx_txid = bitcoinRPC('sendrawtransaction',[unlock_tx.to_hex])

unlockTx_txid
=> 
"4df0f17d418c1e913369e5cdf83c6f047a4156274a2336ffc282837187a30fd2"
```

## 8. Alice がTapyrusブロックチェーンを観測して秘密情報を見つける (Tapyrus)

* Bobがブロードキャストした秘密情報のハッシュ値を含むHTLCアンロックトランザクションを見つけ
* そのランザクションから秘密情報を得る

### 8.1 redeem_scriptに秘密情報のハッシュ値を含むものを見つける

AliceTP

```ruby
# 秘密情報のハッシュ値
secret_hash

# メモリープール内のトランザクションで、redeem_scriptに秘密情報のハッシュ値を含むものを見つける

def watch_mempool(secret_hash)
    txids = tapyrusRPC('getrawmempool',[])
    txids.select{|txid|
        begin
            Tapyrus::Script.parse_from_payload(
                Tapyrus::Tx.parse_from_payload(
                    tapyrusRPC('getrawtransaction',[txid]).htb
                ).inputs[0].script_witness.stack[3]
            ).to_h[:asm].split(' ')[2]==secret_hash
        rescue
            false
        end
    }
end

# 監視ループ

def watch_loop(secret_hash,sec)
    r=[]
    while r==[]
        r=watch_mempool(secret_hash)
        sleep sec
        print "*"
    end
    return r[0]
end

# 10秒ごとに監視する
txid = watch_loop(secret_hash,10)
```

### 8.2 HTLCアンロックトランザクションから秘密情報を見つける

AliceTP

```ruby
def find_secret(txid)
    Tapyrus::Tx.parse_from_payload(tapyrusRPC('getrawtransaction',[txid]).htb).inputs[0].script_sig.chunks[1]
end

secret =find_secret(txid)
```

## 9 AliceがBobがブロードキャストした秘密情報を使って BobのHTLCロックトランザクションをアンロックする (bitcoin)

これによって、AliceはTPCトークンの代金をbitcoinで回収することができます。

### 9.1 HTLCアンロックトランザクション作成メソッド

AliceBC

```ruby
# 秘密情報 (Tapyrusブロックチェーンから得た）
secret = 'DAO24hChallenge'

# redeem script
redeem_scriptBC

# HTLCロックトランザクションの トランザクションID
lockTx_txidBC = "15bf6dd6b33ee44f2011c73c2fa90e8fa0e5c1fa83a5f783ddef059b8efd80c6"

```

```ruby
def unlock_HTLC_transaction(secret, redeem_scriptBC, lockTx_txidBC, key, fee)
    # HTLCロックトランザクションを得る
    htlc_tx = bitcoinRPC('decoderawtransaction',[ bitcoinRPC('getrawtransaction',[lockTx_txidBC])])
    # HTLCロックトランザクションの scriptPubKey
    scriptPubKey_p2wsh = Bitcoin::Script.from_string("0 #{redeem_scriptBC.to_sha256}")
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
    tx.in << Bitcoin::TxIn.new(out_point: Bitcoin::OutPoint.from_txid(lockTx_txidBC, htcl_lockTx_vout))
    # 報酬用のP2WPKH outputの作成
    tx.out << Bitcoin::TxOut.new(value: reward_satoshi, script_pubkey: Bitcoin::Script.parse_from_addr(key.to_p2wpkh))
    # sighashを作成
    sighash = tx.sighash_for_input(0, redeem_scriptBC, sig_version: :witness_v0, amount: deposit_satoshi, hash_type: Bitcoin::SIGHASH_TYPE[:all])
    # SHIGHASH_TYPE ALLでBobが署名
    sigBob = key.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
    tx.in[0].script_witness.stack << sigBob
    # witness scriptの追加
    tx.in[0].script_witness.stack << secret
    tx.in[0].script_witness.stack << [1].pack("C")
    tx.in[0].script_witness.stack << redeem_scriptBC.to_payload
    return tx
end

```

### HTLC アンロックトランザクションの生成 (bitcoin)

```ruby
unlock_tx = unlock_HTLC_transaction(secret, redeem_scriptBC, lockTx_txidBC, keyAlice, 0.00002)
```

### HTLC アンロックトランザクションのブロードキャスト (bitcoin)

```ruby
htcl_unlockTx_txid = bitcoinRPC('sendrawtransaction',[unlock_tx.to_hex])
htcl_unlockTx_txid
=> 
"4df0f17d418c1e913369e5cdf83c6f047a4156274a2336ffc282837187a30fd2"
```




