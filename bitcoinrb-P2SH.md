# bitcoinrbでP2WSHを実装

2022/02/22

bitcoinでスマートコントラクトを記述するときの基本は P2WSH です

P2WSHの代表例はマルチシグですが、それ以外のケースも重要です。

## bitcoinrb WiKi

開発者の安土さんによる Wikiです。
基本的な使用法は、ここでわかります。

[https://github.com/chaintope/bitcoinrb/wiki](https://github.com/chaintope/bitcoinrb/wiki)

## bitcoinrb のクラスとメソッドの一覧

[https://www.rubydoc.info/gems/bitcoinrb/0.1.5](https://www.rubydoc.info/gems/bitcoinrb/0.1.5)

#### bitcoinRPC

```ruby
require 'bitcoin'
require 'net/http'
require 'json'
include Bitcoin
include Bitcoin::Opcodes

Bitcoin.chain_params = :signet

HOST="localhost"
PORT=38332          # mainnetの場合は 8332
RPCUSER="hoge"      # bitcoin core RPCユーザ名
RPCPASSWORD="hoge"  # bitcoin core パスワード
FEE = 0.00002       # 手数料

# bitcoin core RPC を利用するメソッド
def bitcoinRPC(method, params)
    http = Net::HTTP.new(HOST, PORT)
    request = Net::HTTP::Post.new('/')
    request.basic_auth(RPCUSER, RPCPASSWORD)
    request.content_type = 'application/json'
    request.body = { method: method, params: params, id: 'jsonrpc' }.to_json
    JSON.parse(http.request(request).body)["result"]
end

```


## マルチシグアドレスへの送金

#### アドレスの生成

```ruby
alice=bitcoinRPC('getnewaddress', [])
bob=bitcoinRPC('getnewaddress', [])
carol=bitcoinRPC('getnewaddress', [])
david=bitcoinRPC('getnewaddress', [])
```

#### 公開鍵

```ruby
pubkey_a = bitcoinRPC('getaddressinfo', [alice])["pubkey"]
pubkey_b = bitcoinRPC('getaddressinfo', [bob])["pubkey"]
pubkey_c = bitcoinRPC('getaddressinfo', [carol])["pubkey"]
pubkey_d = bitcoinRPC('getaddressinfo', [david])["pubkey"]
```

#### マルチシグアドレスの生成

2 of 3 マルチシグの例

`createmultisig <必要署名数> '[<公開鍵1>,<公開鍵2>,...,<公開鍵m>]' <アドレスタイプ>`

```ruby
multisig_addr= bitcoinRPC('createmultisig', [2,[pubkey_a, pubkey_b, pubkey_a], 'bech32'])
```

生成されたマルチシグアドレスと redeemScriptの確認

```ruby
 multisig_addr
=> 
{"address"=>"tb1qmlzzudzmhq4jzs56k6zk0hxzzcyhcudzz6me4s4fy8xyc9tyr76qkkuxw0",
 "redeemScript"=>
  "5221024e8e5bff159084938ddb2054f3f8445d8781ac53dcb20149404c1030bd143a9a21027bc785bf5b0647d8c876b693a2cff7dfe9b7daf409b83cf4597e6c182c37a3c821024e8e5bff159084938ddb2054f3f8445d8781ac53dcb20149404c1030bd143a9a53ae",
 "descriptor"=>
  "wsh(multi(2,024e8e5bff159084938ddb2054f3f8445d8781ac53dcb20149404c1030bd143a9a,027bc785bf5b0647d8c876b693a2cff7dfe9b7daf409b83cf4597e6c182c37a3c8,024e8e5bff159084938ddb2054f3f8445d8781ac53dcb20149404c1030bd143a9a))#fjj3fhln"}
```

#### マルチシグのP2WSHアドレス

```ruby
p2wshaddr = multisig_addr["address"]
```

#### マルチシグの場合のP2WSHの scriptPubKey

```ruby
scriptPubKey0 = Bitcoin::Script.parse_from_addr(p2wshaddr)
```

####  P2WPKH（output）トランザクションの作成

```ruby
# 最初のUTXOを利用することにします
utxo = bitcoinRPC('listunspent', [])[0]
amount = 0.001
change = utxo["amount"]-amount-FEE
addrChange = utxo["address"]

# P2WSHトランザクションの構成

def p2wsh_transaction(p2wshaddr, amount, utxo, change, addrChange)
    # トランザクションのテンプレートの生成
    tx = Bitcoin::Tx.new
    # トランザクションのinputの構成
    tx = make_inputs(tx,utxo)
    # トランザクションのoutputの構成
    tx = make_outputs(tx,amount, p2wshaddr, change, addrChange)
    return tx
end
```

トランザクションのinputの構成

```ruby
def make_inputs(tx, utxo)
    # UTXOをinputから参照するための txid と vout としてエンコードする
    outpoint = Bitcoin::OutPoint.from_txid(utxo["txid"], utxo["vout"])
    # エンコードした参照をトランザクションのinputに埋め込む
    tx.in << Bitcoin::TxIn.new(out_point: outpoint)
    return tx
end
```

トランザクションのP2WSH outputの構成

```ruby
def make_outputs(tx, amount, p2wshaddr, change, addrChange)
    # 送金用outputの構成(P2WSH)
        # 金額を satoshiの整数に変換
    amount_satoshi = (amount*(10**8)).to_i
        # ビットコインアドレスから p2wpkhのscript_pubkey を生成
    scriptPubKey0 = Bitcoin::Script.parse_from_addr(p2wshaddr)
        # エンコードしたscript_pubkeyをトランザクションのoutputに埋め込む
    tx.out << Bitcoin::TxOut.new(value: amount_satoshi , script_pubkey: scriptPubKey0)

    # おつり用outputの構成(P2PKH)
        # 金額を satoshiの整数に変換
    change_satoshi =  (change*(10**8)).to_i
        # ビットコインアドレスから p2wpkhのscript_pubkey を生成
    scriptPubKey1 = Bitcoin::Script.parse_from_addr(addrChange)
        # エンコードしたscript_pubkeyをトランザクションのoutputに埋め込む
    tx.out << Bitcoin::TxOut.new(value: change_satoshi, script_pubkey: scriptPubKey1)
    return tx
end
```

未署名のトランザクションの確認

```ruby
tx = p2wsh_transaction(p2wshaddr, 0.01, utxo, change, addrChange)

tx.to_h
=> 
{:txid=>"04f7e474c90d0f38ac3bf7b91a0bc432d68c0075ae7c8ef6b257569d24858cb7",
 :hash=>"04f7e474c90d0f38ac3bf7b91a0bc432d68c0075ae7c8ef6b257569d24858cb7",
 :version=>1,
 :size=>125,
 :vsize=>125,
 :locktime=>0,
 :vin=>[{:txid=>"b9736d3b7b29860e9a5385e13b458ff291801621617cfcc7c253c2842015a10f", :vout=>0, :script_sig=>{:asm=>"", :hex=>""}, :sequence=>4294967295}],
 :vout=>
  [{:value=>0.01,
    :script_pubkey=>
     {:asm=>"0 8f7d40104cd9caacf29b97ca5f9010440f91f7f1719bcf2772daeac3b4f28239",
      :hex=>"00208f7d40104cd9caacf29b97ca5f9010440f91f7f1719bcf2772daeac3b4f28239",
      :type=>"witness_v0_scripthash",
      :req_sigs=>1,
      :addresses=>["tb1q3a75qyzvm892eu5mjl99lyqsgs8eral3wxdu7fmjmt4v8d8jsgus3cxg32"]},
    :n=>0},
   {:value=>0.00898,
    :script_pubkey=>
     {:asm=>"0 431a5ea6cfab13c4cee6e335d36b2899333f456d",
      :hex=>"0014431a5ea6cfab13c4cee6e335d36b2899333f456d",
      :type=>"witness_v0_keyhash",
      :req_sigs=>1,
      :addresses=>["tb1qgvd9afk04vfufnhxuv6ax6egnyen73tdsc7rdl"]},
    :n=>1}]}

```


トランザクションへの署名

```ruby
def sign_inputs(utxos, tx)
    utxos.each.with_index{|utxo,index|
        # UTXOのscriptPubKey をオブジェクト化する
        script_pubkey = Bitcoin::Script.parse_from_payload(utxo["scriptPubKey"].htb)
        # scriptPubKey の送金先アドレス
        myaddr = script_pubkey.to_addr
        # UTXOの送付先アドレスの秘密鍵（署名鍵）
        priv = bitcoinRPC('dumpprivkey', [myaddr])
        # 署名鍵オブジェクト
        key = Bitcoin::Key.from_wif(priv)
        # UTXOの金額
        satoshi = (utxo["amount"]*(10**8)).to_i
        case script_pubkey.type
        when "witness_v0_keyhash"   # UTXOがP2WPKHタイプ
            # トランザクションのハッシュ値を計算
            sighash = tx.sighash_for_input(index, script_pubkey, sig_version: :witness_v0, amount: satoshi)
            # トランザクションへの署名＋署名タイプ情報を付加
            sig = key.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
            # witness scriptの追加
            tx.in[index].script_witness.stack << sig
            # 公開鍵の追加
            tx.in[index].script_witness.stack << key.pubkey.htb
        end
    }
    return tx
end

tx = sign_inputs([utxo], tx)

tx.to_h

=> 
{:txid=>"04f7e474c90d0f38ac3bf7b91a0bc432d68c0075ae7c8ef6b257569d24858cb7",
 :hash=>"c0121ee436d696c054625d09e536f2b9f16a5d354904c448579b5e4d578c4569",
 :version=>1,
 :size=>234,
 :vsize=>153,
 :locktime=>0,
 :vin=>
  [{:txid=>"b9736d3b7b29860e9a5385e13b458ff291801621617cfcc7c253c2842015a10f",
    :vout=>0,
    :script_sig=>{:asm=>"", :hex=>""},
    :txinwitness=>
     ["30440220080d8879a6dd3672c0c4de7e82809319205d420e1589a59eb024d1b2640f326a022057ae836a91c2aa71c1eac31ac3cda3ddc29e34002796cee172f48bb74256ea7601",
      "02ee5e30fff4b4ef5a5a7a097657da464a116593f4d6de31aacfafde42cafd565d"],
    :sequence=>4294967295}],
 :vout=>
  [{:value=>0.01,
    :script_pubkey=>
     {:asm=>"0 8f7d40104cd9caacf29b97ca5f9010440f91f7f1719bcf2772daeac3b4f28239",
      :hex=>"00208f7d40104cd9caacf29b97ca5f9010440f91f7f1719bcf2772daeac3b4f28239",
      :type=>"witness_v0_scripthash",
      :req_sigs=>1,
      :addresses=>["tb1q3a75qyzvm892eu5mjl99lyqsgs8eral3wxdu7fmjmt4v8d8jsgus3cxg32"]},
    :n=>0},
   {:value=>0.00898,
    :script_pubkey=>
     {:asm=>"0 431a5ea6cfab13c4cee6e335d36b2899333f456d",
      :hex=>"0014431a5ea6cfab13c4cee6e335d36b2899333f456d",
      :type=>"witness_v0_keyhash",
      :req_sigs=>1,
      :addresses=>["tb1qgvd9afk04vfufnhxuv6ax6egnyen73tdsc7rdl"]},
    :n=>1}]}

```

デプロイ

```ruby

```















## P2WSH送金プログラム

```ruby
require 'bitcoin'
require 'net/http'
require 'json'
include Bitcoin
include Bitcoin::Opcodes

Bitcoin.chain_params = :signet

HOST="localhost"
PORT=38332          # mainnetの場合は 8332
RPCUSER="hoge"      # bitcoin core RPCユーザ名
RPCPASSWORD="hoge"  # bitcoin core パスワード
FEE = 0.00002       # 手数料

# bitcoin core RPC を利用するメソッド
def bitcoinRPC(method, params)
    http = Net::HTTP.new(HOST, PORT)
    request = Net::HTTP::Post.new('/')
    request.basic_auth(RPCUSER, RPCPASSWORD)
    request.content_type = 'application/json'
    request.body = { method: method, params: params, id: 'jsonrpc' }.to_json
    JSON.parse(http.request(request).body)["result"]
end

# Bitcoin送金メソッド
    # 送金先アドレス、送金金額

def send_bitcoin(addr, amount)
    # 所持金残高を確認
    balance = bitcoinRPC('getbalance', [])
    if balance < (amount+FEE) then
        puts "error (残高不足)"
    else
        # 送金金額＋手数料をぎりぎり上回るUTXOリストの作成
        utxos = consuming_utxos(amount+FEE)
        # 送金に使用するUTXOの総額
        fund = utxos.map{|utxo|utxo["amount"]}.sum
        # UTXOの総額 - 送金金額 - 手数料 = おつり
        change = fund-amount-FEE
        # おつり用アドレス（UTXOの０番目を受け取ったアドレスを利用する）
        addrChange = utxos[0]["address"]
        # トランザクションの構成（P2WPKH)
        tx = p2wpkh_transaction(addr, amount, utxos, change, addrChange)
        # トランザクションへの署名
        tx = sign_inputs(utxos, tx)
        # ビットコインネットワークへのデプロイ
        return bitcoinRPC('sendrawtransaction', [tx.to_hex])
    end
end


# P2WPKHトランザクションの構成

def p2wpkh_transaction(addr,amount, utxos, change, addrChange)
    # トランザクションのテンプレートの生成
    tx = Bitcoin::Tx.new
    # トランザクションのinputの構成
    tx = make_inputs(tx,utxos)
    # トランザクションのoutputの構成
    tx = make_outputs(tx,amount, addr, change, addrChange)
    return tx
end

# トランザクションのinputの構成

def make_inputs(tx, utxos)
    utxos.each{|utxo|
        # UTXOをinputから参照するための txid と vout としてエンコードする
        outpoint = Bitcoin::OutPoint.from_txid(utxo["txid"], utxo["vout"])
        # エンコードした参照をトランザクションのinputに埋め込む
        tx.in << Bitcoin::TxIn.new(out_point: outpoint)
    }
    return tx
end

# トランザクションのoutputの構成

def make_outputs(tx,amount, addr, change, addrChange)
    # 送金用outputの構成
        # 金額を satoshiの整数に変換
    amount_satoshi = (amount*(10**8)).to_i
        # ビットコインアドレスから p2wpkhのscript_pubkey を生成
    scriptPubKey0 = Bitcoin::Script.parse_from_addr(addr)
        # エンコードしたscript_pubkeyをトランザクションのoutputに埋め込む
    tx.out << Bitcoin::TxOut.new(value: amount_satoshi , script_pubkey: scriptPubKey0)
    # おつり用outputの構成
        # 金額を satoshiの整数に変換
    change_satoshi =  (change*(10**8)).to_i
        # ビットコインアドレスから p2wpkhのscript_pubkey を生成
    scriptPubKey1 = Bitcoin::Script.parse_from_addr(addrChange)
        # エンコードしたscript_pubkeyをトランザクションのoutputに埋め込む
    tx.out << Bitcoin::TxOut.new(value: change_satoshi, script_pubkey: scriptPubKey1)
    return tx
end

# トランザクションへの署名

def sign_inputs(utxos, tx)
    utxos.each.with_index{|utxo,index|
        # UTXOのscriptPubKey をオブジェクト化する
        script_pubkey = Bitcoin::Script.parse_from_payload(utxo["scriptPubKey"].htb)
        # scriptPubKey の送金先アドレス
        myaddr = script_pubkey.to_addr
        # UTXOの送付先アドレスの秘密鍵（署名鍵）
        priv = bitcoinRPC('dumpprivkey', [myaddr])
        # 署名鍵オブジェクト
        key = Bitcoin::Key.from_wif(priv)
        # UTXOの金額
        satoshi = (utxo["amount"]*(10**8)).to_i
        case script_pubkey.type
        when "witness_v0_keyhash"   # UTXOがP2WPKHタイプ
            # トランザクションのハッシュ値を計算
            sighash = tx.sighash_for_input(index, script_pubkey, sig_version: :witness_v0, amount: satoshi)
            # トランザクションへの署名＋署名タイプ情報を付加
            sig = key.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
            # witness scriptの追加
            tx.in[index].script_witness.stack << sig
            # 公開鍵の追加
            tx.in[index].script_witness.stack << key.pubkey.htb
        end
    }
    return tx
end

# 送金金額＋手数料をぎりぎり上回るUTXOリストの作成

def consuming_utxos(amount)
    # ワレットの未使用のUTXOの一覧を得る
    unspent = bitcoinRPC('listunspent', [])
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
    # 引数は，送金先アドレス，送金金額，手数料


amount = 0.01
alice=bitcoinRPC('getnewaddress', [])
    # 送金の実行。実行結果はトランザクションID
txid = send_bitcoin(alice, amount)
```
