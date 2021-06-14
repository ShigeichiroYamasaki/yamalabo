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

#!/bin/bash
brew update
brew upgrade

brew install gcc
brew install git
brew install leveldb
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
brew install rbenv
```

#### rbenvのインストールスクリプトのパーミッションの変更と実行

```bash
chmod u+x install_rbenv.sh
./install_rbenv.sh
```

#### rbenv を使ったRubyのインストール

```bash
rbenv install 3.0.1
rbenv global 3.0.1
```

シェルの再起動

```
exec $SHELL -l
```


## ubuntu


インストールスクリプトファイル

### rbenv のインストール

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
yarn install
yarn upgrade
sudo n stable
sudo apt purge -y nodejs npm
exec $SHELL -l
sudo apt install -y sqlite3 libsqlite3-dev
sudo apt install -y libssl-dev libreadline-dev zlib1g-dev

apt-get install -y libreadline-dev zlib1g-dev
rm -fr ~/.rbenv

git clone https://github.com/rbenv/rbenv.git ~/.rbenv
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
echo 'export PATH="$HOME/.rbenv/bin:$PATH"' >> ~/.zshrc
echo 'eval "$(rbenv init -)"' >> ~/.zshrc
source ~/.zshrc
```

#### rbenvのインストールスクリプトのパーミッションの変更と実行

```bash
chmod u+x install_rbenv.sh
./install_rbenv.sh
```

シェルの再起動

```
exec $SHELL -l
```


#### rbenv を使ったRubyのインストール

```bash
rbenv install 3.0.1
rbenv global 3.0.1
```




## バージョンの確認

```
ruby -v
```

