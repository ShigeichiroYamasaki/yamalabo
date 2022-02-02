# Ethereum Solidity


Remix

## Ropstenノードに登録

Infuraサービスにブラウザで接続

`https://infura.io`

* ユーザ登録
* 新規プロジェクト作成（Ethereum) Ropstern-test

#### Project IDとProject SECRET　とROPSTENネットワークへのエンドポイントを得る

例

```
KEYS

PROJECT ID
7f19fb19a9124a978477c81a38d14670
PROJECT SECRET
5cd9f22becdb4d4e9daa11a0e703beb7

ENDPOINTS  ROPSTEN

https://ropsten.infura.io/v3/7f19fb19a9124a978477c81a38d14670
wss://ropsten.infura.io/ws/v3/7f19fb19a9124a978477c81a38d14670
```

保存する

### web3 のインストール

```
brew install node
brew install yarn
berw install pure

npm install web3 
yarn add web3 
```

### node.js

```
node
Welcome to Node.js v17.4.0.
Type ".help" for more information.
>
``` 

### Ethereum用モジュールをrequire

```
> var Web3=require('web3');
```

### web3オブジェクトのインスタンスを生成して、InfuraサービスのRopstenに接続する

```
var web3=new Web3(new Web3.providers.HttpProvider('https://ropten.infura.io/https://ropsten.infura.io/v3/7f19fb19a9124a978477c81a38d14670'));
```

```
var createAccounts=function (){
  var aliceKeys=web3.eth.accounts.create();
  console.log(aliceKeys);
  var bobKeys=web3.eth.accounts.create();
  console.log(bobKeys);
};
createAccounts()
```

実行結果の例

```
{
  address: '0x5e9F8127CbE225834683837Ef1768Adf174B500c',
  privateKey: '0x497dff22aa5af915e8bec10223b1083f8d61cf8866c71e5db7d9427ed3c5187f',
  signTransaction: [Function: signTransaction],
  sign: [Function: sign],
  encrypt: [Function: encrypt]
}
{
  address: '0xbaCC8907C6f061A6c9C063eAc6AbD630a07D2506',
  privateKey: '0x60802c469782e4d85cd88531b0d2eb272c165e8d7a4a6b61c2e67c3ee5683c18',
  signTransaction: [Function: signTransaction],
  sign: [Function: sign],
  encrypt: [Function: encrypt]
}
undefined
```

### faucetから Etherを入手する

`https://faucet.egorfine.com`
`https://faucet.ropsten.be`


### 残高確認

```
> var alice='0x5e9F8127CbE225834683837Ef1768Adf174B500c';
> var bob='0xbaCC8907C6f061A6c9C063eAc6AbD630a07D2506';

var getBalance= function () {
    web3.eth.getBalance('0x5e9F8127CbE225834683837Ef1768Adf174B500c').then(console.log);
};
getBalance();

```
