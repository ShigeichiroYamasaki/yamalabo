# Tapyrus 投票ワレット

最終更新 2022/12/24 Shigeichiro Yamasaki

tapyrusrb を使ってP2SH の送金と受領を行うトランザクションを作成する

投票トークンを取得するためのスクリプトとしては、修正版HTLCを使う．

### 修正版HTLCの問題点

修正版HTLCのロックトランザクションをアンロックして投票トークンを得るためには ロックトランザクションのUTXOのtxid vout とハッシュ値の原像のペアを知っている必要がある．

選挙管理者は，メンバーからハッシュ値をもらってロックトランザクションを生成し， そのUTXOのtxidをメンバーに返す．つまり，選挙管理者はメンバーとUTXOの対応を知っている．
このため，アンロックしたアドレスが新たに生成したものでもどのメンバーのものかわかってしまう．

したがって，投票のプライバシの観点から選挙管理者に対するブラインド化などの措置が必要だが，今回はブロックチェーンの解析による追跡を防ぐだけにする．選挙管理委員会に対するブラインド化は必要だが，今後の課題とする．

### 修正版HTLCの利点

修正版HTLCでは，新規に生成されたアドレスで投票トークンを取得し，そのアドレスから投票できるので，ブロックチェーンの解析だけで投票者を知ることは基本的にできない．

### 修正版HTLCのunlocking script

ハッシュの原像を示せば，誰でも自分宛てに資金を送金できる

* 通常のHTLC

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

* 修正版HTLC

ハッシュ値の原像を知っていれば，だれでも（どのアドレスを使っても）資金をアンロックできる


```
        [HTLC の scriptSig]
<Bobの署名> 
<Bobの公開鍵>
<Secret> 
OP_TRUE
------------連接--------------
       [HTLCの redeem script] 
OP_IF
    OP_SHA256 <Secretのハッシュ値> OP_EQUALVERIFY 
OP_ELSE
    <ロックするブロック数> OP_CSV 
    OP_DROP  
    <Aliceの公開鍵>
OP_ENDIF
OP_CHECKSIG
```

### Tapyrus のカラー付きUTXO

カラー付きUTXOの構成は，scriptPubKey のスクリプトの先頭に <カラー識別子> OP_COLOR を記述することで実現できる

* CP2PKH (Colored P2PKH)

scriptPubKey

```
<カラー識別子> 
OP_COLOR 
OP_DUP 
OP_HASH160 
<公開鍵のハッシュ値> 
OP_EQUALVERIFY 
OP_CHECKSIG
```

* CP2SH (Colored P2SH)

scriptPubKey

```
<カラー識別子> 
OP_COLOR 
OP_HASH160 
<[redeem script]のハッシュ値>
OP_EQUAL
```
#### カラー識別子

| タイプ | トークン種別 | ペイロード|
| :--: | :--: | :--|
| 0xC1 | RT | 発行inputのscriptPubKeyのSHA256ハッシュ値|
| 0xC2 | NRT| 発行inputのoutpointのSHA256ハッシュ値|
| 0xC3| NFT | 発行inputのoutpointのSHA256ハッシュ値|

同一の発行inputのoutpointのSHA256ハッシュ値を複数作ることができないので，NRTやNFTを再発行することはできない．

### カラー付きアドレス

一般的に，トークン送信者が送信先のP2PKHアドレスやP2SHアドレスをもとに カラー付きアドレスを生成します．

* バージョンバイト

|タイプ	|prod|	dev|
|:--:|:--:|:--:|
|CP2PKH	|0x01|	0x70|
|CP2SH|	0x06|	0xc5|

* ペイロード


```
CP2PKH: <Color識別子（33バイト）>|<公開鍵ハッシュ（20バイト）>
CP2SH: <Color識別子（33バイト）>|<スクリプトハッシュ（20バイト）>
```

## tapyrusrb トークンHTLC の基本

* 資金（投票トークン）をHTLCでロックしておく
* ハッシュの原像を使ってロックしたトークンを取り出す
* ハッシュの原像を "Yamalabo DAO member Alice" とする

### トランザクション構成の基本部分の生成

