# tapyrusrb

最終更新 2022/12/24 Shigeichiro Yamasaki

Tapyrus API をRuby から操作する rubygemsの基本

## tapyrusrb WiKi

開発者の安土さんによる Wikiです。
基本的な使用法は、ここでわかります。

[https://github.com/chaintope/tapyrusrb/wiki](https://github.com/chaintope/tapyrusrb/wiki)

## tapyrusrb のクラスとメソッドの一覧

[https://www.rubydoc.info/gems/tapyrus/0.3.0](https://www.rubydoc.info/gems/tapyrus/0.3.0)

## インストール方法

###  install LevelDB

for Ubuntu

```bash
sudo apt-get install libleveldb-dev
```

for Mac

```bash
brew install --build-from-source leveldb
```

共通

```bash
gem install leveldb-ruby
```

```bash
gem install tapyrus
```

## tapyrus core を起動しておく

### tapyrus RPC


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
        return r[1]
    end
end
```

### テスト用アカウントの作成

```ruby
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
```

### 自分のワレットの秘密鍵をインポートする

```ruby
tapyrusRPC('importprivkey',[keyAlice.key.to_wif])
tapyrusRPC('importprivkey',[keyBob.key.to_wif])
tapyrusRPC('importprivkey',[keyCarol.key.to_wif])
tapyrusRPC('importprivkey',[keyDavid.key.to_wif])
tapyrusRPC('importprivkey',[keyErza.key.to_wif])
tapyrusRPC('importprivkey',[keyFranck.key.to_wif])

```


### 実行確認

```ruby
balance = tapyrusRPC('getbalance',[])
=> 29.12661982

utxos = tapyrusRPC('listunspent',[])
=> 
[{"txid"=>"47556d8f51fd5bc5d39eb0d50fd881fd26b0351c8ec39f094a8c6a5966080b27",
...

txid = tapyrusRPC('sendtoaddress',[alice, 0.001])
=> "aa5b4c6607d4a33e0d2ece8d27ba29c3bca4ee387c67bd6c91aa758dd8b131b5"

tx = tapyrusRPC('gettransaction',[txid])
```

### UTXOの作成

とりあえず10個に分割し
その後さらにに50個に分割する

```ruby
def newutxo(amount)
    address=tapyrusRPC('getnewaddress',["user#{rand(10000)}"])
    txid=tapyrusRPC('sendtoaddress',[address, amount])
    puts txid
end

6.times do newutxo(0.1) end

# 20分後(10分後ではまだUTXOになっていません）
50.times do newutxo(0.01) end

# さらに10分待つ
```

### トークン発行

使用するUTXO

```ruby
# TPCトークンを選択
tpcs = tapyrusRPC('listunspent',[]).select{|x|x["token"]=="TPC"}

=> 
{"txid"=>"1659e271e8fabd50e07f2a41873ce033d4c65ed236f768b017299a98b4b4400d",
 "vout"=>0,
 "address"=>"1Aq7soZtCEuaHkjExL9kTWQPsJGW1REb8e",
 "token"=>"TPC",
 "amount"=>0.001,
 "label"=>"alice",
 "scriptPubKey"=>"76a9146bd2d6835c85f0f4beb6641861c60df138d1c9f588ac",
 "confirmations"=>1,
 "spendable"=>true,
 "solvable"=>true,
 "safe"=>true}

```

#### Non-Reissuable Token の発行

```ruby
def mint_NRT(amount)
    tpcs = tapyrusRPC('listunspent',[]).select{|x|x["token"]=="TPC"}
    # NRTの発行
    nrt = tapyrusRPC('issuetoken',[2, amount, tpcs[0]["txid"], tpcs[0]["vout"]])
end

# 1000トークン発行
nrt=mint_NRT(1000)

=> 
{"color"=>"c26a15e5ade9c5c137e04ce7e200b3a9566dcefb403781221794e9471145738cb2",
 "txid"=>"c95295be03d965bd31ebf699c7c9b0923d81afe0d172e5b5b6d48e56bd597c5a"}
```

NRTトークンのcolorとUTXOの確認

```ruby
color1 = nrt["color"]

# 10分後

nrts = tapyrusRPC('listunspent',[]).select{|x|x["token"]==color1}
=> 
[{"txid"=>"c95295be03d965bd31ebf699c7c9b0923d81afe0d172e5b5b6d48e56bd597c5a",
  "vout"=>0,                   
  "address"=>"4ZqfnakPgRvbWtEwnywDfoTkfENr4teS45dVnBfuZgRYfL9BYSRtMP8KczL14PWKYufac4rnn4fwxa7",
  "token"=>"c26a15e5ade9c5c137e04ce7e200b3a9566dcefb403781221794e9471145738cb2",
  "amount"=>1000,              
  "scriptPubKey"=>             
   "21c26a15e5ade9c5c137e04ce7e200b3a9566dcefb403781221794e9471145738cb2bca914186460b6b034f6c76c6fe57483a5737326cca13387",           
  "confirmations"=>4,
  "spendable"=>true,
  "solvable"=>true,
  "safe"=>true}] 
```


#### NFTの発行

```ruby
def mint_NFT
    tpcs = tapyrusRPC('listunspent',[]).select{|x|x["token"]=="TPC"}
    # NFTの発行
    nft = tapyrusRPC('issuetoken',[3, 1, tpcs[0]["txid"], tpcs[0]["vout"]])
end

# NFT発行
nft=mint_NFT
```

NFTトークンのcolorとUTXOの確認

```ruby
color2 = nft["color"]

# 10分後

nfts = tapyrusRPC('listunspent',[]).select{|x|x["token"]==color2}
=> 
[{"txid"=>"669a3062518dc15beaed588748e055474a4feec8e279dc15c3742614713ce716",
  "vout"=>0,                   
  "address"=>"4Zw7sGTmZHgkfS8L1VSe7nz1Mpq4YjE2XfXqhWbJ8gQGoZ1FSkLjTJqkw24BNjTvg7QxYRie15a1dvw",
  "token"=>"c3335194a791210d8fe60d5f3c767f238b410c115c708c987098ffd61d56bf25e1",
  "amount"=>1,                 
  "scriptPubKey"=>             
   "21c3335194a791210d8fe60d5f3c767f238b410c115c708c987098ffd61d56bf25e1bca914e0af93c9eb8494e9ddbd63a3b839bf718856d6c687",           
  "confirmations"=>3,          
  "spendable"=>true,
  "solvable"=>true,
  "safe"=>true}]  
```

#### Reissuable Token の発行


```ruby
def mint_RT(amount)
    tpcs = tapyrusRPC('listunspent',[]).select{|x|x["token"]=="TPC"}
    # RTの発行
    rt = tapyrusRPC('issuetoken',[1, amount, tpcs[0]["scriptPubKey"]])
end

# RT発行
rt=mint_RT(1000)
```

RTトークンのcolorとUTXOの確認

```ruby
color3 = rt["color"]

# 10分後

rts = tapyrusRPC('listunspent',[]).select{|x|x["token"]==color3}
=> 
[{"txid"=>"669a3062518dc15beaed588748e055474a4feec8e279dc15c3742614713ce716",
  "vout"=>0,                   
  "address"=>"4Zw7sGTmZHgkfS8L1VSe7nz1Mpq4YjE2XfXqhWbJ8gQGoZ1FSkLjTJqkw24BNjTvg7QxYRie15a1dvw",
  "token"=>"c3335194a791210d8fe60d5f3c767f238b410c115c708c987098ffd61d56bf25e1",
  "amount"=>1,                 
  "scriptPubKey"=>             
   "21c3335194a791210d8fe60d5f3c767f238b410c115c708c987098ffd61d56bf25e1bca914e0af93c9eb8494e9ddbd63a3b839bf718856d6c687",           
  "confirmations"=>3,          
  "spendable"=>true,
  "solvable"=>true,
  "safe"=>true}]  
```

### カラー付きアドレスの生成

UTXOからカラーの一覧を出す

```ruby
def colors
  utxos=tapyrusRPC('listunspent',[])
  return utxos.map{|x|x["token"]}.uniq
end

c=colors

=> 
["TPC",                          
 "c2c61090ce2cc17b30b2234355dcf4baa7a212f12dcbbed4facccc08e3984ad446",
 "c3335194a791210d8fe60d5f3c767f238b410c115c708c987098ffd61d56bf25e1",
 "c26a15e5ade9c5c137e04ce7e200b3a9566dcefb403781221794e9471145738cb2",
 "c1265b7a6ce4237625f4bbbb2bcc03bd8cc5d0b1574ae17d0d452afd483eabb51d"]
```

カラー付きアドレスを生成する

```ruby
addrNRT1 = tapyrusRPC('getnewaddress',["user#{rand(10000)}",color1])
=> "vt8xz9MePQj3DrCR4xCBU2E1N64mRHRreSX7t6b4g9FzVMRhyG6KarYNkgtg8FhVTpHLBpcspNsNAP"
addrNFT1 = tapyrusRPC('getnewaddress',["user#{rand(10000)}",color2])
=> "vw6aD8hZXoNBBLWRehHPBS3QtWZQ2oscdyTD9gucbuS2Wndq61srxpaoHz3fBVeWJcum6GPvis3mbW"
addrRT1 = tapyrusRPC('getnewaddress',["user#{rand(10000)}",color3])
=> "vgt1L6fTUm1wjKsbo1Gkipbp74nveakeTRaAMgzwu6VUJapbi7nTb9mwH5vcm4HCDCaDqmp59PB6ke"
```

### トークンの送付

```ruby
tt1=tapyrusRPC('transfertoken',[addrNRT1,10])

tt2=tapyrusRPC('transfertoken',[addrNFT1,1])

tt3=tapyrusRPC('transfertoken',[addrRT1,15])
```

### トランザクションの解析

```ruby
tapyrusRPC('decoderawtransaction',[tapyrusRPC('getrawtransaction',[tt1])])

tapyrusRPC('decoderawtransaction',[tapyrusRPC('getrawtransaction',[tt2])])

tapyrusRPC('decoderawtransaction',[tapyrusRPC('getrawtransaction',[tt3])])
```

###  Tapyrus送金プログラム

```ruby
#  Tapyrus送金メソッド
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
# 送金のテスト 
# 送金の実行。実行結果はトランザクションID
txid = send_tapyrus(bob, 0.0001, alice)

```
