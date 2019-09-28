# Libra
Facebook Libra テストネットノードの構築

## 資料サイト


[https://developers.libra.org/docs/my-first-transaction](https://developers.libra.org/docs/my-first-transaction)
## ローカルネットワークでのlibra_swarmの構成

*  custodial client
*  validator node
*  


## Libra Coreのクローンとビルド

### Libra Coreリポジトリのクローンを作成する

```bash
git clone https://github.com/libra/libra.git
```

### testnetブランチをチェックアウトする

```bash
git checkout testnet
```

### 依存関係のインストール

Libra Coreをセットアップするには、次のようにlibraディレクトリに移動し、セットアップスクリプトを実行して依存関係をインストールします。

```bash
cd libra
./scripts/dev_setup.sh
```

セットアップスクリプトは、次のアクションを実行します。

* rustupをインストールします— rustupは、Libra Coreが実装されているRustプログラミング言語のインストーラーです。
* 必要なバージョンのrust-toolchainをインストールします。
* CMakeをインストールします—ビルドプロセスを管理します。
* プロトコルバッファ用のコンパイラであるprotocをインストールします。
* Goをインストールします—プロトコルバッファを構築します。


セットアップが失敗した場合は、トラブルシューティングを参照してください

## Libra CLIクライアントをビルドしてテストネットに接続する

Libraテストネットで実行されている検証ノードに接続するには、以下に示すようにクライアントを実行します。

```bash
./scripts/cli/start_cli_testnet.sh
```


このコマンドは、cargo（Rustのパッケージマネージャー）を使用してクライアントを構築および実行し、クライアントをテストネット上の検証ノードに接続します。

クライアントがテストネット上のノードに接続すると、次の出力が表示されます。いつでもクライアントを終了するには、quit次のコマンドを使用します。

```
usage: <command> <args>

Use the following commands:

account | a
  Account operations
query | q
  Query operations
transfer | transferb | t | tb
  <sender_account_address>|<sender_account_ref_id> <receiver_account_address>|<receiver_account_ref_id> <number_of_coins> [gas_unit_price (default=0)] [max_gas_amount (default 10000)] Suffix 'b' is for blocking.
  Transfer coins from account to another.
help | h
  Prints this help
quit | q!
  Exit this client


Please, input commands:

libra%
```


クライアントの構築とテストネットへの接続に問題がある場合は、トラブルシューティングを参照してください。

>> 注：システム上でバリデータノードをローカルで実行する場合は、ローカルバリデータノードを実行するの指示に従ってください。アカウントの作成、コインの鋳造、トランザクションの実行の手順は、testnet上のノードの手順と同じです。

## アリスとボブのアカウントを作成

クライアントがテストネットに接続されると、CLIコマンドを実行して新しいアカウントを作成できます。2人のユーザーのアカウントを作成します（アリスとボブと呼びましょう）。

### ステップ1：CLIクライアントがシステムで実行されているかどうかを確認する

Libraの％コマンドラインプロンプトは、あなたのLibraCLIクライアントが実行されていることを示しています。accountコマンドのヘルプ情報を表示するには、次のように「アカウント」と入力します。

```bash
libra% account
usage: account <arg>

Use the following args for this command:

create | c
  Create an account. Returns reference ID to use in other operations
list | la
  Print all accounts that were created or loaded
recover | r <file path>
  Recover Libra wallet from the file path
write | w <file name>
  Save Libra wallet mnemonic recovery seed to disk
mint | mintb | m | mb <receiver account> <number of coins>
  Mint coins to the account. Suffix 'b' is for blocking
```

### ステップ2：アリスのアカウントを作成する

CLIを使用してアカウントを作成しても、ブロックチェーンは更新されず、ローカルキーペアが作成されるだけです。

アリスのアカウントを作成するには、次のコマンドを入力します。

```bash
libra% account create

成功時のサンプル出力：

>> Creating/retrieving next account from wallet
Created/retrieved account #0 address 3ed8e5fafae4147b2a105a0be2f81972883441cfaaadf93fc0868e7a0253c4a8
```

0はアリスのアカウントのインデックスであり、16進文字列はアリスのアカウントのアドレスです。インデックスは、アリスのアカウントを参照する方法にすぎません。アカウントインデックスはローカルCLIインデックスであり、ユーザーが他のCLIコマンドで使用して、作成したアカウントを簡単に参照できます。インデックスはブロックチェーンにとって無意味です。アリスのアカウントは、お金がミントによってアリスのアカウントに追加されるか、別のユーザーからの送金によってアリスのアカウントに送金される場合にのみ、ブロックチェーン上に作成されます。CLIコマンドで16進アドレスを使用することもできます。アカウントインデックスは、アカウントアドレスの便利なラッパーです。

### ステップ3：ボブのアカウントを作成する

Bobのアカウントを作成するには、アカウント作成コマンドを繰り返します。

```bash
libra% account create
```

成功時のサンプル出力：

```
>> Creating/retrieving next account from wallet
Created/retrieved account #1 address 8337aac709a41fe6be03cad8878a0d4209740b1608f8a81566c9a7d4b95a2ec7
```

1はボブのアカウントのインデックスであり、16進文字列はボブのアカウントのアドレスです。インデックスの詳細については、アリスのアカウントの作成を参照してください。

### ステップ4（オプション）：アカウントのリスト

作成したアカウントをリストするには、次のコマンドを入力します。

```bash
libra% account list
```

成功時のサンプル出力：

```
User account index: 0, address: 3ed8e5fafae4147b2a105a0be2f81972883441cfaaadf93fc0868e7a0253c4a8, sequence number: 0
User account index: 1, address: 8337aac709a41fe6be03cad8878a0d4209740b1608f8a81566c9a7d4b95a2ec7, sequence number: 0
```

アカウントのシーケンス番号は、そのアカウントから送信されたトランザクションの数を示します。そのアカウントから送信されたトランザクションが実行され、ブロックチェーンに保存されるたびに増加します。詳細については、シーケンス番号を参照してください。

## Libra CoinsをAliceおよびBobのアカウントに追加する

testnetのアカウントへのコインの作成と追加は、Faucetを介して行われます。Faucetは、テストネットとともに実行されるサービスです。このサービスは、testnet用鋳造コインを促進するために存在し、存在していませんmainnet。現実世界の価値のないLibraを作成します。それぞれインデックス0とインデックス1でアリスとボブのアカウントを作成したと仮定すると、以下の手順に従って両方のアカウントにLibraを追加できます。


### ステップ1：110 LBRをAliceのアカウントに追加します

Libraをミントしてアリスのアカウントに追加するには、次のコマンドを入力します。

```bash
libra% account mint 0 110
```

0はアリスのアカウントのインデックスです。
110は、アリスのアカウントに追加されるLibraの量です。
account mintコマンドが成功すると、ブロックチェーン上にAliceのアカウントも作成されます。

成功時のサンプル出力：

```
>> Minting coins
Mint request submitted
```

要求が送信されると、（testnet上の検証ノードの）mempoolに正常に追加されたことを意味することに注意してください。必ずしも正常に完了することを意味するわけではありません。後で、口座残高を照会して、マイニングが成功したかどうかを確認します。

アカウントmintコマンドがリクエストを正常に送信しなかった場合は、トラブルシューティングを参照してください

### ステップ2：ボブのアカウントに52 LBRを追加する

LibraをミントしてBobのアカウントに追加するには、次のコマンドを入力します。

```bash
libra% account mint 1 52
```

1はボブのアカウントのインデックスです。
52は、Bobのアカウントに追加されるLibraの量です。
account mintコマンドが成功すると、ブロックチェーン上にBobのアカウントも作成されます。ブロックチェーン上にボブのアカウントを作成する別の方法は、アリスのアカウントからボブのアカウントに送金することです。
成功時のサンプル出力：

```
>> Minting coins
Mint request submitted
```

アカウントmintコマンドがリクエストを正常に送信しなかった場合は、トラブルシューティングを参照してください

### ステップ3：残高を確認する

アリスの口座の残高を確認するには、次のコマンドを入力します。

```
libra% query balance 0
```

成功時のサンプル出力：

```
Balance is: 110
```

Bobのアカウントの残高を確認するには、次のコマンドを入力します。

```bash
libra% query balance 1
```

成功時のサンプル出力：

```
Balance is: 52
```

## トランザクションを送信する

LibraをAliceのアカウントからBobのアカウントに転送するトランザクションを送信する前に、各アカウントのシーケンス番号を照会します。これは、トランザクションを実行すると各アカウントのシーケンス番号がどのように変化するかを理解するのに役立ちます。

### アカウントのシーケンス番号を照会する

```bash
libra% query sequence 0
>> Getting current sequence number
Sequence number is: 0
libra% query sequence 1
>> Getting current sequence number
Sequence number is: 0
```

でquery sequence 0、0はアリスのアカウントのインデックスです。アリスとボブの両方のアカウントのシーケンス番号0は、アリスまたはボブのアカウントからのトランザクションがこれまでに実行されていないことを示します。

## 送金

アリスのアカウントからボブのアカウントに10 LBRを転送するトランザクションを送信するには、次のコマンドを入力します。

```bash
libra% transfer 0 1 10
```

0はアリスのアカウントのインデックスです。
1はボブのアカウントのインデックスです。
10は、アリスのアカウントからボブのアカウントに転送するLibraの数です。
成功時のサンプル出力：

```
>> Transferring
Transaction submitted to validator
To query for transaction status, run: query txn_acc_seq 0 0 <fetch_events=true|false>
```

コマンドquery txn_acc_seq 0 0 true（アカウントおよびシーケンス番号によるトランザクション）を使用して、送信したばかりのトランザクションに関する情報を取得できます。最初のパラメーターは送信者アカウントのローカルインデックスで、2番目のパラメーターはアカウントのシーケンス番号です。このコマンドのサンプル出力を表示するにはを参照してください出力のサンプル。

トランザクションをtestnetのバリデータノードに送信したところ、バリデータのmempoolに含まれていました。これは、必ずしもトランザクションが実行されたことを意味するわけではありません。理論的には、システムが低速または過負荷の場合、結果を確認するのに時間がかかるため、アカウントを照会して複数回確認する必要があります。インデックス0でアカウントをクエリするには、次のコマンドを使用できます query account_state 0.。期待される出力は、サンプル出力セクションに表示されます

転送コマンドのトラブルシューティングについては、トラブルシューティングを参照してください。

ブロック転送コマンド：transferbコマンドの代わりに、transferコマンドを使用できます（以下を参照）。transferbトランザクションがブロックチェーンにコミットされた後にのみ、トランザクションを送信してクライアントプロンプトに戻ります。以下に例を示します。

```bash
libra% transferb 0 1 10
```

送信から実行および保存までのトランザクションのライフサイクルの理解については、トランザクションの寿命を参照してください。

### 転送後のクエリシーケンス番号

```bash
libra% query sequence 0
>> Getting current sequence number
Sequence number is: 1
libra% query sequence 1
>> Getting current sequence number
Sequence number is: 0
```

アリスのアカウントのシーケンス番号1（インデックス0）は、これまでに1つのトランザクションがアリスのアカウントから送信されたことを示しています。Bobのアカウントのシーケンス番号0（インデックス1）は、これまでにBobのアカウントからトランザクションが送信されていないことを示します。アカウントからトランザクションが送信されるたびに、シーケンス番号は1ずつ増加します。

### 譲渡後に両方の口座の残高を確認する

両方のアカウントの最終残高を確認するには、この手順で行ったように、各アカウントの残高を再度クエリします。トランザクション（転送）が正常に実行されると、Aliceのアカウントに100 LBR、Bobのアカウントに62 LBRが表示されます。

```bash
libra% query balance 0
Balance is: 100
libra% query balance 1
Balance is: 62
```

## 成功

Libraテストネットでトランザクションを正常に実行し、アリスのアカウントからボブのアカウントに10 LBRを転送しました！

## トラブルシューティング

### セットアップ

* Rustの更新：
    * rustup updatelibraディレクトリから実行します。
* 更新プロトコル：
    * protocバージョン3.6.0以降に更新します。
* libraディレクトリからセットアップスクリプトを再実行します。
    * ./scripts/dev_setup.sh

### クライアントのビルドと実行

ビルドに失敗した場合は、libraディレクトリからカーゴロックファイルを削除してください。

```bash
rm Cargo.lock
```

クライアントがテストネットに接続しなかった場合：

インターネット接続を確認してください。
クライアントの最新バージョンを使用していることを確認してください。最新のLibra Coreを引き出して、クライアントを再実行します。

```bash
./scripts/cli/start_cli_testnet.sh
```

### アカウントへのお金の鋳造と追加

* testnetで接続した検証ノードが利用できない場合、以下に示すように「サーバーが利用できません」というメッセージが表示されます。

```bash
libra% account mint 0 110
>> Minting coins
[ERROR] Error minting coins: Server unavailable, please retry and/or check **if** host passed to the client is running
```

* トランザクションの送信後に残高が更新されなかった場合は、しばらく待ってから再度残高を照会してください。ブロックチェーンで大量のトランザクションが発生している場合、遅延が発生する可能性があります。それでも残高が更新されない場合は、もう一度ミントを試してください。

* アカウントが存在するかどうかを確認するには、アカウントの状態を照会します。インデックス0のアカウントの場合、次を入力します。

```bash
libra% query account_state 0
```

### 転送コマンド

testnet検証ノード（クライアントが接続されていた）が利用できない場合、またはtestnetへの接続がタイムアウトした場合、次のエラーが表示されます。

```bash
libra% transfer 0 1 10
>> Transferring
[ERROR] Failed to perform transaction: Server unavailable, please retry and/or check if host passed to the client is running
```

転送エラーをトラブルシューティングするには：

* testnetへの接続を確認します。
* 送信者アカウントを照会して、それが存在することを確認します。インデックス0のアカウントに対して次のコマンドを使用します。
    * query account_state 0
* quitまたはを使用してクライアントを終了しq!、次のコマンドを再実行してテストネットに接続できます。
    * ./scripts/cli/start_cli_testnet.sh libraディレクトリから

## 追加のクエリコマンドのサンプル出力

### アカウントとシーケンス番号によるトランザクションのクエリ

この例では、アカウントとシーケンス番号を使用して、単一のトランザクションの詳細を照会します。

```bash
libra% query txn_acc_seq 0 0 true
>> Getting committed transaction by account and sequence number
Committed transaction: SignedTransaction {
 { raw_txn: RawTransaction {
    sender: 3ed8e5fafae4147b2a105a0be2f81972883441cfaaadf93fc0868e7a0253c4a8,
    sequence_number: 0,
    payload: {,
      transaction: peer_to_peer_transaction,
      args: [
        {ADDRESS: 8337aac709a41fe6be03cad8878a0d4209740b1608f8a81566c9a7d4b95a2ec7},
        {U64: 10000000},
      ]
    },
    max_gas_amount: 10000,
    gas_unit_price: 0,
    expiration_time: 1560466424s,
},
 public_key: 55af3fe3f28550a2f1e5ebf073ef193feda44344d94c463b48be202aa0b3255d,
 signature: Signature( R: CompressedEdwardsY: [210, 23, 214, 62, 228, 179, 64, 147, 81, 159, 180, 138, 100, 211, 111, 139, 178, 148, 81, 1, 240, 135, 148, 145, 104, 234, 227, 239, 198, 153, 13, 199], s: Scalar{
  bytes: [203, 76, 105, 49, 64, 130, 162, 81, 22, 237, 159, 26, 80, 181, 111, 94, 84, 6, 152, 126, 181, 192, 62, 103, 130, 94, 246, 174, 139, 214, 3, 15],
} ),
 }
 }
Events:
ContractEvent { access_path: AccessPath { address: 3ed8e5fafae4147b2a105a0be2f81972883441cfaaadf93fc0868e7a0253c4a8, type: Resource, hash: "217da6c6b3e19f1825cfb2676daecce3bf3de03cf26647c78df00b371b25cc97", suffix: "/sent_events_count/" } , index: 0, event_data: AccountEvent { account: 8337aac709a41fe6be03cad8878a0d4209740b1608f8a81566c9a7d4b95a2ec7, amount: 10000000 } }
ContractEvent { access_path: AccessPath { address: 8337aac709a41fe6be03cad8878a0d4209740b1608f8a81566c9a7d4b95a2ec7, type: Resource, hash: "217da6c6b3e19f1825cfb2676daecce3bf3de03cf26647c78df00b371b25cc97", suffix: "/received_events_count/" } , index: 0, event_data: AccountEvent { account: 3ed8e5fafae4147b2a105a0be2f81972883441cfaaadf93fc0868e7a0253c4a8, amount: 10000000 } }
```

トランザクション量はmicrolibraに表示されることに注意してください。

### クエリイベント

次の例では、参照インデックス0のアカウントから「送信済み」イベントをクエリします。このアカウントから1つのトランザクションを送信したため、単一のイベントがあることに気付くでしょう。現在の状態の証明も返されるため、イベントが欠落していないことを確認できます。これは、クエリが「制限」イベントを返さないときに行われます。

```bash
libra% query event 0 sent 0 true 10
>> Getting events by account and event type.
EventWithProof {
  transaction_version: 3,
  event_index: 0,
  event: ContractEvent { access_path: AccessPath { address: e7460e02058b36d28e8eef03f0834c605d3d6c57455b8ec9c3f0a3c8b89f248b, type: Resource, hash: "217da6c6b3e19f1825cfb2676daecce3bf3de03cf26647c78df00b371b25cc97", suffix: "/sent_events_count/" } , index: 0, event_data: AccountEvent { account: 46efbad798a739c088e0e98dd9d592c27c7eb45ba1f8ccbdfc00bd4d7f2947f3, amount: 10000000 } },
  proof: EventProof { ledger_info_to_transaction_info_proof: AccumulatorProof { siblings: [HashValue(62570ae9a994bcb20c03c055667a4966fa50d0f17867dd5819465072fd2c58ba), HashValue(cce2cf325714511e7d04fa5b48babacd5af943198e6c1ac3bdd39c53c87cb84c)] }, transaction_info: TransactionInfo { signed_transaction_hash: HashValue(69bed01473e0a64140d96e46f594bc4b463e88e244b694e962b7e19fde17f30d), state_root_hash: HashValue(5809605d5eed94c73e57f615190c165b11c5e26873012285cc6b131e0817c430), event_root_hash: HashValue(645df3dee8f53a0d018449392b8e9da814d258da7346cf64cd96824f914e68f9), gas_used: 0 }, transaction_info_to_event_proof: AccumulatorProof { siblings: [HashValue(5d0e2ebf0952f0989cb5b38b2a9b52a09e8d804e893cb99bf9fa2c74ab304bb1)] } }
}
Last event state: Some(
    AccountStateWithProof {
        version: 3,
        blob: Some(
            AccountStateBlob {
             Raw: 0x010000002100000001217da6c6b3e19f1825cfb2676daecce3bf3de03cf26647c78df00b371b25cc974400000020000000e7460e02058b36d28e8eef03f0834c605d3d6c57455b8ec9c3f0a3c8b89f248b00e1f50500000000000000000000000001000000000000000100000000000000
             Decoded: Ok(
                AccountResource {
                    balance: 100000000,
                    sequence_number: 1,
                    authentication_key: 0xe7460e02058b36d28e8eef03f0834c605d3d6c57455b8ec9c3f0a3c8b89f248b,
                    sent_events_count: 1,
                    received_events_count: 0,
                },
            )
             },
        ),
        proof: AccountStateProof {
            ledger_info_to_transaction_info_proof: AccumulatorProof {
                siblings: [
                    HashValue(62570ae9a994bcb20c03c055667a4966fa50d0f17867dd5819465072fd2c58ba),
                    HashValue(cce2cf325714511e7d04fa5b48babacd5af943198e6c1ac3bdd39c53c87cb84c),
                ],
            },
            transaction_info: TransactionInfo {
                signed_transaction_hash: HashValue(69bed01473e0a64140d96e46f594bc4b463e88e244b694e962b7e19fde17f30d),
                state_root_hash: HashValue(5809605d5eed94c73e57f615190c165b11c5e26873012285cc6b131e0817c430),
                event_root_hash: HashValue(645df3dee8f53a0d018449392b8e9da814d258da7346cf64cd96824f914e68f9),
                gas_used: 0,
            },
            transaction_info_to_account_proof: SparseMerkleProof {
                leaf: Some(
                    (
                        HashValue(c0fbd63b0ae4abfe57c8f24f912f164ba0537741e948a65f00d3fae0f9373981),
                        HashValue(fc45057fd64606c7ca40256b48fbe486660930bfef1a9e941cafcae380c25871),
                    ),
                ),
                siblings: [
                    HashValue(4136803b3ba779bb2c1daae7360f3f839e6fef16ae742590a6698b350a5fc376),
                    HashValue(5350415253455f4d45524b4c455f504c414345484f4c4445525f484153480000),
                    HashValue(a9a6bda22dd6ee78ddd3a42da152b9bd39797b7da738e9d6023f407741810378),
                ],
            },
        },
    },
)
```

### アカウント状態の照会

この例では、単一のアカウントの状態を照会します。

```bash
libra% query account_state 0
>> Getting latest account state
Latest account state is:
 Account: 3ed8e5fafae4147b2a105a0be2f81972883441cfaaadf93fc0868e7a0253c4a8
 State: Some(
    AccountStateBlob {
     Raw: 0x010000002100000001217da6c6b3e19f1825cfb2676daecce3bf3de03cf26647c78df00b371b25cc9744000000200000003ed8e5fafae4147b2a105a0be2f81972883441cfaaadf93fc0868e7a0253c4a800e1f50500000000000000000000000001000000000000000100000000000000
     Decoded: Ok(
        AccountResource {
            balance: 100000000,
            sequence_number: 1,
            authentication_key: 0x3ed8e5fafae4147b2a105a0be2f81972883441cfaaadf93fc0868e7a0253c4a8,
            sent_events_count: 1,
            received_events_count: 0,
        },
    )
     },
)
 Blockchain Version: 3
```


### ローカル検証ノードを実行する

コンピューター上で検証ノードをローカルで起動し、Libraテストネットに接続されていない独自のローカルブロックチェーンネットワークを作成するには、Libra Coreのセットアップで説明されているビルドスクリプトを実行し、Libra Coreリポジトリのルートディレクトリに移動します、libra_swarm次のように実行します。

```bash
$ cd ~/libra
$ cargo run -p libra_swarm -- -s
```

-p libra_swarm -cargo にlibra_swarmパッケージを実行させ、1つのノードで構成されるローカルブロックチェーンを開始します。

-s オプションは、ローカルクライアントを起動して、ローカルブロックチェーンに接続します。

ノードを起動してLibraブロックチェーンに接続するための追加オプションを表示するには、次を実行します：

```bash
$ cargo run -p libra_swarm -- -h
```

cargo実行コマンドの実行には時間がかかる場合があります。このコマンドの実行がエラーなしで完了すると、Libra CLIクライアントのインスタンスとLibra検証ノードがシステムで実行されています。正常に実行されると、CLIクライアントメニューとlibra%プロンプトを含む出力が表示されます。

## トランザクションの寿命

最初のトランザクションを実行したら、ドキュメントのトランザクションの寿命を参照できます。

* 提出から実行までのトランザクションのライフサイクルを「内部で」見ます。
* Libraエコシステムでトランザクションが送信および実行される際の、Libraバリデーターの各論理コンポーネント間の相互作用の理解。



