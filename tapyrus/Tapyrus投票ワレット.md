# Tapyrus 投票ワレット

最終更新 2022/12/31 Shigeichiro Yamasaki

tapyrusrb を使ってP2SH のremittanceと受領を行うtransactionを作成する

投票トークンを取得するためのスクリプトとしては,修正版HTLCを使う．

### 修正版HTLCの問題点

修正版HTLCのロックtransactionを unlock して投票トークンを得るためには ロックtransactionのUTXOのtxid vout とハッシュ値の原像のペアを知っている必要がある．

選挙管理者は，メンバーからハッシュ値をもらってロックtransactionを生成し， そのUTXOのtxidをメンバーに返す．つまり，選挙管理者はメンバーとUTXOの対応を知っている．
このため， unlock したaddressが新たに生成したものでもどのメンバーのものかわかってしまう．

したがって，投票のプライバシの観点から選挙管理者に対するブラインド化などの措置が必要だが，今回はブロックチェーンの解析による追跡を防ぐだけにする．選挙管理委員会に対するブラインド化は必要だが，今後の課題とする．

### 修正版HTLCの利点

修正版HTLCでは，新規に生成されたaddressで投票トークンを取得し，そのaddressから投票できるので，ブロックチェーンの解析だけで投票者を知ることは基本的にできない．

### 修正版HTLCのunlocking script

ハッシュの原像を示せば，誰でも自分宛てに資金をremittanceできる

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

ハッシュ値の原像を知っていれば，だれでも（どのaddressを使っても）資金を unlock できる


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

カラー付きUTXOの構成は，scriptPubKey のスクリプトの 頭に <カラー識別子> OP_COLOR を記述することで実現できる

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

### カラー付きaddress

一般的に，トークン送信者が送信 のP2PKHaddressやP2SHaddressをもとに カラー付きaddressを生成します．

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

### transaction構成の基本部分の生成

