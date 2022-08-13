# Tapyrus と Bitcoin のダブルチェーンワレットによるアトミックスワップ

最終更新 2022/08/11 Shigeichiro Yamasaki


[Tapyrus と Bitcoin の間でアトミックスワップ](./tapyrus アトミックスワップ.md)は、4つのターミナル間での操作が必要となり非常に煩雑でミスを誘発してしまいます。

これを避けるために、AliceとBob は、それぞれ Tapyrusとbitcoinの2つのチェーンに同時に接続しているワレットを持つことにします。
これによって、操作が非常に簡略化され、人間によるコピー、ペーストなども不要になります。

## 事前準備

ダブルチェーンワレットを簡単に実現する方法は、同じマシンに tapyrus core とbitcoin core を両方ともインストールし、ノードを稼働させてしまうことです。
Alice とBobで2台のマシンを用意することが理想的ですが、1台のマシンにAliceとBobの2つターミナルでアクセスして実施しても問題ありません。

### ダブルチェーンワレット

Tapyrus と bitcoin の2つのチェーンを同時に接続できるワレットを考えます。

これを使うとAlice と Bob は、それぞれ一つのワレットで操作できるようになります。

```
[Alice bitcoin  <=====  bitcoin Bob]
[Alice tapyrus  =====>  tapyrus Bob]
```

### レガシーワレットの作成

```bash
bitcoin-core.cli -named createwallet wallet_name=＜ワレット名＞ descriptors=false
```

### test用コインの入手

tapyrus , bitcoin ともに faucet からテスト用コインを入手しておきます

### アトミックスワップの流れ

1. Bobは、秘密情報のハッシュ値で bitcoin をロックした HTLCロックトランザクションを作成、ブロードキャストします。（bitcoinがロック状態になります）
2. Aliceも、そのハッシュ値で TPC をロックした HTLCロックトランザクションを作成し、ブロードキャストします。（TPCがロック状態になります）
3. Bobは、ロック状態のTPCをアンロックするために、秘密情報（ハッシュ値の原像）を埋め込んだ HTLCアンロックトランザクションを作成し、ブロードキャストします。これによってBobは Alice のTPCを入手します。
4. Aliceは、Bobによって TapyrusネットワークにブロードキャストされたHTLCアンロックトランザクションから、秘密情報（ハッシュ値の原像）を入手します。
5. Aliceは、秘密情報（ハッシュ値の原像）を使って bitcoin の HTLCアンロックトランザクションを作成し、ブロードキャストします。これによって Aliceは代金の bitcoin を入手します。

![](./images/tapyrus-swap01.png)

## 1 Alice の準備

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
#---------------------------
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
```

## 2 Bobの準備

Bob用ターミナルで実行

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
# -----------
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

## 3. BobがAliceに秘密情報のハッシュ値を作成

* Bobが秘密情報を生成する →　ここでは　'DAO24hChallenge'　とします

Bob

```ruby
# 秘密情報
secret = 'DAO24hChallenge'
# 秘密情報のハッシュ値
secret_hash=Bitcoin.sha256(secret).bth
```

## 4. Bob は Aliceへ bitcoin を送金しようとする

HTLCロックトランザクションでの送金

### unlocking script と redeem scriptの連接 (witness)

```
#  unlocking script 
<Aliceの署名> 
<Secret> 
OP_TRUE
------------連接--------------
# redeem script
OP_IF
    OP_SHA256 <Secretのハッシュ値> OP_EQUALVERIFY 
    <Aliceの公開鍵>
OP_ELSE
    <ロックするブロック数> OP_CSV 
    OP_DROP  
    <Bobの公開鍵>
