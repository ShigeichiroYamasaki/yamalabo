# Ethereum mainnet 構築


## ethereum (parity)

### インストールスクリプト

新規マシンにインストール


```bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y snap
sudo snap install parity
```

## mainnetの接続(同期）

```
parity &
```

```bash
~$ 2022-01-29 18:17:49  Starting Parity-Ethereum/v2.7.2-stable-2662d19-20200206/x86_64-unknown-linux-gnu/rustc1.41.0
2022-01-29 18:17:49  Keys path /home/yamasaki/snap/parity/16707/.local/share/io.parity.ethereum/keys/ethereum
2022-01-29 18:17:49  DB path /home/yamasaki/snap/parity/16707/.local/share/io.parity.ethereum/chains/ethereum/db/906a34e69aec8c0d
2022-01-29 18:17:49  State DB configuration: fast
2022-01-29 18:17:49  Operating mode: active
2022-01-29 18:17:50  Configured for Ethereum using Ethash engine
2022-01-29 18:17:50  Public node URL: enode://6bdd9b519d0be67780a66b131b7fc674bcdefb022943c1aee2a32e727ea1ff5b913cd1ccee006a3bda79a6335c253b0471bd6c8703afe61a23134cbfb0910c6f@192.168.0.251:30303
2022-01-29 18:17:51  Updated conversion rate to Ξ1 = US$2539.77 (1874935.4 wei/gas)
2022-01-29 18:18:25     0/25 peers   920 bytes chain 3 MiB db 0 bytes queue 1 KiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:18:55     0/25 peers   920 bytes chain 3 MiB db 0 bytes queue 1 KiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:19:25  Syncing       #0 0xd4e5…8fa3     0.00 blk/s    0.0 tx/s    0.0 Mgas/s    123+    0 Qed      #127    1/25 peers   920 bytes chain 3 MiB db 215 KiB queue 216 KiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:19:30  Syncing    #1144 0xfddc…2484   228.71 blk/s    0.0 tx/s    0.0 Mgas/s      0+    0 Qed     #1144    1/25 peers   1 MiB chain 8 MiB db 0 bytes queue 419 KiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:19:35  Syncing    #2288 0xc6cb…37a3   228.75 blk/s    0.0 tx/s    0.0 Mgas/s      0+    0 Qed     #2288    1/25 peers   2 MiB chain 11 MiB db 0 bytes queue 775 KiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:19:40  Syncing    #3517 0x0a46…6f7b   245.80 blk/s    0.0 tx/s    0.0 Mgas/s     41+    1 Qed     #3562    2/25 peers   3 MiB chain 16 MiB db 73 KiB queue 1 MiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:19:45  Syncing    #7063 0x3040…22d1   708.92 blk/s    0.0 tx/s    0.0 Mgas/s    201+    0 Qed     #7270    2/25 peers   5 MiB chain 26 MiB db 314 KiB queue 2 MiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:19:50  Syncing   #10781 0x9ef7…8937   743.60 blk/s    0.0 tx/s    0.0 Mgas/s    390+    0 Qed    #11176    2/25 peers   7 MiB chain 41 MiB db 605 KiB queue 2 MiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:19:55  Syncing   #14997 0xac22…ef2e   843.03 blk/s    0.0 tx/s    0.0 Mgas/s      0+    0 Qed    #14997    3/25 peers   7 MiB chain 52 MiB db 0 bytes queue 4 MiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:20:00  Syncing   #17529 0x16a1…54f5   506.30 blk/s    0.0 tx/s    0.0 Mgas/s      0+    0 Qed    #17529    3/25 peers   6 MiB chain 57 MiB db 0 bytes queue 8 MiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:20:05  Syncing   #22453 0xac92…f3f7   984.80 blk/s    0.0 tx/s    0.0 Mgas/s   2176+    2 Qed    #24765    2/25 peers   5 MiB chain 56 MiB db 3 MiB queue 5 MiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:20:10  Syncing   #25404 0xce98…28a2   589.85 blk/s    0.0 tx/s    0.0 Mgas/s      0+    0 Qed    #25404    3/25 peers   5 MiB chain 56 MiB db 0 bytes queue 8 MiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:20:15  Syncing   #29184 0x5c1d…6ba3   755.60 blk/s    0.0 tx/s    0.0 Mgas/s   3703+    0 Qed    #32893    2/25 peers   8 MiB chain 65 MiB db 6 MiB queue 7 MiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:20:20  Syncing   #34100 0xf89d…d60e   982.22 blk/s    0.0 tx/s    0.0 Mgas/s   3868+    1 Qed    #37973    2/25 peers   6 MiB chain 65 MiB db 6 MiB queue 7 MiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:20:25  Syncing   #39834 0x5964…8707  1146.77 blk/s    0.0 tx/s    0.0 Mgas/s   3083+    0 Qed    #42926    2/25 peers   5 MiB chain 65 MiB db 5 MiB queue 7 MiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:20:30  Syncing   #45582 0xcfa5…98d2  1148.02 blk/s    0.0 tx/s    0.0 Mgas/s    770+    3 Qed    #46357    2/25 peers   6 MiB chain 65 MiB db 1 MiB queue 8 MiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:20:35  Syncing   #49357 0x97bd…5d82   755.00 blk/s  327.2 tx/s    7.2 Mgas/s     41+    0 Qed    #49404    2/25 peers   5 MiB chain 65 MiB db 84 KiB queue 7 MiB sync  RPC:  0 conn,    0 req/s,    0 µs
2022-01-29 18:20:40  Syncing   #52578 0xb87d…d287   643.50 blk/s  359.9 tx/s   27.8 Mgas/s      0+    0 Qed    #52578    2/25 peers   5 MiB chain 65 MiB db 0 bytes queue 8 MiB sync  RPC:  0 conn,    0 req/s,    0 µs
```

## parityに対する JSON RPC による基本操作

* eth：ブロックチェーンの操作
* net：p2pネットワークステータス
* admin：ノードの管理
* miner：マイニング
* txpool：トランザクションメモリプール
* web3：単位変換など


### eth

同期状態

```
curl --data '{"method":"eth_syncing","params":[],"id":1,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST localhost:8545
```


ブロック高

```
curl --data '{"method":"eth_blockNumber","params":[],"id":1,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST localhost:8545
```

ブロック高によるブロック

```
curl --data '{"method":"eth_getBlockByNumber","params":["0x1b4",true],"id":1,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST localhost:8545
```


### parity


```
curl --data '{"method":"parity_allTransactionHashes","params":[],"id":1,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST localhost:8545

curl --data '{"method":"parity_listAccounts","params":[5,null],"id":1,"jsonrpc":"2.0"}' -H "Content-Type: application/json" -X POST localhost:8545

```