```ruby
require 'tapyrus'
require 'json'
require "open3"

include Tapyrus
include Tapyrus::Opcodes
FEE = 0.00002    # 手数料
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
# 鍵オブジェクト
keyAlice = key.derive(1)
keyBob  = key.derive(2)
keyCarol = key.derive(3)
keyDavid = key.derive(4)
keyErza = key.derive(5)
keyFranck = key.derive(6)
# テスト用の秘密鍵
priv_alice = keyAlice.priv
priv_bob  = keyBob.priv
priv_carol = keyCarol.priv
priv_erza = keyErza.priv
priv_franck = keyFranck.priv
## address
alice = keyAlice.addr
bob  = keyBob.addr
carol = keyCarol.addr
david = keyDavid.addr
erza = keyErza.addr
franck = keyFranck.addr
# 公開鍵
pub_alice = keyAlice.pub
pub_bob  = keyBob.pub
pub_carol = keyCarol.pub
pub_david = keyDavid.pub
pub_erza = keyErza.pub
pub_franck = keyFranck.pub
# 自分のワレットの秘密鍵をインポートする
tapyrusRPC('importprivkey',[keyAlice.key.to_wif])
tapyrusRPC('importprivkey',[keyBob.key.to_wif])
tapyrusRPC('importprivkey',[keyCarol.key.to_wif])
tapyrusRPC('importprivkey',[keyDavid.key.to_wif])
tapyrusRPC('importprivkey',[keyErza.key.to_wif])
tapyrusRPC('importprivkey',[keyFranck.key.to_wif])
#
# transaction構成
def send_tapyrus(addr, amount, addr_change)
  # 所持金残高を確認
  balance = tapyrusRPC('getbalance', [])
  if balance < (amount+FEE) then
    puts "error (残高不足)"
  else
    # remittance amount＋手数料をぎりぎり上回るUTXOリストの作成
    utxos = consuming_utxos(amount+FEE, addr, "TPC")
    # remittanceに使用するUTXOの総額
    fund = utxos.map{|utxo|utxo["amount"]}.sum
    # UTXOの総額 - remittance amount - 手数料 =  change
    change = fund-amount-FEE
    # transactionの構成（P2PKH)
    tx = p2pkh_tx(addr, amount, utxos, change, addr_change)
    # transactionへの署名
    tx = sign_inputs(utxos, tx)
    # ビットコインネットワークへのデプロイ
    return tapyrusRPC('sendrawtransaction', [tx.to_hex])
  end
end
# remittance amount＋手数料をぎりぎり上回るUTXOリストの作成
def consuming_utxos(amount, addr, token)
  # ワレットの未使用のUTXOの一覧を得る
  utxos = tapyrusRPC('listunspent', [])
  # 消費可能状態のTPC のUTXOの選定
  spendable_utxos = utxos.select{|t|t["spendable"]==true and t["address"] == addr and t["token"]= token}
  # 使用可能な総額
  fund = spendable_utxos.map{|x|x["amount"]}.sum
  if fund < amount then
    puts "error (残高不足) #{addr}"
  else
    # UTXOを amountで昇順にソートする
    sorted_utxos = spendable_utxos.sort_by{|x|x["amount"]}
    # 少額のUTXOから集めて，指定 amountを上回るぎりぎりのUTXOのリストを作成する
    utxos=[]
    begin
      utxos << sorted_utxos.shift
      balance = utxos.reduce(0){|s,t|s+=t["amount"]}
    end until balance >= amount
    return utxos
  end
end
# transactionのinputの構成
def tx_inputs(tx, utxos)
  utxos.each{|utxo|
    # UTXOをinputから参照するための txid と vout としてエンコードする
    outpoint = Tapyrus::OutPoint.from_txid(utxo["txid"], utxo["vout"])
    # エンコードした参照をtransactionのinputに埋め込む
    tx.in << Tapyrus::TxIn.new(out_point: outpoint)
  }
  return tx
end
# transactionのoutputの構成
def tx_outputs(tx,amount, addr, change, addr_change)
  # remittance用outputの構成
  #  amountを satoshiの整数に変換
  amount_satoshi = (amount*(10**8)).to_i
  # ビットコインaddressから p2pkhのscript_pubkey を生成
  scriptPubKey0 = Tapyrus::Script.parse_from_addr(addr)
  # エンコードしたscript_pubkeyをtransactionのoutputに埋め込む
  tx.out << Tapyrus::TxOut.new(value: amount_satoshi , script_pubkey: scriptPubKey0)
  #  change用outputの構成
  #  amountを satoshiの整数に変換
  change_satoshi = (change*(10**8)).to_i
  # ビットコインaddressから p2wpkhのscript_pubkey を生成
  scriptPubKey1 = Tapyrus::Script.parse_from_addr(addr_change)
  # エンコードしたscript_pubkeyをtransactionのoutputに埋め込む
  tx.out << Tapyrus::TxOut.new(value: change_satoshi, script_pubkey: scriptPubKey1)
  return tx
end
# P2PKHtransactionの構成
def p2pkh_tx(addr,amount, utxos, change, addr_change)
  # transactionのテンプレートの生成
  tx = Tapyrus::Tx.new
  # transactionのinputの構成
  tx = tx_inputs(tx,utxos)
  # transactionのoutputの構成
  tx = tx_outputs(tx,amount, addr, change, addr_change)
  return tx
end
# transactionへの署名
def sign_inputs(utxos, tx)
  utxos.each.with_index{|utxo,index|
    # UTXOのscriptPubKey をオブジェクト化する
    script_pubkey = Tapyrus::Script.parse_from_payload(utxo["scriptPubKey"].htb)
    # scriptPubKey のremittance address
    myaddr = script_pubkey.to_addr
    # UTXOの送付 addressの秘密鍵（署名鍵）
    priv = tapyrusRPC('dumpprivkey', [myaddr]).chomp
    # 署名鍵オブジェクト
    key = Tapyrus::Key.from_wif(priv)
    # UTXOの amount
    satoshi = (utxo["amount"]*(10**8)).to_i
    case script_pubkey.type
    when "pubkeyhash"  # UTXOがP2PKHタイプ
      # transactionのハッシュ値を計算
      sighash = tx.sighash_for_input(index, script_pubkey)
      # transactionへの署名＋署名タイプ情報を付加
      sig = key.sign(sighash, algo: :ecdsa) + [Tapyrus::SIGHASH_TYPE[:all]].pack('C')
      # inputへの署名の追加
      tx.in[index].script_sig << sig
      # inputへの公開鍵の追加
      tx.in[index].script_sig << key.pubkey.htb
    end
  }
  return tx
end
```

#### P2PKH のremittanceテスト

```ruby
# remittanceテスト
txid = send_tapyrus(bob, 0.0001, alice).chomp
```



### 修正版HTLCによるトークンのロック(TPC)

