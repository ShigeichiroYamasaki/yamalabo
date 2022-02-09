# Truffle を使ったSolidity スマートコントラクト開発


スマートコントラクトのテスト駆動開発とデプロイ

## 書籍

「Solidity と Ethereumによる実践スマートコントラクト開発」の内容です

## 事前条件

* EthereumのテストネットであるRopstenのノードを起動
* aliceとbobのEOAアカウントを作成
* aliceとbobのアカウントには、ファウセットから資金を送金済

## 環境

Ubuntu 20.04 /MacOSX
nvm : 0.39.0
node : 16.13.2
npm : 8.1.2
Truffle v5.4.30 (core: 5.4.30)
Solidity - 0.8.11 (solc-js)
Node v16.13.2
Web3.js v1.5.3

## インストール

### nvm (ubuntu)のインストール

```
sudo apt install curl 

curl https://raw.githubusercontent.com/creationix/nvm/master/install.sh | bash
source ~/.profile
```

### nvm (macOSX)のインストール

```
brew install nvm
source $(brew --prefix nvm)/nvm.sh
echo 'source $(brew --prefix nvm)/nvm.sh' >> ~/.zprofile
source .zprofile
```

### Truffleのインストール

```
nvm install 16.13.2 # DO NOT USE >=17.0.0
nvm use 16.13.2
npm i -g truffle  # DO NOT USE sudo
```



#### 作業ディレクトリの作成


```
mkdir greeter
cd greeter
```

## Truffle によるテンプレートの生成

```
truffle init
```

### 作成されたディレクトリとファイル

```
contracts
    Migrations.sol	
migrations		
    1_initial_migration.js
test			
truffle-config.js
```


## Truffleコマンド

* truffle compile
    * ディレクトリ内のコントラクトをコンパイルする
* truffle migrate
    * migrationsディレクトリ内のスクリプトを実行してコンパイル済みのコントラクトをデプロイする
* truffle test
    * testディレクトリ内のテストを実行する
* truffle develop 
    * テスト環境の実行
* truffle deploy
    * コードのデプロイ
* 



### Greeter (Hello World! プログラム）の生成

```
truffle create contract Greeter
truffle create test Greeter
truffle create migration Greeter
```

#### 生成されたテンプレート

contracts/Greeter.sol 

```
// SPDX-License-Identifier: MIT
pragma solidity >=0.4.22 <0.9.0;

contract Greeter {
  constructor() public {
  }
}
```

contracts/Migrations.sol 

```
// SPDX-License-Identifier: MIT
pragma solidity >=0.4.22 <0.9.0;

contract Migrations {
  address public owner = msg.sender;
  uint public last_completed_migration;

  modifier restricted() {
    require(
      msg.sender == owner,
      "This function is restricted to the contract's owner"
    );
    _;
  }

  function setCompleted(uint completed) public restricted {
    last_completed_migration = completed;
  }
}
```

migrations/1_initial_migration.js 

```
const Migrations = artifacts.require("Migrations");

module.exports = function (deployer) {
  deployer.deploy(Migrations);
};
```

migrations/1643812291_greeter.js 

```
module.exports = function(_deployer) {
  // Use deployer to state migration tasks.
};
```

test/greeter.js 

```
const Greeter = artifacts.require("Greeter");

/*
 * uncomment accounts to access the test accounts made available by the
 * Ethereum client
 * See docs: https://www.trufflesuite.com/docs/truffle/testing/writing-tests-in-javascript
 */
contract("Greeter", function (/* accounts */) {
  it("should assert true", async function () {
    await Greeter.deployed();
    return assert.isTrue(true);
  });
});

```

## Migration Fileの編集



migrations/1643812291_greeter.js 

```
const Greeter = artifacts.require("Greeter");
module.exports = function(_deployer) {
  _deployer.deploy(Greeter);
};
```

## Testの実行

```
truffle test
```

