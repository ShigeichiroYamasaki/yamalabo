# Ethereum ERC721トークンの作成



[Openzeppelinの Contract Wizard](https://docs.openzeppelin.com/contracts/4.x/wizard) を利用

[REMIX](https://remix.ethereum.org/#optimize=false&runs=200&evmVersion=null&version=soljson-v0.8.7+commit.e28d00a7.js)で EthereumのRopsten テストネットワークにデプロイする

### 前提

* [geth で ropstenネットワークに接続](https://github.com/ShigeichiroYamasaki/yamalabo/blob/master/ethereum-Ropsten.md) faucetを利用して、テスト用のEtherを持つアカウントを準備している
* [Solidity言語仕様を知っている](https://github.com/ShigeichiroYamasaki/yamalabo/blob/master/ethereum-Solidity.md)



## 自分のノードでgethを起動しRemix と接続する

ropsten ネットワーク
--http.corsdomain　オプションでREMIXサイト `https://remix.ethereum.org` を指定して http接続する

```
$ geth --ropsten --syncmode "snap" --datadir "./ropsten" --http --http.corsdomain https://remix.ethereum.org --http.api "eth,net,web3,admin,miner,txpool,personal" --allow-insecure-unlock -- console 2>> ./ropsten/geth_err.log


Welcome to the Geth JavaScript console!

instance: Geth/v1.10.15-stable/darwin-arm64/go1.17.5
coinbase: 0xe56e63c5cc3c062ee39d725e1d241b126e75d3ff
at block: 11960558 (Sun Feb 13 2022 23:44:12 GMT+0900 (JST))
 datadir: /Users/shigeichiroyamasaki/Ethereum/ropsten
 modules: admin:1.0 debug:1.0 eth:1.0 ethash:1.0 miner:1.0 net:1.0 personal:1.0 rpc:1.0 txpool:1.0 web3:1.0

To exit, press ctrl-d or type exit
> 
```

### デプロイ用アカウントに ether の残高があることを確認

デプロイなどにガス代が必要

NFTのテストのために、他にも3〜４個程度のアカウントを作成しておく


```
> eth.getBalance(eth.accounts[0]) # alice
163109375469575313
> eth.getBalance(eth.accounts[1]) # bob
100937000344379000
> eth.getBalance(eth.accounts[2]) # carol
100000000000000000
> eth.getBalance(eth.accounts[3]) # deivid
100000000000000000
> eth.getBalance(eth.accounts[4]) # taro
0
```

## REMIXの設定

### DEPLOY & RUN TRANSACTIONSの設定

* ENVIRONMENTのネットワークとして `web3 Provider`  を選択する

![](./img/ethereum-erc721-1.png)

* External node request を確認して、OKをクリックする


![](./img/ethereum-erc721-2.png)


* ACCOUNTのところにEtherを所持するアカウントが設定されていることを確認する


![](./img/ethereum-erc721-3.png)

### 確認終了後、REMIXと Ropstenノードの接続をいったん解除する

ENVIRONMENTで、JavaScript(London) などに接続を変更する


## OpenzeppelinのウィザードサイトでNFTのコントラクトを作成する

ERC721ボタンをクリックする

![](./img/ethereum-erc721-4.png)


### ERC721 コントラクトの作成

#### NFTの対象となるデータを決める

ここでは、githubのこのページをトークンの対象にします。

`https://github.com/ShigeichiroYamasaki/yamalabo/blob/master/ethereum-ERC721.md`

#### NFTの名前を決定する

`Yamasaki Lab Token`

### NFTの通貨記号を決める

`YLT`

#### Base URIを設定する

NFTは、対象（Deed=資産の証書）を識別する Token ID が付きます。
このBase URIは、Token ID （資産の識別子）と連接されて  Token URIになります。


```
https://github.com/ShigeichiroYamasaki/yamalabo/blob/master/ethereum-ERC721.md
```

![](./img/ethereum-erc721-5.png)

### features


* Mintable

NFTを発行する権限を持つ

* Burnable

NFTを焼却することができる

* Pausable

異常事態が発生したときなどに、機能を停止状態にできる

* Enumerable

一つのアカウントが所持しているトークンを列挙できる

* URI Storage

Toke ID に対する Token URIを修正できる

### access control

* Ownable

一つのアカウントがNFTへのすべての権限を持つ

* Roles

アクションごとに権限を分離できる

### upgradeability

スマートコントラクトは基本的にアップデート不可能
しかし、アップグレード用プロキシを設定しておけば可能になる。

* Transparen

複雑なプロキシを利用できる

* UUPS

簡単なプロキシしか利用できない

## 作成したERC721のスマートコントラクトをREMIXを使ってデプロイする

Open in Remix ボタンをクリックする

![](./img/ethereum-erc721-6.png)

### REMIXでSolidity のコードをコンパイルする

![](./img/ethereum-erc721-7.png)

### REMIX で ENVIRONMENTを Web3 Provider を選択し、Ropstenに接続

### CONTRACTで　`YamasakiLabToken-...` のような自分のコントラクトを選択

### アカウントのパスワードを入れて署名鍵をアンロックする

```
> personal.unlockAccount(eth.accounts[0])
Unlock account 0xe56e63c5cc3c062ee39d725e1d241b126e75d3ff
Passphrase: 
true
```

### Deployボタンを選択する

![](./img/ethereum-erc721-8.png)

##  コントラクト

```
// SPDX-License-Identifier: MIT
pragma solidity ^0.8.2;

import "@openzeppelin/contracts@4.4.2/token/ERC721/ERC721.sol";
import "@openzeppelin/contracts@4.4.2/token/ERC721/extensions/ERC721Burnable.sol";
import "@openzeppelin/contracts@4.4.2/access/Ownable.sol";

contract YamasakiLabToken is ERC721, ERC721Burnable, Ownable {
    constructor() ERC721("YamasakiLabToken", "YLT") {}

    function _baseURI() internal pure override returns (string memory) {
        return "https://github.com/ShigeichiroYamasaki/yamalabo/blob/master/ethereum-ERC721.md";
    }

    function safeMint(address to, uint256 tokenId) public onlyOwner {
        _safeMint(to, tokenId);
    }
}
```

### 実際にNFTを発行する


アカウントの署名鍵をアンロックする

```
>  personal.unlockAccount(eth.accounts[0])
Unlock account 0xe56e63c5cc3c062ee39d725e1d241b126e75d3ff
Passphrase: 
true
```

#### Remixの Deployed Contractsのデプロイしたコントラクトを確認する

　＞　マークをクリックしてコントラクトへの操作を展開する

![](./img/ethereum-erc721-A.png)

#### NFTの新規発行を行う

展開されたコントラクトへの操作の中で　safeMint　関数によるトランザクションをブロードキャストして新規NFTを発行する。

引数は、発行するNFT所有者のアドレスとなる　eth.accounts[1]のアドレスと tokenidになる整数 12345 

![](./img/ethereum-erc721-9.png)