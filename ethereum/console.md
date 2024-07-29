# Ethereum ブロックチェーンのコンソール操作

最終更新 2024/07/28 Shigeichiro Yamasaki

## Hardhat node の利用

* 自分の Hardhat プロジェクトのプロジェクトルートにいることを前提にします．

* localhost で Hardhat node が稼働してる状態を想定します

```bash
npx hardhat node
```

### ローカルノードへのデプロイ

Hardhat のチュートリアルにあるように igunition コマンドでデプロイできます

```bash
npx hardhat ignition deploy ./ignition/modules/Token.js --network localhost 

=>
...

Deployed Addresses

TokenModule#Token - 0x5FbDB2315678afecb367f032d93F642f64180aa3
```

### hardhat console コマンドで node のプロンプトになります

```bash
npx hardhat console --network localhost 

Type ".help" for more information.
>

```

### config オブジェクトの確認

hardhat の設定ファイルの状態

```js
> config

{
  solidity: { compilers: [ [Object] ], overrides: {} },
  networks: {
    hardhat: {
      hardfork: 'cancun',

...

  },
  gasReporter: { enabled: false, currency: 'USD' }
}
> 
```

### etheres オブジェクトの確認

@nomicfoundation/hardhat-ethers としてインストール済の etheres.js のオブジェクトの状態

```js

> ethers
{
  version: '6.13.2',
  decodeBytes32String: [Function: decodeBytes32String],
  encodeBytes32String: [Function: encodeBytes32String],
  AbiCoder: [class AbiCoder],
 
 ...

  getContractAt: [Function: getContractAt],
  getContractAtFromArtifact: [Function: getContractAtFromArtifact],
  deployContract: [Function: bound deployContract] AsyncFunction
}

```

### ethereum との非同期の応答を受け取る方法

javaSCript の await で非同期に受け取った内容出力すればよい

`ethers.getSigners()` で外部所有アカウントの一覧を得る例

```js
> await ethers.getSigners()

=>
[
  HardhatEthersSigner {
    _gasLimit: 30000000,
    address: '0xf39Fd6e51aad88F6F4ce6aB8827279cffFb92266',
    provider: HardhatEthersProvider {
      _hardhatProvider: [LazyInitializationProviderAdapter],
      _networkName: 'localhost',
      _blockListeners: [],
      _transactionHashListeners: Map(0) {},
      _eventListeners: []
    }
  },

  ...

  HardhatEthersSigner {
    _gasLimit: 30000000,
    address: '0x8626f6940E2eb28930eFb4CeF49B2d1F2C9C1199',
    provider: HardhatEthersProvider {
      _hardhatProvider: [LazyInitializationProviderAdapter],
      _networkName: 'localhost',
      _blockListeners: [],
      _transactionHashListeners: Map(0) {},
      _eventListeners: []
    }
  }
]
undefined
```

配列のインデックス1 を取り出す場合

```js
> (await ethers.getSigners())[1]
HardhatEthersSigner {
  _gasLimit: 30000000,
  address: '0x70997970C51812dc3A010C7d01b50e0d17dc79C8',
  provider: HardhatEthersProvider {
    _hardhatProvider: LazyInitializationProviderAdapter {
      _providerFactory: [AsyncFunction (anonymous)],
      _emitter: [EventEmitter],
      _initializingPromise: [Promise],
      provider: [BackwardsCompatibilityProviderAdapter]
    },
    _networkName: 'localhost',
    _blockListeners: [],
    _transactionHashListeners: Map(0) {},
    _eventListeners: []
  }
}
```