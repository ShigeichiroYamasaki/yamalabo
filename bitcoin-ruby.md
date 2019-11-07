# bitcin-ruby チュートリアル

## インストール

### bitcoin core のインストール



## testnetで稼働させる


## testnet の bitcoin faucet でビットコインを入手する

[Bitcoin testnet3 faucet](https://coinfaucet.eu/en/btc-testnet/)

### testnet でノードを稼働させる

bitcoin.conf 

```
testnet = 3
# txindexの設定が必要
txindex = 1
server = 1
rest = 1
rpcuser = yamalabo
rpcpassword = yozoranomukou
rpcport = 18332
```
### openassets ruby のインストール

```
gem install bitcoin
gem install ecdsa
gem install openassets-ruby
gem install openssl
gem install bech32
gem install rqrcode_png
gem install sqlite3
gem install rails
gem install rqrcode_png
```

## bitcoin-ruby によるBitcoin RPC 

### ライブラリと定数

```ruby
require 'bitcoin'
require 'net/http'
require 'json'
require 'uri'
require 'rqrcode'
require 'rqrcode_png'
require 'uri'
require 'securerandom'

# testネット
Bitcoin.network = :testnet3
# ユーザID
RPCUSER = "yamalabo"        # JSON RPC のためのユーザ名
# パスワード
RPCPASSWORD = パスワード    # JSON RPC のためのパスワード
HOST="localhost"          # JSON RPC のhost
PORT=18332                # ポート番号

```

### bitcoin RPC

```ruby
#bitcoindへのHTTPアクセスするメソッド

def bitcoinRPC(method,param)
    http = Net::HTTP.new(HOST, PORT)
    request = Net::HTTP::Post.new('/')
    request.basic_auth(RPCUSER, RPCPASSWORD)
    request.content_type = 'application/json'
    request.body = {method: method, params: param, id: 'jsonrpc'}.to_json
    return JSON.parse(http.request(request).body)["result"]
end
```

## 自分の秘密鍵と公開鍵を得る

* dumpprivkey でWIF形式の秘密鍵を得る
* base58変換とヘッダ、チェックサムを削除する

```ruby
def get_privkey(address,psw)
	bitcoinRPC('walletpassphrase',[psw, 30000])
	wif_key=bitcoinRPC('dumpprivkey',[address])
	return Bitcoin::Key.from_base58(wif_key)
	bitcoinRPC('walletlock',[])
end


privkey=get_privkey(MY_ADDR, RPCPASSWORD)
=> #<Bitcoin::Key:0x00007fee388d1098 @key=#<OpenSSL::PKey::EC:0x00007fee388d0e68>, @pubkey_compressed=true>

```

### 16進数形式の秘密鍵と公開鍵

```ruby
 privkey.priv   
=> "56b18b9400ec672418b2dfe4b6f1e24fcaacce357c00f33ac35ea91571e5a207"

pubkey=privkey.pub 
=> "03569febd1ba6725e19d4f13670435cff2e45ec623df8c2f4a905c9dab22b7e157"
```

## bitcoin の送金

```ruby
def send_btc(dest_addr,amount,psw)
	bitcoinRPC('walletpassphrase',[psw, 300])
	return bitcoinRPC('sendtoaddress',[dest_addr, amount])
	bitcoinRPC('walletlock',[])
end
```

### ワレットの状態をチェックする

####  ワレットの残高を得る

```ruby
balance=bitcoinRPC('getbalance',[]) 	
```
#### UTXOのリストを得る

```ruby
utxos=bitcoinRPC('listunspent',[])	
```

#### アドレスの一覧を得る

```ruby
addrs=utxos.map{|x|x["address"]}
```

#### 手数料のデフォルト金額の設定

```ruby
bitcoinRPC('settxfee',[0.00001]) 
```

#### 暗号化された秘密鍵を復号化と再暗号化

```ruby
PSW='yozoranomukou'
# パスワード　、保持時間（ミリ秒）
bitcoinRPC('walletpassphrase',[PSW,3000]) 
bitcoinRPC('walletlock',[])
```

### bitcoinの送金

```ruby
# 送金先アドレス
dest_addr="tb1qct7uxdjydtlf0jtrwvgue259adzugx5yltm6qv"
# 送金金額
amount=0.001

txid = send_btc(dest_addr,amount,PSW)
```

### トランザクションの内容の確認

```ruby
tx_data=bitcoinRPC('gettransaction', [txid])

rtx = bitcoinRPC('getrawtransaction',[txid])

tx=bitcoinRPC('decoderawtransaction',[rtx])
```

#### トランザクションの内容

```ruby
 tx.keys 
=> ["txid", "hash", "version", "size", "vsize", "weight", "locktime", "vin", "vout"]
```

```ruby
tx["txid"]
=> "68d006ff87125dc52c0d36501ea980e276479129296c102d39ba035e071760a2"

tx["hash"]
=> "d54f414f1bb6b2adc1904346e3c3ce109a2d5c79d2d8c840b91e397bdb40dadd"

tx["version"]
=> 2

tx["size"]
=> 247

tx["vsize"]
=> 166

tx["weight"]
=> 661

tx["locktime"]
=> 1569758

tx["vin"]
=> [{"txid"=>"d53bec8a4c95870b107f547cf4c271796080e5556bb5ebb167182d76b1acfdb4", "vout"=>1, "scriptSig"=>{"asm"=>"0014dd659d380b1fd6db51a7974d5a41eb32c742c802", "hex"=>"160014dd659d380b1fd6db51a7974d5a41eb32c742c802"}, "txinwitness"=>["304402201f699b530801d0e7799dbd1d45ed117595ecb3d470625b5054babc46c7f9569a02205fe3c426297967835a288f311666b3bb1810ef2e95deab7f0fb16d8664a05a8001", "02d7343ec72f14b60e71b16a2a91ee010c48327a24de01d2435fecb07a9646d99e"], "sequence"=>4294967294}]

 tx["vout"]
=> [{"value"=>0.03502033, "n"=>0, "scriptPubKey"=>{"asm"=>"OP_HASH160 4bd2882750d8cddc51a15b8a5dbdb84ef6721e9c OP_EQUAL", "hex"=>"a9144bd2882750d8cddc51a15b8a5dbdb84ef6721e9c87", "reqSigs"=>1, "type"=>"scripthash", "addresses"=>["2MzA8sgRqCiheJxtbRtCMBkGBRHKY4PBXCs"]}}, {"value"=>0.001, "n"=>1, "scriptPubKey"=>{"asm"=>"OP_HASH160 9f6cd340267747c3e1c79394289828081e369ab5 OP_EQUAL", "hex"=>"a9149f6cd340267747c3e1c79394289828081e369ab587", "reqSigs"=>1, "type"=>"scripthash", "addresses"=>["2N7nBo6Jg4VRuJC24T9E4PRLfPJ3tWyTYLS"]}}]
```

```ruby
tx["vin"][0].keys
=> ["txid", "vout", "scriptSig", "txinwitness", "sequence"]

tx["vin"][0]["txid"]
=> "d53bec8a4c95870b107f547cf4c271796080e5556bb5ebb167182d76b1acfdb4"

tx["vin"][0]["vout"]
=> 1

tx["vin"][0]["scriptSig"]
=> {"asm"=>"0014dd659d380b1fd6db51a7974d5a41eb32c742c802", "hex"=>"160014dd659d380b1fd6db51a7974d5a41eb32c742c802"}


tx["vin"][0]["txinwitness"]
=> ["304402201f699b530801d0e7799dbd1d45ed117595ecb3d470625b5054babc46c7f9569a02205fe3c426297967835a288f311666b3bb1810ef2e95deab7f0fb16d8664a05a8001", "02d7343ec72f14b60e71b16a2a91ee010c48327a24de01d2435fecb07a9646d99e"]

tx["vin"][0]["sequence"]
=> 4294967294
```

```ruby
tx["vout"][0]   
=> {"value"=>0.03502033, "n"=>0, "scriptPubKey"=>{"asm"=>"OP_HASH160 4bd2882750d8cddc51a15b8a5dbdb84ef6721e9c OP_EQUAL", "hex"=>"a9144bd2882750d8cddc51a15b8a5dbdb84ef6721e9c87", "reqSigs"=>1, "type"=>"scripthash", "addresses"=>["2MzA8sgRqCiheJxtbRtCMBkGBRHKY4PBXCs"]}}

tx["vout"][1]
=> {"value"=>0.001, "n"=>1, "scriptPubKey"=>{"asm"=>"OP_HASH160 9f6cd340267747c3e1c79394289828081e369ab5 OP_EQUAL", "hex"=>"a9149f6cd340267747c3e1c79394289828081e369ab587", "reqSigs"=>1, "type"=>"scripthash", "addresses"=>["2N7nBo6Jg4VRuJC24T9E4PRLfPJ3tWyTYLS"]}}

tx["vout"][0].keys 
=> ["value", "n", "scriptPubKey"]

tx["vout"][0]["value"]
=> 0.03502033

tx["vout"][0]["n"]
=> 0

tx["vout"][1]["n"]
=> 1

tx["vout"][0]["scriptPubKey"]
=> {"asm"=>"OP_HASH160 4bd2882750d8cddc51a15b8a5dbdb84ef6721e9c OP_EQUAL", "hex"=>"a9144bd2882750d8cddc51a15b8a5dbdb84ef6721e9c87", "reqSigs"=>1, "type"=>"scripthash", "addresses"=>["2MzA8sgRqCiheJxtbRtCMBkGBRHKY4PBXCs"]}
```

```ruby
tx["vout"][0]["scriptPubKey"].keys 
=> ["asm", "hex", "reqSigs", "type", "addresses"]

tx["vout"][0]["scriptPubKey"]["asm"]
=> "OP_HASH160 4bd2882750d8cddc51a15b8a5dbdb84ef6721e9c OP_EQUAL"

tx["vout"][0]["scriptPubKey"]["hex"]
=> "a9144bd2882750d8cddc51a15b8a5dbdb84ef6721e9c87"

tx["vout"][0]["scriptPubKey"]["reqSigs"]
=> 1

tx["vout"][0]["scriptPubKey"]["type"]
=> "scripthash"

tx["vout"][0]["scriptPubKey"]["addresses"]
=> ["2MzA8sgRqCiheJxtbRtCMBkGBRHKY4PBXCs"]
```


### ビットコイン支払い請求の作成

```ruby
require 'uri'

# 送金先アドレス、送金金額、請求元名、品名
def bitcoin_urn(addr, amount, label, message)
	urn= "bitcoin:#{addr}?amount=#{amount}&label=URI.encode(label)&message=#{URI.encode(message)}"
end
```

```ruby
url=bitcoin_urn(dest_addr, amount, "satoshi book", "book")
```

### スクリプト

2+4=6 を検証する例

```ruby
script=Bitcoin::Script.from_string("2 4 OP_ADD 6 OP_EQUAL")

script.to_string
=> "2 4 OP_ADD 6 OP_EQUAL"

script.to_payload
=> "RT\x93V\x87"

script.to_binary
=> "RT\x93V\x87"

script.run
=> true
```

動きを追跡する

```ruby
irb(main):125:0> script.debug[0]
=> []
irb(main):126:0> script.debug[1]
=> "OP_2"
irb(main):127:0> script.debug[2]
=> [2]
irb(main):128:0> script.debug[3]
=> "OP_4"
irb(main):129:0> script.debug[4]
=> [2, 4]
irb(main):130:0> script.debug[5]
=> "OP_ADD"
irb(main):131:0> script.debug[6]
=> [6]
irb(main):132:0> script.debug[7]
=> "OP_6"
irb(main):133:0> script.debug[8]
=> [6, 6]
irb(main):134:0> script.debug[9]
=> "OP_EQUAL"
irb(main):135:0> script.debug[10]
=> [1]
```

```ruby
def trace(script)
  (0..).each do |n|
    r=script.debug[n]
    puts r
    break if r==nil
  end
end


trace(script)
OP_2
2
OP_4
2
4
OP_ADD
6
OP_6
6
6
OP_EQUAL
1
RESULT

=> nil
```

## トランザクションの作成と送金

* インプット用UTXOの準備
* トランザクションのスケルトンの作成
* インプットの作成
* アウトプットの作成
* 署名
* トランザクションを一度に作成する
* ブロードキャスト

### インプット用UTXOの準備

```ruby
utxos=bitcoinRPC('listunspent',[])

utxos[0]
=> {"txid"=>"0150a3892e92de98b55eaff9f0894c8973a645da6d4691f8f74da21ed2f94a06", "vout"=>1, "address"=>"2N7nBo6Jg4VRuJC24T9E4PRLfPJ3tWyTYLS", "label"=>"kindai1", "redeemScript"=>"00140721a231d84508c010b692bda246bce5340ca264", "scriptPubKey"=>"a9149f6cd340267747c3e1c79394289828081e369ab587", "amount"=>0.0005, "confirmations"=>25655, "spendable"=>true, "solvable"=>true, "desc"=>"sh(wpkh([c10f0931/0'/0'/2']023ce3855a13f435a9a35dae89c76492480f008a341b6a8623ae04064204089ec1))#2l09cvcw", "safe"=>true}
```

#### UTXOのトランザクションIDと金額の一覧

[トランザクションID, インデックス(vout), 金額]

```ruby
utxo_list=(0.. (utxos.size-1)).map{|n|[utxos[n]["txid"],utxos[n]["vout"],utxos[n]["amount"]]}
=> [["0150a3892e92de98b55eaff9f0894c8973a645da6d4691f8f74da21ed2f94a06", 1, 0.0005], ["8b9f6230aa923283730ec51a7d28f23a13f05aa5d367e5f73a09711a346be61f", 0, 0.0001], ["845077b20c73d6e6ae67c971b08506ca5469659b992d1850972cb8c0213f4320", 0, 0.00399834], ["7d3a5fc2f91d1d3fd10b8e3d2d484663c3a8e28627a6216e86995208637ce426", 0, 0.001], ["7d3a5fc2f91d1d3fd10b8e3d2d484663c3a8e28627a6216e86995208637ce426", 1, 9.744e-05], ["fd9a50b635baf3eae809c49e89535bd6da5da4b63001122fa631580c4cde2531", 0, 0.001], ["2f9ad561040102cefd007ad71d1e6d59c6bed48a0d0e383235d187e811a57b68", 0, 0.001], ["68d006ff87125dc52c0d36501ea980e276479129296c102d39ba035e071760a2", 0, 0.03502033], ["68d006ff87125dc52c0d36501ea980e276479129296c102d39ba035e071760a2", 1, 0.001], ["d53bec8a4c95870b107f547cf4c271796080e5556bb5ebb167182d76b1acfdb4", 0, 0.001], ["52b51863bf048b5902d6845ab37e682cdd8ef72acb62986fb57189503f8306ef", 1, 0.001]]
```

#### インプットとして利用するUTXO

* UTXOのトランザクションID（トランザクションハッシュ）
* vout
* 金額

```ruby
prev_txid=utxo_list[0][0]
prev_vout=utxo_list[0][1]
prev_amount=utxo_list[0][2]
```

### トランザクションのスケルトンの作成

```ruby
tx = Bitcoin::Protocol::Tx.new
```

### インプットの作成

#### インプット部の構成

```ruby
tx_in = Bitcoin::Protocol::TxIn.from_hex_hash(prev_txid, prev_vout)

=> #<Bitcoin::Protocol::TxIn:0x00007fb8e41a3958 @prev_out_hash="\x06J\xF9\xD2\x1E\xA2M\xF7\xF8\x91Fm\xDAE\xA6s\x89L\x89\xF0\xF9\xAF^\xB5\x98\xDE\x92.\x89\xA3P\x01", @prev_out_index=1, @script_sig_length=0, @script_sig="", @sequence="\xFF\xFF\xFF\xFF", @script_witness=#<Bitcoin::Protocol::ScriptWitness:0x00007fb8e41a2120 @stack=[]>>
```

#### 作成したインプット部をトランザクションのスケルトンに組み込む

```ruby
tx.add_in(tx_in)
=> [#<Bitcoin::Protocol::TxIn:0x00007fb8e41a3958 @prev_out_hash="\x06J\xF9\xD2\x1E\xA2M\xF7\xF8\x91Fm\xDAE\xA6s\x89L\x89\xF0\xF9\xAF^\xB5\x98\xDE\x92.\x89\xA3P\x01", @prev_out_index=1, @script_sig_length=0, @script_sig="", @sequence="\xFF\xFF\xFF\xFF", @script_witness=#<Bitcoin::Protocol::ScriptWitness:0x00007fb8e41a2120 @stack=[]>>]
```

### アウトプットの作成

* 送金先アドレス
* 送金金額(satoshi)
* 手数料(1000 satoshi)

```ruby
dest_addr
=> "2N7nBo6Jg4VRuJC24T9E4PRLfPJ3tWyTYLS"

value=(prev_amount*100000000-1000).to_i
=> 49000

tx_out = Bitcoin::Protocol::TxOut.value_to_address(value, dest_addr)
=> #<Bitcoin::Protocol::TxOut:0x00007fb8e41e03f8 @value=49000, @pk_script_length=23, @pk_script="\xA9\x14\x9Fl\xD3@&wG\xC3\xE1\xC7\x93\x94(\x98(\b\x1E6\x9A\xB5\x87">
```

#### 作成したアウトプットをトランザクションのスケルトンに組み込む

```ruby
tx.add_out(tx_out)
=> [#<Bitcoin::Protocol::TxOut:0x00007fb8e41e03f8 @value=49000, @pk_script_length=23, @pk_script="\xA9\x14\x9Fl\xD3@&wG\xC3\xE1\xC7\x93\x94(\x98(\b\x1E6\x9A\xB5\x87">]
```

#### 作成したトランザクションのアウトプットのスクリプト

```ruby
pk_script=tx.out[0].pk_script 
=> "\xA9\x14\x9Fl\xD3@&wG\xC3\xE1\xC7\x93\x94(\x98(\b\x1E6\x9A\xB5\x87"

script=Bitcoin::Script.new(pk_script)
script.to_string
=> "OP_HASH160 9f6cd340267747c3e1c79394289828081e369ab5 OP_EQUAL"
```

#### rawトランザクションの確認

```ruby
raw_tx=tx.to_payload.bth
=> "0100000001064af9d21ea24df7f891466dda45a673894c89f0f9af5eb598de922e89a350010100000000ffffffff0168bf00000000000017a9149f6cd340267747c3e1c79394289828081e369ab58700000000"
```

getrawtransactionで内容の確認　

```ruby
tx_content=bitcoinRPC('getrawtransaction',[raw_tx])

```

### 署名

* 作成したトランザクション

```ruby

```



### トランザクションを一度に作成する

Bitcoin::Builder::TxBuilder を利用する

* UTXOのvout 
* 署名鍵
```ruby
tx = Bitcoin::Builder::TxBuilder.new do |t|
  t.input do |i|
    i.prev_out prev_tx, 0
    i.signature_key key
  end
  t.output do |o|
    o.value 12345 # 0.00012345 BTC
    o.to key.addr
  end
end
```

### ブロードキャスト



