```ruby
require 'tapyrus'
require 'json'
require "open3"

include Tapyrus
include Tapyrus::Opcodes
FEE = 0.00002       # 手数料
Tapyrus.chain_params = :prod

# tapyrus-cli コマンドのフルパス
Tapyrus_cli ='~/tapyrus-core-0.5.1/bin/tapyrus-cli'

# RPC
def tapyrusRPC(method,params)
    r=Open3.capture3("#{Tapyrus_cli} #{method} #{params.join(' ')}")
    if r[1] == "" then
        begin
            return JSON.parse(r[0])
        rescue JSON::ParserError
            return r[0]
        end
    else
        puts r[1]
    end
end

# テスト用アカウントと鍵
mnemonic = Tapyrus::Mnemonic.new('english')
word_list =["ozone", "bounce", "hurdle", "weird", "token", "exclude", "remember", "swear", "knife", "bitter", "blossom", "horn", "repair", "aspect", "girl", "merit", "palace", "boring", "pottery", "relax", "sunset", "lucky", "elephant", "ticket"]
seed = mnemonic.to_seed(word_list)
master_key = Tapyrus::ExtKey.generate_master(seed)
# derive path 'm/0H'
key = master_key.derive(0, true)
#  鍵オブジェクト
keyAlice = key.derive(1)
keyBob   = key.derive(2)
keyCarol = key.derive(3)
keyDavid = key.derive(4)
keyErza  = key.derive(5)
keyFranck = key.derive(6)

# テスト用の秘密鍵
priv_alice = keyAlice.priv
priv_bob   = keyBob.priv
priv_carol = keyCarol.priv
priv_erza  = keyErza.priv
priv_franck = keyFranck.priv

## アドレス
alice = keyAlice.addr
bob   = keyBob.addr
carol = keyCarol.addr
david = keyDavid.addr
erza  = keyErza.addr
franck = keyFranck.addr

# 公開鍵
pub_alice = keyAlice.pub
pub_bob   = keyBob.pub
pub_carol = keyCarol.pub
pub_david = keyDavid.pub
pub_erza  = keyErza.pub
pub_franck = keyFranck.pub

# 自分のワレットの秘密鍵をインポートする
tapyrusRPC('importprivkey',[keyAlice.key.to_wif])
tapyrusRPC('importprivkey',[keyBob.key.to_wif])
tapyrusRPC('importprivkey',[keyCarol.key.to_wif])
tapyrusRPC('importprivkey',[keyDavid.key.to_wif])
tapyrusRPC('importprivkey',[keyErza.key.to_wif])
tapyrusRPC('importprivkey',[keyFranck.key.to_wif])

#####################
# トランザクション構成の基本部分
#
# 送金先アドレス、送金金額，おつりアドレス
def send_tapyrus(addr, amount, addr_change)
    # 所持金残高を確認
    balance = tapyrusRPC('getbalance', [])
    if balance < (amount+FEE) then
        puts "error (残高不足)"
    else
        # 送金金額＋手数料をぎりぎり上回るUTXOリストの作成
        utxos = consuming_utxos(amount+FEE)
        # 送金に使用するUTXOの総額
        fund = utxos.map{|utxo|utxo["amount"]}.sum
        # UTXOの総額 - 送金金額 - 手数料 = おつり
        change = fund-amount-FEE
        # トランザクションの構成（P2PKH)
        tx = p2pkh_tx(addr, amount, utxos, change, addr_change)
        # トランザクションへの署名
        tx = sign_inputs(utxos, tx)
        # ビットコインネットワークへのデプロイ
        return tapyrusRPC('sendrawtransaction', [tx.to_hex])
    end
end
# 送金金額＋手数料をぎりぎり上回るUTXOリストの作成
def consuming_utxos(amount)
    # ワレットの未使用のUTXOの一覧を得る
    unspent = tapyrusRPC('listunspent', [])
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
# トランザクションのinputの構成
def tx_inputs(tx, utxos)
    utxos.each{|utxo|
        # UTXOをinputから参照するための txid と vout としてエンコードする
        outpoint = Tapyrus::OutPoint.from_txid(utxo["txid"], utxo["vout"])
        # エンコードした参照をトランザクションのinputに埋め込む
        tx.in << Tapyrus::TxIn.new(out_point: outpoint)
    }
    return tx
end
# トランザクションのoutputの構成
def tx_outputs(tx,amount, addr, change, addr_change)
    # 送金用outputの構成
    # 金額を satoshiの整数に変換
    amount_satoshi = (amount*(10**8)).to_i
    # ビットコインアドレスから p2pkhのscript_pubkey を生成
    scriptPubKey0 = Tapyrus::Script.parse_from_addr(addr)
    # エンコードしたscript_pubkeyをトランザクションのoutputに埋め込む
    tx.out << Tapyrus::TxOut.new(value: amount_satoshi , script_pubkey: scriptPubKey0)
    # おつり用outputの構成
    # 金額を satoshiの整数に変換
    change_satoshi =  (change*(10**8)).to_i
    # ビットコインアドレスから p2wpkhのscript_pubkey を生成
    scriptPubKey1 = Tapyrus::Script.parse_from_addr(addr_change)
    # エンコードしたscript_pubkeyをトランザクションのoutputに埋め込む
    tx.out << Tapyrus::TxOut.new(value: change_satoshi, script_pubkey: scriptPubKey1)
    return tx
end

# P2PKHトランザクションの構成
def p2pkh_tx(addr,amount, utxos, change, addr_change)
    # トランザクションのテンプレートの生成
    tx = Tapyrus::Tx.new
    # トランザクションのinputの構成
    tx = tx_inputs(tx,utxos)
    # トランザクションのoutputの構成
    tx = tx_outputs(tx,amount, addr, change, addr_change)
    return tx
end
# トランザクションへの署名
def sign_inputs(utxos, tx)
    utxos.each.with_index{|utxo,index|
        # UTXOのscriptPubKey をオブジェクト化する
        script_pubkey = Tapyrus::Script.parse_from_payload(utxo["scriptPubKey"].htb)
        # scriptPubKey の送金先アドレス
        myaddr = script_pubkey.to_addr
        # UTXOの送付先アドレスの秘密鍵（署名鍵）
        priv = tapyrusRPC('dumpprivkey', [myaddr]).chomp
        # 署名鍵オブジェクト
        key = Tapyrus::Key.from_wif(priv)
        # UTXOの金額
        satoshi = (utxo["amount"]*(10**8)).to_i
        case script_pubkey.type
        when "pubkeyhash"   # UTXOがP2PKHタイプ
            # トランザクションのハッシュ値を計算
            sighash = tx.sighash_for_input(index, script_pubkey)
            # トランザクションへの署名＋署名タイプ情報を付加
            sig = key.sign(sighash) + [Tapyrus::SIGHASH_TYPE[:all]].pack('C')
            # inputへの署名の追加
            tx.in[index].script_sig << sig
            # inputへの公開鍵の追加
            tx.in[index].script_sig << key.pubkey.htb
        end
    }
    return tx
end
```

