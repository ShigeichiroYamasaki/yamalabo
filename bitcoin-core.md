# bitcoin core インストール

## インストールスクリプト



★ユーザ名／パスワードの指定

### testnet

install-bitcoincore-testnet.sh

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

### mainnet

install-bitcoincore-mainnet.sh

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
mainnet=1 
txindex=1 
server=1  
rest=1
rpcuser=ユーザ名
rpcpassword=パスワード
rpcport=8332 
EOF

bitcoind &
```

### 実行

```bash
chmod a+x install-bitcoincore.sh
```

## bitcoin core アップデートスクリプト

update-bitcoincore.sh

### mainnet/ testnet 共通

```bash
#!/bin/bash
bitcoin-cli stop
sleep 300
sudo apt update
sudo apt upgrade -y
sleep 2
sudo apt-get install -y bitcoind
bitcoind &
```

--
## 詳細説明

### apt でインストール

```bash
sudo apt-add-repository ppa:bitcoin/bitcoin

# enterキーを押す

sudo apt-get update
sudo apt-get install -y bitcoind
```

### bitcoind の一時起動と環境ファイルの作成

```bash
bitcoind &
```

### bitcoind の停止

bitcoin-cli を使って停止させる

```bash
bitcoin-cli stop
```

これで ~/.bitcoin の下に起動環境が作成される

## 設定ファイルbitcoin.confの作成

~/.bitcoin の下に bitcoin.conf を作成する

```bash
nano ~/.bitcoin/bitcoin.conf
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

### bitcoind の停止

bitcoin-cli を使って停止させる

```bash
bitcoin-cli stop
```


## 自動起動設定

cron を使って設定

* ベアボーンではユーザは yamalabo
* raspberry pi ではユーザは ubuntu
*

### ベアボーン

```bash
crontab -u yamalabo -e

# 1 nano エディタを選ぶ
```

### raspberry pi

```bash
crontab -u ubuntu -e

# 1 nano エディタを選ぶ
```

### crontab の編集

以下を最後に追加

```
# ...

@reboot /usr/bin/bitcoind -deamon
```

^(コントロール)o ^(コントロール)x でnanoエディタを保存終了

### 再起動で確認

```bash
sudo reboot
```

再起動後

bitcoind のhelpが出力されれば成功

```bash
bitcoin-cli help
```

