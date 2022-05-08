# WEBサーバの構築

## apache2 のインストール

```bash
sudo apt install apache2
```

* apache2 の起動

```bash
sudo service apache2 start
```

## 自分のページの作成

```bash
cd /var/www/html

sudo nano index.html
```

## HTML

```html
<meta charset="UTF-8">
<html>
<head>
</head>
<body>
<h1>”自分の名前”のページ</h1>

</body>
</html>

```

## ブラウザでURLでアクセスする

```
http://グローバルアドレス/index.html
```

## form 文を含むページ

自分のページを修正する

```bash
sudo nano index.html
```

```html
<meta charset="UTF-8">
<html>
 <head>
 </head>
 <body>
  <h1>あまぞん”自分の名前”</h1>
  <form method='GET' action='/cgi-bin/test.rb'>
    <p>商品</p>
    <input type='text' name='product'>
    <p>数量</p>
    <input type='text' name='amount'>
    </p><p>
    <input type='submit' value='注文'>
    </p>
  </form>
	</body>
</html>
```

## CGI プログラムを動かす

## ruby のインストール

```bash
sudo apt install ruby
```


### apache2 にCGIとして実行できるようにする

```bash
sudo a2enmod cgid 
```

### apache2を再起動

```bash
sudo service apache2 restart
```


#### apache2 の設定ファイルのディレクトリ

```bash
cd /usr/lib/cgi-bin
```

### スクリプトエイリアス用ディレクトリへ移動してディレクトリの作成所有権変更

```bash
cd /usr/lib/cgi-bin/
```

### CGIスクリプトの作成

```bash
sudo nano test.rb
```

1行目のは#! は「シェバング」と呼ばれるもので、スクリプトを実行する言語処理系を呼び出すためのもの

```ruby
#!/usr/bin/env ruby
require 'cgi'
cgi=CGI.new

puts "Content-type: text/html"

puts "\n\n"

puts "
<html>
<head>
<meta charset='utf-8'/>
</head>
<body>
<h1>お買い上げありがとうございます</h1>
<p>明日の８時に#{cgi['product']}を#{cgi['amount']}個お届けします</p>
</body>
</html>
"
```

### スクリプトに実行権限を与える

```bash
sudo chmod a+x test.rb
```

### スクリプトのテスト（デバッグ）

CGIではなくコマンドとして実行してみる

```bash
./test.rb
```

入力待ち状態になれば成功

試しに入力も入れてみる

入力の終了は、^d (コントロールキー＋ｄ)

```
product=ぽるしぇ&amount=100
```

その結果、以下のようなHTMLが出力されれば成功

```html
<html>
<head>
<meta charset='utf-8'/>
</head>
<body>
<h1>お買い上げありがとうございます</h1>
<p>明日の８時にぽるしぇを100個お届けします</p>
</body>
</html>
```

http経由で実行してみる

```bash
sudo apt install curl
```

```bash
curl http://localhost/cgi-bin/test.rb?product=ぽるしぇ&amount=100
```

### MacOSX のsafari の場合

「環境設定」→　「webサイト」→「ポップアップウィンドウ」でデフォルトを許可に

1. AWS Educate にサインイン

1. AWS Account

1. AWS Educate Starter Account

1.  AWS Console

    開かないときは，ブラウザのポップアップグロックの設定を解除

1. 仮想マシンの起動

    * Ubuntu Server 20.04 LTS (HVM), SSD Volume Type 64ビットx86 を選択

    * 確認と作成

    * 起動

    * メニューから「新しいキーペアの作成」を選択（すでに持っている人は「既存のキーペアを選択」でキーペア名を選択してもよいです）

        キーペア名は，自分の名前にする

1. キーファイルのダウンロード

        ファイルのダウンロード先は，自分のPCのホームディレクトリ

    * マックの場合は，「 ダウンロード」ディレクトリあたりにファイルがダウンロードされるので，ホームディレクトリに移動させる
    * 移動させたファイルに対して，ターミナルアプリから以下のようにしてパーミッションを変更
    * 
```
      chmod 400 ファイル名　
```
      
    * 
