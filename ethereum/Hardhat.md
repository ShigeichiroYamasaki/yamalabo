# Hardhat 

2024/06/16
作成，更新 Shigeichiro Yamasaki

## 環境のセットアップ

Node.js と JavaScriptの知識が前提になります

### ubuntu

```bash
sudo apt update
sudo apt install curl git
curl -fsSL https://deb.nodesource.com/setup_20.x | sudo -E bash -
sudo apt-get install -y nodejs
```


### MacOSX

```bash
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.1/install.sh | bash
nvm install 20
nvm use 20
nvm alias default 20
npm install npm --global
```

## Hardhat プロジェクトの新規作成

### プロジェクトフォルダの作成

プロジェクトのフォルダを決めます．

★ このディレクトリを「プロジェクトルート」といいます．

ここでは，hardhat-tutorial という名前のフォルダにします．

```bash
cd ~
mkdir hardhat-projects
cd hardhat-projects/
mkdir hardhat-tutorial
cd hardhat-tutorial
```

以下では，npm 7 を利用します．

### JavaScript プロジェクトの作成


```bash
npm init

=>
This utility will walk you through creating a package.json file.
It only covers the most common items, and tries to guess sensible defaults.

See `npm help init` for definitive documentation on these fields
and exactly what they do.

Use `npm install <pkg>` afterwards to install a package and
save it as a dependency in the package.json file.

Press ^C at any time to quit.
package name: (hardhat-tutorial) 
version: (1.0.0) 
description: 
entry point: (index.js) 
test command: 
git repository: 
keywords: 
author: 
license: (ISC) 
About to write to /Users/shigeichiroyamasaki/hardhat-projects/hardhat-tutorial/package.json:

{
  "name": "hardhat-tutorial",
  "version": "1.0.0",
  "main": "index.js",
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1"
  },
  "author": "",
  "license": "ISC",
  "description": ""
}


Is this OK? (yes) 
```

### Hardhat パッケージモジュールのインストール

```bash
npm install --save-dev hardhat
```

### Hardhat の初期化 （空の設定ファイルの作成）

Create an empty hardhat.config.js を選択して，現在のディレクトリに空の hardhat.config.js ファイルを作成します．

Hardhat の実行には空の hardhat.config.js ファイルで問題ありません．

```bash
npx hardhat init
=>
888    888                      888 888               888
888    888                      888 888               888
888    888                      888 888               888
8888888888  8888b.  888d888 .d88888 88888b.   8888b.  888888
888    888     "88b 888P"  d88" 888 888 "88b     "88b 888
888    888 .d888888 888    888  888 888  888 .d888888 888
888    888 888  888 888    Y88b 888 888  888 888  888 Y88b.
888    888 "Y888888 888     "Y88888 888  888 "Y888888  "Y888

Welcome to Hardhat v2.22.5

? What do you want to do? … 
▸ Create a JavaScript project
  Create a TypeScript project
  Create a TypeScript project (with Viem)
  Create an empty hardhat.config.js
  Quit

```

### タスクとプラグイン

* Task

コンパイルなど Hardhat のコマンドラインから実行する処理のこと

例 compile タスクの実行

```bash
npx hardhat compile
```

* plugin

Hardhatのツールには組み込みのデフォルトのものがありますが，プラグインによって柔軟に別のツールに上書きが可能です．

#### 推奨プラグインのインストール

以下では，推奨プラグインを使って説明します．

推奨プラグインのインストールは以下のようにします．

```bash
npm install --save-dev @nomicfoundation/hardhat-toolbox
```

* hardhat.config.js ファイルの修正

```js
require("@nomicfoundation/hardhat-toolbox");

/** @type import('hardhat/config').HardhatUserConfig */
module.exports = {
  solidity: "0.8.24",
};
```

## スマートコントラクトの作成とコンパイル

### contracts ディレクトリの作成

