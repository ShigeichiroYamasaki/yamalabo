# ubuntuの基本操作

2024/10/14 Shigeichiro Yamasaki

## ファイルシステムの操作

### 現在のディレクトリのパスの確認 (pwdコマンド)

```bash
$ pwd
```

### ディレクトリへの移動 (cdコマンド)


```bash
$ cd ~
$ pwd

$ cd /
$ pwd

$ cd /usr/local
$ pwd

$ cd ..
$ pwd

$ cd ..
$ pwd
```

### 現在のディレクトリのファイルの一覧　(lsコマンド)

```bash
$ ls
```

####  ls コマンドに -l オプションをつけると詳細情報が表示される

```bash
$ ls -l

=>
total 36
drwxr-xr-x 2 yamasaki yamasaki 4096 Jul 19  2018 ビデオ
drwxr-xr-x 2 yamasaki yamasaki 4096 Jul 19  2018 ピクチャ
drwxr-xr-x 2 yamasaki yamasaki 4096 Jul 19  2018 ダウンロード
drwxr-xr-x 2 yamasaki yamasaki 4096 Jul 19  2018 ミュージック
drwxr-xr-x 2 yamasaki yamasaki 4096 Jul 19  2018 テンプレート
drwxr-xr-x 2 yamasaki yamasaki 4096 Jul 19  2018 ドキュメント
drwxr-xr-x 3 yamasaki yamasaki 4096 Feb 17 10:47 デスクトップ
drwxr-xr-x 7 yamasaki yamasaki 4096 Apr 17 08:30 公開

```

### 新しいディレクトリの作成　（mkdirコマンド）

```bash
$ mkdir kadai
$ ls

=>
ビデオ	  ダウンロード	テンプレート  デスクトップ  kadai
ピクチャ  ミュージック	ドキュメント  公開

```

### 新しいファイルの作成 (touchコマンド)

```bash
$ touch test
$ ls

=>
test

```

### ファイルのコピー (cpコマンド)

```bash
$ cp test test1
$ ls

=>
test  test1

```

### ファイルの移動 (mvコマンド)

```bash
$ mkdir work
$ ls

=>
test  test1  work

$ mv test work
$ ls

=>
test1  work

$ cd work/
$ ls

=>
test

```

### ファイル名の変更

```bash
$ mv test test100
$ ls

=>
test100

```

### ファイルの削除 (rmコマンド)

```bash
$ ls

=>
test100

$ rm test100 
$ ls

```

### ディスク（SSD)の空きの確認　（dfコマンド）

-m オプションは　メガバイト単位という意味

```bash
$ df -m

=>
Filesystem ...
/dev/disk1s1   ...
...
/dev/disk2s1   ...

```

## cd コマンドでホームディレクトに移動

```bash
$ cd ~
$ pwd

=>
/home/kindai
```

##  ネットワークの操作

### IPアドレスの確認 (ip addrコマンド)

自分のIPアドレスを確認してください

```bash
$ ip addr

=>
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host 
       valid_lft forever preferred_lft forever
2: enp3s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
    link/ether 74:d4:35:c6:0b:b3 brd ff:ff:ff:ff:ff:ff
    inet 192.168.0.18/24 brd 192.168.0.255 scope global noprefixroute enp3s0
       valid_lft forever preferred_lft forever
    inet6 fe80::8aa6:d0ba:d83d:c568/64 scope link noprefixroute 
       valid_lft forever preferred_lft forever
3: wlp2s0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN group default qlen 1000
    link/ether 54:27:1e:9e:9f:27 brd ff:ff:ff:ff:ff:ff

```

enpXXX の中の inet のところが IP v4 アドレスです


### 標準デバイス名の確認方法

ネットワークカード

```bash
$ ip addr
1: lo: <LOOPBACK,UP,LOWER_UP>
...
2: enp3s0: <BROADCAST,MULTICAST,UP,LOWER_UP>
...
3: wlp2s0: <BROADCAST,MULTICAST> 

```

### pingコマンド

Ctrl-C で終了

```bash
$ ping 8.8.8.8

=>
PING 8.8.8.8 (8.8.8.8) 56(84) bytes of data.
64 bytes from 8.8.8.8: icmp_seq=1 ttl=116 time=11.9 ms
64 bytes from 8.8.8.8: icmp_seq=2 ttl=116 time=13.3 ms
64 bytes from 8.8.8.8: icmp_seq=3 ttl=116 time=13.2 ms
^C
```

## nanoエディタの使い方

### 画面の下のメニューの見方

`    ^X などは、controlキーを押しながらｘを押すという意味`
`    M-X などは、escキーを押してxを押すという意味`

#### 編集したデータの保存方法

` ^O ファイル名を確認して enter で保存`

  
#### カットアンドペースト
 
`   ^K でカットして、^U で貼り付け`
 
#### 文字列の検索

`    ^W 検索文字列`

#### 終了

`     ^X 終了`


