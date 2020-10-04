# HyperLedger Fabric

## ubuntu 18.14LTS

* Docker CE
* Docker-compose 
* Go lang
* Node.js
* npm v5.6.0
* Python
* Hyperledger Fabric

## Docker CE のinstall

事前準備

```bash
sudo apt update
sudo apt upgrade -y
sudo apt remove docker docker-engine docker.io containerd runc
sudo apt install -y \
    apt-transport-https \
    ca-certificates \
    curl \
    gnupg-agent \
    software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo apt-key fingerprint 0EBFCD88
```

```bash
sudo add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
   $(lsb_release -cs) \
   stable"
sudo apt update
sudo apt install -y docker-ce docker-ce-cli containerd.io
```

### Docker compose install

```bash
sudo apt install -y docker-compose
```

###  Node.js install

```bash
sudo apt install -y nodejs npm
```

###  Hyperledger Fabric install

```bash
sudo apt update && sudo apt upgrade -y
```

* ユーザ追加 (hyper)

```bash
sudo adduser hyper
sudo usermod -aG sudo hyper
su - hyper
```

* セットアップ

```bash
sudo npm install npm@5.6.0 -g
sudo usermod -a -G docker $USER
sudo systemctl start docker
sudo systemctl enable docker
```

* go lang のインストール

```bash
wget https://dl.google.com/go/go1.13.6.linux-amd64.tar.gz
tar -xzvf go1.13.6.linux-amd64.tar.gz
sudo mv go/ /usr/local
```

* gopathの修正

nano ~/.bashrc

```bash
#(add these 2 lines to end of .bashrc file)
export GOPATH=/usr/local/go
export PATH=$PATH:$GOPATH/bin
```

### Hyperledger Fabric  install

```bash
curl -sSL http://bit.ly/2ysbOFE | bash -s
```

### Hyperledger Fablric testネットワークに接続

```bash
cd fabric-samples/test-network
./network.sh up
```

```
CONTAINER ID        IMAGE                               COMMAND             CREATED             STATUS                  PORTS                              NAMES
07f99ab46182        hyperledger/fabric-orderer:latest   "orderer"           2 seconds ago       Up 1 second             0.0.0.0:7050->7050/tcp             orderer.example.com
13c4695d5204        hyperledger/fabric-peer:latest      "peer node start"   2 seconds ago       Up 1 second             0.0.0.0:7051->7051/tcp             peer0.org1.example.com
6b2d39b36962        hyperledger/fabric-peer:latest      "peer node start"   2 seconds ago       Up Less than a second   7051/tcp, 0.0.0.0:9051->9051/tcp   peer0.org2.example.com

```

### ネットワーク接続終了

```bash
./network.sh down
```