# Lndのインストール
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

## bitcoind (testnet)でのptarmdの実行

```bash
cd install
./new_nodedir.sh ptermigan-yamalabo
cd ptermigan-yamalabo
```

ptermd の起動

```bash
../ptarmd --network=testnet &
```
