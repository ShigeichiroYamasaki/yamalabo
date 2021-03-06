# Bitcoin core Signet ノードの構築

## install 方法

### MacOSX

[Bitcoin core ダウンロードサイト](https://bitcoin.org/ja/download)から dmg 形式のインストーラをダウンロードする


#### インストーラで bitcoin core をインストールしてアプリを起動

* ドラックアンドドロップでアプリフォルダーにインストール
* 「システム環境設定」の「セキュリティとプライバシー」で，ロックのアイコンをクリックしてパスワード入力
*  bitcoin coreに対して「ダウンロードしたアプリケーションの実行許可」を与える

#### アプリ起動

「アプリケーションフォルダ」の bitcoin core アプリ起動

### ubuntu 20.04LTS

#### snap でインストールする

```bash
sudo snap install bitcoin-core
```

## bitcoin-qt アプリを起動

```
bitcoin-core.qt
```

#### snap で bitcoin core をインストールしたときのコマンドインターフェース

* bitcoin-core.daemon : デーモン起動 (bitcoind)
* bitcoin-core.cli : クライアント (bitcoin-cli)


## bitcoind 設定ファイルを編集（作成）

設定ファイル (bitcoin.conf) を編集

* ubuntu で通常インストールしたときの bitcoin.conf のデフォルトの場所
    * ~/.bitcoin/bitcoin.conf
* ubuntu でsnap でインストールした場bitcoin.conf のデフォルトの場所
    * ~/snap/bitcoin-core/common/.bitcoin/bitcoin.conf
* MacOSX でパッケージインストールしたときのbitcoin.conf のデフォルトの場所
    * ~/Library/Application Support/Bitcoin/bitcoin.conf


### bitcoin-qt (GUI) から設定ファイル (bitcoin.conf) を編集

* MacOSX :「ファイル」メニューの 「preferences」.. 
* ubuntu: 「設定」メニューの「オプション」
* 「設定ファイルを開く」ボタンをクリック
* 設定ファイルを以下のように作成して保存

```
signet=1
txindex=1
daemon=1
server=1
rest=1
[signet]
rpcuser=hoge
rpcpassword=hoge
rpcport=38332
port=38333
fallbackfee=0.0002
```

* 「OK」をクリック

## bitcoin core を一旦終了して再度起動

アイコンが薄い黄緑になっていれば成功

* bitcoin-qt で自分のワレットの作成しておく

## テスト用コインの入手

[https://signet.bc-2.jp/](https://signet.bc-2.jp/)


