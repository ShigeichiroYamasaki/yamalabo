# Ethereum mainnet 構築

2022/02/17 Shigeichiro Yamasaki

## parityのインストール

### ubuntu

```bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y snap
sudo snap install parity
```

### parity の起動（同期開始）

デフォルト設定で同期開始（非常に時間がかかるので高性能のマシンが必要）

```
parity &
```

## Gethのインストール

### ubuntu

```
sudo add-apt-repository -y ppa:ethereum/ethereum
sudo apt-get update
sudo apt-get install ethereum
```

### Gethの起動（同期開始）（研究室のベアボーンマシンで数日）

geth  1.10.15-stable-8be800ff

* snap : 高速同期モード
* http : によるAPIの利用を許可する
* mainnet : メインネットへの接続

```
geth --mainnet --syncmode "snap" --http --http.addr "192.168.0.251" --http.api "eth,net,admin,miner, txpool, web3, personal"
```

ssh で作業していた場合は、nohup でバックグラウンドにしておけばログアウトしても停止しません。

```
nohup geth --mainnet --syncmode "snap" --http --http.addr "192.168.0.251" --http.api "eth,net,admin,miner, txpool, web3, personal" &
```

## JSON RPC による基本操作

* eth：ブロックチェーンの操作
* net：p2pネットワークステータス
* admin：ノードの管理
* miner：マイニング
* txpool：トランザクションメモリプール
* web3：単位変換など
* personal : アカウント管理

### 一般的なコマンド：

* personal_newAccount  (パスワード)：アカウントを作成
* personal_unlockAccount  ()：アカウントのロックを解除
* eth_accounts：システムのアカウントを列挙
* eth_coinbase : コインベースのアカウントの確認
* miner_setEtherbase  (eth_accounts[index]) : コインベースアカウントの変更
* eth_getBalance  () ：アカウントの残高
* eth_blockNumber：ブロックの総数
* eth_getTransaction  () ：トランザクションを取得
* eth_getBlock  ()：ブロックを取得
* eth_sendTransaction  ({from: eth_accounts[source_index], to: eth_accounts[destination_index], value: web3_toWei(amount, "ether")}) : 送金
* eth_getTransactionReceipt("transaction_address") : トランザクションの実行結果（レシート）の確認
* eth_mining : マイニング中か確認
* miner_start  ()：マイニングを開始
* miner_stop  ()：マイニングを停止
* miner_hashrate : マイニングHashrateの確認
* web3_fromWei  ()：WeiをEtherに変換
* web3_toWei  ()：EtherをWeiに変換
* admin_addPeer  ()：他のノードに接続
* net_listening  : 疎通確認
* net_peerCount : 接続されているノード数
* admin_nodeInfo : ノードの情報
* admin_peers : 接続されているノード情報

## 操作

### 同期状態

```
curl --data '{"method":"eth_syncing","params":[],"id":1,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST 192.168.0.251:8545
```

### ブロック高によるブロックヘッダの表示

```
curl --data '{"method":"eth_getBlockByNumber","params":["0x1b4",true],"id":1,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST 192.168.0.251:8545
```