プロジェクトルートの下に contracts というディレクトリを作成します．

```bash
mkdir contracts
cd contracts
```

### Solidity プログラムの作成

contractディレクトリの下に Solidity言語のプログラムのファイルを作成します．
ここでは，Token.sol というファイル名にします.

Solidityプログラムのソースコードには .sol という拡張子をつけます．

* Token.sol

```bash
nano Token.sol
```

```js
//SPDX-License-Identifier: UNLICENSED

// Solidity files have to start with this pragma.
// It will be used by the Solidity compiler to validate its version.
pragma solidity ^0.8.0;


// This is the main building block for smart contracts.
contract Token {
    // Some string type variables to identify the token.
    string public name = "My Hardhat Token";
    string public symbol = "MHT";

    // The fixed amount of tokens, stored in an unsigned integer type variable.
    uint256 public totalSupply = 1000000;

    // An address type variable is used to store ethereum accounts.
    address public owner;

    // A mapping is a key/value map. Here we store each account's balance.
    mapping(address => uint256) balances;

    // The Transfer event helps off-chain applications understand
    // what happens within your contract.
    event Transfer(address indexed _from, address indexed _to, uint256 _value);

    /**
     * Contract initialization.
     */
    constructor() {
        // The totalSupply is assigned to the transaction sender, which is the
        // account that is deploying the contract.
        balances[msg.sender] = totalSupply;
        owner = msg.sender;
    }

    /**
     * A function to transfer tokens.
     *
     * The `external` modifier makes a function *only* callable from *outside*
     * the contract.
     */
    function transfer(address to, uint256 amount) external {
        // Check if the transaction sender has enough tokens.
        // If `require`'s first argument evaluates to `false`, the
        // transaction will revert.
        require(balances[msg.sender] >= amount, "Not enough tokens");

        // Transfer the amount.
        balances[msg.sender] -= amount;
        balances[to] += amount;

        // Notify off-chain applications of the transfer.
        emit Transfer(msg.sender, to, amount);
    }

    /**
     * Read only function to retrieve the token balance of a given account.
     *
     * The `view` modifier indicates that it doesn't modify the contract's
     * state, which allows us to call it without executing a transaction.
     */
    function balanceOf(address account) external view returns (uint256) {
        return balances[account];
    }
}
```

### コントラクトのコンパイル

```bash
npx hardhat compile

=>
Downloading compiler 0.8.24
Compiled 1 Solidity file successfully (evm target: paris).
```

## コントラクトのテスト

コントラクトのテストは ローカルノードである Hardhat Networkで実施します．

コントラクトの操作は，ethere.js を利用します

テスト実行は Mocha を利用します．

### テストの作成

プロジェクトルートの下に test という名前のディレクトリを作成します．

```bash
cd ..
mkdir test
cd test
```
JavaScript のテストフレームワーク CHai を利用する

また， ethers.js を利用して操作を行います

ここではテストプログラムを Token.js とします．

* Token.js

```bash
nano Token.js
```

```js
const { expect } = require("chai");

describe("Token contract", function () {
  it("Deployment should assign the total supply of tokens to the owner", async function () {
    const [owner] = await ethers.getSigners();

    const hardhatToken = await ethers.deployContract("Token");

    const ownerBalance = await hardhatToken.balanceOf(owner.address);
    expect(await hardhatToken.totalSupply()).to.equal(ownerBalance);
  });
});
```

### テストの実行

```bash
npx hardhat test

=>


  Token contract
    ✔ Deployment should assign the total supply of tokens to the owner (435ms)


  1 passing (436ms)


```

#### テスト内容

```js
const [owner] = await ethers.getSigners();
```

ethers.js  を使っています．

ethers.getSigners() は，トランザクション送信者のEthereumアカウントを返します．

```js
const hardhatToken = await ethers.deployContract("Token");
```

ethers.deployContract("Token") は，このコントラクトをデプロイします．

