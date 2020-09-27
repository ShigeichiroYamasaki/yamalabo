# bitcin-cli チュートリアル(testnet)

## インストール

### bitcoin core のインストール

## testnetで稼働させる

### testnet でノードを稼働させる

bitcoin.conf 

```
testnet = 3
# txindexの設定が必要
txindex = 1
server = 1
rest = 1
rpcuser = yamalabo
rpcpassword = yozoranomukou
rpcport = 18332
```

## 新規アドレスの作成

### bech32アドレスを２つ作成

* legacy: 
	Base58エンコードされたP2PKHアドレス
* p2sh-segwit （デフォルト） :
	P2WPKHをP2SHでネストしたP2SHアドレス
* bech32: 
	Bech32のアドレスフォーマットでP2WPKHのアドレス
	(mainnetは”bc1”から始まり、testnetは”tb1”から始まる)

```bash
#	bitcoin-cli getnewaddress ラベル アドレスタイプ


$ bitcoin-cli getnewaddress mybech32 bech32
tb1qct7uxdjydtlf0jtrwvgue259adzugx5yltm6qv

$ bitcoin-cli getnewaddress nextbech32 bech32

tb1qaecqaprgrk5f6hsxyxpqek2dlzk067w4p9hf7l
```

### ラベルの一覧

```
$ bitcoin-cli listlabels
[
  "",
  "mybech32",
  "nextbech32",
]
```

### アドレスの確認

```bash
#	bitcoin-cli getaddressesbylabel ラベル

# ラベルmybech32のアドレス
bitcoin-cli getaddressesbylabel mybech32

{
  "tb1qct7uxdjydtlf0jtrwvgue259adzugx5yltm6qv": {
    "purpose": "receive"
  }
}

# ラベルnextbech32のアドレス
bitcoin-cli getaddressesbylabel nextbech32
{
  "tb1qaecqaprgrk5f6hsxyxpqek2dlzk067w4p9hf7l": {
    "purpose": "receive"
  }
}
```

## bitcoin faucet でtestnet の ビットコインを入手する

mybech32のアドレス を使ってbitcoinを得る

