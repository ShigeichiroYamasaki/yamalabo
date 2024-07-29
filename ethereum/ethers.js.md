# ethers.js 入門

ethereum RPC の javaSCript ラッピングクライアントの使い方

最終更新 2024/07/28 Shigeichiro Yamasaki

コンソールから実行します

## ethers のAPI一覧


```js
> ethers

=>
{
  version: '6.13.2',
  decodeBytes32String: [Function: decodeBytes32String],
  encodeBytes32String: [Function: encodeBytes32String],
  AbiCoder: [class AbiCoder],
  ConstructorFragment: [class ConstructorFragment extends Fragment],
  ErrorFragment: [class ErrorFragment extends NamedFragment],
  EventFragment: [class EventFragment extends NamedFragment],
  Fragment: [class Fragment],
  FallbackFragment: [class FallbackFragment extends Fragment],
  FunctionFragment: [class FunctionFragment extends NamedFragment],
  NamedFragment: [class NamedFragment extends Fragment],
  
  ...

  },
  getSigner: [Function: getSigner],
  getSigners: [Function: getSigners],
  getImpersonatedSigner: [Function: getImpersonatedSigner],
  getContractFactory: [Function: bound getContractFactory] AsyncFunction,
  getContractFactoryFromArtifact: [Function: getContractFactoryFromArtifact],
  getContractAt: [Function: getContractAt],
  getContractAtFromArtifact: [Function: getContractAtFromArtifact],
  deployContract: [Function: bound deployContract] AsyncFunction
}
> 
```

## ethers.js の基本要素

ethereum RPC の基本要素を javaSCript オブジェクトにしたものです

* Provider :  ethereum ネットワーク
* Signer :    署名鍵をもつオブジェクト
* Contract :  特定のコントラクトをjavaScriptオブジェクトに見せたもの


### Provider

hardhat network に接続している場合

```js
> ethers.provider

=>
HardhatEthersProvider {
  _hardhatProvider: LazyInitializationProviderAdapter {
    _providerFactory: [AsyncFunction (anonymous)],
    _emitter: EventEmitter {
      _events: [Object: null prototype] {},
      _eventsCount: 0,
      _maxListeners: undefined,
      [Symbol(shapeMode)]: false,
      [Symbol(kCapture)]: false
    },
    _initializingPromise: Promise {
      [BackwardsCompatibilityProviderAdapter],
      [Symbol(async_id_symbol)]: 148,
      [Symbol(trigger_async_id_symbol)]: 15
    },
    provider: BackwardsCompatibilityProviderAdapter {
      _wrapped: [AutomaticGasPriceProvider],
      _provider: [AutomaticGasPriceProvider],
      sendAsync: [Function: bound sendAsync],
      send: [Function: bound send],
      _sendJsonRpcRequest: [Function: bound _sendJsonRpcRequest] AsyncFunction
    }
  },
  _networkName: 'localhost',
  _blockListeners: [],
  _transactionHashListeners: Map(0) {},
  _eventListeners: []
}
```

ブロック番号の取得

```js
> await ethers.provider.getBlockNumber()

=>
2
```

metamask で確認したアカウントのアドレスが `0x8626f6940E2eb28930eFb4CeF49B2d1F2C9C1199` のとき

```js
> await ethers.provider.getBalance('0x8626f6940E2eb28930eFb4CeF49B2d1F2C9C1199') 
10000000000000000000000n
```

### Signer

```js
>  await ethers.getSigners()

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

  },
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
> 
```

index 1 と 2 の Signer

```js
const s1= (await ethers.getSigners())[1]

=>
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

const s2= (await ethers.getSigners())[2]

=>
HardhatEthersSigner {
  _gasLimit: 30000000,
  address: '0x3C44CdDdB6a900fa2b585dd299e03d12FA4293BC',
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

Signer s1 がメッセージに署名する

```js
> await s1.signMessage("Hello World")

=>
'0xdc009b92222e2770cf41436a9969b014dd7881229daa3c9b2e425057363b8d2e638a8f065554383ca5f0c68b1af8a96b2f004356bb1af3feb91d14a8e819303f1c'

