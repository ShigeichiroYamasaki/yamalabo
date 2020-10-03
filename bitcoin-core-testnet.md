# bitcoin testnet 構築

## bitcoin core インストール

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
```

