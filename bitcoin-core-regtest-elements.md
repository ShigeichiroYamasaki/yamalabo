# bitcoin regtest 構築

## bitcoin core インストール

### インストールスクリプト



★ユーザ名／パスワードの指定

### testnet

>> ユーザ名、パスワードの修正が必要

install-bitcoincore-regtest.sh


```bash
#!/bin/bash
sudo apt update
sudo apt upgrade -y
sudo apt install snapd
sudo snap install bitcoin-core
cat << EOF > ~/.bitcoin/bitcoin.conf
regtest=1
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
bitcoin-core.daemon -regtest
```

### クライアント接続

```bash
bitcoin-core.cli -regtest help
```


