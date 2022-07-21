# tapyrusrb

最終更新 2022/07/20 Shigeichiro Yamasaki

Tapyrus API をRuby から操作する rubygemsの基本

## tapyrusrb WiKi

開発者の安土さんによる Wikiです。
基本的な使用法は、ここでわかります。

[https://github.com/chaintope/tapyrusrb/wiki](https://github.com/chaintope/tapyrusrb/wiki)

## tapyrusrb のクラスとメソッドの一覧

[https://www.rubydoc.info/gems/tapyrus/0.3.0](https://www.rubydoc.info/gems/tapyrus/0.3.0)

## インストール方法

###  install LevelDB

for Ubuntu

```bash
sudo apt-get install libleveldb-dev
```

for Mac

```bash
brew install --build-from-source leveldb
```

共通

```bash
gem install leveldb-ruby
```

```bash
gem install tapyrus
gem install tapyrus-rpc
gem install tapyrus-api-client
```

## tapyrus core を起動しておく

