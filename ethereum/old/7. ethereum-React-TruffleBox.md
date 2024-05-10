# React Truffle Box によるスマートコントラクトのフロントエンド

## 環境セットアップ

```
npm install truffle
```

```
mkdir greeter-dapp
cd greeter-dapp

truffle unbox react
```

ディレクトリ構成
この client/src 部分にReactアプリがある

```
├── LICENSE
├── client
│   ├── README.md
│   ├── node_modules
│   ├── package-lock.json
│   ├── package.json
│   ├── public
│   ├── src
│   └── yarn.lock
├── contracts
│   ├── Migrations.sol
│   └── SimpleStorage.sol
├── migrations
│   ├── 1_initial_migration.js
│   └── 2_deploy_contracts.js
├── test
│   ├── TestSimpleStorage.sol
│   └── simplestorage.js
└── truffle-config.js

```

### Truffle サーバの起動

```
truffle develop
```

127.0.0.1:8545　でサーバが起動し、Ether を持ったアカウントが生成される

```
Truffle Develop started at http://127.0.0.1:8545/

Accounts:
(0) 0x45695678e125b34bd79592ae9be2a5946332b353
(1) 0xd6781fcfee1b5ed0942cff54cedcc59c3ad7768f
(2) 0x71e252673b2632be605e53a1dac8a653c88b5f1c
(3) 0x55d56f111ac9960c6a1eb32e43fce16140b9ff3d
(4) 0xa600eaa807ec32a3f420c1ad3a98df601eba97ce
(5) 0xcfc551857db8115251773086af2356012b973e04
(6) 0x596bb86a7477f6068f9eb43df6f2417e8a1dca53
(7) 0xad70c568c8f5a4ecd12cf6151ec0114b5f876f43
(8) 0x28d7482d61bab934587e73f761bb64bf1c3e8bc8
(9) 0x44d8d518f7b9eefc8ea6842f92f5d9731fb4950c

Private Keys:
(0) cabd9503a867fd128195e4b76a3b67fb63fc1974e496502c18b9d93779420b41
(1) cc1016d559c7031772434212a016b0fa88e2834aa623488f6dc9a20cda1edb0a
(2) 094728324941d5a90d535664fa0424a5b812c4af1ed8f8b5872c8d517f47364f
(3) a0c146544e4a8145722141f81d3d758dac28a595dfc4f2475967fb6b3330ce6c
(4) a800c23cf264f2f117da6ab772aa2a69c4c8e8c834d0bbdbdccf07b98ee89711
(5) a162ac882bfb135557cbad1d06925af7dafd91fce10597f2e7d2b5c42f649de8
(6) 78804111dd73a8b94c687b1817979a33935289fe31517c57615fb79ab0f395ee
(7) 03fe3b81db8f98b8976cf42f51e78e1832b61d74a0aebe71bc7e6215cfefff5b
(8) 9810b5a7439ffb8628b328748594d05b24ce1de37917ff56977d409d243ba4d4
(9) 7c86f68c36ea645f99f80ea04294175721ede13c4486be9acc13253212901d89

Mnemonic: cram roast zone announce edit math battle spread calm bridge admit pioneer

⚠️  Important ⚠️  : This mnemonic was created for you by Truffle. It is not secure.
Ensure you do not use it on production blockchains, or else you risk losing funds.

truffle(develop)> 
```

### 