# bitcoinrbでP2WSHを実装

bitcoinでスマートコントラクトを記述するときの基本は P2WSH です

P2WSHの代表例はマルチシグですが、それ以外のケースも重要です。



## マルチシグアドレスへの送金の例

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
FEE = 0.00002       # 手数料

# bitcoin core RPC を利用するメソッド
def bitcoinRPC(method, params)
    http = Net::HTTP.new(HOST, PORT)
    request = Net::HTTP::Post.new('/')
    request.basic_auth(RPCUSER, RPCPASSWORD)
    request.content_type = 'application/json'
    request.body = { method: method, params: params, id: 'jsonrpc' }.to_json
    JSON.parse(http.request(request).body)["result"]
end

# Bitcoin送金メソッド
    # 送金先アドレス、送金金額

def send_bitcoin(addr, amount)
    # 所持金残高を確認
    balance = bitcoinRPC('getbalance', [])
    if balance < (amount+FEE) then
        puts "error (残高不足)"
    else
        # 送金金額＋手数料をぎりぎり上回るUTXOリストの作成
        utxos = consuming_utxos(amount+FEE)
        # 送金に使用するUTXOの総額
        fund = utxos.map{|utxo|utxo["amount"]}.sum
        # UTXOの総額 - 送金金額 - 手数料 = おつり
        change = fund-amount-FEE
        # おつり用アドレス（UTXOの０番目を受け取ったアドレスを利用する）
        addrChange = utxos[0]["address"]
        # トランザクションの構成（P2WPKH)
        tx = p2wpkh_transaction(addr, amount, utxos, change, addrChange)
        # トランザクションへの署名
        tx = sign_inputs(utxos, tx)
        # ビットコインネットワークへのデプロイ
        return bitcoinRPC('sendrawtransaction', [tx.to_hex])
    end
end


# P2WPKHトランザクションの構成

def p2wpkh_transaction(addr,amount, utxos, change, addrChange)
    # トランザクションのテンプレートの生成
    tx = Bitcoin::Tx.new
    # トランザクションのinputの構成
    tx = make_inputs(tx,utxos)
    # トランザクションのoutputの構成
    tx = make_outputs(tx,amount, addr, change, addrChange)
    return tx
end

# トランザクションのinputの構成

def make_inputs(tx, utxos)
    utxos.each{|utxo|
        # UTXOをinputから参照するための txid と vout としてエンコードする
        outpoint = Bitcoin::OutPoint.from_txid(utxo["txid"], utxo["vout"])
        # エンコードした参照をトランザクションのinputに埋め込む
        tx.in << Bitcoin::TxIn.new(out_point: outpoint)
    }
    return tx
end

# トランザクションのoutputの構成

def make_outputs(tx,amount, addr, change, addrChange)
    # 送金用outputの構成
        # 金額を satoshiの整数に変換
    amount_satoshi = (amount*(10**8)).to_i
        # ビットコインアドレスから p2wpkhのscript_pubkey を生成
    scriptPubKey0 = Bitcoin::Script.parse_from_addr(addr)
        # エンコードしたscript_pubkeyをトランザクションのoutputに埋め込む
    tx.out << Bitcoin::TxOut.new(value: amount_satoshi , script_pubkey: scriptPubKey0)
    # おつり用outputの構成
        # 金額を satoshiの整数に変換
    change_satoshi =  (change*(10**8)).to_i
        # ビットコインアドレスから p2wpkhのscript_pubkey を生成
    scriptPubKey1 = Bitcoin::Script.parse_from_addr(addrChange)
        # エンコードしたscript_pubkeyをトランザクションのoutputに埋め込む
    tx.out << Bitcoin::TxOut.new(value: change_satoshi, script_pubkey: scriptPubKey1)
    return tx
end

# トランザクションへの署名

def sign_inputs(utxos, tx)
    utxos.each.with_index{|utxo,index|
        # UTXOのscriptPubKey をオブジェクト化する
        script_pubkey = Bitcoin::Script.parse_from_payload(utxo["scriptPubKey"].htb)
        # scriptPubKey の送金先アドレス
        myaddr = script_pubkey.to_addr
        # UTXOの送付先アドレスの秘密鍵（署名鍵）
        priv = bitcoinRPC('dumpprivkey', [myaddr])
        # 署名鍵オブジェクト
        key = Bitcoin::Key.from_wif(priv)
        # UTXOの金額
        satoshi = (utxo["amount"]*(10**8)).to_i
        case script_pubkey.type
        when "witness_v0_keyhash"   # UTXOがP2WPKHタイプ
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
    return tx
end

# 送金金額＋手数料をぎりぎり上回るUTXOリストの作成

def consuming_utxos(amount)
    # ワレットの未使用のUTXOの一覧を得る
    unspent = bitcoinRPC('listunspent', [])
    # 消費可能状態のUTXOの選定
    spendable_utxos = unspent.select{|t|t["spendable"]==true}
    # UTXOを金額で昇順にソートする
    sorted_utxos = spendable_utxos.sort_by{|x|x["amount"]}
    # 少額のUTXOから集めて，指定金額を上回るぎりぎりのUTXOのリストを作成する
    utxos=[]
    begin
        utxos << sorted_utxos.shift
        balance = utxos.reduce(0){|s,t|s+=t["amount"]}
    end until balance >= amount
    return utxos
end


# 送金のテスト
    # 引数は，送金先アドレス，送金金額，手数料


amount = 0.01
alice=bitcoinRPC('getnewaddress', [])
    # 送金の実行。実行結果はトランザクションID
txid = send_bitcoin(alice, amount)

```