OP_ENDIF
OP_CHECKSIG
```

### 4.1  HTLC ロックトランザクション作成メソッド　(bitcoin)

Bob ==> bitcoin ==> Alice

BobBC

```ruby
# addrAliceBC 送信先アドレス (Alice)
# addrBobBC 送信元アドレス (Bob)
# secret_hash 秘密情報のハッシュ値 
# pubkeyAliceBC Aliceの公開鍵
# pubkeyBobBC Bobの公開鍵
# deposit HTLCでロックする資金の金額
# fee　手数料
# lockDays ロック時間（日数）
# 未署名のbitcoinのHTLCロックトランザクションの生成
def unsigned_HTCL_lock_transactionBC(addrAliceBC, addrBobBC, secret_hash, pubkeyAliceBC, pubkeyBobBC, deposit, fee, lockDays)
    # 所持金残高を確認
    balance = bitcoinRPC('getbalance', [])
    if balance < (deposit +fee) then
        puts "error (残高不足)"
    else
        # 送金金額＋手数料をぎりぎり上回るUTXOリストの作成
        utxos = consuming_utxosBC(deposit + fee)
        # 送金に使用するUTXOの総額
        fund = utxos.map{|utxo|utxo["amount"]}.sum
        # UTXOの総額 - 送金金額 - 手数料 = おつり
        change = fund - deposit - fee
        # redeem script の生成
        redeem_scriptBC = create_redeem_scriptBC(secret_hash, pubkeyAliceBC, lockDays, pubkeyBobBC)
        # bitconトランザクションの構成
        tx, p2wshaddr = create_p2wsh_txBC(utxos, deposit, addrAliceBC, addrBobBC, change, redeem_scriptBC)
        return [tx, redeem_scriptBC, p2wshaddr]
    end
end
# 送金金額＋手数料をぎりぎり上回るUTXOリストの作成(witness_v0_keyhash のみ）
def consuming_utxosBC(amount)
    # ワレットの未使用のUTXOの一覧を得る
    unspent = bitcoinRPC('listunspent', [])
    # 消費可能状態のUTXOの選定
    spendable_utxos = unspent.select{|t|
        t["spendable"]==true and
        Bitcoin::Script.parse_from_payload(unspent[0]["scriptPubKey"].htb).type ==  "witness_v0_keyhash"
    }
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
# bitcoinのredeem scriptの生成
def create_redeem_scriptBC(secret_hash, pubkeyAliceBC, lockDays, pubkeyBobBC)
    # ロックする日数をブロック数（リトルエンディアン）に変換する
    locktime =  (6*24* lockDays).to_bn.to_s(2).reverse.bth
    return Bitcoin::Script.new << OP_IF << OP_SHA256 << secret_hash << OP_EQUALVERIFY << pubkeyAliceBC.htb << OP_ELSE << locktime << OP_CSV << OP_DROP << pubkeyBobBC.htb << OP_ENDIF << OP_CHECKSIG
end
# bitcon p2wshトランザクションの構成
def create_p2wsh_txBC(utxos, deposit, addrAliceBC, addrBobBC, change, redeem_scriptBC)
    # トランザクションテンプレートの生成
    tx = Bitcoin::Tx.new
    # トランザクションのinputの構成
    tx = make_inputsBC(tx, utxos)
    # トランザクションのoutputの構成
    tx, p2wshaddr = make_p2wsh_outputsBC(tx, deposit, addrBobBC, change, redeem_scriptBC)
    return [tx, p2wshaddr]
end
# トランザクションのinputの構成
def make_inputsBC(tx, utxos)
    utxos.each{|utxo|
        # UTXOをinputから参照するための txid と vout としてエンコードする
        outpoint = Bitcoin::OutPoint.from_txid(utxo["txid"], utxo["vout"])
        # エンコードした参照をトランザクションのinputに埋め込む
        tx.in << Bitcoin::TxIn.new(out_point: outpoint)
    }
    return tx
end
# トランザクションのoutputの構成
def make_p2wsh_outputsBC(tx, deposit, addrBobBC, change, redeem_scriptBC)
    # HTLCロックトランザクションのp2wshタイプの scriptPubKey
    scriptPubKey_p2wsh = Bitcoin::Script.from_string("0 #{redeem_scriptBC.to_sha256}")
    #   P2WSH アドレスの生成
    p2wshaddr = scriptPubKey_p2wsh.to_addr
    #   金額を satoshiの整数に変換
    deposit_satoshi = (deposit*(10**8)).to_i
    # デポジット用　P2WSH outputの作成
    tx.out << Bitcoin::TxOut.new(value: deposit_satoshi, script_pubkey:  Bitcoin::Script.parse_from_addr(p2wshaddr))
    # おつり用 scriptPubKey
    scriptPubKey_change = Bitcoin::Script.parse_from_addr(addrBobBC)
    #   金額を satoshiの整数に変換
    change_satoshi =  (change*(10**8)).to_i
    # おつり用　p2wpkh タイプのoutputの作成
    tx.out << Bitcoin::TxOut.new(value: change_satoshi, script_pubkey: scriptPubKey_change)
    return [tx, p2wshaddr]
