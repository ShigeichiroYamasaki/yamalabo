# 山崎研究室実験ネットワーク

##ハード

### ベアボーン：　512Gb 23 台 (meinnet)
	
* (M+SM+EM+FS)+P  12 台
* (M+SM+EM+FS)+L  11台

### ベアボーン：　256Gb   2台 (testnet) lnd

* (T+ST+ET+FS)+L　２台
	
### ラズベリーパイ　：40台 (testnet)

* (T+ST+ET)+P　40台

## 稼働させるサービス

* bitcoind (mainnet) (M)
* bitcoind (testnet) (T)
* ethereum (mainnet) (EM)
* ethereum (tesnet) (ET)
* IPFS (FS)
* bitcoinサイドチェーン (elements mainnet) (SM)
* bitcoinサイドチェーン (elements testnet) (ST)
* ightning network (ptermigan mainnet) 
* lightning network (ptermigan testnet)
* lightning network (lnd mainnet) 
* lightning network (lnd testnet)

## ノードの種類

### mainnet

* (M+SM+EM+FS)+P
* (M+SM+EM+FS)+L

### testnet

* (T+ST+ET+FS)+P
* (T+ST+ET+FS)+L


## 利用するポート番号一覧

### bitcoind (mainnet)

* TCP 8333 : ノード接続待ち受け
* TCP 8332 : RPC待ち受け

### bitcoind (testnet)

* TCP 18333 : ノード接続待ち受け
* TCP 18332 : RPC待ち受け

### lightning network (ptermigan mainnet)

* TCP 6000 	: ifpaytt
* TCP 9735 	: lightningd (mainnetもtestnetも)
* TCP 9736 	: lightningd (mainnetもtestnetも)

### lightning network (lnd mainnet)

* TCP 9735 	: lightningd (mainnetもtestnetも)
* TCP 9736 	: lightningd (mainnetもtestnetも)

### lightning network (ptermigan testnet)

* bitcoinサイドチェーン (elements)
* ethereum (mainnet)
* ethereum (tesnet)

* UDP 30303 
* TCP 30303

### IPFS

* TCP 4001 IPFSゲートウェイ
* TCP 8080 IPFSゲートウェイ

## インストール方法

### bitcoin core

```bash
sudo apt-add-repository ppa:bitcoin/bitcoin

# enter

sudo apt-get update

sudo apt-get install bitcoind
```

bitcoind の起動

```bash
bitcoind &
```

bitcoind の停止

```bash
bitcoin-cli stop
```

これで ~/.bitcoin の下に起動環境が作成される

#### bitcoin.conf

~/.bitcoin の下に bitcoin.conf を作成する

```bash
nano bitcoin.conf
```

mainnet

```
mainnet=1 
txindex=1  
server=1   
rest=1      
rpcuser= "yamalabo"
rpcpassword= "yozoranomukou"
rpcport=8332 
```

testnet

```
testnet=3
txindex=1  
server=1   
rest=1      
rpcuser= "yamalabo"
rpcpassword= "yozoranomukou"
rpcport=18332 
```

### bitcoind の起動

同期させる

```bash
bitcoind &
```

## etherum (Geth) のインストール

```bash
sudo apt-get install software-properties-common
sudo add-apt-repository -y ppa:ethereum/ethereum
sudo apt update
sudo apt install ethereum
```

### gethのtestnetでの起動

テストネットの種類

* ROPSTEN (Revived) - Proof Of Work
* KOVAN - Proof Of Authority (Parity only)
* RINKEBY - Clique Consensus (Geth only)

```bash
geth --testnet --cache=1024 console
```

### gethのmainnetでの起動

```bash
geth --cache=1024 console
```


## Lightning Network

### ptermigan のインストール

```bash
sudo apt install -y git autoconf pkg-config build-essential libtool python3 wget jq bc

git clone https://github.com/nayutaco/ptarmigan.git
cd ptarmigan
make full
```
