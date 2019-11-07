# rails 6 でbootstrap4を利用する


## 事前準備

### 必要なライブラリのインストール

```bash
sudo apt install git
sudo apt install autoconf bison build-essential libssl-dev libyaml-dev libreadline6-dev zlib1g-dev libncurses5-dev libffi-dev libgdbm5 libgdbm-dev
sudo apt install nodejs

git clone https://github.com/rbenv/rbenv.git ~/.rbenv
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bash_profile
~/.rbenv/bin/rbenv init  
eval "$(rbenv init -)"
echo '~/.rbenv/bin/rbenv init' >> ~/.bash_profile
source ~/.bash_profile
source ~/.bashrc

```

### 最新の安定版バージョンのRubyをインストールする

```bash
rbenv install 2.6.5
rbenv global 2.6.5
```

### gemのインストール

```bash
gem install rails
gem install sqlite3
```

### yarn のインストール

Rials 6 は、node.jsのパッケージマネージャとしてnpmではなくyarnを使うようになったので、最初にyarn をインストールする必要がある


MacOSX
```bash
brew install yarn
```

ubuntu

```bash
curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list

sudo apt update && sudo apt install -y yarn
```

### Webpacker

Rails6でwebpackerが標準になった

Webpackerとは、モダンなフロントエンド開発を強力にサポートするWebpackをRuby on Railsで使うためのgemパッケージ

Railsのプロジェクトを作成後、webpackerのインストールが必須

## 典型的なRails6 + Bootstrap4 アプリ




```bash
rails new myapp --webpack=react
```

webワレット作成

```bash
rails new declining-webwallet --webpack=react
```

```bash
cd myapp
bundle install
```


### 簡単なblogをscaffold で作ってみる


```bash
 rails g scaffold blog title body:text
 
 rails db:migrate

```

### パッケージのインストール

```bash
 yarn add bootstrap bootstrap-material-design jquery popper.js
```
 
### pack内のapplication.jsの記述
 
```bash
 nano app/javascript/packs/application.js
```
 
 以下を最後に追加
 
``` app/javascripts/pack/application.js
 
import 'bootstrap-material-design'
import '../stylesheets/application'
```

### application.scss作成

```bash
mkdir app/javascript/stylesheets
touch app/javascript/stylesheets/application.scss
```

```bash
nano app/javascript/stylesheets/application.scss
```

ファイルの内容

```
@import '~bootstrap-material-design/scss/bootstrap-material-design';
```

### レイアウトのapplication.erb.htmlの修正

```bash
nano app/views/layouts/application.html.erb
```

* 修正箇所（stylesheet_link_tag　→　stylesheet_pack_tag）
*  <%= yield %> →
    \<div class="container">
    <%= yield %>
    \</div>


```html
<!DOCTYPE html>
<html>
  <head>
    <title>Myapp</title>
    <%= csrf_meta_tags %>
    <%= csp_meta_tag %>
    <%= stylesheet_pack_tag 'application', media: 'all', 'data-turbolinks-track': 'reload' %>
    <%= javascript_pack_tag 'application', 'data-turbolinks-track': 'reload' %>
  </head>

  <body>
　   <div class="container">
    <%= yield %>
    </div>
  </body>
</html>

```

### config/webpack/environment.js を編集する

```bash
nano config/webpack/environment.js
```

```javascript
const { environment } = require('@rails/webpacker')
const webpack = require('webpack')
environment.plugins.append('Provide', new webpack.ProvidePlugin({
    $: 'jquery',
    jQuery: 'jquery',
    Popper: ['popper.js', 'default']
 }))
module.exports = environment
```

### webpackerのインストール

```bash
rails  webpacker:install
```


### サーバーの起動


```bash
rails s -b 0.0.0.0
```

### ブラウザで確認

```url
http://IPアドレス:3000/blogs
```

blogのデータを数件入れておく

## Bootstrap4の利用


 app/views/blogs/index.html.erb の表を修正

```bash
nano app/views/blogs/index.html.erb 
```


修正前

```html
<table >
```

修正後

```html
<table class="table">
```

一覧を確認する


