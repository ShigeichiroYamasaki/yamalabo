# bitcoin regtest 構築

## bitcoin core インストール

### インストールスクリプト



★ユーザ名／パスワードの指定

### testnet

>> ユーザ名、パスワードの修正が必要

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


### 実行

```bash
chmod a+x install-bitcoincore-testnet.sh

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


## 自動起動設定

cron を使って設定

* ベアボーンではユーザは yamalabo
* raspberry pi ではユーザは ubuntu


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



## Rail のインストール

```bash
gem install sqlite3
gem install rails
```