#### P2PKH の送金テスト

```ruby
# 送金テスト
txid = send_tapyrus(bob, 0.0001, alice).chomp
```



### 修正版HTLCによる投票トークンのロック(TPC)

```ruby
#####################################
# 秘密情報
secret = "Yamalabo DAO member Alice"
secret_hash=Tapyrus.sha256(secret)
# 原像を知っていれば誰でもアンロックできるHTLC
# secret_hash 秘密情報のハッシュ値
# pub_sender 送金者の公開鍵
# addr_sender 送金者のアドレス
# pub_receiver 受領者の公開鍵
# lock_days　ロックする日数

def send_htlc_au(amount, addr_change, secret_hash, pub_sender, addr_sender, lock_days)
    # 所持金残高を確認
    balance = tapyrusRPC('getbalance', [])
    if balance < (amount+FEE) then
        puts "error (残高不足)"
    else
        # 送金金額＋手数料をぎりぎり上回るUTXOリストの作成
        utxos = consuming_utxos(amount+FEE)
        # 送金に使用するUTXOの総額
        fund = utxos.map{|utxo|utxo["amount"]}.sum
        # UTXOの総額 - 送金金額 - 手数料 = おつり
        change = fund-amount-FEE
        # redeem scriptの生成
        locktime = (6*24*lock_days).to_bn.to_s(2).reverse.bth
        redeem_script = Tapyrus::Script.new << OP_IF << OP_SHA256 << secret_hash.bth << OP_EQUALVERIFY << OP_ELSE << locktime << OP_CSV << OP_DROP << pub_sender << OP_ENDIF << OP_CHECKSIG
        # トランザクションの構成（P2SH)
        tx = p2sh_tx(utxos, amount, redeem_script, addr_change)
        # トランザクションへの署名
        tx = sign_inputs(utxos, tx)
        # トランザクションのデプロイ
        txid = tapyrusRPC('sendrawtransaction',[tx.to_hex])
        # p2shトランザクションのアンロックに必要な情報の出力
        return tx, txid.chomp, redeem_script.to_hex
    end
end
# p2sh 未署名トランザクションの構成
def p2sh_tx(utxos, amount, redeem_script, addr_change)
    # おつり = UTXOの総額 - 送金金額 - 手数料
    change = (utxos.map{|utxo|utxo["amount"]}.sum)-amount-FEE
    # トランザクションのスケルトン
    tx = Tapyrus::Tx.new
    # トランザクションのinputの構成
    tx = tx_inputs(tx,utxos)
    # P2SHトランザクションのoutputの構成
    tx = p2sh_outputs(tx, amount, redeem_script, change, addr_change)
    return tx
end
# p2shトランザクションのoutputの構成
def p2sh_outputs(tx, amount, redeem_script, change, addr_change)
    # satoshi変換
    change_satoshi = (change*(10**8)).to_i
    amount_satoshi = (amount*(10**8)).to_i
    # redeem_script を p2shアドレスに変換する
    p2sh_addr = redeem_script.to_p2sh.addresses[0]
    #  p2shアドレスからscript pubkey を生成
    scriptPubKey0 = Tapyrus::Script.parse_from_addr(p2sh_addr)
    # 作成したscript pubkey outputに設定する
    tx.out << Tapyrus::TxOut.new(value: amount_satoshi, script_pubkey: scriptPubKey0)
    # おつり用script pub key の構成(P2PKH)
    scriptPubKey1 = Tapyrus::Script.parse_from_addr(addr_change)
    # おつり用のoutput
    tx.out << Tapyrus::TxOut.new(value: change_satoshi, script_pubkey: scriptPubKey1)
    return tx
end
```