```ruby
#####################################
# 秘密情報
secret = "Yamalabo DAO member Alice"
secret_hash=Tapyrus.sha256(secret)
# 原像を知っていれば誰でも unlock できるHTLC
# secret_hash 秘密情報のハッシュ値
# pub_sender remittance者の公開鍵
# addr_sender remittance者のaddress
# pub_receiver 受領者の公開鍵
# lock_days　ロックする日数
def send_htlc_au(amount, addr_change, secret_hash, pub_sender, addr_sender, lock_days)
  # 所持金残高を確認
  balance = tapyrusRPC('getbalance', [])
  if balance < (amount+FEE) then
    puts "error (残高不足)"
  else
    # remittance amount＋手数料をぎりぎり上回るUTXOリストの作成
    utxos = consuming_utxos(amount+FEE, addr_sender, "TPC")
    # remittanceに使用するUTXOの総額
    fund = utxos.map{|utxo|utxo["amount"]}.sum
    # UTXOの総額 - remittance amount - 手数料 =  change
    change = fund-amount-FEE
    # redeem scriptの生成
    locktime = (6*24*lock_days).to_bn.to_s(2).reverse.bth
    redeem_script = Tapyrus::Script.new << OP_IF << OP_SHA256 << secret_hash.bth << OP_EQUALVERIFY << OP_ELSE << locktime << OP_CSV << OP_DROP << pub_sender << OP_ENDIF << OP_CHECKSIG
    # transactionの構成（P2SH)
    tx = p2sh_tx(utxos, amount, redeem_script, addr_change)
    # transactionへの署名
    tx = sign_inputs(utxos, tx)
    # transactionのデプロイ
    txid = tapyrusRPC('sendrawtransaction',[tx.to_hex])
    # p2shtransactionの unlock に必要な情報の出力
    return tx, txid.chomp, redeem_script.to_hex
  end
end
# p2sh 未署名transactionの構成
def p2sh_tx(utxos, amount, redeem_script, addr_change)
  #  change = UTXOの総額 - remittance amount - 手数料
  change = (utxos.map{|utxo|utxo["amount"]}.sum)-amount-FEE
  # transactionのスケルトン
  tx = Tapyrus::Tx.new
  # transactionのinputの構成
  tx = tx_inputs(tx,utxos)
  # P2SHtransactionのoutputの構成
  tx = p2sh_outputs(tx, amount, redeem_script, change, addr_change)
  return tx
end
# p2shtransactionのoutputの構成
def p2sh_outputs(tx, amount, redeem_script, change, addr_change)
  # satoshi変換
  change_satoshi = (change*(10**8)).to_i
  amount_satoshi = (amount*(10**8)).to_i
  # redeem_script を p2shaddressに変換する
  p2sh_addr = redeem_script.to_p2sh.addresses[0]
  # p2shaddressからscript pubkey を生成
  scriptPubKey0 = Tapyrus::Script.parse_from_addr(p2sh_addr)
  # 作成したscript pubkey outputに設定する
  tx.out << Tapyrus::TxOut.new(value: amount_satoshi, script_pubkey: scriptPubKey0)
  #  change用script pub key の構成(P2PKH)
  scriptPubKey1 = Tapyrus::Script.parse_from_addr(addr_change)
  #  change用のoutput
  tx.out << Tapyrus::TxOut.new(value: change_satoshi, script_pubkey: scriptPubKey1)
  return tx
end
```

#### 修正版HTLCtransactionのremittanceテスト

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

### 修正HTLC  unlock transactionの構成


共通部分は省略


### 修正HTLC  unlock transaction


```ruby
#  unlock する主体に渡す情報を代入文の文字列にしたもの
# p2sh でロックされたtransactionのtxid
htlc_txid 
# redeem script の16進数形式
redeem_script_hex
#  unlock した資金は addr = bob にremittanceするものとする

def unlock_htlc_au(htlc_txid, redeem_script_hex, pub_addr, addr, key, secret)
  # 16進数形式redeem script の復元
  redeem_script = Tapyrus::Script.parse_from_payload(redeem_script_hex.htb)
  #  unlock 対象transactionとUTXOを確定する
  locked_tx = Tapyrus::Tx.parse_from_payload(tapyrusRPC('getrawtransaction',[htlc_txid]).htb)
  # ロックされているUTXO 0がp2shであることがわかっている
  p2sh_utxo = locked_tx.out[0]
  utxo_value = p2sh_utxo.value  # この amountの単位は satoshi
  # P2SH のUTXOのoutpoint
  outpoint = Tapyrus::OutPoint.from_txid(htlc_txid, 0)
  #  unlock transactionの構成
  tx = Tapyrus::Tx.new
  # inputの構成
  # P2SH のoutputをinputにする
  tx.in << Tapyrus::TxIn.new(out_point: outpoint)
  #output の構成 (P2PKH) addressへremittance
  scriptPubKey0 = Tapyrus::Script.parse_from_addr(addr)
  # script_pubkey0をtransactionのoutputに埋め込む
  tx.out << Tapyrus::TxOut.new(value: utxo_value-(FEE*(10**8)).to_i , script_pubkey: scriptPubKey0)
  #  unlock transactionの署名対象のハッシュ値 sighash
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
  # 署名したtransactionをブロードキャストする
  p2sh_txid = tapyrusRPC('sendrawtransaction', [tx.to_hex])
  return p2sh_txid.chomp, tx
end
```

#### 修正HTLCの unlock 

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


###  NRT発行transaction(CP2PKH)


