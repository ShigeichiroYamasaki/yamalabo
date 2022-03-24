# Tapyrus core testnet ノード構築

以下 ubuntu 20.04LTS

## snap を使ったtapyrus coreのインストール

#### snap のinstall

```bash
sudo apt install -y snap
sudo apt install -y snapd
```

#### tapyrus coreのインストール

```bash
sudo nsap install tapyurus-core
```

## tapyurus testnet node 設定

とりあえず  tapyrus coreを起動する

設定ができていなので、すぐにエラーで停止する
しかし、これでsnapなどのディレクトリが作成される

```bash
tapyrus-core.daemon
```

