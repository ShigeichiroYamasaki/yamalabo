# 山崎研究室実験ネットワーク

##ハードの準備状況

実際の申請では、10台ずつ増加させておく

### ベアボーン：　512Gb 23 台 

ubuntu 18.04.2 LTS (B512)

### ベアボーン：　256Gb   2台 

ubuntu 18.04.2 LTS (B256)

### ラズベリーパイ　：256Gb 40台 

Ubuntu 18.04.2 LTS (R256)

## 稼働させるサービス

### bitcoin (bitcoin core)
#### lightning network (ptermigan, Ind)
#### sidechain (elements)
### Ethereum (parity)
### IPFS
### Libra

## main net サービス

ベアボーン(512Gb) 23台

* bitcoind (BC)　（20台）
	* ightning network (ptermigan) (BC+LNP)　（10/20台）
	* ightning network (Ind) (BC+LNI)　（10/20台）
	* Elements (BC+EL) (10/20台)
* Ethereum (parity)(ET) (2台)
* IPFS(IP) (1台)	

## test net サービス

ベアボーン(256Gb) 2台
ラズベリーパイ(256Gb) 40台

* bitcoind (BC)　（ラズベリーパイ 40台）
	* ightning network (ptermigan) (BC+LNP)　（20/40台）
	* ightning network (Ind) (BC+LNI)　（20/40台）
	* Elements (BC+EL) （40/40台）
* Ethereum (parity)(ET) (ベアボーン(256Gb) 1台)
* Libra (ベアボーン(256Gb) 1台)


## 利用するポート番号一覧

### bitcoind (mainnet)

* ★TCP 8333 : ノード接続待ち受け
* TCP 8332 : RPC待ち受け

### bitcoind (testnet)

* ★TCP 18333 : ノード接続待ち受け
* TCP 18332 : RPC待ち受け

### lightning network (ptermigan mainnet)

* ★TCP 6000 	: ifpaytt (If Pay Then That)
* ★TCP 9735 	: lightningd (mainnetもtestnetも)
* ★TCP 9736 	: lightningd (mainnetもtestnetも)

### lightning network (lnd mainnet)

* ★TCP 9735 	: lightningd (mainnetもtestnetも)
* ★TCP 9736 	: lightningd (mainnetもtestnetも)

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
