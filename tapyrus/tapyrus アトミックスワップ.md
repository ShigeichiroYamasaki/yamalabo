# Tapyrus と Bitcoin の間でアトミックスワップ

最終更新 2022/08/05 Shigeichiro Yamasaki

とりあえず Tapyrus側のトークンは、TPCとします

ストーリーとしては、Alice が所持している （デポジットしている）TPCを BobがBTCで購入するイメージです

* 秘密情報は Bob (bitcoinでTPCを購入する人）が生成し、そのハッシュ値をAliceに伝えます
* Bobは、その同じハッシュ値で bitcoin をロックしたHTLCロックトランザクションをブロードキャストします。
* Aliceはそのハッシュ値でTPCをロックしたHTLCロックトランザクションをブロードキャストします。
* ロックされたTPCをアンロックするために、Bobは秘密情報を埋め込んだトランザクションをブロードキャストしなければなりません
* Aliceは、ブロードキャストされた秘密情報を入手し、bitcoinのHTLCロックトランザクションをアンロックします。

## 双方向のHTLC

* Alice: Tapyrus
* Bob : bitcoin

Alice, Bobは別マシンで実行することが望ましい。
同じマシンでも別ターミナルで実施する

### Alice とBobの事前準備

アドレス、鍵、UTXOを準備しておく

### BobがAliceに秘密情報のハッシュ値をおしえる

* Bobが秘密情報を生成する →　ここでは　'DAO24hChallenge'　とします
* Bobが秘密情報のハッシュ値を生成して、Aliceに伝える
* Bobの公開鍵もAliceに伝える
* AliceはBobにAliceの公開鍵を伝える

### Bob は Aliceへ bitcoin を送金しようとする

* Bobがbitcoin のHTLC ロックトランザクションを作成
* ブロードキャストまで行う

### Alice からBobに TPC を送金しようとする

* Aliceが TPCのHTLC ロックトランザクションを作成
* ブロードキャストまで行う

### BobがCarolから秘密情報を得てアンロックする

* HTLCアンロックトランザクションをブロードキャストする

### Alice が秘密情報を見つける

* Aliceは、ブロックチェーンをモニタする
* 秘密情報のハッシュ値を含むHTLCアンロックトランザクションを見つけると
* トランザクションから秘密情報を得る

### 秘密情報を得たAliceがHTLCアンロックトランザクションを作成する

* Alice がHTLCアンロックトランザクションをブロードキャストしてBobからのbitcoinの資金を得る


## Aliceの準備(Tapyrus)

Alice用ターミナルで実行

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
addrAlice = tapyrusRPC("getnewaddress", [])

# Aliceの秘密鍵
privAlice = tapyrusRPC("dumpprivkey", [addrAlice])

# Aliceの鍵オブジェクト(WIF形式の秘密鍵から生成）
keyAlice=Tapyrus::Key.from_wif(privAlice)

# Aliceの公開鍵
pubkeyAlice = keyAlice.pubkey

# Aliceに送金してUTXOを用意する
tapyrusRPC('sendtoaddress',[addrAlice, 0.0003])
tapyrusRPC('sendtoaddress',[addrAlice, 0.0004])
tapyrusRPC('sendtoaddress',[addrAlice, 0.0005])
tapyrusRPC('sendtoaddress',[addrAlice, 0.0006])
tapyrusRPC('sendtoaddress',[addrAlice, 0.0007])

# 10分後（マイニングされるのを待つ）
```


## Bobの準備(bitcoin)

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
addrBob = bitcoinRPC("getnewaddress", [])

# Bobの秘密鍵
privBob = bitcoinRPC("dumpprivkey", [addrBob])

# Bobの鍵オブジェクト(WIF形式の秘密鍵から生成）
keyBob = Bitcoin::Key.from_wif(privBob)

# Bobの公開鍵
pubkeyBob = keyBob.pubkey
=> 
"037344b60d92625fb2b1f903c959eae0c5ac07a98f4f5d89c00fb2cfafe36d51d3"

# Bobに送金しておく (0.0002)のUTXOを4個
bitcoinRPC('sendtoaddress',[addrBob, 0.0003])
bitcoinRPC('sendtoaddress',[addrBob, 0.0004])
bitcoinRPC('sendtoaddress',[addrBob, 0.0005])
bitcoinRPC('sendtoaddress',[addrBob, 0.0006])
bitcoinRPC('sendtoaddress',[addrBob, 0.0007])

# 10分後（マイニングされるのを待つ）
```

## BobがAliceに秘密情報のハッシュ値をおしえる

* Bobが秘密情報を生成する →　ここでは　'DAO24hChallenge'　とします
* Bobが秘密情報のハッシュ値を生成して、Aliceに伝える
* Bobの公開鍵もAliceに伝える
* AliceはBobにAliceの公開鍵を伝える

```ruby
# 秘密情報
secret = 'DAO24hChallenge'
# 秘密情報のハッシュ値
secret_hash=Bitcoin.sha256(secret).bth
=> "23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15"
```

### BobからAliceへ

```ruby
# Bobの公開鍵
pubkeyBob = "037344b60d92625fb2b1f903c959eae0c5ac07a98f4f5d89c00fb2cfafe36d51d3"
# 秘密情報のハッシュ値
secret_hash = "23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15"
```

### Alice から Bobへ

```ruby
# Aliceの公開鍵
pubkeyAlice = "0223e1fe76cf8c0ef4c4531a71d14d7e4d3010d34324ba1ad934122ebe35c2e985"
```

## Bob は Aliceへ bitcoin を送金しようとする (bitcoin)

* Bobがbitcoin のHTLC ロックトランザクションを作成
* ブロードキャストまで行う


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
        # witness 領域にAliceのsighash へ署名をプッシュする
        tx.in[0].script_witness.stack << signature
        # witness 領域にAliceの公開鍵（バイナリ形式）をプッシュする
        tx.in[0].script_witness.stack <<  key.pubkey.htb
        # Aliceによる、完成したHTLCロックトランザクションのブロードキャスト
        return [tx, redeem_script.to_hex]
    else
        puts "you don't have enough funds for deposit"
    end
end
```

### HTLC ロックトランザクションの生成とredeem_scriptの生成

```ruby
lockTx, redeem_script  = create_HTCL_lock_transaction(keyBob, secret_hash, pubkeyAlice, 0.0005, 0.00002, 10)

redeem_script
=> "63a82023ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c1588210223e1fe76cf8c0ef4c4531a71d14d7e4d3010d34324ba1ad934122ebe35c2e9856702a005b27521027b14d72f48077413ca77ace53c900fa2c4fbc4465f8025e244044cd1ebb72dc168ac"
```

### HTLC ロックトランザクションのブロードキャスト

```ruby
lockTx_txid = bitcoinRPC('sendrawtransaction',[lockTx.to_hex])

# HTLCロックトランザクションのトランザクションID（Bobにわたす）
htcl_lockTx_txid 
=> 
"1501924bac50fc8f66367179f4f669921e9e2eb273b0a642b43c7e9fc026fe7a"
```

### BobはAlice に redeem_scriptを渡す

Alice側の処理

```ruby
redeem_script_Bob = "63a82023ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c1588210223e1fe76cf8c0ef4c4531a71d14d7e4d3010d34324ba1ad934122ebe35c2e9856702a005b27521027b14d72f48077413ca77ace53c900fa2c4fbc4465f8025e244044cd1ebb72dc168ac"
```

## Alice からBobに TPC を送金しようとする (Tapyrus)

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
        return [tx, redeem_script.to_hex]
    else
        puts "you don't have enough funds for deposit"
    end
end
```

### HTLC ロックトランザクションとredeem_scriptの生成

```ruby
lockTx, redeem_script  = create_HTCL_lock_transaction(keyAlice, secret_hash, pubkeyBob, 0.0005, 0.00002, 10, addrAlice)

redeem_script
=> "63a82023ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c158821037344b60d92625fb2b1f903c959eae0c5ac07a98f4f5d89c00fb2cfafe36d51d36702a005b275210223e1fe76cf8c0ef4c4531a71d14d7e4d3010d34324ba1ad934122ebe35c2e98568ac"
```

### HTLC ロックトランザクションのブロードキャスト

```ruby
 lockTx_txid = tapyrusRPC('sendrawtransaction',[lockTx.to_hex])
=> "15bf6dd6b33ee44f2011c73c2fa90e8fa0e5c1fa83a5f783ddef059b8efd80c6"
```


### BobがCarolから秘密情報を使って AliceのHTLCロックトランザクションをアンロックする

* HTLCアンロックトランザクションを作成する
* HTLCアンロックトランザクションをブロードキャストする


```ruby
```


### Alice が秘密情報を見つける

* Aliceは、ブロックチェーンをモニタする
* 秘密情報のハッシュ値を含むHTLCアンロックトランザクションを見つけると
* トランザクションから秘密情報を得る


```ruby
```


### 秘密情報を得たAliceがHTLCアンロックトランザクションを作成する

* Alice がHTLCアンロックトランザクションをブロードキャストしてBobからのbitcoinの資金を得る


```ruby
```













































