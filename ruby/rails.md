# rails8

2025/01/31
Shigeichiro Yamasaki

## Rubyのインストール

ここを参照してください　[Rubyのインストール](./ruby.md)

## nvmのインストール

ここを参照してください [JavaScript](../javascript/JavaScript.md)


### Railsインストール

```bash
gem update --system
gem update

gem install bundler
gem install sqlite3
gem install json-jwt
gem install jwt
gem install rails
```

## Railsアプリのモデル例

Railsアプリを作成するためには、まずモデルを設計する必要があります

ここでは、仮想通貨のワレットを作成してみます。
bitcoin coreなどにはワレット機能があるので、フロントエンドを作成するだけならDB作成は冗長なだけですが、ここではあえて冗長構成にします。

```
Market: 市場
    name

Wallet: 個人が所有するワレット
    market_id
    owner:所有者
Token: トークン
    market_id
    color: トークンのカラー
Address:
    wallet_id:ワレット
    color: トークンのカラー
```

###  モデル間の関連

```
Market 1 : n Saifu
Market 1 : n Token
Saifu 1 : n Address
```

## Railsプロジェクトの生成

アセットパイプラインは使用しません

```bash
rails new wallet 
cd wallet
```

```
bundle install
```

#### scaffold で MVCを生成

```bash
rails g scaffold Market name
rails g scaffold Saifu market_id:integer owner
rails g scaffold Token market_id:integer color
rails g scaffold Address saifu_id:integer color
```

#### モデル間の関連を定義

* Marketモデル

nano app/models/market.rb 

```ruby
class Market < ApplicationRecord
  has_many :saifus
  has_many :tokens
end
```


* Walletモデル

nano app/models/saifu.rb 

```ruby
class Saifu < ApplicationRecord
  belongs_to :market
  has_many :addresses
end
```

* Tokenモデル

nano app/models/token.rb 

```ruby
class Token < ApplicationRecord
  belongs_to :saifu
end
```

* Addressモデル

nano app/models/address.rb 

```ruby
class Address < ApplicationRecord
  belongs_to :saifu
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

http://localhost:3000/markets

