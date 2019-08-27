# ptermiganのインストール
* ubuntu 18.14.2LTS
* bitcoin core
* regtest

## github termigan

[https://github.com/nayutaco/ptarmigan](https://github.com/nayutaco/ptarmigan)

## 事前準備

```bash
sudo apt install -y git autoconf pkg-config build-essential libtool python3 wget jq bc
```

## ptermigan のインストール

```bash
git clone https://github.com/nayutaco/ptarmigan.git
cd ptarmigan
make full
```

## bitcoind でのptarmdの構築

```bash
cd install
./new_nodedir.sh ptermigan-yamalabo
cd ptermigan-yamalabo
```

## ptermd の起動

* testnet の場合

```bash
~/ptarmigan/install/ptarmd --network=testnet &
```

* mainnet の場合

```bash
~/ptarmigan/install/ptarmd --network=mainnet &
```

## ptermigan の利用

* work files: current directory
* chain: mainnet/testnet
* port: 9735
* rpcport: 9736
* alias: node_ + node_id[0:6](like: node_03a7f9dff5e6)
* color: #000000(black)
* no IP address in node_announcement
* bitcoind rpcuser/rpcpassword: read from ~/.bitcoin/bitcoin.conf
* options...

## ptarmcli によるデーモンのコントロール

### ノード情報の例

* ノード１ (192.168.0.12)

```bash
~/ptarmigan/install/ptarmcli --getinfo
{
 "result": {
  "node_id": "03cd9ca175b78427de41a85964fe2e9089c205db25bd80997fdd1067e302747c09",
  "node_port": 9735,
  "total_local_msat": 0,
  "block_count": 1519992,
  "peers": []
 }
}

```

* ノード２（192.168.0.22）

```bash
~/ptarmigan/install/ptarmcli --getinfo
{
 "result": {
  "node_id": "03c6eb30c5dc4b95c6aeb7dbb4aa1f337f698613ac783e7bf7f221188c83483d06",
  "node_port": 9735,
  "total_local_msat": 0,
  "block_count": 206303,
  "peers": []
 }
}

```

### ノードとの接続

（ノード１からノード２へ接続）

* 接続先IPアドレス：
* 接続先ノードID：
* ポート番号：

	ptarmcli -c NODE_ID@IPv4_ADDRESS:PORT
	


```bash
~/ptarmigan/instal/ptarmcli -c NODE_ID@IPv4_ADDRESS:PORT
```