end
```

### 4.2 未署名のHTLC ロックトランザクションの生成とredeem_scriptの生成 (bitcoin) 

BobBC

```ruby
# 未署名のHTLC ロックトランザクションの生成

utxBC, redeem_scriptBC, p2wshaddr = unsigned_HTCL_lock_transactionBC(addrAliceBC, addrBobBC, secret_hash, pubkeyAliceBC, pubkeyBobBC, 0.0003, 0.00002, 10)
```

### 4.3 HTLC ロックトランザクションへの署名 (bitcoin) 

```ruby
stxBC = (bitcoinRPC('signrawtransactionwithwallet',[utxBC.to_hex]))["hex"]
```

### 4.4 HTLC ロックトランザクションのブロードキャスト (bitcoin) 

BobBC

```ruby
# HTLC ロックトランザクションのブロードキャスト 
txidBC = bitcoinRPC('sendrawtransaction',[stxBC])
```
### 4.5 （上記をまとめた）HTLCロックトランザクションの送信メソッド (bitcoin)

```ruby
def send_HTLC_lock_transactionBC(addrAliceBC, addrBobBC, secret_hash, pubkeyAliceBC, pubkeyBobBC, deposit, fee, lockDays)
    utxBC, redeem_scriptBC, p2wshaddr = unsigned_HTCL_lock_transactionBC(addrAliceBC, addrBobBC, secret_hash, pubkeyAliceBC, pubkeyBobBC, deposit, fee, lockDays)
    stxBC = (bitcoinRPC('signrawtransactionwithwallet',[utxBC.to_hex]))["hex"]
    txidBC = bitcoinRPC('sendrawtransaction',[stxBC])
    return [txidBC, redeem_scriptBC.to_h[:asm], p2wshaddr]
end
# 実行
txidBC, redeem_scriptBC, p2wshaddrBC = send_HTLC_lock_transactionBC(addrAliceBC, addrBobBC, secret_hash, pubkeyAliceBC, pubkeyBobBC, 0.0003, 0.00002, 10)
```

## 5. Bob と Alice が相互に情報交換する

AliceとBobのアドレス、公開鍵

Bob ==> Alice: 秘密情報のハッシュ値、HTLCロックトランザクションのトランザクションID,redeem_script

### 5.1 Bobの bitcoin からAliceに情報を渡す

BobからAliceにわたす情報を文字列として一括して渡すためのメソッド

Bobのターミナルから

```ruby
def to_Alice(secret_hash, txidBC, redeem_scriptBC, p2wshaddrBC, pubkeyBobBC, pubkeyBobTP, addrBobBC, addrBobTP)
    puts <<-EOS
以下の代入文をAliceのターミナルにペーストして実行してください

secret_hash = '#{secret_hash}'
txidBC = '#{txidBC}'
redeem_scriptBC = '#{redeem_scriptBC}'
p2wshaddrBC = '#{p2wshaddrBC}'
pubkeyBobBC = '#{pubkeyBobBC}'
pubkeyBobTP = '#{pubkeyBobTP}'
addrBobBC = '#{addrBobBC}'
addrBobTP = '#{addrBobTP}'

    EOS
