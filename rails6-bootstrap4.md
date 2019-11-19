# rails 6 でbootstrap4を利用する


## 事前準備

### 必要なライブラリのインストール

以下を全部コピペして実行すればOK

```bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y ssh
sudo apt install -y chkrootkit
sudo apt install -y build-essential 
sudo apt install -y clang
sudo apt install -y cmake
sudo apt install -y golang
sudo apt install -y curl
sudo apt install -y git
sudo apt install -y sqlite3 libsqlite3-dev
sudo apt install -y libssl-dev libreadline-dev zlib1g-dev
apt-get install -y autoconf bison build-essential libssl-dev libyaml-dev libreadline6-dev zlib1g-dev libncurses5-dev libffi-dev libgdbm5 libgdbm-dev
sudo apt install -y nodejs
sudo apt install -y npm
sudo npm install n -g
sudo n stable
sudo apt purge -y nodejs npm
exec $SHELL -l

git clone https://github.com/rbenv/rbenv.git ~/.rbenv
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build

echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bashrc
echo 'eval "$(rbenv init -)"' >> ~/.bash_profile

source ~/.bash_profile
source ~/.bashrc

curl -fsSL https://github.com/rbenv/rbenv-installer/raw/master/bin/rbenv-doctor | bash


curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list

sudo apt update
sudo apt install -y yarn


# 最新の安定版バージョンのRubyをインストールする

rbenv install 2.6.5
rbenv global 2.6.5

### gemのインストール

gem install rails
gem install sqlite3
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

#### Gemfile編集


以下を追加

```
gem 'bootstrap'
gem 'jquery-rails'

```

```bash
bundle install
```

