# Tapyrus ネットワークの構築

2022/03/26

### 参考文献

* [How to set up new Tapyrus Signer Network](https://github.com/chaintope/tapyrus-signer/blob/master/doc/setup.md)
* [How To configure Tapyrus Signer Network](https://github.com/chaintope/tapyrus-signer/blob/master/doc/configuration.md)
* [ How to start a new tapyrus network?.](https://github.com/chaintope/tapyrus-core/blob/master/doc/tapyrus/getting_started.md#how-to-start-a-new-tapyrus-network-1)
* [Tapyrusネットワークを構築してみる その①](https://zenn.dev/shmn7iii/articles/ff647417bfdf16)

## 概要

Tapyrus ネットワークには、Tapyrus Signer NetworkとTapyrus Core Networkがあります。
どちらもまず最初に「トラステッドセットアップ」が必要です。

トラステッドセットアップでは、各署名者の秘密鍵をすべての署名者と共有する必要があります。
ただし、Tapyrus Signer Networkの各署名者は、他のブロックチェーンシステムと同様に「トラストレスな」ものとして扱われる必要があります。

以下は、n人の署名者とn未満のしきい値tを持つ「トラストレスな」ネットワークで署名者を設定するためのプロトコルを示しています。

Tapyrus Signer Networkの設定をサポートするために、コマンドラインユーティリティtapyrus-setupを提供しています

### Tapyrus Signer Networkのパラメータ

Tapyrus Core Networkのパラメータに加えて、Tapyrus Signer Networkは、ノードごとに次のパラメータを使用します。

#### ノードVSS

ノードVSSは、検証可能な秘密分散シェア（VSS）です。
各署名者は、すべての署名者からのノードVSSを持っている必要があります。
ノードの起動には、このVSSをfederations.toml設定ファイルに含めておく必要があります。

#### ノードシークレットシェア

ノードシークレットシェアは、KeyGenerationProtocolで作成されるシェアです。
各署名者は、ノードシークレットシェアを持っている必要があります。
これは、収集されたノードVSSによって計算されます。
その値は、secp256k1曲線上の秘密鍵の32バイトのデータです。

Tapyrus Signer Networkは、各ブロック生成ラウンドに次の固有のパラメーターを使用します。

#### ブロックVSS

ブロックVSSは、署名発行プロトコルで作成されるVSSです。
各署名者は、ブロックVSSを生成し、相互に交換する必要があります。

#### ブロックシークレットシェア

ブロックシークレットシェアは、署名発行プロトコルで作成されるシェアです。
値は、secp256k1曲線上の秘密鍵の32バイトのデータです。

#### ローカル署名

ローカル署名は、署名発行プロトコルで各署名者ノードで生成される署名です。
ローカル署名を収集することにより、最終的な署名を生成できます。
これは、ラグランジュ補間を計算して、公開鍵を集約することで検証できます。

## 事前準備

* signer マシン３台以上、core用マシン１台以上を用意する
    * tpsig1, tpsig2, tpsig3, tpcore1 とする
* Rustのインストール
* tapyrus signer のインストール
* tapyrus coreのインストール

#### Rustのインストール

```
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

~/.bashrc の最後に以下を追加して source で評価する

```
export PATH="$HOME/.cargo/bin:$PATH"
source ~/.bashrc
```

#### tapyrus-signer のインストール

```bash
cd ~
git clone https://github.com/chaintope/tapyrus-signer

cd tapyrus-signer
cargo build --release
```

~/.bashrc の最後に以下を追加して source で評価する

```
export PATH="$HOME/tapyrus-signer/target/release:$PATH"source ~/.bashrc
```

#### tapyrus coreのインストール

tapyrus core v0.5.0 のインストール方法はここにあります

[Tapyrus core v0.5.0 testnet ノード構築](https://github.com/ShigeichiroYamasaki/yamalabo/blob/master/Tapyrus%20core%20testnet%20node.md)

## Signerネットワーク

#### Signer IDとSigner index

Signerノードは、その公開鍵で識別されます。
また、それぞれが固有のインデックスを持ちます。
そのインデックスは公開鍵を辞書順序で整列した順番になります。

#### しきい値署名スキーマ

Tapyrus Signer Network(TSN)のしきい値署名アルゴリズムは、"Provably Secure Distributed Schnorr Signatures and a (t, n) Threshold Scheme for Implicit Certificates"に準拠しています。
n人の内t人のSchnorr署名があれば署名検証が成功します。

##### VSS（Verifiable Secret Sharing) 検証可能秘密共有

しきい値署名スキーマは、erifiable Secret Sharing（VSS))を利用しています。このため、データ・フォーマットにVSSという用語が使われます。

### ネットワークID

dev mode のデフォルトは、1905960821です。
ここではこれを利用します。

### 集約公開鍵の生成

それぞれで鍵を生成し、鍵を公開鍵の辞書順で並べ替える

```ruby
# Signerノードが3個の場合
n=3
signerkeys = (0..(n-1)).map{`tapyrus-setup createkey`.chomp.split(' ')}.sort_by{|k|k[1]}
# 公開鍵リスト
pubkeys= signerkeys.map{|k|k[1]}
# 秘密鍵リスト
privkeys= signerkeys.map{|k|k[0]}
```

##### ノードVSSの生成

```ruby
node_vss=(0..(n-1)).map{|i|params=(0..(n-1)).reduce(""){|c,j|c+=" --public-key=#{pubkeys[j]}"}
    `tapyrus-setup createnodevss #{params} --private-key=#{privkeys[i]} --threshold=2`.chomp.split("\n").map{|x|x.split(":")}.map{|y|y[1]}}
```

#### 集約公開鍵の生成

```ruby
agg_key=(0..(n-1)).map{|i|params=(0..(n-1)).reduce(""){|c,j|c+=" --vss=#{node_vss[j][i]}"}
    `tapyrus-setup aggregate #{params} --private-key=#{privkeys[i]}`.chomp.split(' ')}
```

##### 集約公開鍵

```ruby
agg_pubkey= agg_key[0][0]
```

##### ノードの集約秘密鍵

```ruby
node_secret_share=(0..(n-1)).map{|i|agg_key[i][1]}
```

#### ジェネシスブロックの生成

```ruby
genesis_block = `tapyrus-genesis -signblockpubkey=#{agg_pubkey}`.chomp
```

#### ジェネシスブロックVSSの生成

```ruby
blockvss = (0..(n-1)).map{|i| params=(0..(n-1)).reduce(""){|c,j|c+=" --public-key=#{pubkeys[j]}"}
    `tapyrus-setup createblockvss #{params} --private-key=#{privkeys[i]} --threshold=2`.chomp.split("\n").map{|z|z.split(':')[1]}}
```

#### ブロックの署名

```ruby
block_sig = (0..(n-1)).map{|i| params=(0..(n-1)).reduce(""){|c,j| c+=" --block-vss=#{blockvss[j][i]}"}
    `tapyrus-setup sign #{params} --aggregated-public-key=#{agg_pubkey} --node-secret-share=#{node_secret_share[i]} --private-key=#{privkeys[i]} --block=#{genesis_block} --threshold=2`.chomp}
```

#### 署名付きジェネシスブロック

マスターノードをノード 0にします

```ruby
node = 0
params1 = (0..(n-1)).reduce(""){|c,j|c+=" --sig #{block_sig[j]}"}
params2 = (0..(n-1)).reduce(""){|c,j|c+=" --block-vss #{blockvss[j][node]}"}
params3 = (0..(n-1)).reduce(""){|c,j|c+=" --node-vss #{node_vss[j][node]}"}
signed_genesis_block = `tapyrus-setup computesig #{params1} --private-key #{privkeys[node]} --block #{genesis_block} #{params2} #{params3} --aggregated-public-key #{agg_pubkey} --node-secret-share #{node_secret_share[node]} --threshold 2`.chomp
```


#### ここまでの「トラステッドセットアップ」処理をまとめたメソッド

```ruby
# ノード数 n しきい値 t

def trusted_setup(n, t)
    signerkeys = (0..(n-1)).map{`tapyrus-setup createkey`.chomp.split(' ')}.sort_by{|k|k[1]}
    pubkeys= signerkeys.map{|k|k[1]}
    privkeys= signerkeys.map{|k|k[0]}
    node_vss=(0..(n-1)).map{|i|params=(0..(n-1)).reduce(""){|c,j|c+=" --public-key=#{pubkeys[j]}"}
        `tapyrus-setup createnodevss #{params} --private-key=#{privkeys[i]} --threshold=#{t}`.chomp.split("\n").map{|x|x.split(":")}.map{|y|y[1]}}
    agg_key=(0..(n-1)).map{|i|params=(0..(n-1)).reduce(""){|c,j|c+=" --vss=#{node_vss[j][i]}"}
        `tapyrus-setup aggregate #{params} --private-key=#{privkeys[i]}`.chomp.split(' ')}
    agg_pubkey= agg_key[0][0]
    node_secret_share=(0..(n-1)).map{|i|agg_key[i][1]}
    genesis_block = `tapyrus-genesis -signblockpubkey=#{agg_pubkey}`.chomp
    blockvss = (0..(n-1)).map{|i| params=(0..(n-1)).reduce(""){|c,j|c+=" --public-key=#{pubkeys[j]}"}
        `tapyrus-setup createblockvss #{params} --private-key=#{privkeys[i]} --threshold=#{t}`.chomp.split("\n").map{|z|z.split(':')[1]}}
    block_sig = (0..(n-1)).map{|i| params=(0..(n-1)).reduce(""){|c,j| c+=" --block-vss=#{blockvss[j][i]}"}
        `tapyrus-setup sign #{params} --aggregated-public-key=#{agg_pubkey} --node-secret-share=#{node_secret_share[i]} --private-key=#{privkeys[i]} --block=#{genesis_block} --threshold=#{t}`.chomp}
    node = 0
    params1 = (0..(n-1)).reduce(""){|c,j|c+=" --sig #{block_sig[j]}"}
    params2 = (0..(n-1)).reduce(""){|c,j|c+=" --block-vss #{blockvss[j][node]}"}
    params3 = (0..(n-1)).reduce(""){|c,j|c+=" --node-vss #{node_vss[j][node]}"}
    signed_genesis_block = `tapyrus-setup computesig #{params1} --private-key #{privkeys[node]} --block #{genesis_block} #{params2} #{params3} --aggregated-public-key #{agg_pubkey} --node-secret-share #{node_secret_share[node]} --threshold #{t}`.chomp
    return [pubkeys, privkeys, node_vss, agg_pubkey, node_secret_share, blockvss, signed_genesis_block]
end
```

実行結果例

```ruby
pubkeys, privkeys, node_vss, agg_pubkey, node_secret_share, blockvss, signed_genesis_block =trusted_setup(3,2)

pubkeys
=> ["02890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa", "038d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39a", "03d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c737"]

privkeys
=> ["Kz7U1cxtmjUfLP2gNfiYs7EER1scBPhEZXBQQxcx6vWCWTpmQr98", "KxVRBhfVSrk4hqkuLQshv2B7dTWbjd9CccrxLZfoDTSgdAnFPPVy", "KxRq4NgLkBa28ofd2HiT1PTPUEBYe9QVP3hdmSsgRdoMaChxFS5o"]

node_vss
=> [["02890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa02890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa0002890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa82e1df9ee86cb2f8f8e03a9f350aaf60301d940a5e8087cc0b21072aad070c4ce9e9c6620e508f4e87b40b9d9d4e0750c7a75058402e25705bb5e23b7c6cb84394b22950ec5a587e467c59e6255ffc2d9aaadb9cd5895d407b686ec13bee8fa47552890cf2e018876f1feaa6d9c9fd213ece0675754cc366edbc5a9d2765c183890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa82e1df9ee86cb2f8f8e03a9f350aaf60301d940a5e8087cc0b21072aad070c4ce9e9c6620e508f4e87b40b9d9d4e0750c7a75058402e25705bb5e23b7c6cb84394b22950ec5a587e467c59e6255ffc2d9aaadb9cd5895d407b686ec13bee8fa47552890cf2e018876f1feaa6d9c9fd213ece0675754cc366edbc5a9d2765c183", "02890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa038d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39a0002890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa82e1df9ee86cb2f8f8e03a9f350aaf60301d940a5e8087cc0b21072aad070c4ce9e9c6620e508f4e87b40b9d9d4e0750c7a75058402e25705bb5e23b7c6cb84394b22950ec5a587e467c59e6255ffc2d9aaadb9cd5895d407b686ec13bee8fa494584ef01e8b0e79d99591c82306a8ea94525e168d5ea991542600bf39ce5ecc890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa82e1df9ee86cb2f8f8e03a9f350aaf60301d940a5e8087cc0b21072aad070c4ce9e9c6620e508f4e87b40b9d9d4e0750c7a75058402e25705bb5e23b7c6cb84394b22950ec5a587e467c59e6255ffc2d9aaadb9cd5895d407b686ec13bee8fa494584ef01e8b0e79d99591c82306a8ea94525e168d5ea991542600bf39ce5ecc", "02890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa03d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c7370002890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa82e1df9ee86cb2f8f8e03a9f350aaf60301d940a5e8087cc0b21072aad070c4ce9e9c6620e508f4e87b40b9d9d4e0750c7a75058402e25705bb5e23b7c6cb84394b22950ec5a587e467c59e6255ffc2d9aaadb9cd5895d407b686ec13bee8fa4b35e14d34a36046c440b38e96c4354b3e9d6b5b7a5708fbbba8fa6e14c36fc15890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa82e1df9ee86cb2f8f8e03a9f350aaf60301d940a5e8087cc0b21072aad070c4ce9e9c6620e508f4e87b40b9d9d4e0750c7a75058402e25705bb5e23b7c6cb84394b22950ec5a587e467c59e6255ffc2d9aaadb9cd5895d407b686ec13bee8fa4b35e14d34a36046c440b38e96c4354b3e9d6b5b7a5708fbbba8fa6e14c36fc15"], ["038d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39a02890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa00028d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39ab127f7edc001d7ca3a9025fee7b4d7f21f194b6722d1a9ce2abc5545eca354315c9158b79323236f3a660e12b0ae5dab0b76d0d1bb450a1eae2f7701e06724181a2530e52960810806d188cdd5bfa70b012fff5112ed21e5a88cdb108de0706d22c08a626efcecb1d5100a769ebccbdceb7cd5bbace41578f4c6565814c8e9008d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39ab127f7edc001d7ca3a9025fee7b4d7f21f194b6722d1a9ce2abc5545eca354315c9158b79323236f3a660e12b0ae5dab0b76d0d1bb450a1eae2f7701e06724181a2530e52960810806d188cdd5bfa70b012fff5112ed21e5a88cdb108de0706d22c08a626efcecb1d5100a769ebccbdceb7cd5bbace41578f4c6565814c8e900", "038d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39a038d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39a00028d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39ab127f7edc001d7ca3a9025fee7b4d7f21f194b6722d1a9ce2abc5545eca354315c9158b79323236f3a660e12b0ae5dab0b76d0d1bb450a1eae2f7701e06724181a2530e52960810806d188cdd5bfa70b012fff5112ed21e5a88cdb108de0706d1f962b3faece306267d572e965e820d16bd3cc3ed0542aca5e376ade25cb37b48d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39ab127f7edc001d7ca3a9025fee7b4d7f21f194b6722d1a9ce2abc5545eca354315c9158b79323236f3a660e12b0ae5dab0b76d0d1bb450a1eae2f7701e06724181a2530e52960810806d188cdd5bfa70b012fff5112ed21e5a88cdb108de0706d1f962b3faece306267d572e965e820d16bd3cc3ed0542aca5e376ade25cb37b4", "038d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39a03d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c73700028d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39ab127f7edc001d7ca3a9025fee7b4d7f21f194b6722d1a9ce2abc5545eca354315c9158b79323236f3a660e12b0ae5dab0b76d0d1bb450a1eae2f7701e06724181a2530e52960810806d188cdd5bfa70b012fff5112ed21e5a88cdb108de0706d1c6bcc1cee9f7412fa9adb5c2d1375c5ec2ac2c1f3c4401bc7a87f6436cd86688d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39ab127f7edc001d7ca3a9025fee7b4d7f21f194b6722d1a9ce2abc5545eca354315c9158b79323236f3a660e12b0ae5dab0b76d0d1bb450a1eae2f7701e06724181a2530e52960810806d188cdd5bfa70b012fff5112ed21e5a88cdb108de0706d1c6bcc1cee9f7412fa9adb5c2d1375c5ec2ac2c1f3c4401bc7a87f6436cd8668"], ["03d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c73702890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa0002d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c7378fd1c8117b9611e8f4daf368e17dedb7f26b1964d179149bcea6e47174c58d99d82a2ea9dafb6842882bd119a5d2e43c221d646ce1eaa68f7dfe79fc3dee090547cb3574c3a41d9a7342b8fcde6aaaca1e4498cee2e6846d6212277db9c33c92c2992cfc7fda4c5c5f57ccae929821a224c07df0c1cf68cbaf846ceda75aad60d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c7378fd1c8117b9611e8f4daf368e17dedb7f26b1964d179149bcea6e47174c58d99d82a2ea9dafb6842882bd119a5d2e43c221d646ce1eaa68f7dfe79fc3dee090547cb3574c3a41d9a7342b8fcde6aaaca1e4498cee2e6846d6212277db9c33c92c2992cfc7fda4c5c5f57ccae929821a224c07df0c1cf68cbaf846ceda75aad60", "03d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c737038d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39a0002d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c7378fd1c8117b9611e8f4daf368e17dedb7f26b1964d179149bcea6e47174c58d99d82a2ea9dafb6842882bd119a5d2e43c221d646ce1eaa68f7dfe79fc3dee090547cb3574c3a41d9a7342b8fcde6aaaca1e4498cee2e6846d6212277db9c33c92612003fc3fa63f28c8abb0c75c9b4eb6bd027c83b21c84131a38b5170b371412d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c7378fd1c8117b9611e8f4daf368e17dedb7f26b1964d179149bcea6e47174c58d99d82a2ea9dafb6842882bd119a5d2e43c221d646ce1eaa68f7dfe79fc3dee090547cb3574c3a41d9a7342b8fcde6aaaca1e4498cee2e6846d6212277db9c33c92612003fc3fa63f28c8abb0c75c9b4eb6bd027c83b21c84131a38b5170b371412", "03d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c73703d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c7370002d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c7378fd1c8117b9611e8f4daf368e17dedb7f26b1964d179149bcea6e47174c58d99d82a2ea9dafb6842882bd119a5d2e43c221d646ce1eaa68f7dfe79fc3dee090547cb3574c3a41d9a7342b8fcde6aaaca1e4498cee2e6846d6212277db9c33c92ffa6dafbff7231f531ff94e0269e7bca0ff357fd51b23f9644bf5bcd3f49bc05d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c7378fd1c8117b9611e8f4daf368e17dedb7f26b1964d179149bcea6e47174c58d99d82a2ea9dafb6842882bd119a5d2e43c221d646ce1eaa68f7dfe79fc3dee090547cb3574c3a41d9a7342b8fcde6aaaca1e4498cee2e6846d6212277db9c33c92ffa6dafbff7231f531ff94e0269e7bca0ff357fd51b23f9644bf5bcd3f49bc05"]]

agg_pubkey
=> "03f5017e07b0e06bdd29f62e62975fdca7f39d8fa3a3292361e44673e071737621"

node_secret_share
=> ["5aac406be1b75195a387c1cc0b1eeaa1945c7d3b34b7a16fd234bf56135316a2", "150e7e2c0cff7e050a16b578e58a18740279c9f26086b8330cc3c2279a9a6951", "cf70bbec3847aa7470a5a925bff546452b45f3903b9e6f3207252385f217fd41"]

blockvss
=> [["02890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa02890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa00020926dcf60059a5da7f15261f30e375d876bb7eaf9e688ade33c0a477e6c1eec66b5cda54ae526cf0adc5e4fb6d3e2ee94fd384260d3afe6e2470fc2090b4501b8bd761d1d9bacfdc3792f3e4f1147bec409ec659727cd2b63440c881d1dafbeaaf69d49e7c33561f0a532c82c776682d86d05ee95a3e5464e09ada75f3c1a4c32134ecc1df2f51fc652912161ebf4262a0c78aa5a3c1979029c7ce6a1963cceb0926dcf60059a5da7f15261f30e375d876bb7eaf9e688ade33c0a477e6c1eec694a325ab51ad930f523a1b0492c1d116b02c7bd9f2c50191db8f03de6f4bac14a33abe08ddd1b22eab898824fe537eec2a203b2c07580db571730b2fd017eb9658d7b5138974eaa5e5b7ddb27dcfba955c6ee4ec81af5a2aa84a2a9fb534970b71bc822d145f4e5acef607a6dcf75a0cc5d249421fddbe442fc1757fa27e7aed", "02890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa038d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39a00020926dcf60059a5da7f15261f30e375d876bb7eaf9e688ade33c0a477e6c1eec66b5cda54ae526cf0adc5e4fb6d3e2ee94fd384260d3afe6e2470fc2090b4501b8bd761d1d9bacfdc3792f3e4f1147bec409ec659727cd2b63440c881d1dafbeaaf69d49e7c33561f0a532c82c776682d86d05ee95a3e5464e09ada75f3c1a4c3788851b18995a6ec51e8788a13b05941e5bb49645a2de0a20fbbf650fe5de8db0926dcf60059a5da7f15261f30e375d876bb7eaf9e688ade33c0a477e6c1eec694a325ab51ad930f523a1b0492c1d116b02c7bd9f2c50191db8f03de6f4bac14a33abe08ddd1b22eab898824fe537eec2a203b2c07580db571730b2fd017eb9658d7b5138974eaa5e5b7ddb27dcfba955c6ee4ec81af5a2aa84a2a9fb534970bad5a8c2c5d8799c21655baefe3bcdf9ce7785e6b2d10cb06a35691827966a6d5", "02890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa03d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c73700020926dcf60059a5da7f15261f30e375d876bb7eaf9e688ade33c0a477e6c1eec66b5cda54ae526cf0adc5e4fb6d3e2ee94fd384260d3afe6e2470fc2090b4501b8bd761d1d9bacfdc3792f3e4f1147bec409ec659727cd2b63440c881d1dafbeaaf69d49e7c33561f0a532c82c776682d86d05ee95a3e5464e09ada75f3c1a4c3cfdbb6a133fbfbdc3ea7defe08a170212aaf0823109a29b3f5b01e37e35804cb0926dcf60059a5da7f15261f30e375d876bb7eaf9e688ade33c0a477e6c1eec694a325ab51ad930f523a1b0492c1d116b02c7bd9f2c50191db8f03de6f4bac14a33abe08ddd1b22eab898824fe537eec2a203b2c07580db571730b2fd017eb9658d7b5138974eaa5e5b7ddb27dcfba955c6ee4ec81af5a2aa84a2a9fb534970be8f8962ba6afe5295db56e38ea82652d091e73943a43d7c916ebad85504ed2bd"], ["038d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39a02890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa0002d7896fe431561c8ce3f9ffa869a678d60aa4e6011e8aa50dd447af2f48147774132e47b841f10a4383f3cf1b70ceb8980c1a75c89901c917e482243fefbb8d7152e68a033da2deb2abd585f756c877ff81609405f0cbb856e8feb800c693b33ea8cec2c2c0de2a8648904088c6c3e146329af9d7a5184d94eccf0defcd7acfceb1d0e92ae8ab2046de8be81e3692ce7a42db65e87e3fb44f5ce66de3f86b7a61d7896fe431561c8ce3f9ffa869a678d60aa4e6011e8aa50dd447af2f48147774ecd1b847be0ef5bc7c0c30e48f314767f3e58a3766fe36e81b7ddbbf10446ebedbeb163848fe4635b55487e3be26ea22db893049b4d95ee8c1b996ab73ea6a098af8fca933e62d04512781cc17f2a4f954e9eae3ffffe85a5a88a8b37fcb7f5cb3b2d872e55bf4c739165943794e3ad290512bf254dc60db8d6a77923afc2cd6", "038d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39a038d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39a0002d7896fe431561c8ce3f9ffa869a678d60aa4e6011e8aa50dd447af2f48147774132e47b841f10a4383f3cf1b70ceb8980c1a75c89901c917e482243fefbb8d7152e68a033da2deb2abd585f756c877ff81609405f0cbb856e8feb800c693b33ea8cec2c2c0de2a8648904088c6c3e146329af9d7a5184d94eccf0defcd7acfce602f72ef1f9ac3e8c6c324a93fe8d67db0cbb2afc9cfa63dece036de43befa15d7896fe431561c8ce3f9ffa869a678d60aa4e6011e8aa50dd447af2f48147774ecd1b847be0ef5bc7c0c30e48f314767f3e58a3766fe36e81b7ddbbf10446ebedbeb163848fe4635b55487e3be26ea22db893049b4d95ee8c1b996ab73ea6a098af8fca933e62d04512781cc17f2a4f954e9eae3ffffe85a5a88a8b37fcb7f5c6ad8104c7c73663368815e1a1fd93c1e802fb7387dd743a0681cd6f482a3d1d7", "038d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39a03d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c7370002d7896fe431561c8ce3f9ffa869a678d60aa4e6011e8aa50dd447af2f48147774132e47b841f10a4383f3cf1b70ceb8980c1a75c89901c917e482243fefbb8d7152e68a033da2deb2abd585f756c877ff81609405f0cbb856e8feb800c693b33ea8cec2c2c0de2a8648904088c6c3e146329af9d7a5184d94eccf0defcd7acfce0e8dfcb3568a678aaefa6134493ede811ebbff77155f982c7cd9ffd88f1279c9d7896fe431561c8ce3f9ffa869a678d60aa4e6011e8aa50dd447af2f48147774ecd1b847be0ef5bc7c0c30e48f314767f3e58a3766fe36e81b7ddbbf10446ebedbeb163848fe4635b55487e3be26ea22db893049b4d95ee8c1b996ab73ea6a098af8fca933e62d04512781cc17f2a4f954e9eae3ffffe85a5a88a8b37fcb7f5c21fd4826138ad79f97ec62f0c6643d6a700e427ea6d2266542cf3656ca4b76d8"], ["03d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c73702890a7e2eb3ba1f4da80827618815a8e30c8d3581b6e44f5733df91edcce038aa0002a9daf6c794bacc6b2b913cbc159a430f1d91f4b22eca440aeb9408ea2fd1fb9e765c2062f96ba2f9ddb5562f2138dc8be0c1e2861fa716ced452e05568c79271317b912c1d243d667f4de7a80eef3b1429bc80c1245d6924a31a9c5d0fc221fe3ce9408cd799b623d5a9d558cac21c752b5d8d9e502c18892c42bba2c6cf014a1b583f16580bc9d0bc88f7c1efddf955c1d6bc43eb0231944f790bbb2a9512dba9daf6c794bacc6b2b913cbc159a430f1d91f4b22eca440aeb9408ea2fd1fb9e89a3df9d06945d06224aa9d0dec723741f3e1d79e058e9312bad1fa9973869be7ec50a31032648acf550d8d132e5eda420ea81bcafbc9e800a00f530d996fa65e4b5b5589f9b929d8e3ba72c183aa5905f114e81c939aebfeba9165f79333fef041f8c9bc4f1735dfead9be303eebcfaa5b3e1d7e1a75d7eee0a1bf2e9d4b43e", "03d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c737038d436a4249620445762cfe479ca2ca3611c3225e48d25d2a58d0a7dcc3a6a39a0002a9daf6c794bacc6b2b913cbc159a430f1d91f4b22eca440aeb9408ea2fd1fb9e765c2062f96ba2f9ddb5562f2138dc8be0c1e2861fa716ced452e05568c79271317b912c1d243d667f4de7a80eef3b1429bc80c1245d6924a31a9c5d0fc221fe3ce9408cd799b623d5a9d558cac21c752b5d8d9e502c18892c42bba2c6cf014a7d71b39dd2397abbcf1943d892ef5f461e4c6e703ea09f4e94831058f2c57719a9daf6c794bacc6b2b913cbc159a430f1d91f4b22eca440aeb9408ea2fd1fb9e89a3df9d06945d06224aa9d0dec723741f3e1d79e058e9312bad1fa9973869be7ec50a31032648acf550d8d132e5eda420ea81bcafbc9e800a00f530d996fa65e4b5b5589f9b929d8e3ba72c183aa5905f114e81c939aebfeba9165f79333fefc17de3c667c0ffa1a753e37154aa0d596b77aaae09fb1f13a6559d900644585a", "03d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c73703d8860abbeb64fa1f6b88f0b34ee389953420446a596cb81261c8f2bb1bc1c7370002a9daf6c794bacc6b2b913cbc159a430f1d91f4b22eca440aeb9408ea2fd1fb9e765c2062f96ba2f9ddb5562f2138dc8be0c1e2861fa716ced452e05568c79271317b912c1d243d667f4de7a80eef3b1429bc80c1245d6924a31a9c5d0fc221fe3ce9408cd799b623d5a9d558cac21c752b5d8d9e502c18892c42bba2c6cf014adf8b28254c672ba6e1a98fef3600c5367ac2209c923f0d08d98d14f6baf5db57a9daf6c794bacc6b2b913cbc159a430f1d91f4b22eca440aeb9408ea2fd1fb9e89a3df9d06945d06224aa9d0dec723741f3e1d79e058e9312bad1fa9973869be7ec50a31032648acf550d8d132e5eda420ea81bcafbc9e800a00f530d996fa65e4b5b5589f9b929d8e3ba72c183aa5905f114e81c939aebfeba9165f79333fef7edc3af10a908be54ffa2affa5655db9768c969d8306406c9ecec0a0527dbb35"]]

signed_genesis_block
=> "01000000000000000000000000000000000000000000000000000000000000000000000005a66e69d80180d20d1696c1826cac46bb37e3facb53e3186cdb21ae380a6c4da218b48aaa8f764a1c969aebf761cd2450f341513ff8285415a4f53eedf29947553f4062012103f5017e07b0e06bdd29f62e62975fdca7f39d8fa3a3292361e44673e07173762140a9a4320d6bae355b35ea280f919eeb18d099aae3c37c2ac9d35422819126dbc3fc1472d442ba2e17de7c60d3b7e1ec6fbfef1dc002c288ccf7c6461f2533358f01010000000100000000000000000000000000000000000000000000000000000000000000000000000000ffffffff0100f2052a010000001976a9140a687529b9ce7f9a85465529678353b78e37ac1788ac00000000"
```

### genesis.<networkid> ファイルの作成

ここではネットワークIDを  1905960821 とします。
このファイルの内容は、signed_genesis_block　です。

genesis.1905960821

```
"010000000000000000000000000000000000000000000000000000000000000000000000f238e3b7024706c5b7cf9ecc729fe7b125415fa83d68db3a58110e3c7792d3abd8b1d56b4990b0f2ea01b78891eb7d056a8aa372a40dda2d6a68ae29f87d9f2d663c406201210301bba8c48c69b3809512c9383939f07f5b6c8466b7c35b4c0a9108493925c1a840321dbb38873e1e83ec98720b15cbd8be5255e9cd2af1930bd2d1a26b420b29ed9cf4359a8fd2363aea4436834873ebb5765d1e06f2a49cdbbe22b4ebb223c8d401010000000100000000000000000000000000000000000000000000000000000000000000000000000000ffffffff0100f2052a010000001976a914c66b54c324b9b4f3a1ae144096384813834faedd88ac00000000"
```


## Tapyrus core ネットワークの起動

[Tapyrus core v0.5.0 testnet ノード構築](https://github.com/ShigeichiroYamasaki/yamalabo/blob/master/Tapyrus%20core%20testnet%20node.md)


#### tapyrus.conf の内容

network-id が1905960821　であることに注意

tapyrus.conf 

```
networkid=1905960821
txindex=1
server=1
rest=1
fallbackfee=0.000001
rpcuser="hoge"
rpcpassword="hoge"
rpcbind=0.0.0.0
rpcallowip=128.0.0.1
addseeder=static-seed.tapyrus.dev.chaintope.com
```

#### genesis.1905960821

nano /home/yamasaki/.tapyrus/genesis.1905960821

```
010000000000000000000000000000000000000000000000000000000000000000000000f238e3b7024706c5b7cf9ecc729fe7b125415fa83d68db3a58110e3c7792d3abd8b1d56b4990b0f2ea01b78891eb7d056a8aa372a40dda2d6a68ae29f87d9f2d663c406201210301bba8c48c69b3809512c9383939f07f5b6c8466b7c35b4c0a9108493925c1a840321dbb38873e1e83ec98720b15cbd8be5255e9cd2af1930bd2d1a26b420b29ed9cf4359a8fd2363aea4436834873ebb5765d1e06f2a49cdbbe22b4ebb223c8d401010000000100000000000000000000000000000000000000000000000000000000000000000000000000ffffffff0100f2052a010000001976a914c66b54c324b9b4f3a1ae144096384813834faedd88ac00000000
```

この設定で tapyrusd を起動しておく

yamasaki@tpsig1:~/.tapyrus$ tapyrus-cli getnewaddress
2022-03-28T10:05:25Z [default wallet] keypool reserve 1
2022-03-28T10:05:25Z [default wallet] keypool keep 1
1NX6AHM8LUwDvtKjLmRcYesoDQSvbNqR6Z
yamasaki@tpsig1:~/.tapyrus$ tapyrus-cli dumpprivkey 1NX6AHM8LUwDvtKjLmRcYesoDQSvbNqR6Z
L1pBhxTXfkszpUHRTC53vb5LfZz8uBFjMGu4KD516Lup624uvbSn


## Tapyrus Signer Networkの設定

#### ２つの設定ファイル

* signer.toml: tapyrus-signerdデーモンにパラメータを渡すための設定
* federations.toml: フェデレーション用のパラメータを提供するためのデータファイル

##### signer.toml

to-address は、マイニングによる報酬の送付先

```
[general]
round-duration = 60
round-limit = 15
log-quiet = true
log-level = "info"
daemon = true
pid = "~/tapyrus-signer.pid"
log-file = "~/tapyrus-signer.log"
skip-waiting-ibd = true

[signer]
to-address = "1Co1dFUN..."
public-key = "033cfe7fa..."
federations-file = "/path/to/federations.toml"

[rpc]
rpc-endpoint-host = "127.0.0.1"
rpc-endpoint-port = 2377
rpc-endpoint-user = "user"
rpc-endpoint-pass = "pass"

[redis]
redis-host = "127.0.0.1"
redis-port =  6379
```
