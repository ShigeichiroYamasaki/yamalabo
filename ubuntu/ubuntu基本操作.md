# ubuntuの基本操作

以下，結果をメモしていく

## ファイルシステムの操作

### 現在のディレクトリのパスの確認 (pwdコマンド)

```
$ pwd
```

### ディレクトリへの移動 (cdコマンド)


```
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

```
$ ls

```

####  ls コマンドに -l オプションをつけると詳細情報が表示される

```
$ ls -l
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

```
$ mkdir kadai
$ ls
ビデオ	  ダウンロード	テンプレート  デスクトップ  kadai
ピクチャ  ミュージック	ドキュメント  公開

```

### 新しいファイルの作成 (touchコマンド)

```
$ touch test
$ ls
test

```

### ファイルのコピー (cpコマンド)

```
$ cp test test1
$ ls
test  test1

```

### ファイルの移動 (mvコマンド)

```
$ mkdir work
$ ls
test  test1  work
$ mv test work
$ ls
test1  work
$ cd work/
$ ls
test

```

### ファイル名の変更

```
$ mv test test100
$ ls
test100

```

### ファイルの削除 (rmコマンド)

```
$ ls
test100
$ rm test100 
$ ls

```

### ディスク（SSD)の空きの確認　（dfコマンド）

-m オプションは　メガバイト単位という意味

```
$ df -m
Filesystem ...
/dev/disk1s1   ...
...
/dev/disk2s1   ...

```

## cd コマンドでホームディレクトに移動

```
$ cd ~
$ pwd
/home/kindai

```

##  ネットワークの操作

### IPアドレスの確認 (ip addrコマンド)

自分のIPアドレスを確認してください

```
$ ip addr

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


### 標準デバイス名の確認方法

ネットワークカード

```
$ ip addr
1: lo: <LOOPBACK,UP,LOWER_UP>
...
2: enp3s0: <BROADCAST,MULTICAST,UP,LOWER_UP>
...
3: wlp2s0: <BROADCAST,MULTICAST> 

```

### pingコマンド

Ctrl-C で終了

```
$ ping 8.8.8.8
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

```
$ which ls
/bin/ls

$ which pwd
/bin/pwd

$ which mv
/bin/mv

$ which ip
/sbin/ip
```


## 環境変数

### echo コマンドで環境変数を表示

```
$ echo $LANG
ja_JP.UTF-8

$ echo $SHELL
/bin/bash

$ echo $USER
kindai

$ echo $PATH
```

### 環境変数変更の例

```
$ export LANG=ja_JP.UTF-8

$ date +%x
2019年04月17日

$ export LANG=en_GB.UTF-8

$ date +%x
17/04/19

$ export LANG=en_US.UTF-8

$ date +%x
04/17/2019
```


## sudo コマンド

個人のパスワードでシステム管理者権限の操作を行う

```
$ cd /
$ touch gomi
touch: gomi: Permission denied
$ sudo touch gomi
Password:
$ sudo rm gomi
```

## aptコマンドによるソフトのインストール

sudo で実行する

```
$ sudo apt install ssh
[sudo] yamasaki のパスワード: 

```

インストールされていないコマンドを実行しようとした場合

```
$ traceroute 8.8.8.8
コマンド 'traceroute' が見つかりません。次の方法でインストールできます:
sudo apt install inetutils-traceroute  # version 2:2.2-2, or
sudo apt install traceroute            # version 1:2.1.0-2
```

aptコマンドで tracerouteコマンドをインストール

```
$ sudo apt install traceroute
```

```
$ which traceroute
/usr/sbin/traceroute
```

```
$ traceroute 8.8.8.8
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

