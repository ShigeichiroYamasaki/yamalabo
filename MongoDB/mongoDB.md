# mongoDB

最終更新　2022/07/27 Shigeichiro Yamasaki

NoSQLデータベース

## インストール

公開GPGキーをインポート

```bash
sudo apt install -y curl

curl -fsSL https://www.mongodb.org/static/pgp/server-4.4.asc | sudo apt-key add -
=>
OK
```

```bash
apt-key list
=>
/etc/apt/trusted.gpg
--------------------
pub   rsa4096 2016-10-05 [SC]
      72EC F46A 56B4 AD39 C907  BBB7 1646 B01B 86E5 0310
uid           [  不明  ] Yarn Packaging <yarn@dan.cx>
sub   rsa4096 2016-10-05 [E]
sub   rsa4096 2019-01-02 [S] [有効期限: 2023-01-24]
sub   rsa4096 2019-01-11 [S] [有効期限: 2023-01-24]

pub   rsa4096 2019-05-28 [SC] [有効期限: 2024-05-26]
      2069 1EEC 3521 6C63 CAF6  6CE1 6564 08E3 90CF B1F5
uid           [  不明  ] MongoDB 4.4 Release Signing Key <packaging@mongodb.com>

/etc/apt/trusted.gpg.d/ubuntu-keyring-2012-archive.gpg
------------------------------------------------------
pub   rsa4096 2012-05-11 [SC]
      790B C727 7767 219C 42C8  6F93 3B4F E6AC C0B2 1F32
uid           [  不明  ] Ubuntu Archive Automatic Signing Key (2012) <ftpmaster@ubuntu.com>

/etc/apt/trusted.gpg.d/ubuntu-keyring-2012-cdimage.gpg
------------------------------------------------------
pub   rsa4096 2012-05-11 [SC]
      8439 38DF 228D 22F7 B374  2BC0 D94A A3F0 EFE2 1092
uid           [  不明  ] Ubuntu CD Image Automatic Signing Key (2012) <cdimage@ubuntu.com>

/etc/apt/trusted.gpg.d/ubuntu-keyring-2018-archive.gpg
------------------------------------------------------
pub   rsa4096 2018-09-17 [SC]
      F6EC B376 2474 EDA9 D21B  7022 8719 20D1 991B C93C
uid           [  不明  ] Ubuntu Archive Automatic Signing Key (2018) <ftpmaster@ubuntu.com>
```

```bash
echo "deb [ arch=amd64,arm64 ] https://repo.mongodb.org/apt/ubuntu focal/mongodb-org/4.4 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-4.4.list
=>
deb [ arch=amd64,arm64 ] https://repo.mongodb.org/apt/ubuntu focal/mongodb-org/4.4 multiverse
```

apt でインストール

```bash
sudo apt update

sudo apt install -y mongodb-org
```

## mongoDB のサービス開始

```bash
sudo systemctl start mongod.service

sudo systemctl status mongod
=>
● mongod.service - MongoDB Database Server
     Loaded: loaded (/lib/systemd/system/mongod.service; disabled; vendor preset: enabled)
     Active: active (running) since Wed 2022-07-27 22:46:59 JST; 10s ago
       Docs: https://docs.mongodb.org/manual
   Main PID: 254136 (mongod)
     Memory: 58.9M
     CGroup: /system.slice/mongod.service
             └─254136 /usr/bin/mongod --config /etc/mongod.conf

 7月 27 22:46:59 signet systemd[1]: Started MongoDB Database Server.
```

ブート時に自動起動

```bash
sudo systemctl enable mongod
```

診断コマンド


```bash
mongo --eval 'db.runCommand({ connectionStatus: 1 })'
=>
MongoDB shell version v4.4.15
connecting to: mongodb://127.0.0.1:27017/?compressors=disabled&gssapiServiceName=mongodb
Implicit session: session { "id" : UUID("6aac6836-9724-4598-a4da-d865f6e75f2f") }
MongoDB server version: 4.4.15
{
	"authInfo" : {
		"authenticatedUsers" : [ ],
		"authenticatedUserRoles" : [ ]
	},
	"ok" : 1
}
```
