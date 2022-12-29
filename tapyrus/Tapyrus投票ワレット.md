# Tapyrus 投票ワレット

最終更新 2022/12/29 Shigeichiro Yamasaki

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



### 修正版HTLCによるトークンのロック(TPC)

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
    # アンロックトランザクションの構成
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

tapyrus core のコマンドではなくtapyrusrb で構成する

### NRTのカラーIDの生成

トークンの生成には，まずカラーIDが必要となる．
カラーIDは，それを生成するUTXOのscriptPubKey から生成される．


###   NRT発行トランザクション(CP2PKH)


```ruby
def issue_NRT(addr,token_amount)
    # UTXOの中でタイプがTPCでかつpubkeyhashでamount が FEE*3 より大きいのものを選択
    utxos = tapyrusRPC('listunspent', []).select{|txo|txo["token"]=="TPC" and Tapyrus::Script.parse_from_payload(txo["scriptPubKey"].htb).to_h[:type]=="pubkeyhash" and (txo["amount"] > (FEE*3))}
    # UTXOのTDIDを一つ選ぶ（最も少額のもの）
    utxo = utxos.sort_by{|x|x["amount"]}[0]
    # トランザクションのoutpoint を一つ選ぶ
    outpoint = Tapyrus::OutPoint.from_txid(utxo["txid"], utxo["vout"])
    # outpoint からNRTを生成
    color_id = Tapyrus::Color::ColorIdentifier.non_reissuable(outpoint)
    # UTXO のamountからFEEを差し引いた金額を送金する
    amount_satoshi = ((utxo["amount"] - FEE)*(10**8)).to_i
    ctx = Tapyrus::Tx.new
    ctx.in << Tapyrus::TxIn.new(out_point: outpoint)
    p2pkh = Tapyrus::Script.parse_from_addr(addr)
    cp2pkh = p2pkh.add_color(color_id)
    # 通常のP2PKH output
    ctx.out << Tapyrus::TxOut.new(value: amount_satoshi , script_pubkey: p2pkh)
    # CP2PKH output
    ctx.out << Tapyrus::TxOut.new(value: token_amount , script_pubkey: cp2pkh)
    # 使用する UTXOの scriptPubKey
    script_pubkey = Tapyrus::Script.parse_from_payload(utxo["scriptPubKey"].htb)
    # 受領したワレット内の自分のアドレス
    myaddr = script_pubkey.to_addr
    # 受領したワレット内の自分のアドレスに対応する秘密鍵（署名鍵）
    priv = tapyrusRPC('dumpprivkey', [myaddr]).chomp
    # ワレットの署名鍵オブジェクト
    key = Tapyrus::Key.from_wif(priv)
    # トランザクションのハッシュ値
    sighash = ctx.sighash_for_input(0, script_pubkey)
    # トランザクションへの署名
    sig = key.sign(sighash) + [Tapyrus::SIGHASH_TYPE[:all]].pack('C')
    # インプットへの署名の埋め込み
    ctx.in[0].script_sig << sig
    # インプットへの受領したワレット内の自分のアドレスの公開鍵の埋め込み
    ctx.in[0].script_sig << key.pubkey.htb
    cp2sh_txid = tapyrusRPC('sendrawtransaction', [ctx.to_hex]).chomp
    return cp2sh_txid, color_id
end

# 送金先は alice として実行
cp2sh_txid, color_id = issue_NRT(alice, 10000)

 cp2sh_txid
 
=> "2d617dddd4a8e6affc70a9efc6e1be701118ce0c9d2565c2ad457ce4ecc5c720"

color_id

=> #<Tapyrus::Color::ColorIdentifier:0x00007fb2c44984b8 @payload="x\xE0;T^\xBE\x82V\x1D\xA3\x1D\xE8\xE1\xF0\xDF/\x1C\xCDu\xB5\x8FsQN\xB7\x8C\xEF\xCA2\xDE\xB8@", @type=194>
```


### 投票トークン

上記で作成したトークンを投票トークンとする

```ruby
VToken = color_id
```