```js
const ownerBalance = await hardhatToken.balanceOf(owner.address);
```

デプロイが完了すると hardhatToken というオブジェクトが利用可能になります．

hardhatTokenに対して balanceOf というメソッドを使って Owner の所持金を確認することができます．

```js
expect(await hardhatToken.totalSupply()).to.equal(ownerBalance);
```

hardhatTokenに対して totalSupply というメソッドを使ってトークンの供給量を求めます．
さらに，その値が Ownerの所持金と等しいことを確認します．

#### Token.js の修正例

* Token.js

```bash
nano Token.js
```

```js
const {
  loadFixture,
} = require("@nomicfoundation/hardhat-toolbox/network-helpers");
const { expect } = require("chai");

describe("Token contract", function () {
  async function deployTokenFixture() {
    const [owner, addr1, addr2] = await ethers.getSigners();

    const hardhatToken = await ethers.deployContract("Token");

    // Fixtures can return anything you consider useful for your tests
    return { hardhatToken, owner, addr1, addr2 };
  }

  it("Should assign the total supply of tokens to the owner", async function () {
    const { hardhatToken, owner } = await loadFixture(deployTokenFixture);

    const ownerBalance = await hardhatToken.balanceOf(owner.address);
    expect(await hardhatToken.totalSupply()).to.equal(ownerBalance);
  });

  it("Should transfer tokens between accounts", async function () {
    const { hardhatToken, owner, addr1, addr2 } = await loadFixture(
      deployTokenFixture
    );

    // Transfer 50 tokens from owner to addr1
    await expect(
      hardhatToken.transfer(addr1.address, 50)
    ).to.changeTokenBalances(hardhatToken, [owner, addr1], [-50, 50]);

    // Transfer 50 tokens from addr1 to addr2
    // We use .connect(signer) to send a transaction from another account
    await expect(
      hardhatToken.connect(addr1).transfer(addr2.address, 50)
    ).to.changeTokenBalances(hardhatToken, [addr1, addr2], [-50, 50]);
  });
});

```

```bash
npx hardhat test 

=>


  Token contract
    ✔ Should assign the total supply of tokens to the owner (340ms)
    ✔ Should transfer tokens between accounts


  2 passing (356ms)
```

#### フルテストの例

* Token.js

```bash
nano Token.js
```

```js
const {
  loadFixture,
} = require("@nomicfoundation/hardhat-toolbox/network-helpers");
const { expect } = require("chai");

describe("Token contract", function () {
  async function deployTokenFixture() {
    const [owner, addr1, addr2] = await ethers.getSigners();

    const hardhatToken = await ethers.deployContract("Token");

    // Fixtures can return anything you consider useful for your tests
    return { hardhatToken, owner, addr1, addr2 };
  }

  it("Should assign the total supply of tokens to the owner", async function () {
    const { hardhatToken, owner } = await loadFixture(deployTokenFixture);

    const ownerBalance = await hardhatToken.balanceOf(owner.address);
    expect(await hardhatToken.totalSupply()).to.equal(ownerBalance);
  });

  it("Should transfer tokens between accounts", async function () {
    const { hardhatToken, owner, addr1, addr2 } = await loadFixture(
      deployTokenFixture
    );

    // Transfer 50 tokens from owner to addr1
    await expect(
      hardhatToken.transfer(addr1.address, 50)
    ).to.changeTokenBalances(hardhatToken, [owner, addr1], [-50, 50]);

    // Transfer 50 tokens from addr1 to addr2
    // We use .connect(signer) to send a transaction from another account
    await expect(
      hardhatToken.connect(addr1).transfer(addr2.address, 50)
    ).to.changeTokenBalances(hardhatToken, [addr1, addr2], [-50, 50]);
  });
});

```

