# rails 6 でbootstrap4を利用する


## 事前準備

### 必要なライブラリのインストール

#### ubuntu

```bash
nano install_rbenv.sh
```

```bash
#!/bin/bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y build-essential 
sudo apt install -y clang
sudo apt install -y cmake
sudo apt install -y direnv
sudo apt install -y git
sudo apt install -y nodejs
sudo apt install -y ruby-dev
sudo apt install -y curl
sudo apt install -y imagemagick
curl -fsSL https://github.com/rbenv/rbenv-installer/raw/master/bin/rbenv-doctor | bash
curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list
sudo apt update

sudo apt install -y yarn
sudo apt install -y npm
sudo npm install n -g
sudo n stable
sudo apt purge -y nodejs npm
exec $SHELL -l
sudo apt install -y sqlite3 libsqlite3-dev
sudo apt install -y libssl-dev libreadline-dev zlib1g-dev

apt-get install -y libreadline-dev zlib1g-dev
rm -fr ~/.rbenv
git clone https://github.com/rbenv/rbenv.git ~/.rbenv
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bashrc
echo 'eval "$(rbenv init -)"' >> ~/.bash_profile
source ~/.bash_profile
source ~/.bashrc
exec $SHELL -l
```

```bash
nano install_ruby.sh
```

```bash
#!/bin/bash
rbenv install 2.5.1
rbenv global 2.5.1
```

```bash
nano install_rails.sh
```

----

#### MacOSX

```bash
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

##### 確認

```bash
brew doctor
```

* エラーや警告メッセージがでたら、その指示にしたがう
* 警告などが出なくなるまで brew doctor を繰り返す

```
Your system is ready to brew.
```

が出ればOK


##### インストールスクリプトの作成

```bash
emacs install_rbenv.sh
```

```bash
#!/bin/bash
brew update
brew upgrade

brew install direnv
brew install git
brew install nodejs
brew install ruby-dev
brew install curl
brew install imagemagick
brew install yarn
brew install npm
brew install sqlite3 
brew install readline
brew install zlib
sudo npm install n -g
sudo n stable

rm -fr ~/.rbenv

git clone https://github.com/rbenv/rbenv.git ~/.rbenv
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
curl -fsSL https://github.com/rbenv/rbenv-installer/raw/master/bin/rbenv-doctor | bash

echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bashrc
echo 'eval "$(rbenv init -)"' >> ~/.bash_profile
source ~/.bash_profile
source ~/.bashrc
exec $SHELL -l
```

```bash
nano install_ruby.sh
```

```bash
#!/bin/bash
rbenv install 2.5.1
rbenv global 2.5.1
```

```bash
nano install_rails.sh
```




### rubygems 

```bash
#!/bin/bash
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
chmod a+x install_rbenv.sh
chmod a+x install_ruby.sh
chmod a+x install_rails.sh

./install_rbenv.sh
./install_ruby.sh
./install_rails.sh
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