#### 修正版HTLCトランザクションの送金テスト

```ruby
amount=0.001
addr_change=bob
secret = "Yamalabo DAO member Alice"
secret_hash=Tapyrus.sha256(secret)
pub_sender = pub_alice
addr_sender = alice
pub_receiver = pub_bob
lock_days =100

htlc_tx, htlc_txid, redeem_script_hex = send_htlc_au(amount, addr_change, secret_hash, pub_sender, addr_sender, lock_days)
```

### 修正HTLC アンロックトランザクションの構成


共通部分は省略


### 修正HTLC アンロックトランザクション


```ruby
#  アンロックする主体に渡す情報を代入文の文字列にしたもの
# p2sh でロックされたトランザクションのtxid
htlc_txid 
# redeem script の16進数形式
redeem_script_hex
# アンロックした資金は addr = bob に送金するものとする

def unlock_htlc_au(htlc_txid, redeem_script_hex, pub_addr, addr, key, secret)
    # 16進数形式redeem script の復元
    redeem_script = Tapyrus::Script.parse_from_payload(redeem_script_hex.htb)
    # アンロック対象トランザクションとUTXOを確定する
    locked_tx = Tapyrus::Tx.parse_from_payload(tapyrusRPC('getrawtransaction',[htlc_txid]).htb)
    # ロックされているUTXO  0がp2shであることがわかっている
    p2sh_utxo = locked_tx.out[0]
    utxo_value = p2sh_utxo.value    # この金額の単位は satoshi
    # P2SH のUTXOのoutpoint
    outpoint = Tapyrus::OutPoint.from_txid(htlc_txid, 0)
    # アンロックトランザクションの構成（送金先はaliceとする）
    tx = Tapyrus::Tx.new
    # inputの構成
    # P2SH のoutputをinputにする
    tx.in <<  Tapyrus::TxIn.new(out_point: outpoint)
    #output の構成 (P2PKH) アドレスへ送金
    scriptPubKey0 = Tapyrus::Script.parse_from_addr(addr)
    # script_pubkey0をトランザクションのoutputに埋め込む
    tx.out << Tapyrus::TxOut.new(value: utxo_value-(FEE*(10**8)).to_i , script_pubkey: scriptPubKey0)
    # アンロックトランザクションの署名対象のハッシュ値 sighash
    sighash = tx.sighash_for_input(0, redeem_script, hash_type: Tapyrus::SIGHASH_TYPE[:all])
    # scriptsig の追加
    sig = key.key.sign(sighash) + [Tapyrus::SIGHASH_TYPE[:all]].pack('C')
    # <Bobの署名> 
    # <Bobの公開鍵>
    # <Secret> 
    # OP_TRUE
    tx.in[0].script_sig << sig
    tx.in[0].script_sig << pub_addr
    tx.in[0].script_sig << secret.bth
    tx.in[0].script_sig << OP_1
    tx.in[0].script_sig << redeem_script.to_payload
    # 署名したトランザクションをブロードキャストする
    p2sh_txid = tapyrusRPC('sendrawtransaction', [tx.to_hex])
    return p2sh_txid, tx
end
```