```bash
npx hardhat test 

=>


  Token contract
    Deployment
      ✔ Should set the right owner (460ms)
      ✔ Should assign the total supply of tokens to the owner
    Transactions
      ✔ Should transfer tokens between accounts
      ✔ Should emit Transfer events
      ✔ Should fail if sender doesn't have enough tokens


  5 passing (495ms)


```

## Hardhat Network の利用

### Solidityの console.log 

* Token.js の修正版

contracts ディレクトリに移動して修正

```bash
nano Token.sol
```

```js
//SPDX-License-Identifier: UNLICENSED

// Solidity files have to start with this pragma.
// It will be used by the Solidity compiler to validate its version.
pragma solidity ^0.8.0;

import "hardhat/console.sol";

// This is the main building block for smart contracts.
contract Token {
    // Some string type variables to identify the token.
    string public name = "My Hardhat Token";
    string public symbol = "MHT";

    // The fixed amount of tokens, stored in an unsigned integer type variable.
    uint256 public totalSupply = 1000000;

    // An address type variable is used to store ethereum accounts.
    address public owner;

    // A mapping is a key/value map. Here we store each account's balance.
    mapping(address => uint256) balances;

    // The Transfer event helps off-chain applications understand
    // what happens within your contract.
    event Transfer(address indexed _from, address indexed _to, uint256 _value);

    /**
     * Contract initialization.
     */
    constructor() {
        // The totalSupply is assigned to the transaction sender, which is the
        // account that is deploying the contract.
        balances[msg.sender] = totalSupply;
        owner = msg.sender;
    }

    /**
     * A function to transfer tokens.
     *
     * The `external` modifier makes a function *only* callable from *outside*
     * the contract.
     */
    function transfer(address to, uint256 amount) external {
        // Check if the transaction sender has enough tokens.
        // If `require`'s first argument evaluates to `false`, the
        // transaction will revert.
        require(balances[msg.sender] >= amount, "Not enough tokens");

    console.log(
        "Transferring from %s to %s %s tokens",
        msg.sender,
        to,
        amount
    );

        // Transfer the amount.
        balances[msg.sender] -= amount;
        balances[to] += amount;

        // Notify off-chain applications of the transfer.
        emit Transfer(msg.sender, to, amount);
    }

    /**
     * Read only function to retrieve the token balance of a given account.
     *
     * The `view` modifier indicates that it doesn't modify the contract's
     * state, which allows us to call it without executing a transaction.
     */
    function balanceOf(address account) external view returns (uint256) {
        return balances[account];
    }
}
```

テストの実行とコンソールメッセージの確認

```bash
npx hardhat test

=>
Compiled 2 Solidity files successfully (evm target: paris).


  Token contract
    Deployment
      ✔ Should set the right owner (854ms)
      ✔ Should assign the total supply of tokens to the owner
    Transactions
Transferring from 0xf39fd6e51aad88f6f4ce6ab8827279cfffb92266 to 0x70997970c51812dc3a010c7d01b50e0d17dc79c8 50 tokens
Transferring from 0x70997970c51812dc3a010c7d01b50e0d17dc79c8 to 0x3c44cdddb6a900fa2b585dd299e03d12fa4293bc 50 tokens
      ✔ Should transfer tokens between accounts
Transferring from 0xf39fd6e51aad88f6f4ce6ab8827279cfffb92266 to 0x70997970c51812dc3a010c7d01b50e0d17dc79c8 50 tokens
Transferring from 0x70997970c51812dc3a010c7d01b50e0d17dc79c8 to 0x3c44cdddb6a900fa2b585dd299e03d12fa4293bc 50 tokens
      ✔ Should emit Transfer events
      ✔ Should fail if sender doesn't have enough tokens


  5 passing (899ms)


```

## Sepolia テストネットへのデプロイ

infula を利用して Sepolia テストネットにデプロイする方法を説明します．
mainnet へのデプロイも基本的に同様の方法で実施できます．

### Hardhat Ignitionモジュール

