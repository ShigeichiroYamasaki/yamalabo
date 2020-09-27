# ベアボーンノードのインストール方法

## ubuntu 18.04.3 をインストールUSBの作成


### インストール元 ubuntu-18.04.3-desktop-amd64.iso

[https://ubuntu.com/download/desktop/thank-you?country=JP&version=18.04.3&architecture=amd64](https://ubuntu.com/download/desktop/thank-you?country=JP&version=18.04.3&architecture=amd64)


ダウンロードしたファイルはxzで圧縮されている

#### xz 圧縮ファイルの解凍方法

macOSX の場合

xz  のインストール

```bash
brew install xz
```

解凍（結構時間がかかります）

```bash
xz -d sudo ubuntu-18.04.3-preinstalled-server-arm64+raspi3.img.xz
```

### ddコマンドでインストールUSBを作成する


```bash
# USBメモリを挿す前のストレージデバイスの確認
diskutil list
```

* /dev/disk(n) のリストを確認

* 空のUSBを挿す

```bash
# USBメモリを挿したあとのストレージデバイスの確認
diskutil list
```

挿したUSBの/dev/disk(n) を確認

```bash

# USBメモリのアンマウント
diskutil umountdisk /dev/disk(n)
```


```bash
cd ~

# /dev/disk(n) ではなく /dev/rdisk(n) にする
sudo dd if=./ubuntu-18.04.3-desktop-amd64.iso of=/dev/rdisk(n) bs=1m
```

## USBでubuntu をインストール

省略


### sudoerの設定

パスワード入力なしでsudo が実行できるようにする

自動実行の場合、あらためてパスワードを入れることに意味がないため

```bash
sudo visudo
```

```
...(既存の設定)

yamalabo ALL=(ALL) NOPASSWD:ALL
```

