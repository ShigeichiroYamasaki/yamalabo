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

### Railsのインストール実行

```bash
chmod u+x install_rails.sh

./install_rails.sh
```


## Railsアプリのモデル例

Railsアプリを作成するためには、まずモデルを設計する必要があります

ここでは、仮想通貨のワレットを作成してみます。
bitcoin coreなどにはワレット機能があるので、フロントエンドを作成するだけならDB作成は冗長なだけですが、ここではあえて冗長構成にします。

```
Wallet: 個人が所有するワレット
    owner:所有者

Log: 送金履歴
    date:日時
    sendto: 送金先アドレス
    amount: 金額
```

###  モデル間の関連

```
# 一つのワレットには、複数の送金履歴が対応する
Wallet 1 : n Log    
```

## Railsプロジェクトの生成

アセットパイプラインは使用しません

```bash
rails new bitcoin_wallet --skip-sprockets

cd bitcoin_wallet
bundle install
```

#### scaffold で MVCを生成

```bash
rails g scaffold Wallet owner
rails g scaffold Log date:date sendto amount:float
```

#### モデル間の関連を定義

* Walletモデル

nano app/models/wallet.rb 

```ruby
class Wallet < ApplicationRecord
    has_many :logs
end
```


* Logモデル

nano app/models/log.rb 

```ruby
class Log < ApplicationRecord
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

http://<ipアドレス>:3000/wallets

確認後，control-c でサーバを停止

