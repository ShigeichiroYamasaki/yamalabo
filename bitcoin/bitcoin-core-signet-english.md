# Bitcoin core Signet Node construction

## install method

### MacOSX

Download the dmg format installer from the [Bitcoin core download site] (https://bitcoin.org/ja/download)


#### Install bitcoin core with the installer and launch the app

* Drag and drop to install in app folder
* Click the lock icon in "Security & Privacy" in "System Preferences" and enter the password.
* Give "permission to execute downloaded application" to bitcoin core

#### Launch the app

Start the bitcoin core application in the "application folder"

### ubuntu 20.04LTS

#### Install with snap

```bash
sudo snap install bitcoin-core
```

## Launch the bitcoin-qt app

```
bitcoin-core.qt
```

#### Command interface when installing bitcoin core with snap

* bitcoin-core.daemon: Daemon startup (bitcoind)
* bitcoin-core.cli: Client (bitcoin-cli)


## Edit (create) bitcoind configuration file

Configuration file when installing with snap

```
 ~ / snap / bitcoin-core / common / .bitcoin / bitcoin.conf
```

### Edit the configuration file (bitcoin.conf) from bitcoin-qt (GUI)

* MacOSX: "preferences" in the "File" menu ..
* ubuntu: "Options" in the "Settings" menu
* Click the "Open Configuration File" button
* Create and save the configuration file as follows

```
signet = 1
txindex = 1
daemon = 1
server = 1
rest = 1
[signet]
rpcuser = hoge
rpcpassword = hoge
rpcport = 38332
port = 38333
fallbackfee = 0.0002
```

* Click "OK"

## Quit bitcoin core and start it again

Success if the icon is light yellow-green

* Create your own wallet with bitcoin-qt

## Obtaining test coins

[https://signet.bc-2.jp/] (https://signet.bc-2.jp/)