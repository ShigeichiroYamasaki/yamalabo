# Ruby インストール／アップデート方法

## インストール方法

### ubuntuインストールスクリプトファイル


```bash
nano ruby_install.sh
```

#### バージョン3.0.1. をインストールする場合

```bash
#!/bin/bash

VERSION='3.0.1'

sudo apt update
sudo apt upgrade -y
sudo apt install -y git
sudo apt install -y gcc
sudo apt-get install -y rbenv
cd ~
git clone https://github.com/rbenv/rbenv.git ~/.rbenv
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
sudo apt upgrade ruby-build -y
cd ~/.rbenv/plugins/ruby-build && git pull
cd ~
rbenv install $VERSION
rbenv global $VERSION

echo 'eval "$(rbenv init -)"'> ~/.bash_profile
```

#### インストールスクリプトの実行

```bash
chmod u+x ruby_install.sh

./ruby_install.sh
```

★ダウンロードしてコンパイル完了するまでかなり時間がかかります。

```bash
source ~/.bash_profile
```

### MacOSX

```
nano install_ruby
```

```bash
#!/bin/bash

VERSION='3.0.1'

brew install gcc
brew install git
brew install leveldb
brew install rbenv
cd ~
git clone https://github.com/rbenv/rbenv.git ~/.rbenv
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
brew upgrade rbenv
cd ~/.rbenv/plugins/ruby-build && git pull
cd ~
rbenv install $VERSION
rbenv global $VERSION
```

#### インストールスクリプトの実行


```bash
chmod u+x ruby_install.sh

./ruby_install.sh
```

★ダウンロードしてコンパイル完了するまでかなり時間がかかります。

```bash
source ~/.bash_profile
```


## バージョンの確認

```
ruby -v
```

