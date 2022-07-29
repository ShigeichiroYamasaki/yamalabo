# IPFS

最終更新　2022/07/27 Shigeichiro Yamasaki

[IPFS入門を参照](https://ipfs-book.decentralized-web.jp/)

## 概要

「コンテント（内容）」指向のナビゲーションを行うファイルシステム
URLのような「場所」でナビゲーションを行うのではなく、内容そのもので検索します。

内容「コンテント」は、そのハッシュ値をIDにします。

IPFSのシステムは、ハッシュ値のIDから内容を検索してくれます。
実際のサーバなどの場所はどこかわかならくてもよいことになります。

ノードがコンテントを保管するインセンティブとして Filecoinという仮想通貨が使われます。

## インストール

ubuntu 20.04 LTS

[ビルド済パッケージをダウンロード](https://dist.ipfs.io/#go-ipfs)


```bash
wget https://dist.ipfs.io/kubo/v0.14.0/kubo_v0.14.0_linux-amd64.tar.gz  
tar xfvz kubo_v0.14.0_linux-amd64.tar.gz 
cd kubo
sudo ./install.sh
```

インストール確認

```bash
ipfs version
=>
ipfs version 0.14.0
```

IPFSネットワークへの接続

```bash
ipfs daemon &
```


## IPFSへのファイルの追加参照

### リポジトリの初期化

```bash
ipfs init
=>
generating ED25519 keypair...done
peer identity: 12D3KooWGps7xWmZ55vMKPugF4RtSNLRkjZaMwSaQCkHphFP6KjH
initializing IPFS node at /home/yamasaki/.ipfs
to get started, enter:

	ipfs cat /ipfs/QmQPeNsJPyVWPFDVHb77w8G42Fvo15z4bG2X8D2GhfbSXc/readme
```

.ipfs ディレクトリができていることを確認する

```bash
cd ~
ls -a
=>
.             .cache   .local    .sudo_as_admin_successful        ダウンロード  ビデオ
..            .config  .mozc     .wget-hsts                       テンプレート  ピクチャ
.bash_logout  .gnupg   .profile  kubo                             デスクトップ  ミュージック
.bashrc       .ipfs    .ssh      kubo_v0.14.0_linux-amd64.tar.gz  ドキュメント  公開
```

init した結果の出力メッセージの ipfs cat コマンドをコピーして実行する

```bash
ipfs cat /ipfs/QmQPeNsJPyVWPFDVHb77w8G42Fvo15z4bG2X8D2GhfbSXc/readme
=>
Hello and Welcome to IPFS!

██╗██████╗ ███████╗███████╗
██║██╔══██╗██╔════╝██╔════╝
██║██████╔╝█████╗  ███████╗
██║██╔═══╝ ██╔══╝  ╚════██║
██║██║     ██║     ███████║
╚═╝╚═╝     ╚═╝     ╚══════╝

If you're seeing this, you have successfully installed
IPFS and are now interfacing with the ipfs merkledag!

 -------------------------------------------------------
| Warning:                                              |
|   This is alpha software. Use at your own discretion! |
|   Much is missing or lacking polish. There are bugs.  |
|   Not yet secure. Read the security notes for more.   |
 -------------------------------------------------------

Check out some of the other files in this directory:

  ./about
  ./help
  ./quick-start     <-- usage examples
  ./readme          <-- this file
  ./security-notes

```

### リポジトリへのファイルの追加

"hello world"という内容のtest1.txtという名前のファイルを作成

```bash
echo "hello world" > test1.txt
```

ipfsにこのファイルの内容「コンテント」を追加する

`ipfs add <ファイル名>`

```bash 
ipfs add test1.txt
=>
added QmT78zSuBmuS4z925WZfrqQ1qHaJ56DQaTfyMUF7F8ff5o test1.txt
 12 B / 12 B [===============================================================================] 100.00%
```

QmT78zSuBmuS4z925WZfrqQ1qHaJ56DQaTfyMUF7F8ff5o　は "hello world" のハッシュ値で、IDになる

### IDからコンテントを表示する

`ipfs cat <ID>`

この例では、'hello world' が表示されれば成功

```bash
ipfs cat QmT78zSuBmuS4z925WZfrqQ1qHaJ56DQaTfyMUF7F8ff5o
=>
hello world
```

### ディレクトリの add

`-r` オプションを使う

`ipfs add -r <ディレクトリ>`

ipfs のインストールで利用した kubo ディレクトリを登録してみる

```bash
ls ~/kubo
=>
LICENSE  LICENSE-APACHE  LICENSE-MIT  README.md  install.sh
```

```bash
ipfs add -r ~/kubo
=>
added QmavzmguTQr18Q3TaayxHGrAyYk7Sjejc3WMqciZysiuPQ kubo/LICENSE
added QmdcxcVZw8Pojj1rhTCSp4uoNTV2fY8JwpBb3Bj5XNDMpd kubo/LICENSE-APACHE
added QmVBEq6nnXQR2Ueb6etMFMUVhGM5vu34Y2KfHW5FVdGFok kubo/LICENSE-MIT
added QmVh1g359Sb2YNmegSxSRp5paktWGVLQMwtgYBx1haqwjA kubo/README.md
added QmTNpZqmtr3t7AfNZuhAZWgXCfYhzWMSX1WCWRfypef5sH kubo/install.sh
added QmbUfTkfmeUjgdjPUs6ayKk2PJBY8sq9CuDXuib821ndZG kubo
 3.41 KiB / 3.41 KiB [=======================================================================] 100.00%
```

### ディレクトリを復元する

ファイル一覧を見る

`ipfs ls <ディレクトリのID>`

```bash
ipfs ls QmbUfTkfmeUjgdjPUs6ayKk2PJBY8sq9CuDXuib821ndZG
=>
QmavzmguTQr18Q3TaayxHGrAyYk7Sjejc3WMqciZysiuPQ 508  LICENSE
QmdcxcVZw8Pojj1rhTCSp4uoNTV2fY8JwpBb3Bj5XNDMpd 520  LICENSE-APACHE
QmVBEq6nnXQR2Ueb6etMFMUVhGM5vu34Y2KfHW5FVdGFok 1046 LICENSE-MIT
QmVh1g359Sb2YNmegSxSRp5paktWGVLQMwtgYBx1haqwjA 467  README.md
QmTNpZqmtr3t7AfNZuhAZWgXCfYhzWMSX1WCWRfypef5sH 948  install.sh
```

ディレクトリを cat することはできません

```bash
ipfs cat QmbUfTkfmeUjgdjPUs6ayKk2PJBY8sq9CuDXuib821ndZG
=>
Error: this dag node is a directory
```

### ipfsオブジェクト

ipfsにaddされたデータは、IPFSオブジェクトとして管理されている

```bash
ipfs object get QmbUfTkfmeUjgdjPUs6ayKk2PJBY8sq9CuDXuib821ndZG
=>
{"Links":[{"Name":"LICENSE","Hash":"QmavzmguTQr18Q3TaayxHGrAyYk7Sjejc3WMqciZysiuPQ","Size":519},{"Name":"LICENSE-APACHE","Hash":"QmdcxcVZw8Pojj1rhTCSp4uoNTV2fY8JwpBb3Bj5XNDMpd","Size":531},{"Name":"LICENSE-MIT","Hash":"QmVBEq6nnXQR2Ueb6etMFMUVhGM5vu34Y2KfHW5FVdGFok","Size":1057},{"Name":"README.md","Hash":"QmVh1g359Sb2YNmegSxSRp5paktWGVLQMwtgYBx1haqwjA","Size":478},{"Name":"install.sh","Hash":"QmTNpZqmtr3t7AfNZuhAZWgXCfYhzWMSX1WCWRfypef5sH","Size":959}],"Data":"\u0008\u0001"}
```

## IPFSネットワークへの接続

ipfs デーモンの起動　（TCPポート4001を利用）

```bash
ipfs daemon &
=>
Initializing daemon...
Kubo version: 0.14.0
Repo version: 12
System version: amd64/linux
Golang version: go1.18.3
2022/07/27 21:16:28 failed to sufficiently increase receive buffer size (was: 208 kiB, wanted: 2048 kiB, got: 416 kiB). See https://github.com/lucas-clemente/quic-go/wiki/UDP-Receive-Buffer-Size for details.
Swarm listening on /ip4/127.0.0.1/tcp/4001
Swarm listening on /ip4/127.0.0.1/udp/4001/quic
Swarm listening on /ip4/192.168.0.249/tcp/4001
Swarm listening on /ip4/192.168.0.249/udp/4001/quic
Swarm listening on /ip6/240f:ca:425:1:462b:d29b:66e7:ca55/tcp/4001
Swarm listening on /ip6/240f:ca:425:1:462b:d29b:66e7:ca55/udp/4001/quic
Swarm listening on /ip6/240f:ca:425:1:59a2:2811:51e4:dfd2/tcp/4001
Swarm listening on /ip6/240f:ca:425:1:59a2:2811:51e4:dfd2/udp/4001/quic
Swarm listening on /ip6/240f:ca:425:1::d/tcp/4001
Swarm listening on /ip6/240f:ca:425:1::d/udp/4001/quic
Swarm listening on /ip6/::1/tcp/4001
Swarm listening on /ip6/::1/udp/4001/quic
Swarm listening on /p2p-circuit
Swarm announcing /ip4/106.157.214.199/tcp/4001
Swarm announcing /ip4/127.0.0.1/tcp/4001
Swarm announcing /ip4/127.0.0.1/udp/4001/quic
Swarm announcing /ip4/192.168.0.249/tcp/4001
Swarm announcing /ip4/192.168.0.249/udp/4001/quic
Swarm announcing /ip6/240f:ca:425:1::d/tcp/4001
Swarm announcing /ip6/240f:ca:425:1::d/udp/4001/quic
Swarm announcing /ip6/::1/tcp/4001
Swarm announcing /ip6/::1/udp/4001/quic
API server listening on /ip4/127.0.0.1/tcp/5001
WebUI: http://127.0.0.1:5001/webui
Gateway (readonly) server listening on /ip4/127.0.0.1/tcp/8080
Daemon is ready
```

### ターミナルから接続状態を確認

すごく多数のピアを接続していることがわかる

```bash
ipfs swarm peers
=>
/ip6/2a01:4f8:192:33dd::2/udp/4001/quic/p2p/12D3KooWGNVQ5cEBowFLpDtYGMbfTEXxWEvcwfH51oG17Tpnperw
/ip6/2a01:4f9:4a:286c::2/udp/4001/quic/p2p/12D3KooWLd7SenSa4qLQ8CCSyH1VxMQxbSPyCVyT2jfpnu9W6DWe
/ip6/2a02:c206:3008:6052::1/udp/4001/quic/p2p/12D3KooWBDpgFpSFRDtL6wEJcBqBipPU9HwFXi71J1cr9K4HHa76
/ip6/2a02:c207:2026:61:8ea0::9/udp/4001/quic/p2p/QmcMb4SQVF6jJ85NJJqWwC2zZTHExnGLoB1uKC4ckLQqW6
/ip6/2a02:c207:2027:2353:a4fb::7/tcp/4001/p2p/QmXVQ2AbQHgHwNyheU4htT5zXGMFRoJXqFNiLY13PK9Je7
/ip6/2a02:c207:2027:9217:69c8::14/tcp/4001/p2p/QmZkanN8fRYDv5ePs253YyZfWLr29ukt1pEszfDS5pvksP
/ip6/2a02:c207:2029:2052:cbdd::7/tcp/4001/p2p/QmZHBBrcBtDk7yVzcNUDJBJsZnVGtPHzpTzu16J7Sk6hbp
/ip6/2a02:c207:2029:2052:e7ca::6/tcp/4001/p2p/QmZHBBrcBtDk7yVzcNUDJBJsZnVGtPHzpTzu16J7Sk6hbp
/ip6/2a04:a42:11:0:225:90ff:fea4:32b2/udp/4001/quic/p2p/12D3KooWMgn96XzAKN5gCppChMcqvReQ8NL8fGbTFs3i99F61dBY
/ip6/2a0d:f302:105:447c::1/udp/4001/quic/p2p/12D3KooWG7XVfNuxGtkRUE8pJ24gPaZkamgip38pcqv27gN34wnC

...

```