#### 修正HTLCのアンロック

```ruby
addr = bob
pub_addr = pub_bob
key = keyBob
secret = "Yamalabo DAO member Alice"
unlock_htlc_txid, unlock_htlc_tx = unlock_htlc_au(htlc_txid, redeem_script_hex, pub_addr, addr, key, secret)      
```

## Non-Reissuable Token による投票トークンの発行

#### Bobによる子鍵の生成

```ruby
# トークン専用の鍵
key_for_token = master_key.derive(1, true)
```

送金テストに使用したUTXOを利用する（したがって10分経過後に以下を実施）

```ruby
def mint_NRT(amount,txid)
    # NRTの発行
    nrt = tapyrusRPC('issuetoken',[2, amount, txid, 0])
end

# TXID をひとつ
txid = utxos.map{|x|x["txid"]}[0]


# 投票トークンを10000トークン発行
nrt=mint_NRT(10000,txid)

# 投票トークンのカラー
vtoken = nrt["color"]

=> "c253f956581350856d63ddceb82c19ae27707b335309a5cfab76fa98b7cdfe2db2"
```

### 各メンバーによる自分のカラー付きアドレスの生成（投票トークン用）

```ruby
voter1 = tapyrusRPC('getnewaddress',["voter1", vtoken]).chomp

=> "vq3m89LT4NxGMiKP1u1Qd5f9nJMmmmjBuKRTJjs9ySBr1ziGASUtczUB2znq8uWdpTejBYJPzvVzup"

voter2 = tapyrusRPC('getnewaddress',["voter2", vtoken]).chomp

=> "vq3m89LT4NxGMiKP1u1Qd5f9nJMmmmjBuKRTJjs9ySBr1uskeQhquz6hwQHrMZtEuC176V3mDaBeVV"
```

### カラー付きアドレスへ 1 トークン送付

```ruby
c_txid1 = tapyrusRPC('transfertoken',[voter1,1]).chomp
```


### トークンの残高

amount が1 と 9999 の2つになっていることを確認

```ruby
tapyrusRPC('listunspent',[]).select{|x|x["token"]== vtoken}

=> 
[{"txid"=>"4fbf87c7b9452725d527c15249fcd8f5fe779e346260ea87b4443ad754c12d5e",
  "vout"=>0,
  "address"=>"vshtZw56YGk7UbKnVE37gy7mSyL58xLAxQY4a48DYb2vHgymFGdk99Xxfvsr41zd6QkrX9VNyuEpMn",
  "token"=>"c2b6199605e897800b4340a42e6cc46f020c57f73dac6003e5a80c8c4910f9cb5a",
  "amount"=>1,
  "label"=>"voter1",
  "scriptPubKey"=>
   "21c2b6199605e897800b4340a42e6cc46f020c57f73dac6003e5a80c8c4910f9cb5abc76a914ef5492849186e02e18f341c02ae8b5fbd6fb7e0488ac",
  "confirmations"=>1,
  "spendable"=>true,
  "solvable"=>true,
  "safe"=>true},
 {"txid"=>"4fbf87c7b9452725d527c15249fcd8f5fe779e346260ea87b4443ad754c12d5e",
  "vout"=>1,
  "address"=>"vshtZw56YGk7UbKnVE37gy7mSyL58xLAxQY4a48DYb2vHT7rQywCt8xxLd1G4NqF1KtLZnd1TjdWQp",
  "token"=>"c2b6199605e897800b4340a42e6cc46f020c57f73dac6003e5a80c8c4910f9cb5a",
  "amount"=>9999,
  "scriptPubKey"=>
   "21c2b6199605e897800b4340a42e6cc46f020c57f73dac6003e5a80c8c4910f9cb5abc76a914574a73df5657dacfa24ea40200a773bfcf99ddd588ac",
  "confirmations"=>1,
  "spendable"=>true,
  "solvable"=>true,
  "safe"=>true}]

```