* windowsの場合は，power shell で  pwd コマンドで表示されたパスにファイルを置く
    * windows の場合もパーミションの変更が必要なことがある

 [Windows でパーミッションエラーが出たときの対処方法](https://qiita.com/eltociear/items/02e8b1f5590b49eb9d87)

1.  インスタンスが実行状態を確認（少し時間がかかります）

1.  インスタンスのチェックボックスをチェック

1. 接続ボタンをクリック

1.  SSH クライアントのタブをクリック

    ログインの例をコピーして

1.  ターミナルソフトを起動（ターミナル，power shell）

1. ssh コマンドをペーストして，AWSにログイン




## ubuntu 20.04LTS (64bit)のインストール

[https://docs.aws.amazon.com/ja_jp/AWSEC2/latest/UserGuide/ec2-instance-connect-set-up.html](https://docs.aws.amazon.com/ja_jp/AWSEC2/latest/UserGuide/ec2-instance-connect-set-up.html)

## OSのアップデート

```bash
sudo apt update
sudo apt upgrade -y
```

## apache2 のインストール

```bash
sudo apt install apache2
```


* apache2 の起動

```bash
sudo service apache2 start
```

### apache2 にCGIとして実行できるようにする

```bash
sudo a2enmod cgid 
```

### apache2を再起動

```bash
sudo service apache2 restart
```


## 自分のページの作成


```bash
cd /var/www/html

sudo nano index.html
```

## HTML

既存のデータを削除して以下を書き込む

```html
<meta charset="UTF-8">
<html>
<head>
</head>
<body>
<h1>”自分の名前”のページ</h1>

</body>
</html>

```

## セキュリティグループを修正

* 自分のインスタンスのセキュリティグループ名を確認する　（launch-wizard-２など）
* セキュリティグループのタブを選択
* 自分のセキュリティグループのチェックボックスをチェックする
* 「インバウンドルール」のタブを選択
* 「インバウンドルールを編集」
* 「ルールを追加」
* HTTPを追加　ソースは 0.0.0.0/0
*

## ブラウザでURLでアクセスする

自分のインスタンスの「パブリック IPv4 アドレス」を確認する

```
http://グローバルアドレス/index.html
```

## HTML作成


## form 文を含むページ

自分のページを修正する

```bash
sudo nano index.html
```

```html
<meta charset="UTF-8">
<html>
 <head>
 </head>
 <body>
  <h1>あまぞん”自分の名前”</h1>
  <form method='GET' action='/cgi-bin/test.rb'>
    <p>商品</p>
    <input type='text' name='product'>
    <p>数量</p>
    <input type='text' name='amount'>
    </p><p>
    <input type='submit' value='注文'>
    </p>
  </form>
	</body>
</html>
```

## Ruby インストール

[https://github.com/ShigeichiroYamasaki/yamalabo/blob/master/ruby/ruby.md](https://github.com/ShigeichiroYamasaki/yamalabo/blob/master/ruby/ruby.md)

## CGI プログラムを動かす

#### apache2 の設定ファイルのディレクトリ

```bash
cd /usr/lib/cgi-bin
```

### スクリプトエイリアス用ディレクトリへ移動してディレクトリの作成所有権変更

```bash
cd /usr/lib/cgi-bin/
```

### CGIスクリプトの作成

```bash
sudo nano test.rb
```

1行目のは#! は「シェバング」と呼ばれるもので、スクリプトを実行する言語処理系を呼び出すためのもの

```ruby
#!/usr/bin/env ruby
require 'cgi'
cgi=CGI.new

puts "Content-type: text/html"

puts "\n\n"

puts "
<html>
<head>
<meta charset='utf-8'/>
</head>
<body>
<h1>お買い上げありがとうございます</h1>
<p>明日の８時に#{cgi['product']}を#{cgi['amount']}個お届けします</p>
</body>
</html>
"
```

### スクリプトに実行権限を与える

```bash
sudo chmod a+x test.rb
```

### スクリプトのテスト（デバッグ）

CGIではなくコマンドとして実行してみる

```bash
./test.rb
```

入力待ち状態になれば成功

試しに入力も入れてみる

入力の終了は、^d (コントロールキー＋ｄ)

```
product=ぽるしぇ&amount=100
```

その結果、以下のようなHTMLが出力されれば成功

```html
<html>
<head>
<meta charset='utf-8'/>
</head>
<body>
<h1>お買い上げありがとうございます</h1>
<p>明日の８時にぽるしぇを100個お届けします</p>
</body>
</html>
```

http経由で実行してみる

```bash
sudo apt install curl
```

```bash
curl http://localhost/cgi-bin/test.rb?product=ぽるしぇ&amount=100
```

### http経由で実行してみる

curlコマンドのインストール

```bash
sudo apt install curl
```

curlコマンドで実行

```bash
curl http://＜IPアドレス＞/cgi-bin/test.rb?product=ぽるしぇ&amount=100
```

### ブラウザからやってみる

```
http://＜IPアドレス＞/index.html
```

## RESTful Webアプリケーション


## Railsインストール

```bash
cd ~

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
gem install devise-i18n
gem install devise-i18n-views
```

### 実行

```bash
chmod u+x install_rails.sh

./install_rails.sh

gem update
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
rails g scaffold Blog title body:text
```

#### DB のマイグレーション

```bash
rails db:migrate
```

#### サーバーの起動

```bash
rails s -b 0.0.0.0
```

#### ブラウザから確認

http://＜サーバーのIPアドレス＞:3000/blogs

確認後，control-c でサーバを停止