end
# 実行
to_Alice(secret_hash, txidBC, redeem_scriptBC, pubkeyBobBC, pubkeyBobTP, addrBobBC, addrBobTP)
```

Alice側で実行

```ruby
secret_hash = '23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15'
txidBC = '39ababc3adf8c8d8d61314a0525cc6113be66a10cd8b4dc7ae2cd9b840d466fd'
redeem_scriptBC = 'OP_IF OP_SHA256 23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15 OP_EQUALVERIFY 02c66ac59529fa1afd6638cf2b9d73853e0ad5192b8b7612a7e172bb8ed8bbccf6 OP_ELSE 1440 OP_CSV OP_DROP 031824b67681b831a1cdc800f8fe539510b5e1f15db1ded4b3d1f5ed5185db4e37 OP_ENDIF OP_CHECKSIG'
pubkeyBobBC = '031824b67681b831a1cdc800f8fe539510b5e1f15db1ded4b3d1f5ed5185db4e37'
pubkeyBobTP = '0340665efeaf6216a9a4f9573afb7b8eecdbff88bf2e49ade8dbb2969f30923087'
addrBobBC = 'tb1qqd37xr4q2dasqzj8q4c9n7sy7menvz4x7872tp'
addrBobTP = '1E49FfCRBVoEHgbjviZ4eoiK94PDFCs6Rr'
```


### 5.2 Alice から bitcoin に情報を渡す

Aliceのターミナルから

```ruby
def to_Bob(pubkeyAliceBC, pubkeyAliceTP, addrAliceBC, addrAliceTP)
    puts <<-EOS
以下の代入文をAliceのターミナルにペーストして実行してください

pubkeyAliceBC = '#{pubkeyAliceBC}'
pubkeyAliceTP = '#{pubkeyAliceTP}'
addrAliceBC = '#{addrAliceBC}'
addrAliceTP = '#{addrAliceTP}'

    EOS
end
# 実行
to_Bob(pubkeyAliceBC, pubkeyAliceTP, addrAliceBC, addrAliceTP)
```

Bob側で実行

```ruby
pubkeyAliceBC = '02c66ac59529fa1afd6638cf2b9d73853e0ad5192b8b7612a7e172bb8ed8bbccf6'
pubkeyAliceTP = '0255dc7852e0433a09f0a193701dbe49cd7d4a0bab9b3456038f9c9cf1f58b82cd'
addrAliceBC = 'tb1q9eclts2ns5s0cufywmk2zwd7x37hf0acrrc98v'
addrAliceTP = '1JSPS8sNbHJ9EryoWrrpeyV8tJ8SNRqfGR'
```

## 6. Alice からBobに TPC を送金しようとする (Tapyrus) 

### HTLC (Alice ==> Bob) 

HTLCロックトランザクションのredeem scriptと
HTLCアンロックトランザクションの scriptsig

```
#  scriptsig
<Bobの署名> 
<Secret> 
OP_TRUE
------------連接--------------
# redeem script
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

### 6.1  未署名HTLC ロックトランザクション作成メソッド (Tapyrus)

Alice側で実行する

```ruby
# key　鍵オブジェクト
# secret_hash 秘密情報のハッシュ値
# pubkey 送金先の公開鍵
# deposit HTLCでロックする資金の金額
# fee　手数料
# lockDays ロック時間
def unsigned_HTCL_lock_transactionTP(key, secret_hash, pubkeyBobTP,pubkeyAliceTP, deposit, fee, lockDays)
    balance = tapyrusRPC('getbalance',[])
    utxos= consuming_utxosTP(deposit + fee)
    unless (deposit > balance) or (utxos == []) then
        # <ロックするブロック数> 10日間のブロック数（リトルエンディアン）
        locktime = (6*24*lockDays).to_bn.to_s(2).reverse.bth
        # redeem script
        redeem_script = Tapyrus::Script.new << OP_IF << OP_SHA256 << secret_hash << OP_EQUALVERIFY << pubkeyAliceTP.htb << OP_ELSE << locktime << OP_CSV << OP_DROP << pubkeyBobTP.htb << OP_ENDIF << OP_CHECKSIG
        # HTLCロックトランザクションの scriptPubKey
        scriptPubKey_p2sh=redeem_script.to_p2sh
        # P2SH アドレスの生成
        p2shaddr = scriptPubKey_p2sh.addresses[0]
        # 使用するUTXOの総額
        fund = utxos.map{|utxo|utxo["amount"]}.sum
        # お釣り
        change= fund-deposit-fee
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
        tx.out << Tapyrus::TxOut.new(value: change_satoshi , script_pubkey:  Tapyrus::Script.parse_from_addr(key.to_p2pkh))
        return [tx, redeem_script.to_h[:asm], p2shaddr]
    else
        puts "you don't have enough funds for deposit"
    end
end
# 送金金額＋手数料をぎりぎり上回るUTXOリストの作成(witness_v0_keyhash のみ）
def consuming_utxosTP(amount)
    # ワレットの未使用のUTXOの一覧を得る
    unspent = tapyrusRPC('listunspent', [])
    # 消費可能状態のUTXOの選定
    spendable_utxos = unspent.select{|t|t["spendable"]}
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

### 6.2 未署名HTLC ロックトランザクションとredeem_scriptの生成 (tapyrus)

Alice

```ruby
# HTLC ロックトランザクションの生成
lockTxTP, redeem_scriptTP, p2shaddrTP  = unsigned_HTCL_lock_transactionTP(keyAliceTP, secret_hash, pubkeyBobTP, pubkeyAliceTP, 0.0005, 0.00002, 10)

