# bitcoin regtest 構築

ubutu 20.04LTS

## bitcoin core インストール

### インストールスクリプト

```bash
nano install-bitcoincore-regtest.sh
```


```bash
#!/bin/bash
sudo apt update
sudo apt upgrade -y
sudo apt install snapd
sudo snap install bitcoin-core
cd ~snap/bitcoin-core/common/
mkdir .bitcoin

cat << EOF > ~snap/bitcoin-core/common/.bitcoin/bitcoin.conf
regtest=1
txindex=1
server=1
rest=1
rpcuser="user"
rpcpassword="password"
EOF
```


### インストール実行

```bash
chmod a+x install-bitcoincore-regtest.sh
./install-bitcoincore-regtest.sh


```

### デーモン起動


```bash
bitcoin-core.daemon -regtest
```

### クライアント接続

```bash
bitcoin-core.cli -regtest help
```

### 基本操作

```bash
# aliceのアドレスを生成
bitcoin-core.cli -regtest getnewaddress 'alice'

# ラベルからアドレスを知る
bitcoin-core.cli -regtest getaddressesbylabel alice

# マイニングをする (ハッシュ値を50個作成する）
bitcoin-core.cli -regtest generatetoaddress  50 bcrt1qc6fn0af7nh649qp8dmlmhr6al3pnvchlkhvevg

# 10分以上後　残高が増えていることを確認する
bitcoin-core.cli -regtest getbalance
```

## ubuntu へのRuby インストール

```bash
sudo apt purge ruby rbenv ruby-build
rm -rf ~/.rbenv
sudo apt install -y git
sudo apt install -y gcc

cd ~
git clone https://github.com/rbenv/rbenv.git ~/.rbenv
git clone https://github.com/rbenv/ruby-build.git ~/.rbenv/plugins/ruby-build
sudo apt install -y rbenv

cat << EOF >> .bashrc
export PATH="$HOME/.rbenv/bin:$PATH"
eval "$(rbenv init -)"
EOF

source .bashrc

rbenv install -l

# 最新バージョンをインストールする 2.7.2 の場合
rbenv install 2.7.2
rbenv global 2.7.2
```

## bitcoinrbの利用

```bash
gem install bitcoinrb
```

### irb

```
require 'bitcoin'

```