# Bitcoin core Signet ノードの構築

## install 方法

nano install-bitcoincore-signet.sh

```bash
```bash
#!/bin/bash
sudo apt update
sudo apt upgrade -y
sudo apt install snapd
sudo snap install bitcoin-core
mkdir .bitcoin
cat << EOF > ~/.bitcoin/bitcoin.conf
signet=1
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
chmod a+x inano install-bitcoincore-signet.sh
./install-bitcoincore-signet.sh

```


## bitcoind の起動

```bash
bitcoin-core.daemon -signet
```

## 確認

```bash
bitcoin-cli -datadir=signet getblockcount

bitcoin-core.cli -testnet getbalance

```


