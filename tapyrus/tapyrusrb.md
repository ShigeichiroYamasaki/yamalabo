# tapyrusrb

最終更新 2022/07/22 Shigeichiro Yamasaki

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
include Tapyrus
include Tapyrus::Opcodes

Tapyrus.chain_params = :prod

# tapyrus-cli コマンドのフルパス
Tapyrus_cli ='/home/yamalabo/tapyrus-core-0.5.1/bin/tapyrus-cli'

# RPC
def tapyrusRPC(method,params)
    r=`#{Tapyrus_cli} #{method} #{params.join(' ')}`.chomp
    begin
        return JSON.parse(r)
    rescue JSON::ParserError
        return r
    end
end
```

### 実行確認

```ruby
balance = tapyrusRPC('getbalance',[])
=> 29.12661982

utxos = tapyrusRPC('listunspent',[])
=> 
[{"txid"=>"47556d8f51fd5bc5d39eb0d50fd881fd26b0351c8ec39f094a8c6a5966080b27",
...

alice = tapyrusRPC('getnewaddress',['alice'])
=> "1Aq7soZtCEuaHkjExL9kTWQPsJGW1REb8e"

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
    # NFTの発行
    rt = tapyrusRPC('issuetoken',[1, amount, tpcs[0]["scriptPubKey"]])
end

# RT発行
rt=mint_RT(1000)
```

RTトークンのcolorとUTXOの確認

```ruby
color3 = rt["color"]

# 10分後

nfts = tapyrusRPC('listunspent',[]).select{|x|x["token"]==color3}
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
addrNRT1 = tapyrusRPC('getnewaddress',["user#{rand(10000)}",c[1]])
=> "vt8xz9MePQj3DrCR4xCBU2E1N64mRHRreSX7t6b4g9FzVMRhyG6KarYNkgtg8FhVTpHLBpcspNsNAP"
addrNFT1 = tapyrusRPC('getnewaddress',["user#{rand(10000)}",c[2]])
=> "vw6aD8hZXoNBBLWRehHPBS3QtWZQ2oscdyTD9gucbuS2Wndq61srxpaoHz3fBVeWJcum6GPvis3mbW"
addrRT1 = tapyrusRPC('getnewaddress',["user#{rand(10000)}",c[4]])
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

--

## Railsプロジェクトの生成

アセットパイプラインは使用しません

```bash
rails new tapyrus_wallet --skip-sprockets -css=bootstrap
cd tapyrus_wallet
```

#### Gemfile修正

nano Gemfile

以下を追加

```ruby
gem "tapyrus"
gem "json"
gem "sassc-rails"
```

```
bundle update
bundle install
```

#### scaffold で MVCを生成

```bash
rails g scaffold Market name
rails g scaffold Wallet market_id:integer owner
```

#### モデル間の関連を定義

* Marketモデル

nano app/models/market.rb 

```ruby
class Market < ApplicationRecord
  has_many :wallets
end
```


* Walletモデル

nano app/models/wallet.rb 

```ruby
class Wallet < ApplicationRecord
  belongs_to :market
end
```

### DB のマイグレーション

```bash
rails db:migrate
```

### サーバーの起動

```bash
rails s -b 0.0.0.0
```

### ブラウザから確認

http://106.157.214.199:3000/markets

http://192.168.0.247:3000/markets

### QR codeの表示

Google chart API のQR codeジェネレータを利用

<img src="https://chart.apis.google.com/chart?chs=150x150&cht=qr&chl=http://106.157.214.199:3000/markets" alt="Token Market">

### マーケットのビューの修正

nano app/views/layouts/application.html.erb

```ruby
<!DOCTYPE html>
<html>
  <head>
    <title>TapyrusWallet</title>
    <meta name="viewport" content="width=device-width,initial-scale=1">
    <%= csrf_meta_tags %>
    <%= csp_meta_tag %>

    <%= stylesheet_link_tag "application", "data-turbo-track": "reload" %>
    <%= javascript_importmap_tags %>
  </head>

  <body>
<h3>近畿大学Tokenマーケットプレイス</h3>
    <%= yield %>
  </body>
</html>

```

### ワレットビューの修正

```ruby
<p style="color: green"><%= notice %></p>
<h3>Tapyrusトークン用ワレット</h3>

<%= render @wallet %>

<div>
  <%= link_to "近畿大学Tokenマーケットプレイス", markets_path(1) %>
</div>
```