```ruby
def issue_NRT(addr,token_amount)
  # UTXOの中でタイプがTPCでかつpubkeyhashでamount が FEE*3 より大きいのものを選択
  utxos = consuming_utxos(FEE*4, addr, "TPC")
  # UTXOのTDIDを一つ選ぶ（最も少額のもの）
  utxo = utxos.sort_by{|x|x["amount"]}[0]
  # transactionのoutpoint を一つ選ぶ
  outpoint = Tapyrus::OutPoint.from_txid(utxo["txid"], utxo["vout"])
  # outpoint からNRTを生成
  color_id = Tapyrus::Color::ColorIdentifier.non_reissuable(outpoint)
  # UTXO のamountからFEEを差し引いた amountをremittanceする
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
  # 受領したワレット内の自分のaddress
  myaddr = script_pubkey.to_addr
  # 受領したワレット内の自分のaddressに対応する秘密鍵（署名鍵）
  priv = tapyrusRPC('dumpprivkey', [myaddr]).chomp
  # ワレットの署名鍵オブジェクト
  key = Tapyrus::Key.from_wif(priv)
  # transactionのハッシュ値
  sighash = ctx.sighash_for_input(0, script_pubkey)
  # transactionへの署名
  sig = key.sign(sighash) + [Tapyrus::SIGHASH_TYPE[:all]].pack('C')
  # インプットへの署名の埋め込み
  ctx.in[0].script_sig << sig
  # インプットへの受領したワレット内の自分のaddressの公開鍵の埋め込み
  ctx.in[0].script_sig << key.pubkey.htb
  cp2sh_txid = tapyrusRPC('sendrawtransaction', [ctx.to_hex]).chomp
  return cp2sh_txid, color_id
end

# remittance は alice として実行
cp2sh_txid, color_id = issue_NRT(alice, 10000)

 cp2sh_txid
 
=> "fc52966edc614f8ba57bc87d3a1a36733cdbe9be6711f4e9a4ef8c79b60da232"

color_id_hex = color_id.to_hex

=> "c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4"
```



### 投票トークン

上記で作成したトークンを投票トークンとする

```ruby
color_id_hex = "c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4"
# color_id の復元
VToken = Tapyrus::Color::ColorIdentifier.parse_from_payload(color_id_hex.htb)
```


###  NRTremittancetransaction(CP2PKH)

#### カラー付きaddressの生成（投票用address）

```ruby
def vaddr(addr, color_id)
  p2pkh = Tapyrus::Script.parse_from_addr(addr)
  cp2pkh = p2pkh.add_color(color_id)
  cp2pkh.addresses[0]
end

# Aliceの投票address (vaddr)
vAlice = vaddr(alice,VToken)

=> "vsbVHH6scmw4jNz6iaxjGfh4wYzTcSFArDvVCV4JbvdhNW2tZTU5w1Vw8CH9aKmmJPLkDsAphTMf4b"

# Bobの投票address
vBob = vaddr(bob,VToken)

=> "vsbVHH6scmw4jNz6iaxjGfh4wYzTcSFArDvVCV4JbvdhNUYwFAzcS4NspYSCjUmee7puELjqqMFkCd"
```

#### transfertoken コマンドでの投票権の送付

transfertoken コマンドで指定できるのは，宛先のカラー付きアドレスとトークンの量のみ．


```ruby
txidAlice = tapyrusRPC('transfertoken',[vAlice,10]).chomp

=> "ba09d5ac272fdc5775626afdc9df0be753c7517671127431483c3132d01db0c6"

txidBob = tapyrusRPC('transfertoken',[vBob,10]).chomp

=> "75afb32fc210f0dc89ee49bdb1c61c74e77ddd6423f61cb2dd4e4f58e4bca8e4"
```

#### transfertoken transaction の確認

output として投票トークンのおつりとTPCのおつりが追加されている

