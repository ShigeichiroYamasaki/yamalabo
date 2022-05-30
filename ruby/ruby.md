# Ruby インストール方法

2022/05/20 Shigeichiro Yamasaki

推奨バージョン変更
Ruby 2.7.5 -> 2.7.5

2.7.6 ではbitcoinrbでエラーが起きることがあります

## MacOSXでのインストール

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

このメッセージが出ればOK

```
Your system is ready to brew.
```

* もしエラーや警告が収まらないなら，homebrew を一旦アンインストールしてからhomebrewのインストールからやりなおすと、余計なトラブルを経験せずに済みます。

```bash
# homebrew のアンインストール方法
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/uninstall)"
```

#### 必要（そう）なライブラリのインストール

```bash
brew update
brew upgrade

brew install llvm autoconf gcc make curl direnv readline sqlite3 sqlite-utils libsigsegv leveldb zlib zlib-ng git nodejs node npm m4 wget curl
```

#### rbenvのインストール

```bash
cd ~
rm -fr ~/.rbenv
git clone https://github.com/rbenv/rbenv.git ~/.rbenv
```

#### ログインスクリプト (.zshrc)の編集

`nano .zshrc`

ファイルの最後に次の2行を加えて保存する

```bash
export PATH="~/.rbenv/bin:$PATH"
eval "$(rbenv init -)"
```

#### ログインスクリプトの評価

```bash
source ~/.zshrc
```

#### rbenvコマンドの確認

インストール可能なRubyのバージョンが出てきます

```bash
git clone https://github.com/rbenv/ruby-build.git "$(rbenv root)"/plugins/ruby-build
rbenv install -l


2.6.10
2.7.5
3.0.4
3.1.2
jruby-9.3.4.0
mruby-3.0.0
rbx-5.0
truffleruby-22.0.0.2
truffleruby+graalvm-22.0.0.2

Only latest stable releases for each Ruby implementation are shown.
Use 'rbenv install --list-all / -L' to show all local versions.
```

新しいバージョンのRubyをインストールしたい場合は以下のようにしてrbenvの環境を更新します

```bash
rm -fr ~/.rbenv/plugins/ruby-build
git clone https://github.com/rbenv/ruby-build.git "$(rbenv root)"/plugins/ruby-build
```

### Rubyのインストール

#### バージョンを指定してインストール

最新の3.X.X バージョンは高速ですが、安定していないことがあります。

2.7.5 を推奨することにします。

コンパイルなどを行うので、かなり時間がかかります

```bash
rbenv install 2.7.5
```

#### 利用するRubyのバージョンの指定

rbenvを利用すると複数のバージョンのRubyをインストールできます。
その中で利用するバージョンを以下のようにして指定します

```bash
rbenv global 2.7.5
source .zshrc 
```

Rubyのバージョンの確認

```bash
ruby -v
ruby 2.7.5p203 (2021-11-24 revision f69aeb8314) [x86_64-linux]
```


## Ubuntuでのインストール

#### 必要（そう）なライブラリのインストール

```bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y llvm autoconf gcc clang cmake curl direnv sqlite3 libsqlite3-dev git nodejs m4 libssl-dev zlib1g-dev cmdtest build-essential wget imagemagick libreadline-dev
sudo apt autoremove -y
```

#### rbenvのインストール

```bash
cd ~
rm -fr ~/.rbenv
git clone https://github.com/rbenv/rbenv.git ~/.rbenv
```

#### ログインスクリプト (.bashrc)の編集

`nano ~/.bashrc`

ファイルの最後に次の2行を加えて保存する

```bash
export PATH="~/.rbenv/bin:$PATH"
eval "$(rbenv init -)"
```

#### ログインスクリプトの評価

```bash
source ~/.bashrc
```

#### rbenvコマンドの確認

インストール可能なRubyのバージョンが出てきます

```bash
git clone https://github.com/rbenv/ruby-build.git "$(rbenv root)"/plugins/ruby-build
rbenv install -l


2.6.10
2.7.5
3.0.4
3.1.2
jruby-9.3.4.0
mruby-3.0.0
rbx-5.0
truffleruby-22.1.0
truffleruby+graalvm-22.1.0

Only latest stable releases for each Ruby implementation are shown.
Use 'rbenv install --list-all / -L' to show all local versions.
```

新しいバージョンのRubyをインストールしたい場合は以下のようにしてrbenvの環境を更新します

```bash
rm -fr ~/.rbenv/plugins/ruby-build
git clone https://github.com/rbenv/ruby-build.git "$(rbenv root)"/plugins/ruby-build
```

### Rubyのインストール

#### バージョンを指定してインストール

最新の3.X.X バージョンは高速ですが、安定していないことがあります。
ここでは、2.7.5 を利用することにします。

コンパイルなどを行うので、かなり時間がかかります

```bash
rbenv install 2.7.5
```

#### 利用するRubyのバージョンの指定

rbenvを利用すると複数のバージョンのRubyをインストールできます。
その中で利用するバージョンを以下のようにして指定します

```bash
rbenv global 2.7.5
source .bashrc 
```

Rubyのバージョンの確認

```bash
ruby -v
ruby 2.7.5p219 (2022-04-12 revision c9c2245c0a) [x86_64-linux]
```