###   NRT送金トランザクション(CP2PKH)

#### カラー付きアドレスの生成

```ruby
def cp2pkh_addr(addr, color_id)
    p2pkh = Tapyrus::Script.parse_from_addr(addr)
    cp2pkh = p2pkh.add_color(color_id)
    cp2pkh.addresses[0]
end

# Aliceの投票アドレス
vAlice = cp2pkh_addr(alice,VToken)
# Bobの投票アドレス
vBob = cp2pkh_addr(bob,VToken)
```

#### transfertoken コマンドでの投票権の送付

```ruby
txidAlice = tapyrusRPC('transfertoken',[vAlice,10]).chomp
txidBob = tapyrusRPC('transfertoken',[vBob,10]).chomp
```

### Aliceの投票権UTXO

```ruby
utxos =  tapyrusRPC('listunspent',[])
utxo_vAlice = utxos.select{|x|x["token"]==VToken.to_hex and x["address"]==vAlice}

=> 
[{"txid"=>"7e9e509bc2e231d4f20dc084d69438ad138f4f324374670745ecab83fd552275",
  "vout"=>0,
  "address"=>"vr3ixFLzjK8uw5Hpm3WrXS9qrjTnzwZkBFe8pucGfSZjMT8fX6K2K2dej4VfUSfL5diBSZ9k71yx5N",
  "token"=>"c278e03b545ebe82561da31de8e1f0df2f1ccd75b58f73514eb78cefca32deb840",
  "amount"=>1,
  "scriptPubKey"=>"21c278e03b545ebe82561da31de8e1f0df2f1ccd75b58f73514eb78cefca32deb840bc76a91439f63e11f7f2e7cf5338f699339e8404bf4af42588ac",
  "confirmations"=>32,
  "spendable"=>true,
  "solvable"=>true,
  "safe"=>true}]
```



##  CP2SH

投票トークンをHTLCでロックする


```ruby
# 秘密情報
secret = "Yamalabo DAO member Alice"
# 秘密情報ハッシュ
secret_hash=Tapyrus.sha256(secret)
# ロック日数
lock_days = 10
color_id = VToken
pub_sender = pub_alice

def cp2sh_HTLC_lock(secret_hash, lock_days, color_id, pub_sender)
    # ロックタイム（ブロック数）
    locktime = (6*24*lock_days).to_bn.to_s(2).reverse.bth
    # redeem スクリプト
    redeem_script = Tapyrus::Script.new << OP_IF << OP_SHA256 << secret_hash.bth << OP_EQUALVERIFY << OP_ELSE << locktime << OP_CSV << OP_DROP << pub_sender << OP_ENDIF << OP_CHECKSIG
    # redeem スクリプトハッシュ
    script_hash = redeem_script.to_hash160
    # CP2SH 投票トークンのカラーを付与
    cp2sh = Tapyrus::Script.to_cp2sh(color_id, script_hash)
    return cp2sh
end

cp2sh = cp2sh_HTLC_lock(secret_hash, lock_days, color_id, pub_sender)
# 確認
cp2sh.to_h

=> 
{:asm=>"c278e03b545ebe82561da31de8e1f0df2f1ccd75b58f73514eb78cefca32deb840 OP_COLOR OP_HASH160 439d7da0fc8b4e21f6c2e77ab30b7a131e49ff40 OP_EQUAL",
 :hex=>"21c278e03b545ebe82561da31de8e1f0df2f1ccd75b58f73514eb78cefca32deb840bca914439d7da0fc8b4e21f6c2e77ab30b7a131e49ff4087",
 :type=>"nonstandard",                                  
 :req_sigs=>1,                                          
 :addresses=>["4Zr521aQzVCXQBs7Qbnsb8z7no3xwhMifCp2dBoznjwQWPYRJDk3CUyvioEwPHRGRrJXA3pcYvYFvvy"]}
```


### CP2SH カラー付きアドレスの生成

```ruby
vote_HTLC_addr = cp2sh.addresses[0]

=> "4Zr521aQzVCXQBs7Qbnsb8z7no3xwhMifCp2dBoznjwQWPYRJDk3CUyvioEwPHRGRrJXA3pcYvYFvvy"
```


