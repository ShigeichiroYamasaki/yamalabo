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

## crontab の編集

* ベアボーン

```bash
crontab -u yamalabo -e
```

* raspberry pi

```bash
crontab -u ubuntu -e
```

testnet

```
# ...

@reboot /usr/bin/bitcoind
@reboot ~/ptarmigan/install/ptarmd --network=testnet
```

mainnet

```
# ...

@reboot /usr/bin/bitcoind
@reboot ~/ptarmigan/install/ptarmd --network=mainnet
```
## ptarmcli によるデーモンのコントロール

### ノード情報の例

* ノード１ (192.168.0.12)

```bash
~/ptarmigan/install/ptarmcli --getinfo

{
 "result": {
  "node_id": "02a6321740fc22701fb96b7e79f153076d2d3524a0cd31d39442dcd4f6dccb6871",
  "node_port": 9735,
  "total_local_msat": 0,
  "block_count": 1576313,
  "peers": []
 }
}

```

* ノード２（192.168.0.18）

```bash
~/ptarmigan/install/ptarmcli --getinfo


{
 "result": {
  "node_id": "03c6eb30c5dc4b95c6aeb7dbb4aa1f337f698613ac783e7bf7f221188c83483d06",
  "node_port": 9735,
  "total_local_msat": 0,
  "block_count": 1576313,
  "peers": []
 }
}


```

### ノードとの接続

（ノード１ (192.168.0.12)からノード２（192.168.0.18）へ接続）

* 接続先IPアドレス：192.168.0.18
* 接続先ノードID：03c6eb30c5dc4b95c6aeb7dbb4aa1f337f698613ac783e7bf7f221188c83483d06
* ポート番号：9735

	ptarmcli -c NODE_ID@IPv4_ADDRESS:PORT
	

##### ノード１ (192.168.0.12)から接続

```bash
~/ptarmigan/install/ptarmcli -c 03c6eb30c5dc4b95c6aeb7dbb4aa1f337f698613ac783e7bf7f221188c83483d06@192.168.0.18:9735
```

実行結果

```
[client]connected: 192.168.0.18:9735
[client]node_id=03c6eb30c5dc4b95c6aeb7dbb4aa1f337f698613ac783e7bf7f221188c83483d06
connected peer: 03c6eb30c5dc4b95c6aeb7dbb4aa1f337f698613ac783e7bf7f221188c83483d06
2019-08-29T01:43:58Z [default wallet] keypool reserve 1
2019-08-29T01:43:58Z [default wallet] keypool keep 1
{
 "result": "OK"
}

```

受信側の接続結果

```
connected peer: 02a6321740fc22701fb96b7e79f153076d2d3524a0cd31d39442dcd4f6dccb6871
```

## チャンネルのオープン

	../ptarmcli -c NODE_ID -f AMOUNT_SAT

AMOUNT_SAT: satoshi 単位の通貨量

```bash
../ptarmcli -c 03c6eb30c5dc4b95c6aeb7dbb4aa1f337f698613ac783e7bf7f221188c83483d06 -f 1000
```