```ruby
txBob = tapyrusRPC('getrawtransaction', [txidBob, 1])

=> 
{"txid"=>"75afb32fc210f0dc89ee49bdb1c61c74e77ddd6423f61cb2dd4e4f58e4bca8e4",
 "hash"=>"5a82bf79adf928d4fbd395ac7a9c15d2e6a54c745161648fc001a340381345d9",
 "features"=>1,
 "size"=>623,
 "locktime"=>263216,
 "vin"=>
  [{"txid"=>"1addd9e81de05baff6e0d8a2256bd4d3f96c85b8192c066a46a51ed96d0a58a7",
    "vout"=>1,
    "scriptSig"=>
     {"asm"=>
       "3044022016a32c28811cc2fa40e449fdecc370ec797fa2f5f0cb6b888c16a3cfb18ab0d402201ac7bba8fe665f017011d9900f80bad611ca2e3871bcb335b31cc86630bff747[ALL] 03eac251090d5a9f80d31b49b9dff8c753feceab96799d1e176ae996c30190943d",
      "hex"=>
       "473044022016a32c28811cc2fa40e449fdecc370ec797fa2f5f0cb6b888c16a3cfb18ab0d402201ac7bba8fe665f017011d9900f80bad611ca2e3871bcb335b31cc86630bff747012103eac251090d5a9f80d31b49b9dff8c753feceab96799d1e176ae996c30190943d"},
    "sequence"=>4294967294},
   {"txid"=>"505b0edc0a6b83874d7237978e110dcbd3718121d9c6764cd7c399074bb8c144",
    "vout"=>0,
    "scriptSig"=>
     {"asm"=>
       "304402202eccafafa7f880d2fe6c4633b0f4fbc871e2f7bdf72e882133abf0d4a933c75002207061997c46919efc628f66fecaad2ecdbdc1b9b60806ec53b5eb79ddf3bc72c1[ALL] 03122e5241c75b42b8acdb3f392345bb2ed4c02c9c1487acfcc2871eeb908ab3b7",
      "hex"=>
       "47304402202eccafafa7f880d2fe6c4633b0f4fbc871e2f7bdf72e882133abf0d4a933c75002207061997c46919efc628f66fecaad2ecdbdc1b9b60806ec53b5eb79ddf3bc72c1012103122e5241c75b42b8acdb3f392345bb2ed4c02c9c1487acfcc2871eeb908ab3b7"},
    "sequence"=>4294967294},
   {"txid"=>"b6c4589d39e2f30ad473f3973c6cbfb915f9629bedeaecab4fa98872788292d2",
    "vout"=>1,
    "scriptSig"=>
     {"asm"=>
       "30440220604d84641276f07da923cb063e1b23b617c55ad60a619ab9782e4099d607a9a3022012e65e033d57a9f47fd16094ab2cd5b958dfe82faed61a525d3ef01b7d057242[ALL] 032f04706a999dc831bacf23f84554b0eb026a79bd76a204449a91401d963e7292",
      "hex"=>
       "4730440220604d84641276f07da923cb063e1b23b617c55ad60a619ab9782e4099d607a9a3022012e65e033d57a9f47fd16094ab2cd5b958dfe82faed61a525d3ef01b7d0572420121032f04706a999dc831bacf23f84554b0eb026a79bd76a204449a91401d963e7292"},
    "sequence"=>4294967294}],
 "vout"=>
  [{"token"=>"c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4",
    "value"=>10,
    "n"=>0,
    "scriptPubKey"=>
     {"asm"=>
       "c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4 OP_COLOR OP_DUP OP_HASH160 29b4b5fba49425ce64d9b27fbe5249827269d583 OP_EQUALVERIFY OP_CHECKSIG",
      "hex"=>"21c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4bc76a91429b4b5fba49425ce64d9b27fbe5249827269d58388ac",
      "reqSigs"=>1,
      "type"=>"coloredpubkeyhash",
      "addresses"=>["vsbVHH6scmw4jNz6iaxjGfh4wYzTcSFArDvVCV4JbvdhNUYwFAzcS4NspYSCjUmee7puELjqqMFkCd"]}},
   {"token"=>"c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4",
    "value"=>9980,
    "n"=>1,
    "scriptPubKey"=>
     {"asm"=>
       "c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4 OP_COLOR OP_DUP OP_HASH160 947c43fc94bbb1e70b6520d78f5112f336e7208f OP_EQUALVERIFY OP_CHECKSIG",
      "hex"=>"21c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4bc76a914947c43fc94bbb1e70b6520d78f5112f336e7208f88ac",
      "reqSigs"=>1,
      "type"=>"coloredpubkeyhash",
      "addresses"=>["vsbVHH6scmw4jNz6iaxjGfh4wYzTcSFArDvVCV4JbvdhNeHXvoz7HUhxsv4qC3CnkgPUFekDQU2k8w"]}},
   {"token"=>"TPC",
    "value"=>0.01084753,
    "n"=>2,
    "scriptPubKey"=>
     {"asm"=>"OP_DUP OP_HASH160 947c43fc94bbb1e70b6520d78f5112f336e7208f OP_EQUALVERIFY OP_CHECKSIG",
      "hex"=>"76a914947c43fc94bbb1e70b6520d78f5112f336e7208f88ac",
      "reqSigs"=>1,
      "type"=>"pubkeyhash",
      "addresses"=>["1EY7sWpzzYRJjQbcrNkuobALHaVdBLikGJ"]}}],
 "hex"=>
  "0100000003a7580a6dd91ea5466a062c19b8856cf9d3d46b25a2d8e0f6af5be01de8d9dd1a010000006a473044022016a32c28811cc2fa40e449fdecc370ec797fa2f5f0cb6b888c16a3cfb18ab0d402201ac7bba8fe665f017011d9900f80bad611ca2e3871bcb335b31cc86630bff747012103eac251090d5a9f80d31b49b9dff8c753feceab96799d1e176ae996c30190943dfeffffff44c1b84b0799c3d74c76c6d9218171d3cb0d118e9737724d87836b0adc0e5b50000000006a47304402202eccafafa7f880d2fe6c4633b0f4fbc871e2f7bdf72e882133abf0d4a933c75002207061997c46919efc628f66fecaad2ecdbdc1b9b60806ec53b5eb79ddf3bc72c1012103122e5241c75b42b8acdb3f392345bb2ed4c02c9c1487acfcc2871eeb908ab3b7feffffffd29282787288a94fabeceaed9b62f915b9bf6c3c97f373d40af3e2399d58c4b6010000006a4730440220604d84641276f07da923cb063e1b23b617c55ad60a619ab9782e4099d607a9a3022012e65e033d57a9f47fd16094ab2cd5b958dfe82faed61a525d3ef01b7d0572420121032f04706a999dc831bacf23f84554b0eb026a79bd76a204449a91401d963e7292feffffff030a000000000000003c21c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4bc76a91429b4b5fba49425ce64d9b27fbe5249827269d58388acfc260000000000003c21c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4bc76a914947c43fc94bbb1e70b6520d78f5112f336e7208f88ac518d1000000000001976a914947c43fc94bbb1e70b6520d78f5112f336e7208f88ac30040400",
 "blockhash"=>"39cd44e319014a9720ad1e9cc9a69210f91c9918b5ef0873efe1d1898de47645",
 "confirmations"=>1,
 "time"=>1672412362,
 "blocktime"=>1672412362}
```

