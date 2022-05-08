# Keycloak

## Javaのインストール

#### Oracle JDKのダウンロード

[https://www.oracle.com/java/technologies/downloads/](https://www.oracle.com/java/technologies/downloads/)

自分のOS（MacOSX/ Linuxなど）とアーキテクチャ（x64/ Arm 64など）を指定してダウンロードする

* MacOSX の場合は  .dmg のインストーラでインストールする
* ubuntuの場合 .deb のdebian package ファイルをダウンロード

```bash
sudo apt install ./jdk-17_linux-x64_bin.deb
```

#### Java IDEのインストール（InteliJ IDEA)

JetBrains Toolbox Appにアクセス

[https://www.jetbrains.com/toolbox-app/](https://www.jetbrains.com/toolbox-app/)



OS,アーキテクチャを選択、教育用の無償バージョン(IntelliJ IDEA Community Edition)を選択）

* MacOSX の場合　ダウンロードしたインストーラをドラッグアンドドロップでインストール

## keycloakのインストール

[https://www.keycloak.org/downloads](https://www.keycloak.org/downloads)

* Keycloak WildFly のZIPをダウンロードする
* zipファイルを解凍してホームディレクトリに置き、ディレクトリに移動

## keycloakの起動

```bash
cd bin
./standalone.sh -b 0.0.0.0
```

以下のようなメッセージが出れば成功

```
=========================================================================

  JBoss Bootstrap Environment

  JBOSS_HOME: /Users/shigeichiroyamasaki/keycloak-17.0-2.0

  JAVA: java

  JAVA_OPTS:  -server -Xms64m -Xmx512m -XX:MetaspaceSize=96M -XX:MaxMetaspaceSize=256m -Djava.net.preferIPv4Stack=true -Djboss.modules.system.pkgs=org.jboss.byteman -Djava.awt.headless=true  --add-exports=java.desktop/sun.awt=ALL-UNNAMED --add-exports=java.naming/com.sun.jndi.ldap=ALL-UNNAMED --add-opens=java.base/java.lang=ALL-UNNAMED --add-opens=java.base/java.lang.invoke=ALL-UNNAMED --add-opens=java.base/java.lang.reflect=ALL-UNNAMED --add-opens=java.base/java.io=ALL-UNNAMED --add-opens=java.base/java.security=ALL-UNNAMED --add-opens=java.base/java.util=ALL-UNNAMED --add-opens=java.base/java.util.concurrent=ALL-UNNAMED --add-opens=java.management/javax.management=ALL-UNNAMED --add-opens=java.naming/javax.naming=ALL-UNNAMED

=========================================================================

...

 (576 services are lazy, passive or on-demand)
23:39:43,913 INFO  [org.jboss.as] (Controller Boot Thread) WFLYSRV0060: Http management interface listening on http://127.0.0.1:9990/management
23:39:43,913 INFO  [org.jboss.as] (Controller Boot Thread) WFLYSRV0051: Admin console listening on http://127.0.0.1:9990

```

#### 起動したマシンでブラウザを起動して keycoakサーバに接続

` http://localhost:8080/auth/` にアクセス

サーバを起動しているマシン以外からなら、localhostではなくサーバのIPアドレスを指定する

#### adminの登録

Administration Console　にユーザ名とパスワードを入れて登録する


#### 管理コンソール

Administration Console > をクリック

登録したユーザ名／パスワードでログインする

#### レルムの追加

左の上　Master をマウスダウンすると「Add realm」ボタンが現れる

name を設定する（kindaiなど）

create する

#### ユーザの追加

* Manage のusers をクリック
* add userをクリック
* Username を設定する
* Localeをjaにする
* save


#### アカウント管理コンソールの利用

レルム名がkindaiのとき

`http://localhost:8080/auth/realms/kindai/account/` 

* sign Inボタンをクリック
* usernameを入れる
* パスワードを設定する

#### セッションの確認

ブラウザのウィンドウをもう一つ開き、管理者画面にアクセス

`http://localhost:8080/auth/` 
Administration Console　に移動

* レルム内で
* Manage の Sessions をクリックしてSessions 画面を開く
* Account-consoleを見る
* keycloakで管理されているユーザのセションを確認する

#### レルムの日本語化

* 上の「Themes」 タブを選択
* Internationalization Enabled を onにする
* Default locale を jaにしてsave


## ユーザー定義

Keycloakにおけるユーザの識別ID

#### 管理コンソールの左の Manageの「Users」画面

View all users ボタンで検索する

#### IDの確認

`a9b6ba82-2f0e-470e-8e80-cd4d11d24b85` のようなIDがついていることを確認する

#### Attributes （属性）

ABAC (属性ベースアクセス制御）などに利用される情報

キー：値　で定義されます

#### 管理コンソールの左の「Realm settings」の「Login」でログインポリシーを設定する

* User registration :ログイン画面にユーザ登録リンクがつく
* Edit username：自分でユーザ名を編集できる
* Forgot password ：パスワードリセットができるようになる
* Remember Me：ログイン状態を保存できる
* Verify email：初回ログイン時にメールの有効確認を行う
* Login with email：メールアドレスでログインできる
* Require SSL ：SSL必須

## アカウント管理コンソール

`http(s)://＜ホスト：ポート番号＞/auth/realms/＜レルム＞/account`

`http://localhost:8080/auth/realms/kindai/account` →　「個人情報」


## クライアント（RP) （アプリ）

####  管理コンソールの左の　Clients をクリック

「Create」ボタンをクリックする


* Client-ID: 例　kindai-RP
* Client Protocol :Client Protocol 
* Root URL:

「save」ボタン