## コマンドファイル

### コマンドのファイルの存在場所の確認

```bash
$ which ls

=>
/bin/ls

$ which pwd

=>
/bin/pwd

$ which mv

=>
/bin/mv

$ which ip

=>
/sbin/ip
```


## 環境変数

### echo コマンドで環境変数を表示

```bash
$ echo $LANG
ja_JP.UTF-8

$ echo $SHELL
/bin/bash

$ echo $USER
kindai

$ echo $PATH
```

### 環境変数変更の例

```bash
$ export LANG=ja_JP.UTF-8

$ date +%x

=>
2024年10月14日

$ export LANG=en_GB.UTF-8

$ date +%x

=>
17/04/19

$ export LANG=en_US.UTF-8

$ date +%x
04/17/2019
```


## sudo コマンド

個人のパスワードでシステム管理者権限の操作を行う

```bash
$ cd /
$ touch gomi
touch: gomi: Permission denied
$ sudo touch gomi
Password:
$ sudo rm gomi
```

## aptコマンドによるソフトのインストール

sudo で実行する

* ssh をインストールする場合

```bash
$ sudo apt install ssh

=>
[sudo] yamasaki のパスワード: 

```

インストールされていないコマンドを実行しようとした場合

```bash
$ traceroute 8.8.8.8

=>
コマンド 'traceroute' が見つかりません。次の方法でインストールできます:
sudo apt install inetutils-traceroute  # version 2:2.2-2, or
sudo apt install traceroute            # version 1:2.1.0-2
```

aptコマンドで tracerouteコマンドをインストール

```bash
$ sudo apt install inetutils-traceroute
```

```bash
$ which traceroute

=>
/usr/bin/traceroute
```

```bash
$ traceroute 8.8.8.8

=>
traceroute to 8.8.8.8 (8.8.8.8), 30 hops max, 60 byte packets
 1  _gateway (192.168.0.1)  0.618 ms  0.570 ms  0.599 ms
 2  KD121105103065.ppp-bb.dion.ne.jp (121.105.103.65)  3.927 ms  5.524 ms  3.905 ms
 3  fkoBBAR001-1.bb.kddi.ne.jp (222.227.32.190)  5.502 ms  5.491 ms  5.465 ms
 4  27.85.132.197 (27.85.132.197)  14.435 ms 27.86.41.69 (27.86.41.69)  14.747 ms 27.85.225.29 (27.85.225.29)  14.411 ms
 5  27.85.134.50 (27.85.134.50)  12.116 ms 27.85.228.22 (27.85.228.22)  12.578 ms 27.86.120.178 (27.86.120.178)  12.361 ms
 6  72.14.202.237 (72.14.202.237)  12.207 ms  11.975 ms  12.084 ms
 7  * * *
 8  dns.google (8.8.8.8)  13.111 ms  13.163 ms  13.147 ms

```


## パスワードの変更

passwd コマンド

```bash
passwd

Old Password:

新しいパスワードを２回入れる
```


## IPアドレス関係情報の確認

```bash
$ ip addr
```

### 自分のIPアドレス関係情報の確認する

* Windows Powershell の場合

```bash
$ ipconfig
```

* Mac ターミナルの場合

```bash
$ ifconfig
```

* インターフェース名（有線、無線LAN、ローカルループバック）
* ipアドレス
* サブネットマスク
* ブロードキャストアドレス
* MACアドレス (ether)

## ルーティングテーブルの確認

```bash
$ ip route

default via 192.168.0.1 dev wlp0s20f3 proto static metric 600 
169.254.0.0/16 dev wlp0s20f3 scope link metric 1000 
192.168.0.0/27 dev wlp0s20f3 proto kernel scope link src 192.168.0.27 metric 600 
192.168.0.32/27 dev eno1 proto kernel scope link src 192.168.0.36 metric 100 
192.168.0.64/27 via 192.168.0.35 dev eno1 
192.168.0.96/27 via 192.168.0.35 dev eno1 
192.168.0.128/27 via 192.168.0.34 dev eno1 
192.168.0.160/27 via 192.168.0.34 dev eno1 
192.168.0.192/27 via 192.168.0.33 dev eno1 
192.168.0.224/27 via 192.168.0.33 dev eno1 

```

* デフォルトゲートウェイの確認

## ARPテーブルの確認

インストール

```bash
sudo apt install net-tools
```

```bash
$ arp -a
```

* MACアドレスとIPアドレスの対応

## ping による導通確認

確認したい相手のIPアドレスを知る