Ignitionモージュールは，デプロイを支援するJavaScript 関数です．

#### モジュールディレクトリの作成

プロジェクトルートの下に `./ignition/modules` というディレクトリを作成します．

```bash
mkdir ignition
cd ignition/
mkdir modules
cd modules
```

`./ignition/modules`ディレクトリに以下の `Token.js` ファイルを作成します．

```js
const { buildModule } = require("@nomicfoundation/hardhat-ignition/modules");

const TokenModule = buildModule("TokenModule", (m) => {
  const token = m.contract("Token");

  return { token };
});

module.exports = TokenModule;

```

### Hardhat とネットワークの接続方法

以下のようにネットワーク名を指定して実行します

```bash
npx hardhat ignition deploy ./ignition/modules/Token.js --network <ネットワーク名>
```

プロジェクトルートに移動して，ネットワーク名を指定せずにテストとしてデプロイコマンドを実行して，エラーがないことを確認する．

```bash
cd ../..
npx hardhat ignition deploy ./ignition/modules/Token.js

=>
You are running Hardhat Ignition against an in-process instance of Hardhat Network.
This will execute the deployment, but the results will be lost.
You can use --network <network-name> to deploy to a different network.

Hardhat Ignition 🚀

Deploying [ TokenModule ]

Batch #1
  Executed TokenModule#Token

[ TokenModule ] successfully deployed 🚀

Deployed Addresses

TokenModule#Token - 0x5FbDB2315678afecb367f032d93F642f64180aa3
```


### テストネットワークへのデプロイの事前準備

* infula のアカウントを作成
* infula のAPIキーを取得:
  * INFURA_API_KEY
* Sepolia テストネットでアカウントに foucet から資金を得ておく
* Sepolia で資金を所有するアカウントの秘密鍵を確認する: 
  * SEPOLIA_PRIVATE_KEY


### 設定変数へのキーの登録

```bash
npx hardhat vars set INFURA_API_KEY

=>
✔ Enter value: · ********************************
The configuration variable has been stored in /home/yamasaki/.config/hardhat-nodejs/vars.json
```

```bash
npx hardhat vars set SEPOLIA_PRIVATE_KEY

=>
✔ Enter value: · ****************************************************************
The configuration variable has been stored in /home/yamasaki/.config/hardhat-nodejs/vars.json

```

### 設定ファイルの修正

プロジェクトルートの `hardhat.config.js` を修正

```js
require("@nomicfoundation/hardhat-toolbox");

// Ensure your configuration variables are set before executing the script
const { vars } = require("hardhat/config");

// Go to https://infura.io, sign up, create a new API key
// in its dashboard, and add it to the configuration variables
const INFURA_API_KEY = vars.get("INFURA_API_KEY");

// Add your Sepolia account private key to the configuration variables
// To export your private key from Coinbase Wallet, go to
// Settings > Developer Settings > Show private key
// To export your private key from Metamask, open Metamask and
// go to Account Details > Export Private Key
// Beware: NEVER put real Ether into testing accounts
const SEPOLIA_PRIVATE_KEY = vars.get("SEPOLIA_PRIVATE_KEY");

module.exports = {
  solidity: "0.8.24",
  networks: {
    sepolia: {
      url: `https://sepolia.infura.io/v3/${INFURA_API_KEY}`,
      accounts: [SEPOLIA_PRIVATE_KEY],
    },
  },
};
```

### Sepoliaへのデプロイ

```bash
npx hardhat ignition deploy ./ignition/modules/Token.js --network sepolia

=>
✔ Confirm deploy to network sepolia (11155111)? … yes
Hardhat Ignition 🚀

Deploying [ TokenModule ]

Batch #1
  Executed TokenModule#Token

[ TokenModule ] successfully deployed 🚀

Deployed Addresses

TokenModule#Token - 0x2E911f3975D1c08aF0DF1cC64EDf2267701626b0

```