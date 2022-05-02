# Tapyrus core v0.5.1 testnet ノード構築

2022/05/01更新

#### 環境は ubuntu 20.04LTS

事前に必要そうなライブラリをインストールしておくとトラブルを避けることができます

```bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y ssh openssl llvm autoconf gcc clang cmake make curl direnv sqlite3 libsqlite3-dev git nodejs m4 libssl-dev zlib1g-dev cmdtest build-essential wget curl imagemagick libreadline-dev libleveldb-dev
sudo apt autoremove -y
```

### Tapyrus core

githubのサイト
* [https://github.com/chaintope/tapyrus-core](https://github.com/chaintope/tapyrus-core)

Tapyrusの利用方法
* [https://github.com/chaintope/tapyrus-core/blob/master/doc/tapyrus/getting_started.md](https://github.com/chaintope/tapyrus-core/blob/master/doc/tapyrus/getting_started.md)

bitcoin coreからのフォークなので、基本的にbitcoin coreの知識が使えます

#### Tapyrusのインストール関連情報

次のブログが参考になります

* [Tapyrus Coreをdevモードで起動する 〜Ubuntu編〜](https://zenn.dev/shmn7iii/articles/6dbdf6394a44c1)
* [Tapyrusネットワークを構築してみる その①](https://zenn.dev/shmn7iii/articles/ff647417bfdf16)
* [Tapyrus Coreをdevモードで構築する 〜Docker編〜](https://zenn.dev/shmn7iii/articles/53e533e2ac3756)
* [Tapyrus Core v0.5.0を触ってみる](https://zenn.dev/shmn7iii/articles/65318fa90432f0)

#### Tapyurusの特徴

* フェデレーション(連邦)型のブロックチェーン
    用途ごとに新たにブロックチェーンネットワークを生成して利用します
    したがってネットワークごとにジェネシスブロック（0番目の最初のブロック）が存在します。
* ネイティブなカラードコイン（トークン）生成ができる
    bitcoin でも Ethereumでもトークンはスマートコントラクトによるアプリケーションの一つなので、駆動させるためにはネイティブなbitcoinやEtherが必要です。
* Proof of Authrity型のブロックチェーンで、ブロックの生成は signer networkの信頼点の合意の下に実行されます。

##### Tapyrusネットワークのパラメータ

* ネットワークID
   フェデレーション（連邦）型のTapyrusネットワークでネットワークをスタートするために必要な番号です。
   すべてのTapyrusネットワークは、Tapyrusネットワークに生成されたトラフィックの衝突を回避するために、それぞれ異なるネットワークIDを使用します。

* ネットワークマジックバイト
    マジックバイトは、Tapyrusネットワーク上のノード間で送信されるメッセージを識別する方法として使用されます。マジックバイトは、ネットワークIDから導出されます

* ジェネシスブロック
    Tapyrusでは、bitcoin と異なり、ジェネシスブロックはコードに埋め込まれていません。（bitcoin では、Satosh Nakamotoがジェネシスブロックを生成し、bitcoin のコードに埋め込んでいます）
    Tapyrusのジェネシスブロックは、Tapyrus-genesisと呼ばれるユーティリティを使用して生成され、さらにTapyrus署名者たちによって署名されて正統化されます。そして外部ファイルとして保管されます。
    Tapyrusネットワークには、それぞれに固有のジェネシスブロックがあります。

* 公開鍵の集約
    ジェネシスブロックには、「集約公開鍵」が含まれています。
    これは、検証可能な秘密共有スキームを使用して、開始時にTapyrus署名者のネットワークによって生成されます。
    この集約公開鍵は、各Tapyrusコアノードがブロックプルーフのしきい値署名を検証し、ブロックを受け入れるために使用されます。

## バイナリからの tapyrus coreのインストール

#### バイナリの入手

taapyrus core v 0.5.1 のx86 64 linux用のバイナリは以下から入手可能です
[v0.5.1 release](https://github.com/chaintope/tapyrus-core/releases/)

[tapyrus-core-0.5.1-x86_64-linux-gnu.tar.gz](https://github.com/chaintope/tapyrus-core/releases/download/v0.5.1/tapyrus-core-0.5.1-x86_64-linux-gnu.tar.gz)

```bash
cd ~
wget https://github.com/chaintope/tapyrus-core/releases/download/v0.5.1/tapyrus-core-0.5.1-x86_64-linux-gnu.tar.gz
```
tapyrus core v0.5.1 のx86 64 linuxバイナリが圧縮ファイルでダウンロードされているはずです

```bash
ls
=>
...  tapyrus-core-0.5.1-x86_64-linux-gnu.tar.gz ...
```

tarコマンドによる圧縮ファイルの解凍

```bash
tar xfvz tapyrus-core-0.5.1-x86_64-linux-gnu.tar.gz
```

これで、新しいディレクトリができているはずです

```bash
ls
=>
... tapyrus-core-0.5.1  ...
```

このディレクトリのbinというサブディレクトリに tapyrus coreのバイナリがあります

```bash
cd tapyrus-core-0.5.1
ls
=>
bin  include  lib  share

cd bin
ls
=>
tapyrus-cli  tapyrus-genesis  tapyrus-qt  tapyrus-tx  tapyrusd  test_tapyrus
```

#### ダウンロードしたバイナリにパスを通す

ログインスクリプトで環境変数 PATHにダウンロードしたバイナリへのパスを追加する

```bash
cd ~
nano .bashrc
```

ファイルの最後に次の行を追加して保存

```bash
...
export PATH="$HOME/tapyrus-core-0.5.1/bin:$PATH"
```

ログインスクリプトを評価してパスを有効化する

```bash
source .bashrc
```

#### tapyrusd を起動

tapyrusd （ノードのデーモン）を起動してみる

しかし、すぐに終了してしまいます。

```bash
tapyrusd
=>
2022-03-25T03:03:39Z Tapyrus Core version v0.5.0 (release build)
2022-03-25T03:03:39Z InitParameterInteraction: parameter interaction: -whitelistforcerelay=1 -> setting -whitelistrelay=1
2022-03-25T03:03:39Z Validating signatures for all blocks.
2022-03-25T03:03:39Z Using the 'sse4(1way),sse41(4way)' SHA256 implementation
2022-03-25T03:03:39Z Default data directory /home/yamasaki/.tapyrus
2022-03-25T03:03:39Z Using data directory /home/yamasaki/.tapyrus/prod-1
2022-03-25T03:03:39Z Using config file /home/yamasaki/.tapyrus/tapyrus.conf
2022-03-25T03:03:39Z Using at most 125 automatic connections (1024 file descriptors available)
2022-03-25T03:03:39Z Reading Genesis Block from [/home/yamasaki/.tapyrus/genesis.dat]
Error: ReadGenesisBlock: unable to read genesis file "/home/yamasaki/.tapyrus/genesis.dat"
2022-03-25T03:03:39Z Shutdown: In progress...
2022-03-25T03:03:39Z Shutdown: done
```

tapyurusd の作業ディレクトリ `.tapyrus`  が作成されているはずです

```bash
ls -a
=>
...  .tapyrus...
```

#### 設定ファイル `tapyrus.conf` の作成

```bash
cd ~/.tapyrus
nano tapyrus.conf
```

##### tapyrus.conf の内容

コインを送金するにはfallbackfeeを設定しておく必要がある

```
networkid=1939510133
txindex=1
server=1
rest=1
fallbackfee=0.000001
rpcuser="hoge"
rpcpassword="hoge"
rpcbind=0.0.0.0
rpcallowip=128.0.0.1
addseeder=static-seed.tapyrus.dev.chaintope.com
```

### genesisブロックのファイル

```bash
cd ~/.tapyrus
nano genesis.1939510133
```

nano genesis.1939510133 の内容

```bash
01000000000000000000000000000000000000000000000000000000000000000000000044cc181bd0e95c5b999a13d1fc0d193fa8223af97511ad2098217555a841b3518f18ec2536f0bb9d6d4834fcc712e9563840fe9f089db9e8fe890bffb82165849f52ba5e01210366262690cbdf648132ce0c088962c6361112582364ede120f3780ab73438fc4b402b1ed9996920f57a425f6f9797557c0e73d0c9fbafdebcaa796b136e0946ffa98d928f8130b6a572f83da39530b13784eeb7007465b673aa95091619e7ee208501010000000100000000000000000000000000000000000000000000000000000000000000000000000000ffffffff0100f2052a010000002776a92231415132437447336a686f37385372457a4b6533766636647863456b4a74356e7a4188ac00000000
```

### tapyrusd の起動

```bash
tapyrusd &
```

ブロックチェーンの初期同期が完了するまで待つ

### tapyrus クライアントの実行

* help

```bash
tapyrus-cli help
```

* getblockchaininfo

```bash
tapyrus-cli getblockchaininfo
=>
{
  "chain": "1939510133",
  "mode": "prod",
  "blocks": 182928,
  "headers": 182928,
  "bestblockhash": "f5a5b21d578aa61a77254564622ecba7fe10894efa24e0af973cd6a27bf1586a",
  "mediantime": 1648174439,
  "verificationprogress": 1,
  "initialblockdownload": false,
  "size_on_disk": 56917539,
  "pruned": false,
  "aggregatePubkeys": [
    {
      "0366262690cbdf648132ce0c088962c6361112582364ede120f3780ab73438fc4b": 0
    }
  ],
  "warnings": ""
}

```

* networkinfo

```bash
tapyrus-cli getnetworkinfo
=>
{
  "version": 50000,
  "subversion": "/Tapyrus Core:0.5.0/",
  "protocolversion": 10000,
  "localservices": "0000000000000405",
  "localrelay": true,
  "timeoffset": 0,
  "networkactive": true,
  "connections": 3,
  "networks": [
    {
      "name": "ipv4",
      "limited": false,
      "reachable": true,
      "proxy": "",
      "proxy_randomize_credentials": false
    },
    {
      "name": "ipv6",
      "limited": false,
      "reachable": true,
      "proxy": "",
      "proxy_randomize_credentials": false
    },
    {
      "name": "onion",
      "limited": true,
      "reachable": false,
      "proxy": "",
      "proxy_randomize_credentials": false
    }
  ],
  "relayfee": 0.00001000,
  "incrementalfee": 0.00001000,
  "localaddresses": [
  ],
  "warnings": ""
}

```

* ジェネシスブロックの確認

```bash
tapyrus-cli getblockhash 0
=> 
038b114875c2f78f5a2fd7d8549a905f38ea5faee6e29a3d79e547151d6bdd8a

tapyrus-cli getblock 038b114875c2f78f5a2fd7d8549a905f38ea5faee6e29a3d79e547151d6bdd8a
=>
{
  "hash": "038b114875c2f78f5a2fd7d8549a905f38ea5faee6e29a3d79e547151d6bdd8a",
  "confirmations": 182930,
  "strippedsize": 304,
  "size": 304,
  "weight": 1216,
  "height": 0,
  "features": 1,
  "featuresHex": "00000001",
  "merkleroot": "51b341a85575219820ad1175f93a22a83f190dfcd1139a995b5ce9d01b18cc44",
  "immutablemerkleroot": "846521b8ff0b89fee8b99d089ffe403856e912c7fc34486d9dbbf03625ec188f",
  "tx": [
    "846521b8ff0b89fee8b99d089ffe403856e912c7fc34486d9dbbf03625ec188f"
  ],
  "time": 1589269151,
  "mediantime": 1589269151,
  "xfieldType": 1,
  "xfield": "0366262690cbdf648132ce0c088962c6361112582364ede120f3780ab73438fc4b",
  "proof": "2b1ed9996920f57a425f6f9797557c0e73d0c9fbafdebcaa796b136e0946ffa98d928f8130b6a572f83da39530b13784eeb7007465b673aa95091619e7ee2085",
  "nTx": 1,
  "nextblockhash": "53fd2c341e5829672a5c4c4b461ce4f3ec75101d451df2ccef11c5087b8c0381"
}
```


#### 自分のアドレスを生成

```bash
tapyrus-cli getnewaddress bob
=>
13dsUJTSCACmpr7PCQsGQiUWN3Ziu38oVA
```

## Faucetからコインを得る

[`https://testnet-faucet.tapyrus.dev.chaintope.com`](https://testnet-faucet.tapyrus.dev.chaintope.com)

★　503 Service Temporarily Unavailable　と出ることがありますが、辛抱強くアクセスを繰り返してください。反応も非常に遅いですが、待ちましょう。

生成した自分のアドレスをサイトに入れて「Get coins!」をクリック。
これの反応も非常に遅いですが、待ちましょう。

同じグローバルIPアドレスからは1日に1回だけしかコインを受け取ることができません

#### 残高を確認

10分以上経過後

```bash
tapyrus-cli getbalance
=>
0.00060566
```

#### UTXOを確認

```bash
tapyrus-cli listunspent
=>
[
  {
    "txid": "e410782f14ccd6f8974ad308e4e3764e448ae25b7d6b3a5a0af0ab8b87b33877",
    "vout": 1,
    "address": "13dsUJTSCACmpr7PCQsGQiUWN3Ziu38oVA",
    "token": "TPC",
    "amount": 0.00060566,
    "label": "alice",
    "scriptPubKey": "76a9141ce95b6563d49d95b7ff9f0b7345a798ff47f53588ac",
    "confirmations": 7,
    "spendable": true,
    "solvable": true,
    "safe": true
  }
]

```

## 送金

新しいアドレスを生成

```bash
tapyrus-cli getnewaddress alice
=>
1GUrWJ1sGeYYUS8fu8GP6ad7jPyADya2Fb

tapyrus-cli getnewaddress bob
=>
19duDdmEvnPTsFcL9P8cQh1idf2TRNuzru

tapyrus-cli getnewaddress carol
=>
14qtfhPHArQr9mahPutysQA7Y9Nqjf4y4N

tapyrus-cli getnewaddress david
=>
1ph4Yq54zdQ4UKwjdfrkw8zamBa5Kpe4N

tapyrus-cli getnewaddress eve
=>
19BsKCX5Yi59w7m3yi6ipCsRqQXoc7sS2j

tapyrus-cli getnewaddress franck
=>
1CP8WGaNd9xZYTJ7j6HJUz79kHfgX3pMh1
```

新しいアドレスへ送金

```bash
tapyrus-cli sendtoaddress 14qtfhPHArQr9mahPutysQA7Y9Nqjf4y4N 0.0003
=> 
9a5a34640f3d6a89ee1365375bd94cc0ed68a251d751edf9fe7940523ff37195
```

```bash
tapyrus-cli listunspent
[
  {
    "txid": "9a5a34640f3d6a89ee1365375bd94cc0ed68a251d751edf9fe7940523ff37195",
    "vout": 0,
    "address": "14qtfhPHArQr9mahPutysQA7Y9Nqjf4y4N",
    "token": "TPC",
    "amount": 0.00030000,
    "label": "carol",
    "scriptPubKey": "76a9142a27791d64b5707d307eef20349b7d8deb9f616088ac",
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "safe": true
  },
  {
    "txid": "9a5a34640f3d6a89ee1365375bd94cc0ed68a251d751edf9fe7940523ff37195",
    "vout": 1,
    "address": "1ABDsodqkn2pM2enKN874UDKVjZYmRmctm",
    "token": "TPC",
    "amount": 0.00030341,
    "scriptPubKey": "76a91464a80a3aba8baa4ff710d37a09af3f127007701c88ac",
    "confirmations": 1,
    "spendable": true,
    "solvable": true,
    "safe": true
  }
]
```

## トークン

tapyrusには、BCHのOP_GROUP オペコードに類似したカラードコイン機能があります

[Tapyrusのトークンの仕様](https://github.com/chaintope/tapyrus-core/blob/v0.5.0/doc/tapyrus/colored_coin.md)

トークンの発行、移転、焼却の基本機能があります。

[Tapyrus上でTPC（tapyrus）以外のトークンをサポートするための仕様](https://gist.github.com/azuchi/8fff51de048c6fced31531544583fff2)

### トークンの発行

`issuetoken <token_type> <amount> <txid> <index>`

<token_type>は Reissuable Token は 1, Non-Reissuable Tokenは 2, NFTは 3 です。
<amount>はトークンの発行枚数。
<txid> <index>は、発行のために使用するUTXO

ただし、Reissuable Tokenの場合は、scriptPubKey

#### Non-Reissuable Token の発行

```bash
tapyrus-cli issuetoken 2 1000 3ccfbe4a213545e6bd53fffedac0b56deab168c90f7f4e5beab2713673512e7a 0
=>
{
  "color": "c2866471e5a47fd54afe755674211615f358aa0535e825266876ad81b95a326271",
  "txid": "2b617b6b660055917476d89d634983fdd87c0c8e6bce0d7d19c12e3612d1367b"
}

```

#### NFT の発行

```bash
tapyrus-cli issuetoken 3 1 3ccfbe4a213545e6bd53fffedac0b56deab168c90f7f4e5beab2713673512e7a 1
=>
{
  "color": "c342f902ff12fc1a0041ee9715c2fe327760b55b65abc34be2807a6d5cf1adc2c1",
  "txid": "72fbae441fcc31f36eb23b6363e3d3c96e530eaf58340205c6398a8f21a99544"
}

```

#### Reissuable Token の発行

使用するUTXOのscriptPubKeyを引数に指定する

```bash
tapyrus-cli issuetoken 1 100 76a9142226bf3bb2909e001dfd0fbc2a0d9e045382f6b988ac
=>
{
  "color": "c18052819dfb017498f89e82cb233073c4b6fabf20605e2ab9ef021eb008a15fd5",
  "txids": [
    "49294c2deb2fd7af096f2eaa8783fcf2df65abe2f9e2a772dfc0ca1fb890b346",
    "c4e1f16129f1df138bed0b6b0e0acb10278e054d1d0f8d484b1ff0f2ce84a524"
  ]
}
```



#### Reissuable Token の再発行

reissuetoken <color> <value>

```bash
tapyrus-cli reissuetoken c1f11a9a709939d650c2df0b30ec7d566bc8ef1ecc3d1e954f1e3fb6387e0e7082 100
=>
{
  "color": "c1f11a9a709939d650c2df0b30ec7d566bc8ef1ecc3d1e954f1e3fb6387e0e7082",
  "txids": [
    "d5e0be30cb95be6ad9578c67a5349d9e4aafb940338a1faacee9d851056bc44f",
    "def9453068265c5b33d267201a4425befbf25636c8683eba50816982526fb47e"
  ]
}
```

### トークン発行トランザクションの内容の確認

#### Non-Reissuable Token

txid を指定する

 "token"フィールドの値が "color"になっている

```bash
tapyrus-cli gettransaction 99050cb3c0f82ca6158463b60748cb7f2f1bd6e277850ba6e94b981f1729a8ab
=>
{
  "token": "TPC",
  "amount": 0.00000000,
  "fee": -0.00000258,
  "confirmations": 0,
  "trusted": false,
  "txid": "99050cb3c0f82ca6158463b60748cb7f2f1bd6e277850ba6e94b981f1729a8ab",
  "walletconflicts": [
  ],
  "time": 1648197686,
  "timereceived": 1648197686,
  "bip125-replaceable": "unknown",
  "details": [
    {
      "address": "4Zpvh7dZpV34UA8DiFsVGyoxFUjykpKNyrCpinpSzY3CGVH9xCCMERgU3gox9g1VR8pZgnzekn9ZhF1",
      "category": "receive",
      "token": "c24ea64767f57d27f9689402954fbc6ceecd90ae6c7375836817a3b562e1248411",
      "amount": 10,
      "vout": 0
    }
  ],
  "hex": "01000000019571f33f524079fef9ed51d751a268edc04cd95b376513ee896a3d0f64345a9a000000006a47304402201ee3c85a4c401adbb8fb03161af491ae8c8dc13a8a8bbc1442ff1705f2eff15b022018be6393382eb7d53a33b51a6b22867bc96608e0d480339d58b4af3cf46ec323012103c5272e78192ce8ad269e833351954a79bd8cabe3adf5f9281c2039770644d282feffffff020a000000000000003a21c24ea64767f57d27f9689402954fbc6ceecd90ae6c7375836817a3b562e1248411bca9141d54178b8c3926e539c5c4da24ac1d6ee33f5d36872e740000000000001976a91466cb5f3072b595ea936c214810fbc3486a0ddbec88acadca0200"
}
```

#### NFT

 "token"フィールドの値が "color"になっている
 
```bash
tapyrus-cli gettransaction 54a0f24a416c2358fd0b5b8f0a2b2b7798f95a038cce170b728ee562f833e65d
=>
{
  "token": "TPC",
  "amount": 0.00000000,
  "fee": -0.00000258,
  "confirmations": 0,
  "trusted": false,
  "txid": "54a0f24a416c2358fd0b5b8f0a2b2b7798f95a038cce170b728ee562f833e65d",
  "walletconflicts": [
  ],
  "time": 1648197848,
  "timereceived": 1648197848,
  "bip125-replaceable": "unknown",
  "details": [
    {
      "address": "4ZyawFZ7vwoaVRSQXBYdzaFX8FnPmE1mL4tkqfxq2iaHezk5nnnvQtTW95vq5RaUUZbM1DdwmYpvn8k",
      "category": "receive",
      "token": "c38e674bf2613870ead5a3d52ca81ac63903dfbe9960410ae93e47f2031e72bddb",
      "amount": 1,
      "vout": 0
    }
  ],
  "hex": "01000000019571f33f524079fef9ed51d751a268edc04cd95b376513ee896a3d0f64345a9a010000006a47304402205f3fd0d92c441a3520a236ed9e39322e4ddc23a296f6a4d52b6bcea795d3ec640220039682da04fd531709f6b8738c84bdb6db896c7f148dac5ffe2a4bf481e51011012103d833c45553d5d196a46ed1e13a56f068503692bfaa3669c91878bbaf836dd0a3feffffff0201000000000000003a21c38e674bf2613870ead5a3d52ca81ac63903dfbe9960410ae93e47f2031e72bddbbca9144eee5a9d7cb4bf39777ecacc1f98092e1c93a8828783750000000000001976a914c5599868c9c2b57a429e90c5a8acac1603406a6188acadca0200"
```

#### Reissuable Token

出力されるトランザクションIDが2つある

1つ目のトランザクションの "token"フィールドの値は TPC
2つ目のトランザクションの "token"フィールドの値が "color"になっている


```bash
tapyrus-cli gettransaction def9453068265c5b33d267201a4425befbf25636c8683eba50816982526fb47e
{
  "token": "TPC",
  "amount": 0.00000000,
  "fee": -0.00000258,
  "confirmations": 0,
  "trusted": false,
  "txid": "def9453068265c5b33d267201a4425befbf25636c8683eba50816982526fb47e",
  "walletconflicts": [
  ],
  "time": 1648201151,
  "timereceived": 1648201151,
  "bip125-replaceable": "unknown",
  "details": [
    {
      "address": "4ZnPkx2PQ2neLZ2abhpMfSym3B1V1bxGuAbUKvkPENerxMkMJTtKjEg3szPwcWVDaWPDHU2wymydBUW",
      "category": "receive",
      "token": "c1f11a9a709939d650c2df0b30ec7d566bc8ef1ecc3d1e954f1e3fb6387e0e7082",
      "amount": 100,
      "label": "",
      "vout": 0
    }
  ],
  "hex": "01000000014fc46b0551d8e9ceaa1f8a3340b9af4a9e9d34a5678c57d96abe95cb30bee0d5000000006a4730440220798d03646bcd42c9d3a6cbc8688fa026974a03de631e7b79b5a9e89ebbbdb0cd022063335bd6a502404eef33c59984b6ea4d5fb2fd10f23ab6660e82998111322f3b012103a8b17425211774a33fc6c2882468b7856d14e53a4c8298c7ec3cd504c6a62e14feffffff0264000000000000003a21c1f11a9a709939d650c2df0b30ec7d566bc8ef1ecc3d1e954f1e3fb6387e0e7082bca9143c39f87dc0eda15a2d852aecf991e9f4063cad59871e4d0000000000001976a9145f49d75ed451946ed5e0bd3337bf2aa511e76e1088acb4ca0200"
}

```

hex 部分を解析してみる


```js
tapyrus-cli decoderawtransaction 01000000014fc46b0551d8e9ceaa1f8a3340b9af4a9e9d34a5678c57d96abe95cb30bee0d5000000006a4730440220798d03646bcd42c9d3a6cbc8688fa026974a03de631e7b79b5a9e89ebbbdb0cd022063335bd6a502404eef33c59984b6ea4d5fb2fd10f23ab6660e82998111322f3b012103a8b17425211774a33fc6c2882468b7856d14e53a4c8298c7ec3cd504c6a62e14feffffff0264000000000000003a21c1f11a9a709939d650c2df0b30ec7d566bc8ef1ecc3d1e954f1e3fb6387e0e7082bca9143c39f87dc0eda15a2d852aecf991e9f4063cad59871e4d0000000000001976a9145f49d75ed451946ed5e0bd3337bf2aa511e76e1088acb4ca0200
{
  "txid": "def9453068265c5b33d267201a4425befbf25636c8683eba50816982526fb47e",
  "hash": "95bb7e3b44cab9dcc046120068743598d0283b084469a1e5ea68cee0e5a8ed05",
  "features": 1,
  "size": 258,
  "locktime": 182964,
  "vin": [
    {
      "txid": "d5e0be30cb95be6ad9578c67a5349d9e4aafb940338a1faacee9d851056bc44f",
      "vout": 0,
      "scriptSig": {
        "asm": "30440220798d03646bcd42c9d3a6cbc8688fa026974a03de631e7b79b5a9e89ebbbdb0cd022063335bd6a502404eef33c59984b6ea4d5fb2fd10f23ab6660e82998111322f3b[ALL] 03a8b17425211774a33fc6c2882468b7856d14e53a4c8298c7ec3cd504c6a62e14",
        "hex": "4730440220798d03646bcd42c9d3a6cbc8688fa026974a03de631e7b79b5a9e89ebbbdb0cd022063335bd6a502404eef33c59984b6ea4d5fb2fd10f23ab6660e82998111322f3b012103a8b17425211774a33fc6c2882468b7856d14e53a4c8298c7ec3cd504c6a62e14"
      },
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "token": "c1f11a9a709939d650c2df0b30ec7d566bc8ef1ecc3d1e954f1e3fb6387e0e7082",
      "value": 100,
      "n": 0,
      "scriptPubKey": {
        "asm": "c1f11a9a709939d650c2df0b30ec7d566bc8ef1ecc3d1e954f1e3fb6387e0e7082 OP_COLOR OP_HASH160 3c39f87dc0eda15a2d852aecf991e9f4063cad59 OP_EQUAL",
        "hex": "21c1f11a9a709939d650c2df0b30ec7d566bc8ef1ecc3d1e954f1e3fb6387e0e7082bca9143c39f87dc0eda15a2d852aecf991e9f4063cad5987",
        "reqSigs": 1,
        "type": "coloredscripthash",
        "addresses": [
          "4ZnPkx2PQ2neLZ2abhpMfSym3B1V1bxGuAbUKvkPENerxMkMJTtKjEg3szPwcWVDaWPDHU2wymydBUW"
        ]
      }
    },
    {
      "token": "TPC",
      "value": 0.00019742,
      "n": 1,
      "scriptPubKey": {
        "asm": "OP_DUP OP_HASH160 5f49d75ed451946ed5e0bd3337bf2aa511e76e10 OP_EQUALVERIFY OP_CHECKSIG",
        "hex": "76a9145f49d75ed451946ed5e0bd3337bf2aa511e76e1088ac",
        "reqSigs": 1,
        "type": "pubkeyhash",
        "addresses": [
          "19gqeonnnWPijwb2TiHs3zWYhii1w6GskQ"
        ]
      }
    }
  ]
}
```

### トークンの焼却

burntoken <color> <value>

#### NFTの焼却

```bash
tapyrus-cli burntoken c342f902ff12fc1a0041ee9715c2fe327760b55b65abc34be2807a6d5cf1adc2c1 1
```

### トークンの送金

`getnewaddress ( <color> )`

`transfertoken <address> <value>`


```
tapyrus-cli getnewaddress "c3b9c4be2f73cd2f9e1c5e072468a76f931a241d4ff4647dfdacdeba28c4bee157"
=>
15SSb9pWzPj5yrd8B7xcK5Ys6Wv6FrTW79

tapyrus-cli transfertoken 15SSb9pWzPj5yrd8B7xcK5Ys6Wv6FrTW79 1
01858822e8b0bae02850560621e009398775974c5ffc0061dcab3dd76f108cf4

```