[Bitcoin testnet3 faucet](https://coinfaucet.eu/en/btc-testnet/)


## 秘密鍵と公開鍵の操作

ワレットを暗号化している場合は、パスフレーズで復号化しておく必要がある

### ワレットアンロック

```bash
#	 bitcoin-cli walletpassphrase パスフレーズ 秒数

bitcoin-cli walletpassphrase yozoranomukou 3600
```

### 秘密鍵

```bash
#	  bitcoin-cli dumpprivkey ビットコインアドレス

$ bitcoin-cli dumpprivkey tb1qaecqaprgrk5f6hsxyxpqek2dlzk067w4p9hf7l

cRuhxB4hgzBCP9QRp64yvoxvP2no19YpdGXTQ3Cz5TUMHUMLUi81

$ bitcoin-cli dumpprivkey tb1qct7uxdjydtlf0jtrwvgue259adzugx5yltm6qv

cQVDru2TKdYcgS1C5exyhhdvgYxm2ZkTZ1xRbVWPEs1QFCUCmPBz
```

### ワレットをロックする

```bash
$ bitcoin-cli walletlock
```

## bitcoin の送金

* 送金先アドレス: tb1qct7uxdjydtlf0jtrwvgue259adzugx5yltm6qv 
* 送金金額: 0.001 

sendtoaddress を実行するとトランザクションIDが返ってくる

```bash
bitcoin-cli walletpassphrase yozoranomukou 3600

bitcoin-cli sendtoaddress tb1qct7uxdjydtlf0jtrwvgue259adzugx5yltm6qv 0.001

# トランザクションID
006137ca0aecc010364dd3a3914ce00f8ae7a8a097b215378d9b16b56478e105
```

## トランザクションの解析

```bash
bitcoin-cli gettransaction 006137ca0aecc010364dd3a3914ce00f8ae7a8a097b215378d9b16b56478e105

{
  "amount": 0.00000000,
  "fee": -0.00000141,
  "confirmations": 0,
  "trusted": true,
  "txid": "006137ca0aecc010364dd3a3914ce00f8ae7a8a097b215378d9b16b56478e105",
  "walletconflicts": [
  ],
  "time": 1565243705,
  "timereceived": 1565243705,
  "bip125-replaceable": "no",
  "details": [
    {
      "address": "tb1qct7uxdjydtlf0jtrwvgue259adzugx5yltm6qv",
      "category": "send",
      "amount": -0.00100000,
      "label": "mybech32",
      "vout": 0,
      "fee": -0.00000141,
      "abandoned": false
    },
    {
      "address": "tb1qct7uxdjydtlf0jtrwvgue259adzugx5yltm6qv",
      "category": "receive",
      "amount": 0.00100000,
      "label": "mybech32",
      "vout": 0
    }
  ],
  "hex": "0200000000010160ca87bd68f4498b82ae0cb27fbbd924084d5f513e9a59765efb95e353488a6c0000000000feffffff02a086010000000000160014c2fdc336446afe97c9637311ccaa85eb45c41a847177390000000000160014f82685498d26fe3530a30aa59f8fd6811bc38d250247304402203016c4eb66bb999ee56c0d37777b47d4c2a28190f8bed43d419c605da9d2bf55022059d94f3c566c7ed9a2c0f66f14c57a8b6cfd787ced6c656ecbd316d6c040e59c012103569febd1ba6725e19d4f13670435cff2e45ec623df8c2f4a905c9dab22b7e157d9001800"
}
```

```bash
bitcoin-cli getrawtransaction 006137ca0aecc010364dd3a3914ce00f8ae7a8a097b215378d9b16b56478e105

0200000000010160ca87bd68f4498b82ae0cb27fbbd924084d5f513e9a59765efb95e353488a6c0000000000feffffff02a086010000000000160014c2fdc336446afe97c9637311ccaa85eb45c41a847177390000000000160014f82685498d26fe3530a30aa59f8fd6811bc38d250247304402203016c4eb66bb999ee56c0d37777b47d4c2a28190f8bed43d419c605da9d2bf55022059d94f3c566c7ed9a2c0f66f14c57a8b6cfd787ced6c656ecbd316d6c040e59c012103569febd1ba6725e19d4f13670435cff2e45ec623df8c2f4a905c9dab22b7e157d9001800
```


```bash
bitcoin-cli decoderawtransaction 0200000000010160ca87bd68f4498b82ae0cb27fbbd924084d5f513e9a59765efb95e353488a6c0000000000feffffff02a086010000000000160014c2fdc336446afe97c9637311ccaa85eb45c41a847177390000000000160014f82685498d26fe3530a30aa59f8fd6811bc38d250247304402203016c4eb66bb999ee56c0d37777b47d4c2a28190f8bed43d419c605da9d2bf55022059d94f3c566c7ed9a2c0f66f14c57a8b6cfd787ced6c656ecbd316d6c040e59c012103569febd1ba6725e19d4f13670435cff2e45ec623df8c2f4a905c9dab22b7e157d9001800

{
  "txid": "006137ca0aecc010364dd3a3914ce00f8ae7a8a097b215378d9b16b56478e105",
  "hash": "2ac2063d89fa09a4e75e8adaa1eabcf513647d440ca7baece4f4b2ef7f3e6ee9",
  "version": 2,
  "size": 222,
  "vsize": 141,
  "weight": 561,
  "locktime": 1573081,
  "vin": [
    {
      "txid": "6c8a4853e395fb5e76599a3e515f4d0824d9bb7fb20cae828b49f468bd87ca60",
      "vout": 0,
      "scriptSig": {
        "asm": "",
        "hex": ""
      },
      "txinwitness": [
        "304402203016c4eb66bb999ee56c0d37777b47d4c2a28190f8bed43d419c605da9d2bf55022059d94f3c566c7ed9a2c0f66f14c57a8b6cfd787ced6c656ecbd316d6c040e59c01",
        "03569febd1ba6725e19d4f13670435cff2e45ec623df8c2f4a905c9dab22b7e157"
      ],
      "sequence": 4294967294
    }
  ],
  "vout": [
    {
      "value": 0.00100000,
      "n": 0,
      "scriptPubKey": {
        "asm": "0 c2fdc336446afe97c9637311ccaa85eb45c41a84",
        "hex": "0014c2fdc336446afe97c9637311ccaa85eb45c41a84",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qct7uxdjydtlf0jtrwvgue259adzugx5yltm6qv"
        ]
      }
    },
    {
      "value": 0.03766129,
      "n": 1,
      "scriptPubKey": {
        "asm": "0 f82685498d26fe3530a30aa59f8fd6811bc38d25",
        "hex": "0014f82685498d26fe3530a30aa59f8fd6811bc38d25",
        "reqSigs": 1,
        "type": "witness_v0_keyhash",
        "addresses": [
          "tb1qlqng2jvdymlr2v9rp2jelr7ksydu8rf9z8n48u"
        ]
      }
    }
  ]
}
```

## 未使用のUTXO一覧

```bash
bitcoin-cli listunspent
```

```
[
  {
    "txid": "006137ca0aecc010364dd3a3914ce00f8ae7a8a097b215378d9b16b56478e105",
    "vout": 0,
    "address": "tb1qct7uxdjydtlf0jtrwvgue259adzugx5yltm6qv",
    "label": "mybech32",
    "scriptPubKey": "0014c2fdc336446afe97c9637311ccaa85eb45c41a84",
    "amount": 0.00100000,
    "confirmations": 5,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([ab449ebb/0'/0'/8']03569febd1ba6725e19d4f13670435cff2e45ec623df8c2f4a905c9dab22b7e157)#hz5uwx2j",
    "safe": true
  },
  {
    "txid": "006137ca0aecc010364dd3a3914ce00f8ae7a8a097b215378d9b16b56478e105",
    "vout": 1,
    "address": "tb1qlqng2jvdymlr2v9rp2jelr7ksydu8rf9z8n48u",
    "scriptPubKey": "0014f82685498d26fe3530a30aa59f8fd6811bc38d25",
    "amount": 0.03766129,
    "confirmations": 5,
    "spendable": true,
    "solvable": true,
    "desc": "wpkh([ab449ebb/0'/1'/8']03057fc695107499fa9a2e653594e3dc38fe39ae78f7af1b748c64e4aba5cff09a)#22jl7949",
    "safe": true
  },
  {
    "txid": "0150a3892e92de98b55eaff9f0894c8973a645da6d4691f8f74da21ed2f94a06",
    "vout": 1,
    "address": "2N7nBo6Jg4VRuJC24T9E4PRLfPJ3tWyTYLS",
    "label": "kindai1",
    "redeemScript": "00140721a231d84508c010b692bda246bce5340ca264",
    "scriptPubKey": "a9149f6cd340267747c3e1c79394289828081e369ab587",
    "amount": 0.00050000,
    "confirmations": 28901,
    "spendable": true,
    "solvable": true,
    "desc": "sh(wpkh([c10f0931/0'/0'/2']023ce3855a13f435a9a35dae89c76492480f008a341b6a8623ae04064204089ec1))#2l09cvcw",
    "safe": true
  },
  {
    "txid": "b6353bc29e7d4ecebcb2ac23ca1ab01ba5f1a0080679b4b1c110b4df199f0a19",
    "vout": 0,
    "address": "2N5y2pbbbPZnw3d5KPtqoeJwTJBL8fJqUug",
    "redeemScript": "0014b124b2c65089eb9393a398ca5842c349cd480a90",
    "scriptPubKey": "a9148b89a875e6b8dce1cc5c55aab4c6d97cc4862d8a87",
    "amount": 0.03401867,
    "confirmations": 5,
    "spendable": true,
    "solvable": true,
    "desc": "sh(wpkh([ab449ebb/0'/1'/7']03d6fcf9cdb14c7dc1c3508d9ebab7eb03d042b0947de045c7b534552efeb50939))#emhjdxul",
    "safe": true
  },
    {
    "txid": "52b51863bf048b5902d6845ab37e682cdd8ef72acb62986fb57189503f8306ef",
    "vout": 1,
    "address": "2N7nBo6Jg4VRuJC24T9E4PRLfPJ3tWyTYLS",
    "label": "kindai1",
    "redeemScript": "00140721a231d84508c010b692bda246bce5340ca264",
    "scriptPubKey": "a9149f6cd340267747c3e1c79394289828081e369ab587",
    "amount": 0.00100000,
    "confirmations": 3668,
    "spendable": true,
    "solvable": true,
    "desc": "sh(wpkh([c10f0931/0'/0'/2']023ce3855a13f435a9a35dae89c76492480f008a341b6a8623ae04064204089ec1))#2l09cvcw",
    "safe": true
  }
]
```






## bitcoin-cli コマンド

help で機能一覧を確認

```bash
$ bitcoin-cli help
== Blockchain ==
getbestblockhash
getblock "blockhash" ( verbosity )
getblockchaininfo
getblockcount
getblockhash height
getblockheader "blockhash" ( verbose )
getblockstats hash_or_height ( stats )
getchaintips
getchaintxstats ( nblocks "blockhash" )
getdifficulty
getmempoolancestors "txid" ( verbose )
getmempooldescendants "txid" ( verbose )
getmempoolentry "txid"
getmempoolinfo
getrawmempool ( verbose )
gettxout "txid" n ( include_mempool )
gettxoutproof ["txid",...] ( "blockhash" )
gettxoutsetinfo
preciousblock "blockhash"
pruneblockchain height
savemempool
scantxoutset "action" [scanobjects,...]
verifychain ( checklevel nblocks )
verifytxoutproof "proof"

== Control ==
getmemoryinfo ( "mode" )
getrpcinfo
help ( "command" )
logging ( ["include_category",...] ["exclude_category",...] )
stop
uptime

== Generating ==
generate nblocks ( maxtries )
generatetoaddress nblocks "address" ( maxtries )

== Mining ==
getblocktemplate "template_request"
getmininginfo
getnetworkhashps ( nblocks height )
prioritisetransaction "txid" ( dummy ) fee_delta
submitblock "hexdata" ( "dummy" )
submitheader "hexdata"

== Network ==
addnode "node" "command"
clearbanned
disconnectnode ( "address" nodeid )
getaddednodeinfo ( "node" )
getconnectioncount
getnettotals
getnetworkinfo
getnodeaddresses ( count )
getpeerinfo
listbanned
ping
setban "subnet" "command" ( bantime absolute )
setnetworkactive state

== Rawtransactions ==
analyzepsbt "psbt"
combinepsbt ["psbt",...]
combinerawtransaction ["hexstring",...]
converttopsbt "hexstring" ( permitsigdata iswitness )
createpsbt [{"txid":"hex","vout":n,"sequence":n},...] [{"address":amount},{"data":"hex"},...] ( locktime replaceable )
createrawtransaction [{"txid":"hex","vout":n,"sequence":n},...] [{"address":amount},{"data":"hex"},...] ( locktime replaceable )
decodepsbt "psbt"
decoderawtransaction "hexstring" ( iswitness )
decodescript "hexstring"
finalizepsbt "psbt" ( extract )
fundrawtransaction "hexstring" ( options iswitness )
getrawtransaction "txid" ( verbose "blockhash" )
joinpsbts ["psbt",...]
sendrawtransaction "hexstring" ( allowhighfees )
signrawtransactionwithkey "hexstring" ["privatekey",...] ( [{"txid":"hex","vout":n,"scriptPubKey":"hex","redeemScript":"hex","witnessScript":"hex","amount":amount},...] "sighashtype" )
testmempoolaccept ["rawtx",...] ( allowhighfees )
utxoupdatepsbt "psbt"

== Util ==
createmultisig nrequired ["key",...] ( "address_type" )
deriveaddresses "descriptor" ( range )
estimatesmartfee conf_target ( "estimate_mode" )
getdescriptorinfo "descriptor"
signmessagewithprivkey "privkey" "message"
validateaddress "address"
verifymessage "address" "signature" "message"

== Wallet ==
abandontransaction "txid"
abortrescan
addmultisigaddress nrequired ["key",...] ( "label" "address_type" )
backupwallet "destination"
bumpfee "txid" ( options )
createwallet "wallet_name" ( disable_private_keys blank )
dumpprivkey "address"
dumpwallet "filename"
encryptwallet "passphrase"
getaddressesbylabel "label"
getaddressinfo "address"
getbalance ( "dummy" minconf include_watchonly )
getnewaddress ( "label" "address_type" )
getrawchangeaddress ( "address_type" )
getreceivedbyaddress "address" ( minconf )
getreceivedbylabel "label" ( minconf )
gettransaction "txid" ( include_watchonly )
getunconfirmedbalance
getwalletinfo
importaddress "address" ( "label" rescan p2sh )
importmulti "requests" ( "options" )
importprivkey "privkey" ( "label" rescan )
importprunedfunds "rawtransaction" "txoutproof"
importpubkey "pubkey" ( "label" rescan )
importwallet "filename"
keypoolrefill ( newsize )
listaddressgroupings
listlabels ( "purpose" )
listlockunspent
listreceivedbyaddress ( minconf include_empty include_watchonly "address_filter" )
listreceivedbylabel ( minconf include_empty include_watchonly )
listsinceblock ( "blockhash" target_confirmations include_watchonly include_removed )
listtransactions ( "label" count skip include_watchonly )
listunspent ( minconf maxconf ["address",...] include_unsafe query_options )
listwalletdir
listwallets
loadwallet "filename"
lockunspent unlock ( [{"txid":"hex","vout":n},...] )
removeprunedfunds "txid"
rescanblockchain ( start_height stop_height )
sendmany "" {"address":amount} ( minconf "comment" ["address",...] replaceable conf_target "estimate_mode" )
sendtoaddress "address" amount ( "comment" "comment_to" subtractfeefromamount replaceable conf_target "estimate_mode" )
sethdseed ( newkeypool "seed" )
setlabel "address" "label"
settxfee amount
signmessage "address" "message"
signrawtransactionwithwallet "hexstring" ( [{"txid":"hex","vout":n,"scriptPubKey":"hex","redeemScript":"hex","witnessScript":"hex","amount":amount},...] "sighashtype" )
unloadwallet ( "wallet_name" )
walletcreatefundedpsbt [{"txid":"hex","vout":n,"sequence":n},...] [{"address":amount},{"data":"hex"},...] ( locktime options bip32derivs )
walletlock
walletpassphrase "passphrase" timeout
walletpassphrasechange "oldpassphrase" "newpassphrase"
walletprocesspsbt "psbt" ( sign "sighashtype" bip32derivs )

== Zmq ==
getzmqnotifications
```

