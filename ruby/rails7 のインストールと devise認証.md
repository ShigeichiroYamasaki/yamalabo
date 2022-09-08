# rails7 のインストールと devise認証

最終修正 2022/07/21

### Rubyのインストール

ここを参照してください　[Rubyのインストール](./ruby.md)

#### yarnのインストール

```bash
curl -o- -L https://yarnpkg.com/install.sh | bash
source ~/.bashrc
```

#### nvmのインストール

```bash
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.37.2/install.sh | bash
source ~/.bashrc
```

#### node 12.13.0 のインストール

```bash
nvm install 12.13.0
nvm use v12.13.0
```


### Railsインストール

```bash
gem install bundler
gem install sqlite3
gem install json-jwt
gem install jwt
gem install nokogiri
gem install devise
gem install bootstrap
gem install jquery-rails
gem install bootstrap-sass
gem install sass-rails
gem install devise-i18n
gem install devise-i18n-views
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
Market 1 : n Wallet
Market 1 : n Token
Wallet 1 : n Address
```

## Railsプロジェクトの生成

アセットパイプラインは使用しません

```bash
rails new tapyrus_wallet --skip-sprockets

cd tapyrus_wallet
```

```
bundle install
```

#### scaffold で MVCを生成

```bash
rails g scaffold Market name
rails g scaffold Wallet market_id:integer owner
rails g scaffold Token market_id:integer color
rails g scaffold Address wallet_id:integer color
```

#### モデル間の関連を定義

* Marketモデル

nano app/models/market.rb 

```ruby
class Market < ApplicationRecord
  has_many :wallets
  has_many :tokens
end
```


* Walletモデル

nano app/models/wallet.rb 

```ruby
class Wallet < ApplicationRecord
  belongs_to :market
  has_many :addresses
end
```

* Tokenモデル

nano app/models/token.rb 

```ruby
class Token < ApplicationRecord
  belongs_to :wallet
end
```

* Addressモデル

nano app/models/address.rb 

```ruby
class Address < ApplicationRecord
  belongs_to :wallet
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
