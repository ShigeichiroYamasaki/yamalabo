# ION(アイオン）のインストール

最終更新　2022/07/31 Shigeichiro Yamasaki

DID（Decentralized Identifier）の実装の一つであるIONのインストール

![](https://github.com/decentralized-identity/ion/raw/master/images/logo.png)

[https://tsmatz.wordpress.com/2020/09/01/did-sidetree-ion/](https://tsmatz.wordpress.com/2020/09/01/did-sidetree-ion/)

開発元のマイクロソフトの解説ページ

 ![](https://d10nlab-prd-app-image.s3.ap-northeast-1.amazonaws.com/variants/aln8jgqb75txsjralygfx3ko4zwp/a995f8972e5d6d5993e5c77cbd8550c80b3681f3919a95feea65027a8c801bf6)
 
 引用: https://query.prod.cms.rt.microsoft.com/cms/api/am/binary/RE2DjfY 

## ION

* IONは bitcoin のブロックチェーンを利用するDIDシステムです
* Sidetree は、DIDプロトコルです。
* IONは、Node.js で実装されています。
* Content Addressable Store にIPFSを利用します。
* ローカルリポジトリには、MongoDBを使用します

![](https://tsmatz.files.wordpress.com/2020/09/20200904_ion_architecture.jpg?w=829)

引用：https://tsmatz.files.wordpress.com/2020/09/20200904_ion_architecture.jpg?w=829

### Sidetree

IONは sidetreeというBitcoin のセカンドレイヤ技術を利用しています

#### Sidetree の３階層とその実装システム


|要素| システム|
| :--| :--|
|Content-Addressed Storage System（CAS）|IPFS|
|Sidetree Node | ION node (Bitcoinのセカンドレイヤ）|
|Blockchain Peer Network | Bitcoin|


 * Content-Addressed Storage System（CAS）: データをポインタでリンクして保存するストレージシステム。（CASのエンドポイント情報はブロックチェーン上に固定されて保存される）
 * Sidetree Node : DID Documentに対するオペレーション(作成、解決、更新、削除等)を行うためのプログラム（パグリックブロックチェーンのセカンドレイヤ）
 * Blockchain Peer Network : DIDやそれに付随するDID SubjectやDID Documentを保存するための改ざん耐性を持つパブリックブロックチェーンによる基盤



![](https://techcommunity.microsoft.com/t5/image/serverpage/image-id/196855i991A3EAF0316F716/image-dimensions/659x758?v=1.0)

引用：　https://techcommunity.microsoft.com/t5/identity-standards-blog/ion-booting-up-the-network/ba-p/1441552

 
## IONのgithub

[ION](https://github.com/decentralized-identity/ion)

[ION Installation Guide](https://github.com/decentralized-identity/ion/blob/master/install-guide.md)

[sidetree](https://github.com/decentralized-identity/sidetree)

## インストールの準備（ubuntu 20.04LTS)

```bash
sudo apt install -y snap snapd
sudo apt install build-essential
sudo apt install git curl wget
```

### nodeのインストール

```bash
sudo snap install node --classic --channel=14

npm install typescript
```



### bitcoin testnetノードの構築

```bash
cd ~
wget https://bitcoin.org/bin/bitcoin-core-22.0/bitcoin-22.0-x86_64-linux-gnu.tar.gz
tar xfvz bitcoin-22.0-x86_64-linux-gnu.tar.gz
```

`nano ~.bashrc`

最後に追加

```
PATH="~/bitcoin-22.0/bin":$PATH
```

```bash
source .bashrc
```

#### bitcoin coreの起動テスト

```bash
bitcoind

<ctrl>-c でbitcoind 終了
```

#### bitcoin.conf ファイルの作成

`nano .bitcoin/bitcoin.conf`

IONでは、txindex=1 の指定が必要

##### mainnetの場合

```
mainnet=1
txindex=1
server=1
daemon=1
rpcuser=hoge
rpcpassword=hoge
rpcbind=127.0.0.1
rpcport=8332
fallbackfee=0.0002
```

##### testnetの場合

```
testnet=1
txindex=1
server=1
daemon=1
rpcuser=hoge
rpcpassword=hoge
[test]
rpcbind=127.0.0.1
rpcport=18332
fallbackfee=0.0002
```

bitcoind の起動

```bash
cd ~
bitcoin-core.daemon &
```

IBD の完了まで待つ（testnetの場合２〜３時間、mainnet の場合は1日くらい）


## bitcoin core の名前無しワレットの生成

createwallet で名前付きワレットを作成すると秘密鍵を取り出すことができなくなります。
IONでは、アドレスに対応する秘密鍵で署名できることが必須なので、秘密鍵を取り出せる必要があります。
そのために名前なしワレットを生成します。

```bash
bitcoin-cli -named createwallet wallet_name="" descriptors=false
=>

{
  "name": "",
  "warning": ""
}
```

## IPFSのインストール

インストールし、リポジトリを初期化して、ipfsデーモンを起動する

```bash
wget https://dist.ipfs.io/kubo/v0.14.0/kubo_v0.14.0_linux-amd64.tar.gz  
tar xfvz kubo_v0.14.0_linux-amd64.tar.gz 
cd kubo
sudo ./install.sh

ipfs version
=>
ipfs version 0.14.0

ipfs init
=>
generating ED25519 keypair...done
peer identity: 12D3KooWB1kFBf6EQo6PhT3EhZvQs25YxqtEbTa9xXmgQRJXiG3b
initializing IPFS node at /home/yamasaki/.ipfs
to get started, enter:

	ipfs cat /ipfs/QmQPeNsJPyVWPFDVHb77w8G42Fvo15z4bG2X8D2GhfbSXc/readme
	
ipfs cat /ipfs/QmQPeNsJPyVWPFDVHb77w8G42Fvo15z4bG2X8D2GhfbSXc/readme
=>
Hello and Welcome to IPFS!

██╗██████╗ ███████╗███████╗
██║██╔══██╗██╔════╝██╔════╝
██║██████╔╝█████╗  ███████╗
██║██╔═══╝ ██╔══╝  ╚════██║
██║██║     ██║     ███████║
╚═╝╚═╝     ╚═╝     ╚══════╝

If you're seeing this, you have successfully installed
IPFS and are now interfacing with the ipfs merkledag!

 -------------------------------------------------------
| Warning:                                              |
|   This is alpha software. Use at your own discretion! |
|   Much is missing or lacking polish. There are bugs.  |
|   Not yet secure. Read the security notes for more.   |
 -------------------------------------------------------

Check out some of the other files in this directory:

  ./about
  ./help
  ./quick-start     <-- usage examples
  ./readme          <-- this file
  ./security-notes

```

##### IPFSディレクトリ

```bash
cd ~
ls -a
=>
.              .. 
...
 .ipfs    
...
```

##### IPFSネットワークへの接続

```bash
ipfs daemon &

# 接続状態を確認
ipfs swarm peers
```

[IPFS](../IPFS/IPFS.md)

## MongoDBのインストール

```bash
cd ~
sudo apt install curl
curl -fsSL https://www.mongodb.org/static/pgp/server-4.4.asc | sudo apt-key add -
=>
OK

echo "deb [ arch=amd64,arm64 ] https://repo.mongodb.org/apt/ubuntu focal/mongodb-org/4.4 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-4.4.list

sudo apt update
sudo apt install -y mongodb-org

sudo systemctl start mongod.service
sudo systemctl status mongod
=>
● mongod.service - MongoDB Database Server
     Loaded: loaded (/lib/systemd/system/mongod.service; disabled; vendor preset: enabled)
     Active: active (running) since Fri 2022-07-29 15:53:52 JST; 14s ago
       Docs: https://docs.mongodb.org/manual
   Main PID: 54310 (mongod)
     Memory: 61.0M
     CGroup: /system.slice/mongod.service
             └─54310 /usr/bin/mongod --config /etc/mongod.conf



sudo systemctl enable mongod

mongo --eval 'db.runCommand({ connectionStatus: 1 })'
=>
MongoDB shell version v4.4.15
connecting to: mongodb://127.0.0.1:27017/?compressors=disabled&gssapiServiceName=mongodb
Implicit session: session { "id" : UUID("60ea1ef9-1359-46a9-a422-39d85d38fa30") }
MongoDB server version: 4.4.15
{
	"authInfo" : {
		"authenticatedUsers" : [ ],
		"authenticatedUserRoles" : [ ]
	},
	"ok" : 1
}

```

[MongoDB](../mongoDB/mongoDB.md)


## IONのインストール

[ION Installation Guide](https://github.com/decentralized-identity/ion/blob/master/install-guide.md)

```bash
git clone https://github.com/decentralized-identity/ion

cd ion
```

## ION用のbitcoin 署名鍵生成

```bash
# アドレス生成
bitcoin-cli getnewaddress
=>
tb1quvmg9smzp4rc3mxpetqnacuhfrz446xhluk2un

# アドレスの秘密鍵
bitcoin-cli dumpprivkey tb1quvmg9smzp4rc3mxpetqnacuhfrz446xhluk2un
=>
cSmuSnPQqRtTdrRCZidGJo1rkyTcaAAhcj4kXuaTanbCucE2xqYq
```


### bitcoin とMongoDBとの連携

testnetの場合

* MongoDBのポート番号　27017
* bitcoinDataDirectory: /home/yamasaki/.bitcoin
* bitcoinPeerUri: http://localhost:18332
* bitcoinWalletOrImportString: <アドレスの秘密鍵>
* mongoDbConnectionString: mongodb://localhost:27017
* bitcoinRpcUsername: "hoge"
* bitcoinRpcPassword: "hoge"

mainnetの場合

* MongoDBのポート番号　27017
* bitcoinDataDirectory: /home/yamasaki/.bitcoin
* bitcoinPeerUri: http://localhost:8332
* bitcoinWalletOrImportString: <アドレスの秘密鍵>
* mongoDbConnectionString: mongodb://localhost:27017
* bitcoinRpcUsername: "hoge"
* bitcoinRpcPassword: "hoge"


### IONの設定ファイル

テンプレートを編集する(testnetの場合)

```bash
nano json/testnet-bitcoin-config.json
```

```json
{
  "bitcoinDataDirectory": "/home/yamasaki/.bitcoin",
  "bitcoinFeeSpendingCutoffPeriodInBlocks": 1,
  "bitcoinFeeSpendingCutoff": 0.001,
  "bitcoinPeerUri": "http://localhost:18332",
  "bitcoinRpcUsername": "hoge",
  "bitcoinRpcPassword": "hoge",
  "bitcoinWalletOrImportString": "cSmuSnPQqRtTdrRCZidGJo1rkyTcaAAhcj4kXuaTanbCucE2xqYq",
  "databaseName": "ion-testnet-bitcoin",
  "genesisBlockNumber": 1900000,
  "logRequestError": true,
  "mongoDbConnectionString": "mongodb://localhost:27017/",
  "port": 3002,
  "sidetreeTransactionFeeMarkupPercentage": 1,
  "sidetreeTransactionPrefix": "ion:",
  "transactionPollPeriodInSeconds": 60,
  "valueTimeLockUpdateEnabled": false,
  "valueTimeLockAmountInBitcoins": 0,
  "valueTimeLockPollPeriodInSeconds": 600,
  "valueTimeLockTransactionFeesAmountInBitcoins": 0.0001,
  "didMethodName": "ion:test"
}
```

`nano json/testnet-core-config.json`

修正なし

```json
{
  "batchingIntervalInSeconds": 600,
  "blockchainServiceUri": "http://127.0.0.1:3002",
  "databaseName": "ion-testnet-core",
  "didMethodName": "ion:test",
  "ipfsHttpApiEndpointUri": "http://127.0.0.1:5001",
  "maxConcurrentDownloads": 20,
  "mongoDbConnectionString": "mongodb://localhost:27017/",
  "observingIntervalInSeconds": 60,
  "port": 3000
}

```

#### .tsファイルの確認

```bash
cd ~/ion/src
ls
=>
bitcoin.ts  core.ts

cat bitcoin.ts
cat core.ts
```

### IONのビルド

```bash
cd ~/ion
npm install
=>
npm WARN read-shrinkwrap This version of npm is compatible with lockfileVersion@1, but package-lock.json was generated for lockfileVersion@2. I'll try to do my best with it!
npm WARN lifecycle The node binary used for scripts is /snap/bin/node but npm is using /snap/node/6331/bin/node itself. Use the `--scripts-prepend-node-path` option to include the path for the node binary npm was executed with.

> secp256k1@4.0.3 install /home/yamasaki/ion/node_modules/secp256k1
> node-gyp-build || exit 0


> husky@1.3.1 install /home/yamasaki/ion/node_modules/husky
> node husky install

husky > setting up git hooks
husky > done

> es5-ext@0.10.61 postinstall /home/yamasaki/ion/node_modules/es5-ext
>  node -e "try{require('./_postinstall')}catch(e){}" || exit 0

npm WARN optional SKIPPING OPTIONAL DEPENDENCY: fsevents@1.2.13 (node_modules/fsevents):
npm WARN notsup SKIPPING OPTIONAL DEPENDENCY: Unsupported platform for fsevents@1.2.13: wanted {"os":"darwin","arch":"any"} (current: {"os":"linux","arch":"x64"})
npm WARN optional SKIPPING OPTIONAL DEPENDENCY: fsevents@2.3.2 (node_modules/gulp-nunjucks-render/node_modules/fsevents):
npm WARN notsup SKIPPING OPTIONAL DEPENDENCY: Unsupported platform for fsevents@2.3.2: wanted {"os":"darwin","arch":"any"} (current: {"os":"linux","arch":"x64"})

added 1092 packages from 697 contributors and audited 1089 packages in 23.047s

91 packages are looking for funding
  run `npm fund` for details

found 5 vulnerabilities (2 moderate, 3 high)
  run `npm audit fix` to fix them, or `npm audit` for details



# ビルド
npm config set scripts-prepend-node-path true

npm run build
=>
> ion@1.0.4 build /home/yamasaki/ion
> tsc && copyfiles "json/**/*.json" dist && copyfiles "tests/**/*.json" dist && copyfiles "tests/**/*.js*" dist
```

##  ION Bitcoin microserviceの実行

環境変数の設定

`nano ~/.bashrc`

```
...

export ION_CORE_CONFIG_FILE_PATH="/home/yamasaki/ion/json/testnet-core-config.json"
export ION_CORE_VERSIONING_CONFIG_FILE_PATH="/home/yamsaki/ion/json/testnet-core-versioning.json"
```

```bash
cd ~/ion
npm run bitcoin

=>

> ion@1.0.4 bitcoin /home/yamasaki/ion
> node dist/src/bitcoin.js

Environment variable ION_BITCOIN_CONFIG_FILE_PATH undefined, using default path ../json/testnet-bitcoin-config.json instead.
Environment variable ION_BITCOIN_VERSIONING_CONFIG_FILE_PATH undefined, using default ION bitcoin versioning config path ../json/testnet-bitcoin-versioning.json.
Creating bitcoin wallet using the import string passed in.
Sidetree bitcoin service configuration:
(node:62455) Warning: Accessing non-existent property 'MongoError' of module exports inside circular dependency
(Use `node --trace-warnings ...` to show where the warning was created)
(node:62455) DeprecationWarning: current Server Discovery and Monitoring engine is deprecated, and will be removed in a future version. To use the new Server Discover and Monitoring engine, pass option { useUnifiedTopology: true } to the MongoClient constructor.
CommandSucceededEvent {
  connectionId: 'localhost:27017',
  requestId: 1,
  commandName: 'listCollections',
  duration: 3,
  reply: {
    cursor: {
      id: 0,
      ns: 'ion-testnet-bitcoin.$cmd.listCollections',
      firstBatch: [Array]
    },
    ok: 1
  }
}
Collection 'service' found.
CommandSucceededEvent {
  connectionId: 'localhost:27017',
  requestId: 2,
  commandName: 'listCollections',
  duration: 1,
  reply: {
    cursor: {
      id: 0,
      ns: 'ion-testnet-bitcoin.$cmd.listCollections',
      firstBatch: [Array]
    },
    ok: 1
  }
}
Collection 'blocks' found.
CommandSucceededEvent {
  connectionId: 'localhost:27017',
  requestId: 3,
  commandName: 'listCollections',
  duration: 1,
  reply: {
    cursor: {
      id: 0,
      ns: 'ion-testnet-bitcoin.$cmd.listCollections',
      firstBatch: [Array]
    },
    ok: 1
  }
}
Collection 'transactions' found.
Sending jRPC request: id: 72620v1b99t, method: createwallet
Wallet with name sidetreeDefaultWallet already exists.
Sending jRPC request: id: 5tcfm3qlldl, method: loadwallet
Wallet with name sidetreeDefaultWallet already loaded.
Checking if bitcoin contains a wallet for n2ELz5MWZgoB7Qi3SEHxHKCJUneC5dqX74
Checking if bitcoin wallet for n2ELz5MWZgoB7Qi3SEHxHKCJUneC5dqX74 exists
Sending jRPC request: id: 6830nit5mqn, method: getaddressinfo
Configuring bitcoin peer to watch address n2ELz5MWZgoB7Qi3SEHxHKCJUneC5dqX74. This can take up to 10 minutes.
Sending jRPC request: id: hoagcislik, method: importpubkey


```

## ION core サービスの起動

bitcoind が動いていることが前提です


```bash
cd ~/ion
npm run core &

=>
> ion@1.0.4 core /home/yamasaki/ion
> node dist/src/core.js

Environment variable ION_CORE_CONFIG_FILE_PATH undefined, using default core config path ../json/testnet-core-config.json instead.
Environment variable ION_CORE_VERSIONING_CONFIG_FILE_PATH undefined, using default core versioning config path ../json/testnet-core-versioning.json instead.
(node:62608) Warning: Accessing non-existent property 'MongoError' of module exports inside circular dependency
(Use `node --trace-warnings ...` to show where the warning was created)
(node:62608) DeprecationWarning: current Server Discovery and Monitoring engine is deprecated, and will be removed in a future version. To use the new Server Discover and Monitoring engine, pass option { useUnifiedTopology: true } to the MongoClient constructor.
CommandSucceededEvent {
  connectionId: 'localhost:27017',
  requestId: 1,
  commandName: 'listCollections',
  duration: 3,
  reply: {
    cursor: {
      id: 0,
      ns: 'ion-testnet-core.$cmd.listCollections',
      firstBatch: []
    },
    ok: 1
  }
}
Collection 'service' does not exists, creating...
CommandSucceededEvent {
  connectionId: 'localhost:27017',
  requestId: 2,
  commandName: 'create',
  duration: 13,
  reply: { ok: 1 }
}


...


Fetching Sidetree transactions from blockchain service...
Fetching URI 'http://127.0.0.1:3002/transactions'...
Event emitted: sidetree_observer_loop_failure
Encountered unhandled and possibly fatal Observer error, must investigate and fix:
FetchError: request to http://127.0.0.1:3002/transactions failed, reason: connect ECONNREFUSED 127.0.0.1:3002
    at ClientRequest.<anonymous> (/home/yamasaki/ion/node_modules/node-fetch/lib/index.js:1491:11)
    at ClientRequest.emit (events.js:400:28)
    at Socket.socketErrorListener (_http_client.js:475:9)
    at Socket.emit (events.js:400:28)
    at emitErrorNT (internal/streams/destroy.js:106:8)
    at emitErrorCloseNT (internal/streams/destroy.js:74:3)
    at processTicksAndRejections (internal/process/task_queues.js:82:21) {
  type: 'system',
  errno: 'ECONNREFUSED',
  code: 'ECONNREFUSED'
}
Waiting for 60 seconds before fetching and processing transactions again.
```

しばらく我慢してまっていると同期が始まります。

