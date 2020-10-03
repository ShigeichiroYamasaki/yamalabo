# HyperLedger Fabric

## ubuntu 18.14LTS

* Docker v18.09
* Docker-compose v1.24
* Go lang v1.12
* Node.js v8.9.4
* npm v5.6.0
* Python v2.7
* Hyperledger Fabric v1.4.4

## Fabcarディレクトリ構成

### Chaincodeファイル

```
~/fabric-samples/chaincode/fabcar/go/配下
~/fabric-samples/chaincode/fabcar/javascript/lib/配下
```

### Chaincode実行用SDKファイル

```
~/fabric-samples/fabcar/javascript/配下
```


### インストール

```bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y apt-transport-https
sudo apt install -y ca-certificates
sudo apt install -y curl
sudo apt install -y software-properties-common
sudo snap install docker
docker-compose 
sudo apt install -y docker-compose 
curl -L git.io/nodebrew | perl - setup
nodebrew install-binary v8.9.4
nodebrew use v8.9.4
sudo apt  install -y golang-go
curl -sSL http://bit.ly/2ysbOFE | bash -s
echo 'export PATH=$PATH:$HOME/fabric-sample/bin' >> ~/.bash_profile
source ~/.bash_profile 

```