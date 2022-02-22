echo 'export PATH="HOME/.rbenv/bin:PATH"' >> ~/.zshrc
source ~/.zshrc
git clone [GitHub - rbenv/ruby-build: Compile and install Ruby](https://github.com/sstephenson/ruby-build.git) ~/.rbenv/plugins/ruby-build

rbenv install 3.0.3
rbenv global 3.0.3

## rm -fr ~/.rbenv

# Ruby インストール方法

2022/02/18

## MacOSXの事前準備

### homebrewのインストールと環境整備

```bash
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

### 確認

```bash
brew doctor
```

* エラーや警告メッセージがでたら、その指示にしたがう
* 警告などが出なくなるまで brew doctor を繰り返す

```
Your system is ready to brew.
```

が出ればOK

* もしエラーや警告が収まらないなら，homebrew をアンインストールして，インストールからやりなおす

```bash
# homebrew のアンインストール

ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/uninstall)"
```

## Ubuntuの事前準備

```bash
#!/binbash
sudo apt update
sudo apt upgrade -y
sudo apt install -y gcc make git clang m4 nodejs 
sudo apt install -y libssl-dev zlib1g-dev
sudo apt install -y cmdtest
sudo apt install -y sqlite3
sudo apt install -y libsqlite3-dev
sudo apt install -y build-essential
sudo apt install -y npm
sudo npm install --global yarn

```



### Rubyのインストール

```bash
rm -fr ~/.rbenv
git clone https://github.com/rbenv/rbenv.git ~/.rbenv
echo 'export PATH="~/.rbenv/bin:$PATH"' >> ~/.bashrc
~/.rbenv/bin/rbenv init >> ~/.bashrc
source ~/.bashrc
git clone https://github.com/rbenv/ruby-build.git "$(rbenv root)"/plugins/ruby-build

rbenv install 3.1.0
rbenv global 3.1.0
```

## Rubyのインストール(MacOSX)

```bash
nano install_ruby.sh
```

```bash
#!/bin/zsh

brew update
brew upgrade

brew install llvm
brew install make
brew install curl
brew install direnv
brew install libsigsegv
brew install zlib
brew install zlib-ng
brew install git
brew install imagemagick
brew install nodejs
brew install node
brew install npm
brew install readline 
brew install doctest
brew install sqlite3
brew install sqlite-utils
brew install leveldb
brew install yarn
npm install --global yarn

```

#### rbenvのインストールスクリプトのパーミッションの変更と実行

```bash
chmod u+x install_ruby.sh
./install_ruby.sh
```

## rm -fr ~/.rbenv

## ubuntu

#### rbenv を使ったRubyのインストール

rbenvと関連ライブラリのインストールスクリプトの作成

```bash
nano install_ruby.sh
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
sudo apt install -y curl
sudo apt install -y imagemagick
sudo apt install -y rbenv
sudo apt install -y yarn
sudo apt install -y zlib
sudo apt install -y readline
sudo apt install -y npm
sudo npm install n -g
sudo apt install -y sqlite3 libsqlite3-dev
sudo apt install -y libssl-dev libreadline-dev zlib1g-dev

# rm -fr ~/.rbenv
git clone https://github.com/rbenv/rbenv.git ~/.rbenv
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bashrc
echo 'eval "$(rbenv init -)"' >> ~/.bashrc
git -C ~/.rbenv/plugins/ruby-build pull

rbenv install 3.0.3
rbenv global 3.0.3
```

```bash
chmod u+x install_ruby.sh
./install_ruby.sh
```
