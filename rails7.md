# rails7 のインストールと devise認証


## Rubyのインストール(ubuntu)

```bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y gcc
sudo apt install -y clang
sudo apt install -y make
sudo apt install -y libssl-dev zlib1g-dev
sudo apt install -y git
sudo apt install -y nodejs
sudo apt install -y cmdtest
sudo apt install -y sqlite3
sudo apt install -y libsqlite3-dev
sudo apt install -y build-essential
sudo apt install -y npm
sudo npm install --global yarn

rm -fr ~/.rbenv
git clone https://github.com/sstephenson/rbenv.git ~/.rbenv
echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bashrc
echo 'eval "$(rbenv init - bash)"' >> ~/.bashrc
source ~/.bashrc
git clone https://github.com/sstephenson/ruby-build.git ~/.rbenv/plugins/ruby-build

rbenv install 3.0.3
rbenv global 3.0.3
```

## Rubyのインストール(MacOSX)

```bash
brew update
brew upgrade -y
brew install gcc
brew install make
brew install libsigsegv
brew install zlib-ng
brew install git
brew install nodejs
brew install node
 brew install readline 

brew install doctest
brew install sqlite3
brew install sqlite-utils
npm install --global yarn


rm -fr ~/.rbenv
git clone https://github.com/sstephenson/rbenv.git ~/.rbenv
echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.zprofile
source ~/.zprofile
git clone https://github.com/sstephenson/ruby-build.git ~/.rbenv/plugins/ruby-build

rbenv install 3.0.3
rbenv global 3.0.3
```

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



```

### 実行

```bash
chmod u+x install_rails.sh

./install_rails.sh
```

### Railsのテスト

```bash
rails new test1

cd test1
bundle install
```

#### scaffold で生成

```bash
rails g scaffold User name
```

#### Gemfile編集

```bash
nano Gemfile
```


```ruby
gem 'bootstrap-sass', '~> 3.3.6'
gem 'sass-rails', '>= 3.2'
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


body　タグ部分に以下のように div タグを追加(<%= yield %>　をdiv で囲む)

```html
  ...

  <body>
    <div class="container">
    <%= yield %>
    </div>
  </body>
  
...
```

### サーバー再起動


```bash
rails s -b 0.0.0.0
```


###  ブラウザをリロードかけてレイアウトの変化を確認







