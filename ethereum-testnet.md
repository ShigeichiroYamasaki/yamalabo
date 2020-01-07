# Ethereum testnet 構築

## ethereum

### インストールスクリプト


★ユーザ名／パスワードの指定

### testnet

>> ユーザ名、パスワードの修正が必要

```
nano install-ethereum-testnet.sh
```


```bash
#!/bin/bash
sudo apt update
sleep 2
sudo apt upgrade -y
sleep 2
sudo apt-get install -y apt-file
sudo apt-file update

expect -c "
  set timetout -1
  spawn sudo apt-add-repository ppa:bitcoin/bitcoin
  expect {
    \"ENTER\" { send \"\\n\"}
  }
  interact
"
sleep 2
sudo apt-get update
sudo apt-get install -y bitcoind
sleep 2
bitcoind &
sleep 10
bitcoin-cli stop
sleep 30
cat << EOF > ~/.bitcoin/bitcoin.conf
testnet=3
txindex=1  
server=1   
rest=1      
rpcuser=ユーザ名
rpcpassword=パスワード
rpcport=18332 
EOF

bitcoind &
```


### 実行

```bash
chmod a+x install-bitcoincore-testnet.sh

```
