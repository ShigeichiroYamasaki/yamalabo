# ptarmiganのインストール
* ubuntu 18.14.2LTS
* bitcoin core
* regtest

## インストールスクリプト

install-ptarmigan.sh

### tesntnet

```bash
#!/bin/bash
~/ptarmigan/install/ptarmcli --stop
sudo apt install -y git autoconf pkg-config build-essential libtool python3 wget jq bc
sleep 2
git clone https://github.com/nayutaco/ptarmigan.git
cd ptarmigan
make full
sleep 2
cd install
sleep 2
~/ptarmigan/install/new_nodedir.sh ptarmigan-yamalabo
cd ~/ptarmigan/install/ptarmigan-yamalabo
sleep 2
echo "export PATH=\$PATH:~/ptarmigan/install/" >> ~/.bashrc
source ~/.bashrc
~/ptarmigan/install/ptarmd --network=testnet &
```

### mainnet

```bash
#!/bin/bash
~/ptarmigan/install/ptarmcli --stop
sudo apt install -y git autoconf pkg-config build-essential libtool python3 wget jq bc
sleep 2
git clone https://github.com/nayutaco/ptarmigan.git
cd ptarmigan
make full
sleep 2
cd install
sleep 2
~/ptarmigan/install/new_nodedir.sh ptarmigan-yamalabo
cd ~/ptarmigan/install/ptarmigan-yamalabo
sleep 2
echo "export PATH=\$PATH:~\/ptarmigan\/install" >> ~/.bashrc
source ~/.bashrc
~/ptarmigan/install/ptarmd --network=mainnet &
```


```bash
chmod a+x install-ptarmigan.sh
```

```bash
./install-ptarmigan.sh
```

## github ptarmigan

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

## ノードへのbitcoinの送金

ligntning networkのノードとペイメントチャンネルを開設するためには、ノードがbitcoinを持っている必要がある。


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
../ptarmcli -c 032d3e5885101b3af7d1fe9a739691566e820d171ba90fb13a827e548314831f06 -f 300000
{
 "result": {
  "status": "Progressing"
 }
}

```

### 確認

```bash
$ ptarmcli --getinfo
{
 "result": {
  "node_id": "02a6321740fc22701fb96b7e79f153076d2d3524a0cd31d39442dcd4f6dccb6871",
  "node_port": 9735,
  "total_local_msat": 300000000,
  "block_count": 1578573,
  "peers": [{
    "role": "client",
    "status": "normal operation",
    "node_id": "032d3e5885101b3af7d1fe9a739691566e820d171ba90fb13a827e548314831f06",
    "channel_id": "75e2a64bec2652cf4911abc6bee1de9129ec446571e8890140dbf08f61d1ace9",
    "short_channel_id": "1578571x128x0",
    "funding_tx": "e9acd1618ff0db400189e8716544ec2991dee1bec6ab1149cf5226ec4ba6e275",
    "funding_vout": 0,
    "confirmation": 3,
    "feerate_per_kw": 253,
    "announcement_signatures": "not exchanged",
    "local": {
     "msatoshi": 300000000,
     "commit_num": 0,
     "num_htlc_outputs": 0
    },
    "remote": {
     "msatoshi": 0,
     "commit_num": 0,
     "num_htlc_outputs": 0
    }
   }]
 }
}
```

## 1ブロック以上進むのを待つ

```bsh
bitcoin-cli getblockcount
1578577
```

### 確認

```bash
{
 "result": {
  "node_id": "02a6321740fc22701fb96b7e79f153076d2d3524a0cd31d39442dcd4f6dccb6871",
  "node_port": 9735,
  "total_local_msat": 300000000,
  "block_count": 1578643,
  "peers": [{
    "role": "client",
    "status": "normal operation",
    "node_id": "032d3e5885101b3af7d1fe9a739691566e820d171ba90fb13a827e548314831f06",
    "channel_id": "75e2a64bec2652cf4911abc6bee1de9129ec446571e8890140dbf08f61d1ace9",
    "short_channel_id": "1578571x128x0",
    "funding_tx": "e9acd1618ff0db400189e8716544ec2991dee1bec6ab1149cf5226ec4ba6e275",
    "funding_vout": 0,
    "confirmation": 73,
    "feerate_per_kw": 253,
    "announcement_signatures": "exchanged",
    "local": {
     "msatoshi": 300000000,
     "commit_num": 12,
     "num_htlc_outputs": 0
    },
    "remote": {
     "msatoshi": 0,
     "commit_num": 12,
     "num_htlc_outputs": 0
    }
   }]
 }
}
```

## invoice生成（受領者）

```bash
ptarmcli --createinvoice 1000
{
 "result": {
  "hash": "16b2428653836ea06712329667a46c82be664ee879fe349ec7e4e11f6baa7551",
  "amount_msat": 1000,
  "bolt11": "lntb10n1pwhlt78np4q2nry96qls38q8aeddl8nu2nqakj6dfy5rxnr5u5gtwdfakued58zpp5z6ey9pjnsdh2qecjx2tx0frvs2lxvnhg08lrf8k8uns376a2w4gsdqqhwlxfp0xrmrf7ql5d3xuv9h5gqyfr7epjwx968jmuvq799fvurn9ftfye3lpwngyjszj43z7pk63vg8jffm04j8tkjfpxmpdk8kvpusqwvh9lf",
  "note": "no payable-amount channel"
 }
}
```

## 送金（送金者）

```bash
ptarmcli --sendpayment lntb10n1pwhlvtxnp4q2nry96qls38q8aeddl8nu2nqakj6dfy5rxnr5u5gtwdfakued58zpp5p67rthvfmecfchs0z3v9pnsa35ewfs3xg2z8hc5s2nzwz8ur9cdsdqqdr3fcddkxdekkl4alvszyhj82pkaqkn7jz6647qmeuwea5r0k83nm98wcqalhdn7xs3pk24lf3nwxju9attvhfdqpd8nsx0aqc6lvhgpmsrxly
```

```bash
{
 "result": {
  "payment_id": 0
 }
}
```

### 送金の確認

```bash
 ptarmcli --listpayment=0
{
 "result": [{
   "payment_id": 0,
   "payment_hash": "0ebc35dd89de709c5e0f145850ce1d8d32e4c22642847be29054c4e11f832e1b",
   "additional_amount_msat": 0,
   "block_count": 1578643,
   "retry_count": 0,
   "max_retry_count": 10,
   "auto_remove": "false",
   "state": "failed",
   "invoice": "lntb10n1pwhlvtxnp4q2nry96qls38q8aeddl8nu2nqakj6dfy5rxnr5u5gtwdfakued58zpp5p67rthvfmecfchs0z3v9pnsa35ewfs3xg2z8hc5s2nzwz8ur9cdsdqqdr3fcddkxdekkl4alvszyhj82pkaqkn7jz6647qmeuwea5r0k83nm98wcqalhdn7xs3pk24lf3nwxju9attvhfdqpd8nsx0aqc6lvhgpmsrxly"
  }]
}

```

