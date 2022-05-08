# rails7 のインストールと devise認証


## Rubyのインストール

[Rubyのインストール](./ruby.md)

★ Ruby 3.1.0 と Rails 7 の組み合わせにはまだ問題があります。

### Railsインストールスクリプトの作成

```bash
nano install_rails.sh
```

```bash
#!/bin/bash

gem install bundler
gem install sqlite3
gem install json-jwt
gem install jwt
gem install nokogiri
gem install devise
gem install bootstrap

gem install rails
gem install jquery-rails
gem install bootstrap-sass
gem install sass-rails
gem install devise-i18n
gem install devise-i18n-views
```

### 実行

```bash
chmod u+x install_rails.sh

./install_rails.sh
```

### Railsの起動テスト

```bash
rails new test1

cd test1
bundle install
```

#### scaffold で生成

```bash
rails g scaffold person name
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

http://localhost:3000/people

確認後，control-c でサーバを停止

## bootstrap のテスト

#### Gemfile編集

```bash
nano Gemfile
```

以下を追加

```ruby
gem 'bootstrap-sass', '~> 3.3.6'
gem 'sass-rails', '>= 3.2'
```

```bash
bundle install
```

### css のファイル名を.css から .scssに変更

```bash
mv app/assets/stylesheets/application.css app/assets/stylesheets/application.scss
```


### SCSSを編集

```bash
nano app/assets/stylesheets/application.scss
```

最後に以下を追加

```
@import "bootstrap-sprockets";
@import "bootstrap"; 
```

以下は削除

```bash
*= require_self
*= require_tree .
```

### application.js を修正

```bash
app/assets/javascripts/application.js
```

以下を追加する

```javascript
//= require jquery
//= require bootstrap-sprockets
```

### サーバー再起動


```bash
rails s -b 0.0.0.0
```

確認後，ctr-c で停止

## deviseによるユーザ管理のテスト

```bash
nano Gemfile
```

以下を追加

```ruby
gem 'devise'
```

```bash
bundle install
```

### devise のジェネレーター

```bash
rails g devise:install
```

### deviseの Userモデルの作成

```bash
rails g devise User

rails db:migrate
```

### サーバ起動テスト


```bash
rails s -b 0.0.0.0
```

接続確認

```
http://<ホストIP>:3000/users/sign_in
```

「NoMethodError」や「Routing Error」が出ている場合は、Rails（Puma）を再起動させてからアクセスする