```
Compiling your contracts...
===========================
✔ Fetching solc version list from solc-bin. Attempt #1
✔ Downloading compiler. Attempt #1.
> Compiling ./contracts/Greeter.sol
> Compiling ./contracts/Migrations.sol
> Compilation warnings encountered:

    Warning: Visibility for constructor is ignored. If you want the contract to be non-deployable, making it "abstract" is sufficient.
 --> project:/contracts/Greeter.sol:5:3:
  |
5 |   constructor() public {
  |   ^ (Relevant source part starts here and spans across multiple lines).


> Artifacts written to /tmp/test--34660-DoJr6FDMAt00
> Compiled successfully using:
   - solc: 0.8.11+commit.d7f03943.Emscripten.clang



  Contract: Greeter
    ✓ should assert true


  1 passing (69ms)

```


## Hello World!コントラクトの作成

contracts/Greeter.sol

```
// SPDX-License-Identifier: MIT
pragma solidity >=0.4.22 <0.9.0;
// remove constractor
contract Greeter {
  function greet() external pure returns(string memory){
    return "Hello, World!";
  }
}
```

test/greeter.js

```
const Greeter = artifacts.require("Greeter");
contract("Greeter", function (/* accounts */) {
  it("should assert true", async function () {
    await Greeter.deployed();
    return assert.isTrue(true);
  });
describe("greet()", () => {
    it("returns 'Hello, World!'", async () => {
      const greeter = await Greeter.deployed();
      const expected = "Hello, World!";
      const actual = await greeter.greet();
      assert.equal(actual, expected, "greeted with'Hello, World!'");
    })
  })
});
```

#### Truffle test を再実行

```
truffle test
```

```
Compiling your contracts...
===========================
> Compiling ./contracts/Greeter.sol
> Compiling ./contracts/Migrations.sol

ParserError: Expected identifier but got '/'
 --> project:/contracts/Greeter.sol:9:5:
  |
9 | test/greeter.js
  |     ^

Compilation failed. See above.
Truffle v5.4.31 (core: 5.4.31)
Node v16.13.2
yamasaki@ethereum:~/greeter$ emacs contracts/Greeter.sol
yamasaki@ethereum:~/greeter$ emacs test/greeter.js 
yamasaki@ethereum:~/greeter$ truffle test

Compiling your contracts...
===========================
> Compiling ./contracts/Greeter.sol
> Compiling ./contracts/Migrations.sol
> Artifacts written to /tmp/test--34768-1nPKTttAxx4N
> Compiled successfully using:
   - solc: 0.8.11+commit.d7f03943.Emscripten.clang



  Contract: Greeter
    ✓ should assert true
    greet()
      ✓ returns 'Hello, World!'


  2 passing (79ms)

```


##  メッセージを変更可能にする


contracts/Greeter.sol

```
// SPDX-License-Identifier: MIT
pragma solidity >=0.4.22 <0.9.0;

contract Greeter {
  string private _greeting = "Hello, World!";

  function greet() external view returns (string memory) {
    return _greeting;
  }

  function setGreeting(string calldata greeting) external {
    _greeting = greeting;
  }
}
```

test/greeter.js

```
const Greeter = artifacts.require("Greeter");
contract("Greeter", function (/* accounts */) {
  it("should assert true", async function () {
    await Greeter.deployed();
    return assert.isTrue(true);
  });
  describe("greet()", () => {
    it("returns 'Hello, World!'", async () => {
      const greeter = await Greeter.deployed();
      const expected = "Hello, World!";
      const actual = await greeter.greet();
      assert.equal(actual, expected, "greeted with 'Hello, World!'");
    });
  });
});
contract("Greeter: update greeting", function (/* accounts */) {
  it("sets greeting to passed in string", async function () {
    const greeter = await Greeter.deployed();
    const expected = "Hi there!";
    await greeter.setGreeting(expected);
    const actual = await greeter.greet();
    assert.equal(actual, expected, "greeing updated'");
  });
});

```

```
truffle test
```

