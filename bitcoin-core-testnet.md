# bitcoin testnet 構築

## bitcoin core インストール
ubuntu 18.04LTS

* WSL2 の ubuntu ではうまくいかない

### testnet インストールスクリプト

>> ユーザ名、パスワードの修正が必要

nano install-bitcoincore-testnet.sh


```bash
#!/bin/bash
sudo apt update
sudo apt upgrade -y
sudo apt install snapd
sudo snap install bitcoin-core
mkdir .bitcoin
cat << EOF > ~/.bitcoin/bitcoin.conf
testnet=3
txindex=1
server=1
rest=1
rpcuser="user"
rpcpassword="password"
rpcport=18332
EOF
```


### インストール実行

```bash
chmod a+x install-bitcoincore-testnet.sh

```

### デーモン起動


```bash
bitcoin-core.daemon -testnet
```

### クライアント接続

```bash
bitcoin-core.cli -testnet help

bitcoin-core.cli -testnet getblockcount

bitcoin-core.cli -testnet getbalance
```

### 自分のアドレス生成

```bash
bitcoin-core.cli -testnet getnewaddress ichiro
tb1qmv2xqghzrp3esph3h3s60ltzj2n507sawxahxd
```

### ファウセットからコインを得る

https://bitcoinfaucet.uo1.net/

自分のアドレスを入れる

２回ほどやる

### 残高確認（１０分後）

```
bitcoin-core.cli -testnet getbalance
```