### HTCL ロックトランザクションへの投票トークンの送付

transfertoken コマンドでの投票権の送付


```ruby
htlc_txid = tapyrusRPC('transfertoken',[vote_HTLC_addr,1000]).chomp

=> "95dad9202f5265714f61725ae9d1af0259da6f86a2ddd86d9e2db765fc8f3772"
```

トランザクションの確認

```ruby
tapyrusRPC('getrawtransaction',[txidHTLC,1])

=> 
{"txid"=>"95dad9202f5265714f61725ae9d1af0259da6f86a2ddd86d9e2db765fc8f3772",
 "hash"=>"9bc4da0bd0b6c9952c05e1620897d9cc3b99d00f96c82ccc1fe8489ba484c83a",
 "features"=>1,                 
 "size"=>474,                   
 "locktime"=>262594,            
 "vin"=>                     
  [{"txid"=>"d5f42ca9608cb35e8d7fd7d4cb28474fb9fc47ec2421ee2edbddc94c88595292",
    "vout"=>1,               
    "scriptSig"=>
     {"asm"=>
       "30440220546dd37a45dbf8c354a7e40c949548e32d9766b967fb1e2aea841075184328a00220380b4484731613f0c77c513865c77e322e0af797ed2c7fefa181b8c840c99f59[ALL] 02e7b4e8a5084b138af56ff0c976289548701cc3eadc318d5807e2ba461a64a091",
      "hex"=>
       "4730440220546dd37a45dbf8c354a7e40c949548e32d9766b967fb1e2aea841075184328a00220380b4484731613f0c77c513865c77e322e0af797ed2c7fefa181b8c840c99f59012102e7b4e8a5084b138af56ff0c976289548701cc3eadc318d5807e2ba461a64a091"},
    "sequence"=>4294967294},
   {"txid"=>"7703460833e879112ca97f715890e0dd8441a29cadb966e545ee0783bf659f8c",
    "vout"=>1,
    "scriptSig"=>
     {"asm"=>
       "304402205cc6da903a61d933274fd9aca84510d60e26d384f2fa9d3d3b2ee83d5418e52102205067f0f9232d4592a08c0c903a9e0387b20bbc4ebffa27c4944c56197190bbc6[ALL] 025efe7861c3ac5554eb23355a0d518b7be58351aa1b2d85ed0e51039c7748fb78",
      "hex"=>
       "47304402205cc6da903a61d933274fd9aca84510d60e26d384f2fa9d3d3b2ee83d5418e52102205067f0f9232d4592a08c0c903a9e0387b20bbc4ebffa27c4944c56197190bbc60121025efe7861c3ac5554eb23355a0d518b7be58351aa1b2d85ed0e51039c7748fb78"},
    "sequence"=>4294967294}],
 "vout"=>
  [{"token"=>"c278e03b545ebe82561da31de8e1f0df2f1ccd75b58f73514eb78cefca32deb840",
    "value"=>1000,
    "n"=>0,
    "scriptPubKey"=>
     {"asm"=>"c278e03b545ebe82561da31de8e1f0df2f1ccd75b58f73514eb78cefca32deb840 OP_COLOR OP_HASH160 439d7da0fc8b4e21f6c2e77ab30b7a131e49ff40 OP_EQUAL",
      "hex"=>"21c278e03b545ebe82561da31de8e1f0df2f1ccd75b58f73514eb78cefca32deb840bca914439d7da0fc8b4e21f6c2e77ab30b7a131e49ff4087",
      "reqSigs"=>1,
      "type"=>"coloredscripthash",
      "addresses"=>["4Zr521aQzVCXQBs7Qbnsb8z7no3xwhMifCp2dBoznjwQWPYRJDk3CUyvioEwPHRGRrJXA3pcYvYFvvy"]}},
   {"token"=>"c278e03b545ebe82561da31de8e1f0df2f1ccd75b58f73514eb78cefca32deb840",
    "value"=>8989,
    "n"=>1,
    "scriptPubKey"=>
     {"asm"=>"c278e03b545ebe82561da31de8e1f0df2f1ccd75b58f73514eb78cefca32deb840 OP_COLOR OP_DUP OP_HASH160 1f1cbf37b0848f02ad26da85e49f794856a65ded OP_EQUALVERIFY OP_CHECKSIG",
      "hex"=>"21c278e03b545ebe82561da31de8e1f0df2f1ccd75b58f73514eb78cefca32deb840bc76a9141f1cbf37b0848f02ad26da85e49f794856a65ded88ac",
      "reqSigs"=>1,
      "type"=>"coloredpubkeyhash",
      "addresses"=>["vr3ixFLzjK8uw5Hpm3WrXS9qrjTnzwZkBFe8pucGfSZjMQghQFXVGyd8qMuJSTbek9ewDWb6AobxKM"]}},
   {"token"=>"TPC",
    "value"=>0.01000946,
    "n"=>2,
    "scriptPubKey"=>
     {"asm"=>"OP_DUP OP_HASH160 1f1cbf37b0848f02ad26da85e49f794856a65ded OP_EQUALVERIFY OP_CHECKSIG",
      "hex"=>"76a9141f1cbf37b0848f02ad26da85e49f794856a65ded88ac",
      "reqSigs"=>1,
      "type"=>"pubkeyhash",
      "addresses"=>["13qWPKXSc2Cm5zDaCgGD1p4N2kMaWTYpZs"]}}],
 "hex"=>
  "0100000002925259884cc9dddb2eee2124ec47fcb94f4728cbd4d77f8d5eb38c60a92cf4d5010000006a4730440220546dd37a45dbf8c354a7e40c949548e32d9766b967fb1e2aea841075184328a00220380b4484731613f0c77c513865c77e322e0af797ed2c7fefa181b8c840c99f59012102e7b4e8a5084b138af56ff0c976289548701cc3eadc318d5807e2ba461a64a091feffffff8c9f65bf8307ee45e566b9ad9ca24184dde09058717fa92c1179e83308460377010000006a47304402205cc6da903a61d933274fd9aca84510d60e26d384f2fa9d3d3b2ee83d5418e52102205067f0f9232d4592a08c0c903a9e0387b20bbc4ebffa27c4944c56197190bbc60121025efe7861c3ac5554eb23355a0d518b7be58351aa1b2d85ed0e51039c7748fb78feffffff03e8030000000000003a21c278e03b545ebe82561da31de8e1f0df2f1ccd75b58f73514eb78cefca32deb840bca914439d7da0fc8b4e21f6c2e77ab30b7a131e49ff40871d230000000000003c21c278e03b545ebe82561da31de8e1f0df2f1ccd75b58f73514eb78cefca32deb840bc76a9141f1cbf37b0848f02ad26da85e49f794856a65ded88acf2450f00000000001976a9141f1cbf37b0848f02ad26da85e49f794856a65ded88acc2010400",
 "blockhash"=>"c6a71800efd20082b1d2f5f4dd1253dda3fa8527bdaa161b91ba4d96d71b4555",
 "confirmations"=>10,
 "time"=>1672231021,
 "blocktime"=>1672231021}
```

