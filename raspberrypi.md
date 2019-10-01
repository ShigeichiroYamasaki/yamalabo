# ラズベリーパイノードのインストール方法

### ubuntu 18.04.3 をインストール

* ベアボーン
[https://ubuntu.com/download/desktop/thank-you?country=JP&version=18.04.3&architecture=amd64](https://ubuntu.com/download/desktop/thank-you?country=JP&version=18.04.3&architecture=amd64)

ubuntu-18.04.3-desktop-amd64.iso

* raspberry pi

[http://cdimage.ubuntu.com/releases/bionic/release/ubuntu-18.04.3-preinstalled-server-arm64+raspi3.img.xz](http://cdimage.ubuntu.com/releases/bionic/release/ubuntu-18.04.3-preinstalled-server-arm64+raspi3.img.xz)

ubuntu-18.04.3-preinstalled-server-arm64+raspi3.img.xz

★ダウンロードしたファルをホームディレクトリに移動させておく

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

## ddコマンド

* ベアボーン　（USBメモリ）

```bash
# USBメモリを挿す前のストレージデバイスの確認
diskutil list
# USBメモリを挿したあとのストレージデバイスの確認
diskutil list
# USBメモリのアンマウント
diskutil umoountdisk /dev/disk(n)
```


```bash
cd ~

# /dev/disk(n) ではなく /dev/rdisk(n) にする
sudo dd if=./ubuntu-18.04.3-desktop-amd64.iso of=/dev/rdisk(n) bs=1m
```

* raspberry pi

```bash
sudo dd if=./ubuntu-18.04.3-preinstalled-server-arm64+raspi3.img.xz of=/dev/rdisk(n) bs=1m

### 事前準備

```bash
sudo apt update
sudo apt upgrade -y
```

#### sshのインストール

```bash
sudo apt install ssh -y
```

#### nano のインストール

```bash
sudo apt install nano -y
```

#### expect のインストール

```bash
sudo apt  install expect -y
```

#### Rubyのインストール

```bash
sudo apt  install ruby -y
```

### curlのインストール

```bash
sudo apt install -y curl
```

### Dockerのインストール

```bash
sudo apt install -y \
     apt-transport-https \
     ca-certificates \
     curl \
     software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

sudo add-apt-repository \
     "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
     $(lsb_release -cs) \
     stable"
sudo apt update
```

### ルートキット検出ツール

```bash
sudo apt install -y chkrootkit
```
### そのほかコンパイラなど

```bash
sudo apt install -y build-essential 
sudo apt install -y clang
sudo apt install -y cmake
sudo apt install -y golang
```

### sudoerの設定

パスワード入力なしでsudo が実行できるようにする

自動実行の場合、あらためてパスワードを入れることに意味がないため

```bash
sudo visudo
```

```
...

yamalabo ALL=(ALL) NOPASSWD:ALL
```
## raspberry pi の初期設定

```bash
# ロケール設定
sudo locale-gen ja_JP.UTF-8
sudo dpkg-reconfigure -f noninteractive locales
echo "LANG=ja_JP.UTF-8" | sudo tee /etc/default/locale
sudo timedatectl set-timezone Asia/Tokyo
```

#### スワップファイルの作成

```bash
sudo fallocate -l 1g /swapfile
sudo chmod 600 /swapfile
sudo mkswap /swapfile 
echo '/swapfile swap swap defaults 0 0' | sudo tee -a /etc/fstab
sudo swapon -a
```

そしてrebootする

### bitcoin core のインストール

[./bitcoin-core.md](./bitcoin-core.md)

### lightning network (ptarmigan)のインストール

[./ptarmigan.md](./ptarmigan.md)

### lightning network (Lnd)のインストール

[./Lnd.md](./Lnd.md)

### サイドチェーン Elements のインストール

[./elements.md](./elements.md)

### etherum (parity) のインストール

[./ethereum.md](./ethereum.md)

### IPFS のインストール

[./IPFS.md](./IPFS.md)

### Libra のインストール

[./libra.md](./libra.md)