```

Compiling your contracts...
===========================
> Compiling ./contracts/Greeter.sol
> Compiling ./contracts/Migrations.sol
> Artifacts written to /tmp/test--34768-1nPKTttAxx4N
> Compiled successfully using:
   - solc: 0.8.11+commit.d7f03943.Emscripten.clang



  Contract: Greeter
    ✓ should assert true
    greet()
      ✓ returns 'Hello, World!'


  2 passing (79ms)

yamasaki@ethereum:~/greeter$ emacs contracts/Greeter.sol
yamasaki@ethereum:~/greeter$ emacs test/greeter.js
yamasaki@ethereum:~/greeter$ truffle test

Compiling your contracts...
===========================
> Compiling ./contracts/Greeter.sol
> Compiling ./contracts/Migrations.sol
> Artifacts written to /tmp/test--34832-oKILYU38RfM4
> Compiled successfully using:
   - solc: 0.8.11+commit.d7f03943.Emscripten.clang



  Contract: Greeter
    ✓ should assert true
    greet()
      ✓ returns 'Hello, World!'

  Contract: Greeter: update greeting
    ✓ sets greeting to passed in string (1275ms)


  3 passing (1s)
```


## 所有者を設定する


contracts/Greeter.sol

```
// SPDX-License-Identifier: MIT
pragma solidity >=0.4.22 <0.9.0;

contract Greeter {
  string private _greeting = "Hello, World!";
  address private _owner;

  constructor() {
    _owner = msg.sender;
  }

  modifier onlyOwner() {
    require(msg.sender == _owner, "Ownable: caller is not the owner");
    _;
  }

  function greet() external view returns (string memory) {
    return _greeting;
  }

  function setGreeting(string calldata greeting) external onlyOwner {
    _greeting = greeting;
  }

  function owner() public view returns (address) {
    return _owner;
  }
}

```

test/greeter.js

```
const Greeter = artifacts.require("Greeter");

contract("Greeter: update greeting", (accounts) => {
  describe("when  message is sent by the owner", () => {
    it("sets greeting is sent by the owner", async () => {
      const greeter = await Greeter.deployed();
      const expected = "The owner changed the mssage";
      await greeter.setGreeting(expected);
      const actual = await greeter.greet();
      assert.equal(actual, expected, "greeting updated");
    });
  });
  describe("when message is sent by another account", () => {
    it("does not set the greeting", async () => {
      const greeter = await Greeter.deployed();
      const expected = await greeter.greet();
      try {
        await greeter.setGreeting("Not the owner", { from: accounts[1] });
      } catch (err) {
        const errorMessage = "Ownable: caller is not the owner";
        assert.equal(err.reason, errorMessage, "greeting should not update");
        return;
      }
      assert(false, "greeting shoud not update");
    });
  });
});
```

```
truffle test
```

```

Compiling your contracts...
===========================
> Compiling ./contracts/Greeter.sol
> Compiling ./contracts/Migrations.sol
> Artifacts written to /tmp/test--34878-FPH7Mv7yAuvo
> Compiled successfully using:
   - solc: 0.8.11+commit.d7f03943.Emscripten.clang



  Contract: Greeter: update greeting
    when  message is sent by the owner
      ✓ sets greeting is sent by the owner (1296ms)
    when message is sent by another account
      ✓ does not set the greeting (1268ms)


  2 passing (3s)
```

## OpenZeppelinの利用

```
npm init -y

npm i openzeppelin-solidity
```


contracts/Greeter.sol

```
// SPDX-License-Identifier: MIT
pragma solidity >=0.4.22 <0.9.0;
import "openzeppelin-solidity/contracts/access/Ownable.sol";

contract Greeter is Ownable {
  string private _greeting = "Hello, World!";

  function greet() external view returns (string memory) {
    return _greeting;
  }

  function setGreeting(string calldata greeting) external onlyOwner {
    _greeting = greeting;
  }
}
```

```
truffle test
```

```
Compiling your contracts...
===========================
> Compiling ./contracts/Greeter.sol
> Compiling ./contracts/Migrations.sol
> Compiling openzeppelin-solidity/contracts/access/Ownable.sol
> Compiling openzeppelin-solidity/contracts/utils/Context.sol
> Artifacts written to /tmp/test--34953-reeU3mRflyCJ
> Compiled successfully using:
   - solc: 0.8.11+commit.d7f03943.Emscripten.clang



  Contract: Greeter: update greeting
    when  message is sent by the owner
      ✓ sets greeting is sent by the owner (262ms)
    when message is sent by another account
      ✓ does not set the greeting (1224ms)


  2 passing (2s)

```

## デプロイ

