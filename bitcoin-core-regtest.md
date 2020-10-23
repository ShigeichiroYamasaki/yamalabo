# bitcoin regtest 構築


## bitcoin core インストール MacOSX 

[Bitcoin core ](https://bitcoincore.org/ja/download/)

dmg でパッケージインストール

設定ファイル

```
regtest=1
txindex=1
server=1
rest=1
rpcuser=hoge
rpcpassword=hoge
[regtest]
rpcport=18332
port=18444
fallbackfee=0.0002

```


## bitcoin core インストール ubutu 20.04LTS

### インストールスクリプト

```bash
nano install-bitcoincore-regtest.sh
```


```bash
#!/bin/bash
sudo apt update
sudo apt upgrade -y
sudo apt install snapd
sudo snap install bitcoin-core
cd ~snap/bitcoin-core/common/
mkdir .bitcoin

cat << EOF > ~/snap/bitcoin-core/common/.bitcoin/bitcoin.conf
regtest=1
txindex=1
server=1
rest=1
rpcuser=hoge
rpcpassword=hoge
[regtest]
rpcport=18332
port=18444
fallbackfee=0.0002
EOF
```


### インストール実行

```bash
chmod a+x install-bitcoincore-regtest.sh
./install-bitcoincore-regtest.sh


```

### デーモン起動


```bash
bitcoin-core.daemon &
```

### クライアント接続

```bash
bitcoin-core.cli help
```

### 基本操作

#### コマンドの一覧を見る

```bash
bitcoin-core.cli help
```

#### コマンドの詳細を見る

```bash
# bitcoin-core.cli help <コマンド>
bitcoin-core.cli help sendtoaddress
 
```

#### 基本操作

```bash
# aliceというラベルで自分のアドレスを生成してみる
bitcoin-core.cli getnewaddress 'alice'

# ラベルからアドレスを知る
bitcoin-core.cli getaddressesbylabel alice

# alice のアドレスの例
# bcrt1qr2c60a7wn95unh86rf396hhaxtnx5yctyd4rdm

# aliceのアドレスマイニングをする (ハッシュ値を50個作成する）
bitcoin-core.cli generatetoaddress 5 <自分のアドレス>

# 約17時間後　(100ブロック後）残高が増えていることを確認する
bitcoin-core.cli getbalance

# bobというラベルでアドレスを生成してみる
bitcoin-core.cli getnewaddress 'bob'

# bcrt1qgpslm9y8uu3pnnr54hu77dpq6mlzkkc6fuvqty

```

##### 送金とトランザクション

最後の行がトランザクションID

```bash
# 送金
bitcoin-core.cli sendtoaddress <送金先アドレス> 67


2020-10-21T10:22:46Z [default wallet] keypool added 1 keys (1 internal), size=2000 (1000 internal)
2020-10-21T10:22:46Z [default wallet] keypool reserve 1003
2020-10-21T10:22:46Z [default wallet] keypool keep 1003
2020-10-21T10:22:46Z [default wallet] Fee Calculation: Fee:4160 Bytes:208 Needed:4160 Tgt:0 (requested 6) Reason:"Fallback fee" Decay 0.00000: Estimation: (-1 - -1) -nan% 0.0/(0.0 0 mem 0.0 out) Fail: (-1 - -1) -nan% 0.0/(0.0 0 mem 0.0 out)
2020-10-21T10:22:46Z [default wallet] CommitTransaction:
CTransaction(hash=0afd2c6c56, ver=2, vin.size=2, vout.size=2, nLockTime=0)
    CTxIn(COutPoint(60e2f247a9, 0), scriptSig=, nSequence=4294967294)
    CTxIn(COutPoint(7e462e7ec1, 0), scriptSig=, nSequence=4294967294)
    CScriptWitness(304402205d8919a61d7a4782626c8f01c2396328bfd5bac9707a40fe5d431c4069e4b8340220522583d7a2992dc6830ffc8c227c009ac2a501839547071f1966fecbfe61c26101, 024d65156bd714f8a02610158dd52b76f21713eab836ad7fd63638137f0d5a8da2)
    CScriptWitness(304402206c28b5fbd264c0827a101dc123fc033541897478c2abb08f1f21a1b10302669702204f0b1155512a9fc135b8838b5853af45186651e40e97433a26e86e4e7d504b5501, 024d65156bd714f8a02610158dd52b76f21713eab836ad7fd63638137f0d5a8da2)
    CTxOut(nValue=51.50000000, scriptPubKey=00144061fd9487e72219cc74adf9ef)
    CTxOut(nValue=48.49995840, scriptPubKey=00149c301fe9cd62d8751e0201537a)
2020-10-21T10:22:46Z [default wallet] AddToWallet 0afd2c6c56186da0bba23029d7ddd1d96188120dc07b232838cee884c02c75a9  new
2020-10-21T10:22:46Z [default wallet] Submitting wtx 0afd2c6c56186da0bba23029d7ddd1d96188120dc07b232838cee884c02c75a9 to mempool for relay
2020-10-21T10:22:46Z [default wallet] AddToWallet 0afd2c6c56186da0bba23029d7ddd1d96188120dc07b232838cee884c02c75a9  
0afd2c6c56186da0bba23029d7ddd1d96188120dc07b232838cee884c02c75a9

# トランザクション ID
# 0afd2c6c56186da0bba23029d7ddd1d96188120dc07b232838cee884c02c75a9
```

##### トランザクションの内容を確認する

```bash
 bitcoin-core.cli gettransaction <トランザクションID>
 

{
  "amount": 0.00000000,
  "fee": -0.00004160,
  "confirmations": 0,
  "trusted": true,
  "txid": "0afd2c6c56186da0bba23029d7ddd1d96188120dc07b232838cee884c02c75a9",
  "walletconflicts": [
  ],
  "time": 1603275766,
  "timereceived": 1603275766,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "bcrt1qgpslm9y8uu3pnnr54hu77dpq6mlzkkc6fuvqty",
      "category": "send",
      "amount": -51.50000000,
      "label": "bob",
      "vout": 0,
      "fee": -0.00004160,
      "abandoned": false
    },
    {
      "address": "bcrt1qgpslm9y8uu3pnnr54hu77dpq6mlzkkc6fuvqty",
      "category": "receive",
      "amount": 51.50000000,
      "label": "bob",
      "vout": 0
    }
  ],
  "hex": "020000000001020488d969d546f3076e4977dd0d163f02694282b8f518f8543507a7a947f2e2600000000000feffffffc5b0611068fc0a47635fdb3f0e42012e386a03687813ccad28fb8fc17e2e467e0000000000feffffff0280c3f632010000001600144061fd9487e72219cc74adf9ef3420d6fe2b5b1a40101521010000001600149c301fe9cd62d8751e0201537a720de7cd160e9e0247304402205d8919a61d7a4782626c8f01c2396328bfd5bac9707a40fe5d431c4069e4b8340220522583d7a2992dc6830ffc8c227c009ac2a501839547071f1966fecbfe61c2610121024d65156bd714f8a02610158dd52b76f21713eab836ad7fd63638137f0d5a8da20247304402206c28b5fbd264c0827a101dc123fc033541897478c2abb08f1f21a1b10302669702204f0b1155512a9fc135b8838b5853af45186651e40e97433a26e86e4e7d504b550121024d65156bd714f8a02610158dd52b76f21713eab836ad7fd63638137f0d5a8da200000000"
}
```

```bash
bitcoin-core.cli getrawtransaction 0afd2c6c56186da0bba23029d7ddd1d96188120dc07b232838cee884c02c75a9
 
 
020000000001020488d969d546f3076e4977dd0d163f02694282b8f518f8543507a7a947f2e2600000000000feffffffc5b0611068fc0a47635fdb3f0e42012e386a03687813ccad28fb8fc17e2e467e0000000000feffffff0280c3f632010000001600144061fd9487e72219cc74adf9ef3420d6fe2b5b1a40101521010000001600149c301fe9cd62d8751e0201537a720de7cd160e9e0247304402205d8919a61d7a4782626c8f01c2396328bfd5bac9707a40fe5d431c4069e4b8340220522583d7a2992dc6830ffc8c227c009ac2a501839547071f1966fecbfe61c2610121024d65156bd714f8a02610158dd52b76f21713eab836ad7fd63638137f0d5a8da20247304402206c28b5fbd264c0827a101dc123fc033541897478c2abb08f1f21a1b10302669702204f0b1155512a9fc135b8838b5853af45186651e40e97433a26e86e4e7d504b550121024d65156bd714f8a02610158dd52b76f21713eab836ad7fd63638137f0d5a8da200000000

```

```bash
bitcoin-core.cli decoderawtransaction 020000000001020488d969d546f3076e4977dd0d163f02694282b8f518f8543507a7a947f2e2600000000000feffffffc5b0611068fc0a47635fdb3f0e42012e386a03687813ccad28fb8fc17e2e467e0000000000feffffff0280c3f632010000001600144061fd9487e72219cc74adf9ef3420d6fe2b5b1a40101521010000001600149c301fe9cd62d8751e0201537a720de7cd160e9e0247304402205d8919a61d7a4782626c8f01c2396328bfd5bac9707a40fe5d431c4069e4b8340220522583d7a2992dc6830ffc8c227c009ac2a501839547071f1966fecbfe61c2610121024d65156bd714f8a02610158dd52b76f21713eab836ad7fd63638137f0d5a8da20247304402206c28b5fbd264c0827a101dc123fc033541897478c2abb08f1f21a1b10302669702204f0b1155512a9fc135b8838b5853af45186651e40e97433a26e86e4e7d504b550121024d65156bd714f8a02610158dd52b76f21713eab836ad7fd63638137f0d5a8da200000000

{
  "txid": "0afd2c6c56186da0bba23029d7ddd1d96188120dc07b232838cee884c02c75a9",
  "hash": "12af4966ec485567c7b36ff62e191b1ce77402c176ae1f4168f8b8b9c6d22e37",
  "version": 2,
  "size": 370,
  "vsize": 208,
  "weight": 832,
  "locktime": 0,
  "vin": [
    {
      "txid": "60e2f247a9a7073554f818f5b8824269023f160ddd77496e07f346d569d98804",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "txinwitness": [
        "304402205d8919a61d7a4782626c8f01c2396328bfd5bac9707a40fe5d431c4069e4b8340220522583d7a2992dc6830ffc8c227c009ac2a501839547071f1966fecbfe61c26101",
        "024d65156bd714f8a02610158dd52b76f21713eab836ad7fd63638137f0d5a8da2"
      ],
      "sequence": 4294967294
    },
    {
      "txid": "7e462e7ec18ffb28adcc137868036a382e01420e3fdb5f63470afc681061b0c5",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "txinwitness": [
        "304402206c28b5fbd264c0827a101dc123fc033541897478c2abb08f1f21a1b10302669702204f0b1155512a9fc135b8838b5853af45186651e40e97433a26e86e4e7d504b5501",
        "024d65156bd714f8a02610158dd52b76f21713eab836ad7fd63638137f0d5a8da2"
      ],
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 51.50000000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 4061fd9487e72219cc74adf9ef3420d6fe2b5b1a",
        "hex": "00144061fd9487e72219cc74adf9ef3420d6fe2b5b1a",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "bcrt1qgpslm9y8uu3pnnr54hu77dpq6mlzkkc6fuvqty"
        ]
      }
    },
    {
      "value": 48.49995840,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 9c301fe9cd62d8751e0201537a720de7cd160e9e",
        "hex": "00149c301fe9cd62d8751e0201537a720de7cd160e9e",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "bcrt1qnscpl6wdvtv828szq9fh5usdulx3vr57eg7xda"
        ]
      }
    }
  ]
}

```

## ubuntu へのRuby インストール

```bash
sudo apt purge ruby rbenv ruby-build
rm -rf ~/.rbenv
sudo apt install -y git
sudo apt install -y gcc

cd ~
git clone https://github.com/rbenv/rbenv.git ~/.rbenv
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
sudo apt install -y rbenv

cat << EOF >> .bashrc
export PATH="$HOME/.rbenv/bin:$PATH"
eval "$(rbenv init -)"
EOF

source .bashrc

rbenv install -l

# 最新バージョンをインストールする 2.7.2 の場合
rbenv install 2.7.2
rbenv global 2.7.2
```


## bitcoinrbの利用

```bash
sudo apt-get install -y libleveldb-dev
gem install bitcoinrb
```

### irb

```bash
irb
```

```ruby
require 'bitcoin'

```

