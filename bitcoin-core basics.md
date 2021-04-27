# Bitcoin core 基本操作

signet ubuntu を想定

## bitcoind を起動

ubuntu snap 

```
bitcoin-core.daemon &
```

## ビットコインネットワークの情報確認

自分のノードのバージョン，プロトコルバージョン，接続ノード数，IPアドレスとポート番号などの情報を表示します。

```
bitcoin-core.cli getnetworkinfo
```

### 接続中のノードの情報一覧

接続中のノードのバージョン，IPアドレス，プロトコルのバージョン，ノードの状態などに関する情報を表示します。

```
bitcoin-core.cli  getpeerinfo
```

## ワレット

### ワレットの作成

```
bitcoin-core.cli  createwallet <ワレット名>
```

```
bitcoin-core.cli  createwallet yamalabo1
bitcoin-core.cli  createwallet yamalabo2
```

### ワレットの一覧

```
bitcoin-core.cli  listwallets
```

### ワレットのロード

```
bitcoin-core.cli loadwallet <ワレット名>
```

```
bitcoin-core.cli loadwallet yamalabo2
```

### ビットコインアドレスの生成

自分のアドレスを生成して，メモしてください

```
bitcoin-core.cli  -rpcwallet=<ワレット名> getnewaddress
```

```
bitcoin-core.cli  -rpcwallet=yamalabo2 getnewaddress
```

ここでは以下のようなアドレスが返ってきたとします

 tb1q4f8mhhdxmwlscxqg623fjun580wtyldut5a6y3



### ビットコインアドレスに対応する公開鍵を確認する

```
bitcoin-core.cli  -rpcwallet=<ワレット名> getaddressinfo <ビットコインアドレス>
```

```
bitcoin-core.cli  -rpcwallet=yamalabo2 getaddressinfo tb1q4f8mhhdxmwlscxqg623fjun580wtyldut5a6y3
```

### ビットコインアドレスに対応する秘密鍵を確認する

```
bitcoin-core.cli  rpcwallet=<ワレット名> dumpprivkey <ビットコインアドレス>
```

```
bitcoin-core.cli  rpcwallet= yamalabo2 dumpprivkey tb1q4f8mhhdxmwlscxqg623fjun580wtyldut5a6y3
```

### テスト用のビットコインの入手方法

[ https://signet.bc-2.jp/]( https://signet.bc-2.jp/)

### 自分のワレットのビットコインの残高の確認

```
bitcoin-core.cli  -rpcwallet=yamalabo2 getbalance
```

