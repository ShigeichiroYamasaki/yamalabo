# rails 6 でbootstrap4を利用する


## Rubyのインストール

[ruby](./ruby.md)



### Railsインストールスクリプトの作成

```bash
nano install_rails.sh
```

```bash
#!/bin/bash

yarn install
yarn upgrade

gem install bundler
gem install sqlite3
gem install json-jwt
gem install jwt
gem install rails
gem install twitter
gem install devise
gem install omniauth
gem install omniauth-twitter
gem install omniauth-facebook
```

### 実行

```bash
chmod u+x install_rails.sh

./install_rails.sh
```

シェルの再起動

```bash
exec $SHELL -l
```


### Railsのテスト

```bash
rails new test1

cd test1
```

#### scaffold で生成

```bash
rails g scaffold User name
```

#### webpackerのインストール

```bash
rails webpacker:install
```

#### Gemfile編集

```bash
nano Gemfile
```

```
ruby '3.0.1'
...

```

の下に以下を追加


```
gem 'bootstrap'
gem 'jquery-rails'

```

```bash
bundle install
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

http://localhost:3000/users

確認後，control-c でサーバを停止


### CSSを編集

```bash
nano app/assets/stylesheets/application.css
```

最後に以下を追加

```css
@import "bootstrap"; 
```

### css のファイル名を.css から .scssに変更

```bash
mv app/assets/stylesheets/application.css app/assets/stylesheets/application.scss
```

### application.js を修正

```bash
nano app/javascript/packs/application.js
```

以下を追加する

```javascript
//=require bootstrap
```

### html を修正

```bash
nano app/views/users/index.html.erb
```

試しにファイルの一番上に以下を記述してみる

```html
<a class="btn btn-primary" href="#" role="button">Link</a>
<button class="btn btn-primary" type="submit">Button</button>
<input class="btn btn-primary" type="button" value="Input">
<input class="btn btn-primary" type="submit" value="Submit">
<input class="btn btn-primary" type="reset" value="Reset">
```


## Rails bootstrap の使い方

### レイアウトの修正

```bash
nano app/views/layouts/application.html.erb
```

head タグの中に stylesheet_link_tag と javascript_pack_tag を追加
body　タグ部分に以下のように div タグを追加(<%= yield %>　をdiv で囲む)

```html
<!DOCTYPE html>
<html>
  <head>
    <title>App</title>
    <%= csrf_meta_tags %>
    <%= csp_meta_tag %>
    <%= stylesheet_link_tag 'application', media: 'all', 'data-turbolinks-track': 'reload' %>
    <%= javascript_pack_tag 'application', 'data-turbolinks-track': 'reload' %>
  </head>
  
...

  <body>
    <div class="container">
    <%= yield %>
    </div>
  </body>
  
...
```

###  ブラウザをリロードかけてレイアウトの変化を確認