# redeem_script
redeem_scriptTP
=> "OP_IF OP_SHA256 23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15 OP_EQUALVERIFY 03efdd1fee448d2dff78fd1eff959c28c36138c4eb2cfe3291d9a0fd2c104b9a73 OP_ELSE 1440 OP_CSV OP_DROP 02fc0056c8e097547608eab61e2effc3264f4444f80e2e9e6d834f3f8ac631736b OP_ENDIF OP_CHECKSIG"
```

### 6.2 HTLC ロックトランザクションへの署名 (tapyrus)

Alice

```ruby
signedTx = (tapyrusRPC('signrawtransactionwithwallet',[lockTxTP.to_hex]))["hex"]
```

### 6.3 HTLC ロックトランザクションのブロードキャスト (tapyrus)

Alice

```ruby
# HTLC ロックトランザクションのブロードキャスト
lockTx_txidTP = tapyrusRPC('sendrawtransaction',[signedTx])
```

### 6.4 以上をまとめたHTLCロックトランザクション送信メソッド (Tapyrus)

Alice 

```ruby
def send_HTLC_lock_transactionTP(keyAliceTP, addrAliceTP, addrBobTP, secret_hash, pubkeyAliceTP, pubkeyBobTP, deposit, fee, lockDays)
    lockTxTP, redeem_scriptTP, p2shaddrTP  = unsigned_HTCL_lock_transactionTP(keyAliceTP, secret_hash, pubkeyBobTP, pubkeyAliceTP, deposit, fee, lockDays)
    signedTx = (tapyrusRPC('signrawtransactionwithwallet',[lockTxTP.to_hex]))["hex"]
    lockTx_txidTP = tapyrusRPC('sendrawtransaction',[signedTx])
    return [lockTx_txidTP, redeem_scriptTP, p2shaddrTP]
end
# 実行
lockTx_txidTP, redeem_scriptTP, p2shaddrTP = send_HTLC_lock_transactionTP(keyAliceTP, addrAliceTP, addrBobTP, secret_hash, pubkeyAliceTP, pubkeyBobTP, 0.0003, 0.00002, 10)
```

### 6.5 Alice が Bob に redeem_scriptとトランザクションIDを渡す (Tapyrus)

Alice

```ruby
def to_Bob(redeem_scriptTP, lockTx_txidTP, p2shaddrTP)
    puts <<-EOS
以下の代入文を Bob のターミナルにペーストして実行してください

redeem_scriptTP = '#{redeem_scriptTP}'
lockTx_txidTP = '#{lockTx_txidTP}'
p2shaddrTP = '#{p2shaddrTP}'

    EOS
end
# 実行
to_Bob(redeem_scriptTP, lockTx_txidTP, p2shaddrTP)
```

Bob側で実行

```ruby
redeem_scriptTP = 'OP_IF OP_SHA256 23ecad109469e17c8b4dcaaacbd0d71ef9841bac75d8f21917c1b038e1602c15 OP_EQUALVERIFY 0340665efeaf6216a9a4f9573afb7b8eecdbff88bf2e49ade8dbb2969f30923087 OP_ELSE 1440 OP_CSV OP_DROP 0255dc7852e0433a09f0a193701dbe49cd7d4a0bab9b3456038f9c9cf1f58b82cd OP_ENDIF OP_CHECKSIG'
lockTx_txidTP = '26c2944d6f71a1899b7a371f63b7e991ba8967736dec64c5404586ca8ee7fabe'
p2shaddrTP = '32QTHdSbNvCWF9oyk1VdcQDrNp9ccESJ5A'
```

## 7. Bobが秘密情報を使って AliceのHTLCロックトランザクションをアンロックする (Tapyrus)

### HTLC (Alice ==> Bob) 

HTLCロックトランザクションのredeem scriptと
HTLCアンロックトランザクションの scriptsig

```
#  scriptsig
<Bobの署名> 
<Secret> 
OP_TRUE
------------連接--------------
# redeem script
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