### Aliceの投票権UTXO

```ruby
utxos_alice = consuming_utxos(FEE*4, alice, VToken.to_hex)

=> 
[{"txid"=>"4772d1fee0d9295b90ea12823c96386b5487461392ab11191a67c4f14fb708cf",
  "vout"=>1,
  "address"=>"16HUWAJye5DGygowEfKtMJ7cFnvEP7wP2e",
  "token"=>"c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4",
  "amount"=>0.00098,
  "label"=>"",
  "scriptPubKey"=>"76a91439f63e11f7f2e7cf5338f699339e8404bf4af42588ac",
  "confirmations"=>2,
  "spendable"=>true,
  "solvable"=>true,
  "safe"=>true}]
```



## CP2SH

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


### CP2SH カラー付きaddressの生成

```ruby
vHTLCaddr = cp2sh.addresses[0]

=> "4Zr521aQzVCXQBs7Qbnsb8z7no3xwhMifCp2dBoznjwQWPYRJDk3CUyvioEwPHRGRrJXA3pcYvYFvvy"
```


### HTCL ロックtransactionへの投票トークンの送付

transfertoken コマンドで投票権 1000票 HTLCでロックする

CP2SH アドレスに送付するだけ

```ruby
vHTLCtxid = tapyrusRPC('transfertoken',[vHTLCaddr,1000]).chomp

=> "65692b560279646979a4348fba9d2cbf48f9fbe3ca3369a9b535574fadb4a6a2"
```

#### coloerd HTLC lock transactionの確認

* input[0] : CP2PKH
* input[1] : P2PKH

* output[0]: CP2SH
* output[1]: CP2PKH
* output[2]: P2PKH

