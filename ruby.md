# Ruby インストール／アップデート方法

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

### MacOSXでのRubyのインストール

rbenvと関連ライブラリのインストールスクリプトの作成

```bash
nano install_rbenv.sh
```

```bash
#!/bin/bash
brew update
brew upgrade

brew install clang
brew install git
brew install leveldb
brew install direnv
brew install git
brew install nodejs
brew install curl
brew install imagemagick
brew install yarn
brew install npm
brew install sqlite3 
brew install readline
brew install zlib
brew install rbenv

# rm -fr ~/.rbenv
git clone https://github.com/rbenv/rbenv.git ~/.rbenv
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.zshrc
echo 'eval "$(rbenv init -)"' >> ~/.zshrc
git -C ~/.rbenv/plugins/ruby-build pull

```

#### rbenvのインストールスクリプトのパーミッションの変更と実行

```bash
chmod u+x install_rbenv.sh
./install_rbenv.sh
```


シェルの再起動

```
source ~/.zshrc
source ~/.zprofile
exec $SHELL -l
```


```bash
rbenv install 3.0.2
rbenv global 3.0.2
```


## ubuntu

#### rbenv を使ったRubyのインストール
rbenvと関連ライブラリのインストールスクリプトの作成

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
echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bash_profile
echo 'eval "$(~/.rbenv/bin/rbenv init -)"' >> ~/.bash_profile
echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.bashrc
echo 'eval "$(rbenv init -)"' >> ~/.bashrc
git -C ~/.rbenv/plugins/ruby-build pull
```

```bash
chmod u+x install_rbenv.sh
./install_rbenv.sh

source ~/.bashrc
```
シェルの再起動

```
exec $SHELL -l
```


```bash
rbenv install 3.0.2
rbenv global 3.0.2
```