```bash
$ ping 192.168.0.35

=>
PING 192.168.0.35 (192.168.0.35) 56(84) bytes of data.
64 bytes from 192.168.0.35: icmp_seq=1 ttl=64 time=0.326 ms
64 bytes from 192.168.0.35: icmp_seq=2 ttl=64 time=0.530 ms
64 bytes from 192.168.0.35: icmp_seq=3 ttl=64 time=0.715 ms
64 bytes from 192.168.0.35: icmp_seq=4 ttl=64 time=0.681 ms
64 bytes from 192.168.0.35: icmp_seq=5 ttl=64 time=0.688 ms
64 bytes from 192.168.0.35: icmp_seq=6 ttl=64 time=0.244 ms
64 bytes from 192.168.0.35: icmp_seq=7 ttl=64 time=0.690 ms
64 bytes from 192.168.0.35: icmp_seq=8 ttl=64 time=0.742 ms


(コントロール ｃ) で終了する
```

* そのマシンまでの往復時間を確認する

### 様々なサイトにping してみる

* GOOGLEのDNSサーバ  8.8.8.8
* yahoo のサーバ www.yahoo.co.jp
* 近畿大学産業理工学部のDNSサーバ 157.13.1.1

### ping した後にARPテーブルを再確認する

```bash
$ arp
```

## traceroute で経路を確認する

```bash
$ traceroute 8.8.8.8

=>
traceroute to 8.8.8.8 (8.8.8.8), 64 hops max, 52 byte packets
 1  ia201wl4 (192.168.0.1)  7.978 ms  5.241 ms  5.784 ms
 2  oha-mx480-bbbas05.qtnet.ad.jp (218.40.227.156)  10.688 ms  10.055 ms  9.346 ms
 3  211.132.104.33 (211.132.104.33)  9.649 ms
    211.132.104.37 (211.132.104.37)  11.368 ms
    211.132.104.33 (211.132.104.33)  9.512 ms
 4  61.203.192.241 (61.203.192.241)  11.010 ms
    61.203.192.237 (61.203.192.237)  11.930 ms
    61.203.192.249 (61.203.192.249)  10.036 ms
 5  61.203.193.126 (61.203.193.126)  25.828 ms
    61.203.193.122 (61.203.193.122)  24.705 ms  25.398 ms
 6  61.203.192.177 (61.203.192.177)  23.924 ms  27.273 ms  24.774 ms
 7  108.170.242.129 (108.170.242.129)  26.158 ms  26.731 ms
    108.170.242.161 (108.170.242.161)  24.342 ms
 8  64.233.174.187 (64.233.174.187)  26.882 ms
    72.14.236.107 (72.14.236.107)  23.827 ms
    209.85.250.45 (209.85.250.45)  23.553 ms
 9  google-public-dns-a.google.com (8.8.8.8)  26.772 ms  24.187 ms  24.120 ms

```

いろいろなサイトまでの経路を確認する

* 演習ネットワークの全マシンのIPアドレス
* yahoo のサーバ www.yahoo.co.jp
* 近畿大学産業理工学部のDNSサーバ 157.13.61.1
* 九工大のwebサーバ  www.kyutech.ac.jp


## DNS関連情報

```bash
$ dig
```

### host コマンドでIPアドレスを確認する

```bash
$ host www.kindai.ac.jp

=>
www.kindai.ac.jp is an alias for kindai-1st-alb01-777703103.ap-northeast-1.elb.amazonaws.com.
kindai-1st-alb01-777703103.ap-northeast-1.elb.amazonaws.com has address 54.199.78.235
kindai-1st-alb01-777703103.ap-northeast-1.elb.amazonaws.com has address 52.68.198.87
```
### host コマンドでnsレコードを確認する

```bash
$ host -t ns kindai.ac.jp

=>
kindai.ac.jp name server tomato.cc.fuk.kindai.ac.jp.
kindai.ac.jp name server rs6000.cc.kindai.ac.jp.
kindai.ac.jp name server nsex.cc.kindai.ac.jp.
kindai.ac.jp name server carrot.cc.fuk.kindai.ac.jp.
```

## nanoエディターの練習

### ファイルの作成

```bash
nano test01.txt
```

### 基本操作

|操作　|キーボード|
| --- | --- |
|一行削除|Ctrl + K|
|Back Space 　	|Ctrl + H|
|Delete	|Ctrl + D|
|貼り付け　|Ctrl + U|
|取り消し|Esc + U|
|やり直し|Esc + E|
|上書き保存|Ctrl + S|
|名前をつけて保存|Ctrl + O|
|閉じる|Ctrl + X|

|移動単位|　　	キーボード|
| ---| --- |
|右| Ctrl + f|
|左| Ctrl + b
|下| Ctrl + n|
|上| Ctrl + p|
|行頭　|Ctrl + a|
|行末　|Ctrl + e|


|検索と置換|キーボード|
| --- | --- |
|後方検索|Ctrl + W|
|前方検索　|Ctrl + Q|
|前方へ続けて検索　|	Alt + W|
|後方へ続けて検索　|	Alt + Q|
|置換　　|Ctrl + WR|

### 試しに次の内容を nano エディタで入力して保存してください

```
network:
  version: 2
  renderer: networkd
  ethernets:
    eth0:
      dhcp4: true
      dhcp6: true
```

