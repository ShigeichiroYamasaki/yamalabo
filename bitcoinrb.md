# bitcoinrb

bitcoin core API をRuby から操作する rubygems

## インストール方法

### Ruby言語のインストール

[Ruby インストール](https://github.com/ShigeichiroYamasaki/yamalabo/blob/master/ruby.md)

### ubuntuでのbitcoinrbのインストール

```bash
sudo apt install -y libleveldb-dev
gem install bitcoinrb
```


### MacOSXでのbitcoinrbのインストール

```bash
brew install --build-from-source leveldb
gem install bitcoinrb
```

## bitcoin core を起動しておく

bitcoin core をAPI を介して利用する。

bitcoind でも. bitcoin-qt でもsnap のbitocoin-core でもよい

接続するネットワークを意識してポート番号などを設定する（以下ではsignetを想定）


###  irbで確認

```bash
irb
```

## bitcoin core RPC API の利用

```ruby
require 'bitcoin'
require 'net/http'
require 'json'
include Bitcoin
include Bitcoin::Opcodes

Bitcoin.chain_params = :signet

HOST="localhost"
PORT=38332          # mainnetの場合は 8332
RPCUSER="hoge"      # bitcoin core RPCユーザ名
RPCPASSWORD="hoge"  # bitcoin core パスワード

# bitcoin core RPC を利用するメソッド
def bitcoinRPC(method, params)
    http = Net::HTTP.new(HOST, PORT)
    request = Net::HTTP::Post.new('/')
    request.basic_auth(RPCUSER, RPCPASSWORD)
    request.content_type = 'application/json'
    request.body = { method: method, params: params, id: 'jsonrpc' }.to_json
    JSON.parse(http.request(request).body)["result"]
end
```

## Bitcoinの送金


```ruby
# 送金先アドレスと送金金額を指定して送金

def send_bitcoin(addr, amount, fee)
    balance = bitcoinRPC('getbalance', [])
    if balance < (amount+fee) then
        puts "error (Not enough funds)"
    else
        # 送金金額＋手数料をぎりぎり上回るUTXOリスト
        utxos = consuming_utxos(amount+fee)
        # inputの資金
        fund = utxos.map{|utxo|utxo["amount"]}.sum
        # おつり
        change = fund-amount-fee 
        # おつり用アドレス
        addrChange =  bitcoinRPC('getnewaddress', [])
        # トランザクションの作成
        tx = Bitcoin::Tx.new
        # input
        utxos.each{|utxo|tx.in << Bitcoin::TxIn.new(out_point: Bitcoin::OutPoint.from_txid(utxo["txid"], utxo["vout"],))}
        # 送金用output
        tx.out << Bitcoin::TxOut.new(value:  (amount*(10**8)).to_i, script_pubkey: Bitcoin::Script.parse_from_addr(addr))
        # おつり用output
        tx.out << Bitcoin::TxOut.new(value:  (change*(10**8)).to_i, script_pubkey: Bitcoin::Script.parse_from_addr(addrChange))
        # 各inputへの署名
        utxos.each.with_index{|utxo,index|
            # UTXOのscriptPubKey をオブジェクト化する
            script_pubkey = Bitcoin::Script.parse_from_payload(utxo["scriptPubKey"].htb)
            # scriptPubKey の送金先アドレス
            addr = script_pubkey.to_addr
            # 送金先アドレスの秘密鍵（署名鍵）
            priv = bitcoinRPC('dumpprivkey', [addr])
            # 署名鍵オブジェクト
            key = Bitcoin::Key.from_wif(priv)
            # UTXOの金額
            satoshi = (utxo["amount"]*(10**8)).to_i
            case script_pubkey.type
            # P2WPKH
            when "witness_v0_keyhash"
                # トランザクションのハッシュ値を計算
                sighash = tx.sighash_for_input(index, script_pubkey, sig_version: :witness_v0, amount: satoshi)
                # トランザクションへの署名＋署名タイプ情報を付加
                sig = key.sign(sighash) + [Bitcoin::SIGHASH_TYPE[:all]].pack('C')
                # witness scriptの追加
                tx.in[index].script_witness.stack << sig
                # 公開鍵の追加
                tx.in[index].script_witness.stack << key.pubkey.htb
            end
        }
        return bitcoinRPC('sendrawtransaction', [tx.to_hex])
    end
end

# 送金金額によって送金で消費するUTXOの選定する
def consuming_utxos(amount)
  unspent = bitcoinRPC('listunspent', [])
  # 消費可能状態のUTXOの選定
  spendable_utxos = unspent.select{|t|t["spendable"]==true}
  # UTXOを金額の昇順にソートする
  sorted_utxos = spendable_utxos.sort_by{|x|x["amount"]}
  # amoutを上回るぎりぎりのUTXOのリスト
  utxos=[]
  begin
      utxos << sorted_utxos.shift
      balance = utxos.reduce(0){|s,t|s+=t["amount"]}
  end until balance >= amount
  return utxos
end


# テスト
addr = "tb1qcfxtwcyflcj36fajurk5wvjwfuggyzzv2qduxd"
amount = 0.01
fee = 0.0002
txid = send_bitcoin(addr, amount, fee)
```

