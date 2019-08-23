# bitcoin core インストール

## apt でインストール

```bash
sudo apt-add-repository ppa:bitcoin/bitcoin

# enterキーを押す

sudo apt-get update
sudo apt-get install bitcoind
```

## bitcoind の一時起動と環境ファイルの作成

```bash
bitcoind &
```

### bitcoind の停止

bitcoin-cli を使って停止させる

```bash
bitcoin-cli stop
```

これで ~/.bitcoin の下に起動環境が作成される

##　設定ファイルbitcoin.confの作成

~/.bitcoin の下に bitcoin.conf を作成する

```bash
nano bitcoin.conf
```

### mainnetの場合

```
mainnet=1 
txindex=1  
server=1   
rest=1      
rpcuser= "ユーザ名"
rpcpassword= "パスワード"
rpcport=8332 
```

testnet

```
testnet=3
txindex=1  
server=1   
rest=1      
rpcuser= "ユーザ名"
rpcpassword= "パスワード"
rpcport=18332 
```

### bitcoind の起動確認

```bash
bitcoind &
```

## 自動起動設定

bitcoind.setvice ファイルの存在を確認

```bash
sudo systemctl list-unit-files bitcoind.service

UNIT FILE        STATE   
bitcoind.service disabled

1 unit files listed.
```

ユーザ名を bitcoin から yamalaboに変更