### UTXOの取得

```ruby
tx = Tapyrus::Tx.parse_from_payload(tapyrusRPC('getrawtransaction',[htlc_txid]).chomp.htb)
```

### 投票 HTLC アンロックトランザクション

input としてHTLC lockトランザクションのUTXOと自分が所有するTPCのUTXO（手数料用）の２つを使用する
手数料用inputは１個だけとする

* input 0: colored HTLC locked value: トークン数 (satoshi)
* input 1: TPC (P2PKH) 手数料用
* vout  0: colored output (CP2SH) value: トークン数（satoshi)
* vout  1: TPC output (P2PKH) おつり

```ruby
# p2sh でロックされたトランザクションのtxid
htlc_txid 
# redeem script の16進数形式
redeem_script_hex = redeem_script.to_hex
# アンロックした資金は addr = bob に送金するものとする
key = keyBob
# 送金先はBob
addr = bob


def unlock_htlc_cp2sh(htlc_txid, color_id, redeem_script_hex, pub_addr, addr, key, secret)
    # 16進数形式redeem script の復元
    redeem_script = Tapyrus::Script.parse_from_payload(redeem_script_hex.htb)
    # アンロック対象トランザクションとUTXOを確定する
    locked_tx = Tapyrus::Tx.parse_from_payload(tapyrusRPC('getrawtransaction',[htlc_txid]).htb)
    # ロックされているUTXO  0が cp2shであることがわかっている
    cp2sh_utxo = locked_tx.out[0]
    # このoutputの金額は NRT の票数（整数）
    token_value = cp2sh_utxo.value    
    # CP2SH のUTXOのoutpoint
    cp2sh_outpoint = Tapyrus::OutPoint.from_txid(htlc_txid, 0)
    # 手数料用のUTXO
    utxos = tapyrusRPC('listunspent', []).select{|txo|txo["token"]=="TPC" and Tapyrus::Script.parse_from_payload(txo["scriptPubKey"].htb).to_h[:type]=="pubkeyhash" and (txo["amount"] > (FEE*3)) and txo["address"] == addr }
    # UTXOのTDIDを一つ選ぶ（最も少額のもの）
    utxo = utxos.sort_by{|x|x["amount"]}[0]
    # おつり
    change = utxo["amount"] - FEE
    
    # トランザクションの構成
    tx = Tapyrus::Tx.new
    
    # inputs
    # CP2SH の outpoint をinputにする
    tx.in <<  Tapyrus::TxIn.new(out_point: cp2sh_outpoint)
    # 手数料用のinputの構成
    outpoint = Tapyrus::OutPoint.from_txid(utxo["txid"], utxo["vout"])
    tx.in << Tapyrus::TxIn.new(out_point: outpoint)
    
    # outputs
    # まず P2PKHアドレスへ送金のscriptPubKeyを作成する
    scriptPubKey = Tapyrus::Script.parse_from_addr(addr)
    # カラー付き scriptPubKeyに変換
    cp2pkh_scriptPubKey = scriptPubKey.add_color(color_id)
    # カラー付きCP2PKH とアンロックする票数を script_pubkeyをoutputに埋め込む
    tx.out << Tapyrus::TxOut.new(value: token_value , script_pubkey:  cp2pkh_scriptPubKey)
    # おつりTPC用 P2PKH
    tx.out << Tapyrus::TxOut.new(value: (change*(10**8)).to_i , script_pubkey:  scriptPubKey)
    
    # sighashの構成
    # アンロックトランザクションの署名対象のハッシュ値 sighash
    sighash0 = tx.sighash_for_input(0, redeem_script, hash_type: Tapyrus::SIGHASH_TYPE[:all])
    # scriptsig の追加
    sig0 = key.key.sign(sighash0) + [Tapyrus::SIGHASH_TYPE[:all]].pack('C')
    # <Bobの署名> 
    # <Bobの公開鍵>
    # <Secret> 
    # OP_TRUE
    tx.in[0].script_sig << sig0
    tx.in[0].script_sig << pub_addr
    tx.in[0].script_sig << secret.bth
    tx.in[0].script_sig << OP_1
    tx.in[0].script_sig << redeem_script.to_payload
    sighash1 = tx.sighash_for_input(1, utxo[ "scriptPubKey"])
    sig1 = key.key.sign(sighash1) + [Tapyrus::SIGHASH_TYPE[:all]].pack('C')
    # <Bobの署名> 
    # <Bobの公開鍵>
    tx.in[1].script_sig << sig1
    tx.in[1].script_sig << pub_addr
    
    return tx
    # 署名したトランザクションをブロードキャストする
    #p2sh_txid = tapyrusRPC('sendrawtransaction', [tx.to_hex])
    #return p2sh_txid, tx
end

unlock_htlc_tx = unlock_htlc_cp2sh(htlc_txid, VToken, redeem_script_hex, pub_bob, bob, keyBob, secret)
```