```ruby
tapyrusRPC('getrawtransaction',[vHTLCtxid,1])

=> 
{"txid"=>"65692b560279646979a4348fba9d2cbf48f9fbe3ca3369a9b535574fadb4a6a2",
 "hash"=>"920286fc574765c17dcc1f2a9ecbc19e68b449a85a39bfd4ec495d749ce76bde",
 "features"=>1,
 "size"=>474,
 "locktime"=>263223,
 "vin"=>
  [{"txid"=>"75afb32fc210f0dc89ee49bdb1c61c74e77ddd6423f61cb2dd4e4f58e4bca8e4",
    "vout"=>1,
    "scriptSig"=>
     {"asm"=>
       "3044022074c9375d7bc466e9404303e2b27a3b3e11f42f1041b63a8b2ed84df3c30f8f3602205028ba108b3d8a16f058722b56f01be562325846b75f7198c21c59b0118eaa79[ALL] 03c25e32b63da161ac66df5314c712de239b183b9a63b51b0d5ab3b42d6946876c",
      "hex"=>
       "473044022074c9375d7bc466e9404303e2b27a3b3e11f42f1041b63a8b2ed84df3c30f8f3602205028ba108b3d8a16f058722b56f01be562325846b75f7198c21c59b0118eaa79012103c25e32b63da161ac66df5314c712de239b183b9a63b51b0d5ab3b42d6946876c"},
    "sequence"=>4294967294},
   {"txid"=>"ba09d5ac272fdc5775626afdc9df0be753c7517671127431483c3132d01db0c6",
    "vout"=>1,
    "scriptSig"=>
     {"asm"=>
       "304402204628265c0f7087dbe2430d0e5954df7aa75b2c2f31738bacaca8a677b08e0a16022047b48596201da5a4705d3fa5cd06dd7a27e8631261feb2eb41eba329638297e7[ALL] 020176c078497f2d1e5860bef896c6e235359f868cc9d7b14ca7898082a2ac7e7c",
      "hex"=>
       "47304402204628265c0f7087dbe2430d0e5954df7aa75b2c2f31738bacaca8a677b08e0a16022047b48596201da5a4705d3fa5cd06dd7a27e8631261feb2eb41eba329638297e70121020176c078497f2d1e5860bef896c6e235359f868cc9d7b14ca7898082a2ac7e7c"},
    "sequence"=>4294967294}],
 "vout"=>
  [{"token"=>"c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4",
    "value"=>1000,
    "n"=>0,
    "scriptPubKey"=>
     {"asm"=>
       "c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4 OP_COLOR OP_HASH160 439d7da0fc8b4e21f6c2e77ab30b7a131e49ff40 OP_EQUAL",
      "hex"=>"21c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4bca914439d7da0fc8b4e21f6c2e77ab30b7a131e49ff4087",
      "reqSigs"=>1,
      "type"=>"coloredscripthash",
      "addresses"=>["4ZscnLcAsNfKYzAogZLKTtDf1ssVcJrQ5snJyZPSpgRUUQbKXG7CG6xo1CNisPJNs549iq8ddS8EPdJ"]}},
   {"token"=>"c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4",
    "value"=>8980,
    "n"=>1,
    "scriptPubKey"=>
     {"asm"=>
       "c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4 OP_COLOR OP_DUP OP_HASH160 ae84268e422adf7b732599fd2144c409786acd9a OP_EQUALVERIFY OP_CHECKSIG",
      "hex"=>"21c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4bc76a914ae84268e422adf7b732599fd2144c409786acd9a88ac",
      "reqSigs"=>1,
      "type"=>"coloredpubkeyhash",
      "addresses"=>["vsbVHH6scmw4jNz6iaxjGfh4wYzTcSFArDvVCV4JbvdhNgfAwfFffWmVjufuLKRsb2W8jXHEAVPYTS"]}},
   {"token"=>"TPC",
    "value"=>0.01003298,
    "n"=>2,
    "scriptPubKey"=>
     {"asm"=>"OP_DUP OP_HASH160 ae84268e422adf7b732599fd2144c409786acd9a OP_EQUALVERIFY OP_CHECKSIG",
      "hex"=>"76a914ae84268e422adf7b732599fd2144c409786acd9a88ac",
      "reqSigs"=>1,
      "type"=>"pubkeyhash",
      "addresses"=>["1GuktN6ZNaUqbQCgzeyzdwGzmT2dvtJPku"]}}],
 "hex"=>
  "0100000002e4a8bce4584f4eddb21cf62364dd7de7741cc6b1bd49ee89dcf010c22fb3af75010000006a473044022074c9375d7bc466e9404303e2b27a3b3e11f42f1041b63a8b2ed84df3c30f8f3602205028ba108b3d8a16f058722b56f01be562325846b75f7198c21c59b0118eaa79012103c25e32b63da161ac66df5314c712de239b183b9a63b51b0d5ab3b42d6946876cfeffffffc6b01dd032313c48317412717651c753e70bdfc9fd6a627557dc2f27acd509ba010000006a47304402204628265c0f7087dbe2430d0e5954df7aa75b2c2f31738bacaca8a677b08e0a16022047b48596201da5a4705d3fa5cd06dd7a27e8631261feb2eb41eba329638297e70121020176c078497f2d1e5860bef896c6e235359f868cc9d7b14ca7898082a2ac7e7cfeffffff03e8030000000000003a21c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4bca914439d7da0fc8b4e21f6c2e77ab30b7a131e49ff408714230000000000003c21c2b206408c1842a3b4a2544585967f53e36a1d0b2f39132893a94312550f43bba4bc76a914ae84268e422adf7b732599fd2144c409786acd9a88ac224f0f00000000001976a914ae84268e422adf7b732599fd2144c409786acd9a88ac37040400"}
```

### UTXOの取得

```ruby
vHTLCtx =  Tapyrus::Tx.parse_from_payload(tapyrusRPC('getrawtransaction',[vHTLCtxid]).htb)
```

### 投票 HTLC  unlock transaction

input としてHTLC locktransactionのUTXOと自分が所有するTPCのUTXO（手数料用）の２つを使用する
手数料用inputは１個だけとする

* input 0: colored HTLC locked(CP2SH) value: トークン数 (satoshi)
* input 1: TPC (P2PKH) 手数料用