### トークン送付トランザクションの内容を確認する

scriptPubKey に OP_COLOR があることを確認する

```ruby
tapyrusRPC('getrawtransaction',[txid,1])

=> 
{"txid"=>"4fbf87c7b9452725d527c15249fcd8f5fe779e346260ea87b4443ad754c12d5e",
 "hash"=>"a8c70b0f741b36131ca648a5a75c3148624a963791dbe0d4a52f6c4198bd1c55",
 "features"=>1,
 "size"=>543,
 "locktime"=>261592,
 "vin"=>
  [{"txid"=>"d4ef0e205ec2c42d4e9c38e3d35252a5a28a96ed6714bd22571a489d2ee31a41",
    "vout"=>1,
    "scriptSig"=>
     {"asm"=>
       "304402204d70e461f2ba4f0e993a2d6c6a09622339ac0ef6aba02234c54ec4018f1f4cbe0220131aec4dc7e0a3d166ba61cd4f4605eb6235e8b40e1050e148a4057e9bef651a[ALL] 032f04706a999dc831bacf23f84554b0eb026a79bd76a204449a91401d963e7292",
      "hex"=>
       "47304402204d70e461f2ba4f0e993a2d6c6a09622339ac0ef6aba02234c54ec4018f1f4cbe0220131aec4dc7e0a3d166ba61cd4f4605eb6235e8b40e1050e148a4057e9bef651a0121032f04706a999dc831bacf23f84554b0eb026a79bd76a204449a91401d963e7292"},
    "sequence"=>4294967294},
   {"txid"=>"9ab8623d9b0979bc20296f5bc80cc010bfb71a54321aa92d432d22883cbe4c02",
    "vout"=>0,
    "scriptSig"=>
     {"asm"=>
       "30440220054872cfe455a1fa061d92db732f9c3bb7125eff15ba8caa78896ace36411d9d02201220fe95ed522b1313bf5b3c73d15c0d3ed5e529c3a7abb0e41b9ac4c2a09357[ALL] 032f04706a999dc831bacf23f84554b0eb026a79bd76a204449a91401d963e7292",
      "hex"=>
       "4730440220054872cfe455a1fa061d92db732f9c3bb7125eff15ba8caa78896ace36411d9d02201220fe95ed522b1313bf5b3c73d15c0d3ed5e529c3a7abb0e41b9ac4c2a093570121032f04706a999dc831bacf23f84554b0eb026a79bd76a204449a91401d963e7292"},
    "sequence"=>4294967294},
   {"txid"=>"b3a197f1bb4cc14a18cf8ca38955a2afec3c5281575886c77e15db222524859f",
    "vout"=>0,
    "scriptSig"=>
     {"asm"=>"76a91460ab54d4b65157bddf89216a40d210f48e981c5588ac",
      "hex"=>"1976a91460ab54d4b65157bddf89216a40d210f48e981c5588ac"},
    "sequence"=>4294967294}],
 "vout"=>
  [{"token"=>"c2b6199605e897800b4340a42e6cc46f020c57f73dac6003e5a80c8c4910f9cb5a",
    "value"=>1,
    "n"=>0,
    "scriptPubKey"=>
     {"asm"=>
       "c2b6199605e897800b4340a42e6cc46f020c57f73dac6003e5a80c8c4910f9cb5a OP_COLOR OP_DUP OP_HASH160 ef5492849186e02e18f341c02ae8b5fbd6fb7e04 OP_EQUALVERIFY OP_CHECKSIG",
      "hex"=>
       "21c2b6199605e897800b4340a42e6cc46f020c57f73dac6003e5a80c8c4910f9cb5abc76a914ef5492849186e02e18f341c02ae8b5fbd6fb7e0488ac",
      "reqSigs"=>1,
      "type"=>"coloredpubkeyhash",
      "addresses"=>["vshtZw56YGk7UbKnVE37gy7mSyL58xLAxQY4a48DYb2vHgymFGdk99Xxfvsr41zd6QkrX9VNyuEpMn"]}},
   {"token"=>"c2b6199605e897800b4340a42e6cc46f020c57f73dac6003e5a80c8c4910f9cb5a",
    "value"=>9999,
    "n"=>1,
    "scriptPubKey"=>
     {"asm"=>
       "c2b6199605e897800b4340a42e6cc46f020c57f73dac6003e5a80c8c4910f9cb5a OP_COLOR OP_DUP OP_HASH160 574a73df5657dacfa24ea40200a773bfcf99ddd5 OP_EQUALVERIFY OP_CHECKSIG",
      "hex"=>
       "21c2b6199605e897800b4340a42e6cc46f020c57f73dac6003e5a80c8c4910f9cb5abc76a914574a73df5657dacfa24ea40200a773bfcf99ddd588ac",
      "reqSigs"=>1,
      "type"=>"coloredpubkeyhash",
      "addresses"=>["vshtZw56YGk7UbKnVE37gy7mSyL58xLAxQY4a48DYb2vHT7rQywCt8xxLd1G4NqF1KtLZnd1TjdWQp"]}},
   {"token"=>"TPC",
    "value"=>0.01006397,
    "n"=>2,
    "scriptPubKey"=>
     {"asm"=>"OP_DUP OP_HASH160 574a73df5657dacfa24ea40200a773bfcf99ddd5 OP_EQUALVERIFY OP_CHECKSIG",
      "hex"=>"76a914574a73df5657dacfa24ea40200a773bfcf99ddd588ac",
      "reqSigs"=>1,
      "type"=>"pubkeyhash",
      "addresses"=>["18xYykQjFPcByUKvLcxTSqkKLiPGz9e6DJ"]}}],
 "hex"=>
  "0100000003411ae32e9d481a5722bd1467ed968aa2a55252d3e3389c4e2dc4c25e200eefd4010000006a47304402204d70e461f2ba4f0e993a2d6c6a09622339ac0ef6aba02234c54ec4018f1f4cbe0220131aec4dc7e0a3d166ba61cd4f4605eb6235e8b40e1050e148a4057e9bef651a0121032f04706a999dc831bacf23f84554b0eb026a79bd76a204449a91401d963e7292feffffff024cbe3c88222d432da91a32541ab7bf10c00cc85b6f2920bc79099b3d62b89a000000006a4730440220054872cfe455a1fa061d92db732f9c3bb7125eff15ba8caa78896ace36411d9d02201220fe95ed522b1313bf5b3c73d15c0d3ed5e529c3a7abb0e41b9ac4c2a093570121032f04706a999dc831bacf23f84554b0eb026a79bd76a204449a91401d963e7292feffffff9f85242522db157ec786585781523cecafa25589a38ccf184ac14cbbf197a1b3000000001a1976a91460ab54d4b65157bddf89216a40d210f48e981c5588acfeffffff0301000000000000003c21c2b6199605e897800b4340a42e6cc46f020c57f73dac6003e5a80c8c4910f9cb5abc76a914ef5492849186e02e18f341c02ae8b5fbd6fb7e0488ac0f270000000000003c21c2b6199605e897800b4340a42e6cc46f020c57f73dac6003e5a80c8c4910f9cb5abc76a914574a73df5657dacfa24ea40200a773bfcf99ddd588ac3d5b0f00000000001976a914574a73df5657dacfa24ea40200a773bfcf99ddd588acd8fd0300",
 "blockhash"=>"45453bf3b47c935146d3099dd82ddb28b48725e191ab00829cd2c7a2bf2f79b1",
 "confirmations"=>2,
 "time"=>1671899383,
 "blocktime"=>1671899383}
```
