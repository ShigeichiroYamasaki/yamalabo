# 音楽ストリーミングサーバ Navidrome

2025/04/07
Shigeichiro Yamasaki

## Navidrome

音楽ストリーミングサーバの構築方法

[Navidrome](https://www.navidrome.org/)

* クライアントとしては，iPhone や andoroid のアプリや web ブラウザが利用できます

[substreamer](https://apps.apple.com/jp/app/substreamer/id1012991665)

### install (MacOSX)

homebrew がインストール済とします

```bash
brew install navidrome
```

* これだけでインストール完了です
* 以下のコマンドで navidrome サーバーを起動できます

```bash
$ navidrome
```

### install (ubuntu)
```bash
sudo apt update
sudo apt upgrade
sudo apt install vim ffmpeg
```

* <ユーザ名> の部分は自分のユーザ名に書き換えてください
  
* ディレクトリの作成

```bash
sudo install -d -o <ユーザ名> /opt/navidrome
sudo install -d -o <ユーザ名> /var/lib/navidrome
```

```bash
wget https://github.com/navidrome/navidrome/releases/download/v0.53.3/navidrome_0.53.3_linux_amd64.tar.gz -O Navidrome.tar.gz
sudo tar -xvzf Navidrome.tar.gz -C /opt/navidrome/
sudo chown -R <ユーザ名> /opt/navidrome
```

###  音源ファイルのフォルダー

`/home/<ユーザ名>/music` とします

```bash
mkdir /home/<ユーザ名>/music
```

### 設定ファイル

```bash
nano /var/lib/navidrome/navidrome.toml
```

```
MusicFolder = "/home/<ユーザ名>/music"
```

```bash
sudo nano /etc/systemd/system/navidrome.service
```
以下のテキストの <ユーザ名> の部分を自分のユーザ名に書き換える

```
[Unit]
Description=Navidrome Music Server and Streamer compatible with Subsonic/Airsonic
After=remote-fs.target network.target
AssertPathExists=/var/lib/navidrome

[Install]
WantedBy=multi-user.target

[Service]
User= <ユーザ名> 
Group= <ユーザ名> 
Type=simple
ExecStart=/opt/navidrome/navidrome --configfile "/var/lib/navidrome/navidrome.toml"
WorkingDirectory=/var/lib/navidrome
TimeoutStopSec=20
KillMode=process
Restart=on-failure

# See https://www.freedesktop.org/software/systemd/man/systemd.exec.html
DevicePolicy=closed
NoNewPrivileges=yes
PrivateTmp=yes
PrivateUsers=yes
ProtectControlGroups=yes
ProtectKernelModules=yes
ProtectKernelTunables=yes
RestrictAddressFamilies=AF_UNIX AF_INET AF_INET6
RestrictNamespaces=yes
RestrictRealtime=yes
SystemCallFilter=~@clock @debug @module @mount @obsolete @reboot @setuid @swap
ReadWritePaths=/var/lib/navidrome

# You can uncomment the following line if you're not using the jukebox This
# will prevent navidrome from accessing any real (physical) devices
#PrivateDevices=yes

# You can change the following line to `strict` instead of `full` if you don't
# want navidrome to be able to write anything on your filesystem outside of
# /var/lib/navidrome.
ProtectSystem=full

# You can uncomment the following line if you don't have any media in /home/*.
# This will prevent navidrome from ever reading/writing anything there.
#ProtectHome=true

# You can customize some Navidrome config options by setting environment variables here. Ex:
#Environment=ND_BASEURL="/navidrome"

```

## サーバーの起動

```bash
sudo systemctl daemon-reload
sudo systemctl start navidrome.service
sudo systemctl status navidrome.service
```

## 最初の管理ユーザ登録


### webブラウザからマシンにアクセスする

[ユーザ登録方法](https://www.navidrome.org/docs/getting-started/)

```uri
http://<サーバーのIPアドレス>:4533/
```

* 管理者ユーザのユーザ名とパスワードを設定する

## 音源の登録

CD などからパソコンに取り込んだ音源ファイルを音源ディレクトリーにコピーする

### CDから取り込んだ音源ファイルの一般的なフォルダー構成

* アーティスト名
  * アルバム名
    * 01 *.mp3

この構成のフォルダーを Navidrome の 音源ディレクトリにコピーする

## substreamer クライアントからの接続

* サーバー名 http://<IPアドレス>:4533/
* ユーザー名／パスワード （）