* vout 0: colored output (CP2SH) value: トークン数（satoshi)
* vout 1: TPC output (P2PKH)  change

```ruby
# p2sh でロックされたtransactionのtxid
vHTLCtxid 
# redeem script の16進数形式
secret = "Yamalabo DAO member Alice"
pub_sender = pub_alice
lock_days = 10
locktime = (6*24*lock_days).to_bn.to_s(2).reverse.bth
redeem_script = Tapyrus::Script.new << OP_IF << OP_SHA256 << secret_hash.bth << OP_EQUALVERIFY << OP_ELSE << locktime << OP_CSV << OP_DROP << pub_sender << OP_ENDIF << OP_CHECKSIG
redeem_script_hex = redeem_script.to_hex
#  unlock した資金は receiver = bob にremittanceするものとする
key = keyBob
# receiver はBob
receiver = bob
pub_receiver = pub_bob
color_id = VToken


def unlock_htlc_cp2sh(vHTLCtxid, color_id, redeem_script_hex, pub_receiver, receiver, key, secret)
  # 16進数形式redeem script の復元
  redeem_script = Tapyrus::Script.parse_from_payload(redeem_script_hex.htb)
  #  unlock 対象transactionとUTXOを確定する
  locked_tx = Tapyrus::Tx.parse_from_payload(tapyrusRPC('getrawtransaction',[vHTLCtxid]).htb)
  # ロックされているUTXO 0が cp2shであることがわかっている
  cp2sh_utxo = locked_tx.out[0]
  # このoutputの amountは NRT の票数（整数）
  token_value = cp2sh_utxo.value  
  # CP2SH のUTXOのoutpoint
  cp2sh_outpoint = Tapyrus::OutPoint.from_txid(vHTLCtxid, 0)
  # 手数料用のUTXO でBobが所有しているもの
  utxos = consuming_utxos(FEE*4, receiver, "TPC")
  # UTXOのTDIDを一つ選ぶ（最も少額のもの）
  utxo = utxos.sort_by{|x|x["amount"]}[0]
  # TPC の change
  change = utxo["amount"] - FEE
  # カラー付きHTCL unlock transactionの構成
  tx = Tapyrus::Tx.new
  # inputs
  # CP2SH の outpoint をinputにする
  tx.in << Tapyrus::TxIn.new(out_point: cp2sh_outpoint)
  # 手数料用のinputの構成
  p2pkh_outpoint = Tapyrus::OutPoint.from_txid(utxo["txid"], utxo["vout"])
  tx.in << Tapyrus::TxIn.new(out_point: p2pkh_outpoint)
  # outputs
  # まず P2PKHaddressへremittanceのscriptPubKeyを作成する
  scriptPubKey = Tapyrus::Script.parse_from_addr(receiver)
  # カラー付き scriptPubKeyに変換
  cp2pkh_scriptPubKey = scriptPubKey.add_color(color_id)
  # カラー付きCP2PKH と unlock する票数を script_pubkeyをoutputに埋め込む
  tx.out << Tapyrus::TxOut.new(value: token_value , script_pubkey: cp2pkh_scriptPubKey)
  #  changeTPC用 P2PKH
  tx.out << Tapyrus::TxOut.new(value: (change*(10**8)).to_i , script_pubkey: scriptPubKey)
  
  # transactionへの署名
  # sighashの構成
  #  unlock transactionの署名対象のハッシュ値 sighash
  sighash0 = tx.sighash_for_input(0, redeem_script, hash_type: Tapyrus::SIGHASH_TYPE[:all])
  # scriptsig の追加
  sig0 = key.key.sign(sighash0, algo: :ecdsa) + [Tapyrus::SIGHASH_TYPE[:all]].pack('C')
  # <Bobの署名> 
  # <Bobの公開鍵>
  # <Secret> 
  # OP_TRUE
  tx.in[0].script_sig << sig0
  tx.in[0].script_sig << pub_receiver.htb
  tx.in[0].script_sig << secret
  tx.in[0].script_sig << OP_1
  tx.in[0].script_sig << redeem_script.to_payload
  # TPC utxo のsighash
  script_pubkey = Tapyrus::Script.parse_from_payload(utxo["scriptPubKey"].htb)
  sighash1 = tx.sighash_for_input(1, script_pubkey)
  sig1 = key.key.sign(sighash1) + [Tapyrus::SIGHASH_TYPE[:all]].pack('C')
  # <Bobの署名> 
  # <Bobの公開鍵>
  tx.in[1].script_sig << sig1
  tx.in[1].script_sig << pub_receiver
  # 署名したtransactionをブロードキャストする
  txid = tapyrusRPC('sendrawtransaction', [tx.to_hex])
  return txid, tx
end

txid, tx = unlock_htlc_cp2sh(vHTLCtxid, VToken, redeem_script_hex, pub_bob, bob, keyBob, secret)
```