### 7.1 HTLCアンロックトランザクション作成メソッド(Tapyrus)

BobTP

```ruby
# secret : 秘密情報（Bobは知っている）
# redeem_scriptTP  : Alice の HTLC ロックトランザクションの redeem_script 
# lockTx_txidTP  : HTLCロックトランザクションの トランザクションID 
# p2shaddrTP : P2SHアドレス
```

HTLCアンロックトランザクション作成メソッド(Tapyrus)

```ruby
def unlock_HTLC_transactionTP(secret, redeem_scriptTP, p2shaddrTP, lockTx_txidTP, addrBobTP, key, fee)
    # HTLCロックトランザクションを得る
    htlc_tx = tapyrusRPC('getrawtransaction',[lockTx_txidTP, 2])
    # HTLCロックトランザクションの scriptPubKey
    htlc_scriptPubKey = Tapyrus::Script.parse_from_addr(p2shaddrTP)
    # redeem scriptの復元
    redeem_script = Tapyrus::Script.from_string(redeem_scriptTP)
    # HTLC のデポジット金額
    deposit=htlc_tx["vout"][0]["value"]
    # HTLCのアンロックによる報酬金額
    reward = deposit-fee
    reward_satoshi = (reward* (10**8)).to_i
    # トランザクションテンプレートの生成
    tx = Tapyrus::Tx.new
    # inputの作成
    outpoint = Tapyrus::OutPoint.from_txid(lockTx_txidTP, 0)
    tx.in << Tapyrus::TxIn.new(out_point: outpoint)
    # outputの作成 報酬用のP2PKH
    script_pubkey = Tapyrus::Script.parse_from_addr(addrBobTP)
    tx.out << Tapyrus::TxOut.new(value: reward_satoshi, script_pubkey: script_pubkey)
    # sighashを作成
    sighash = tx.sighash_for_input(0, redeem_script)
    # SHIGHASH_TYPE ALLでBobが署名
    signature = key.sign(sighash) + [Tapyrus::SIGHASH_TYPE[:all]].pack('C')
    # inputへの埋め込み
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
unlock_txTP = unlock_HTLC_transactionTP(secret,redeem_scriptTP, p2shaddrTP , lockTx_txidTP, addrBobTP,keyBobTP, 0.00002)
```

### 7.3 BobがHTLC アンロックトランザクションをブロードキャスト (Tapyrus)

BobTP

```ruby
# HTLC アンロックトランザクションのブロードキャスト
unlockTx_txid = tapyrusRPC('sendrawtransaction',[unlock_txTP.to_hex])

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

    htlc_scriptPubKey = Tapyrus::Script.parse_from_addr(p2wshaddr)
    # アンロックの対象となるUTXO のvout
    htcl_lockTx_vout=0
    # redeem script
    redeem_script = Bitcoin::Script.from_string(redeem_scriptBC)
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
    # SHIGHASH_TYPE ALLでAliceが署名
    sig = key.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
    tx.in[0].script_witness.stack << sig
    # witness scriptの追加
    tx.in[0].script_witness.stack << secret
    tx.in[0].script_witness.stack << [1].pack("C")
    tx.in[0].script_witness.stack << redeem_script.to_payload
    return tx
end

```

### HTLC アンロックトランザクションの生成 (bitcoin)

```ruby
secret = 'DAO24hChallenge'
unlock_tx = unlock_HTLC_transaction(secret, redeem_scriptBC, lockTx_txidBC, keyAliceBC, 0.00002)
```

### HTLC アンロックトランザクションのブロードキャスト (bitcoin)

```ruby
htcl_unlockTx_txid = bitcoinRPC('sendrawtransaction',[unlock_tx.to_hex])
htcl_unlockTx_txid
=> 
"4df0f17d418c1e913369e5cdf83c6f047a4156274a2336ffc282837187a30fd2"
```