```

s1 が s2 にトランザクションを送信する

```js
let tx = await s1.sendTransaction({ to: s2.address, value: 100000})

=>
TransactionResponse {
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
  },
  blockNumber: 5,
  blockHash: '0x94973af56809fe2a33f44b96ee6bc4f6142b3e32e8b0c47e5b13ec780797239a',
  index: undefined,
  hash: '0xe188dc83184d45a1c7374bbe91d8eea679fcb792244567ef6279772a147d654c',
  type: 2,
  to: '0x3C44CdDdB6a900fa2b585dd299e03d12FA4293BC',
  from: '0x70997970C51812dc3A010C7d01b50e0d17dc79C8',
  nonce: 3,
  gasLimit: 30000000n,
  gasPrice: 1516701500n,
  maxPriorityFeePerGas: 1000000000n,
  maxFeePerGas: 1653950335n,
  maxFeePerBlobGas: null,
  data: '0x',
  value: 100000n,
  chainId: 31337n,
  signature: Signature { r: "0xf842bc6e83ee11fb02ce93774dcd65dd16cdcf4eb2e114fb32349695ecce0359", s: "0x674d7dcf729bb99d33b5187cb8e66c1a3b44e2ebb6726dd39bf4e5585c420fbe", yParity: 1, networkV: null },
  accessList: [],
  blobVersionedHashes: null
}
```

### Contract

デプロイしたコントラクトのアドレス `0x5FbDB2315678afecb367f032d93F642f64180aa3` とします

```js
> const s0= (await ethers.getSigners())[0]
> const contractAddress="0x5FbDB2315678afecb367f032d93F642f64180aa3"
> const abi = ["function transfer(address to, uint256 amount)"," function balanceOf(address account) external view returns (uint256)"]
> const contract = new ethers.Contract(contractAddress, abi, s0)
```

コントラクトオブジェクトの確認

```js
> contract

=>
Contract {
  target: '0x5FbDB2315678afecb367f032d93F642f64180aa3',
  interface: Interface {
    fragments: [ [FunctionFragment], [FunctionFragment] ],
    deploy: ConstructorFragment {
      type: 'constructor',
      inputs: [],
      payable: false,
      gas: null
    },
    fallback: null,
    receive: false
  },
  runner: HardhatEthersSigner {
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
  filters: {},
  fallback: null,
  [Symbol(_ethersInternal_contract)]: {}
}
```

コントラクトオブジェクトへの操作

```js
await contract.balanceOf(s0)

=>
1000000n

await contract.transfer(s1, 100n)

=>
ContractTransactionResponse {
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
  },
  blockNumber: 3,
  blockHash: '0x1c7a8a128df6ab143b94d2a7fc1c284d16afee6ef471ec77e6f376b7420a8caa',
  index: undefined,
  hash: '0x486b0917771a45ad2d50999e916ae07449402ecce0f3ccef9f13fc39962b632c',
  type: 2,
  to: '0x5FbDB2315678afecb367f032d93F642f64180aa3',
  from: '0xf39Fd6e51aad88F6F4ce6aB8827279cffFb92266',
  nonce: 1,
  gasLimit: 30000000n,
  gasPrice: 1674629891n,
  maxPriorityFeePerGas: 1000000000n,
  maxFeePerGas: 1853828455n,
  maxFeePerBlobGas: null,
  data: '0xa9059cbb00000000000000000000000070997970c51812dc3a010c7d01b50e0d17dc79c80000000000000000000000000000000000000000000000000000000000000064',
  value: 0n,
  chainId: 31337n,
  signature: Signature { r: "0x488730e600f356d559bfbc490b43cb06a1195b4a2f9b8f8d78956d234eb01811", s: "0x685756d10c5dceb8d6fbc5b300021b25443a3df165b29f356dbc902c7dcbdfa2", yParity: 0, networkV: null },
  accessList: [],
  blobVersionedHashes: null
}


>  await contract.balanceOf(s0)
999900n

>  await contract.balanceOf(s1)
100n
```