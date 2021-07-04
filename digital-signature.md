# デジタル署名プログラミング

## PKI

### CA作成

MacOSX の場合

```bash
cd /System/Library/OpenSSL/misc
cp CA.pl ~/
cd ~
./CA.pl -newca
```

* C＝JP
* ST=Fukuoka
* L=Iizuka
* O=Kindai
* OU=HOSE
* CN=Joho CA

### リクエスト作成

```bash
openssl genrsa 2048 > yamasaki.key
openssl req -new -key yamasaki.key -out yamasaki.csr
```

* C＝JP
* ST=Fukuoka
* L=Iizuka
* O=Kindai
* OU=HOSE
* CN=Shigeichiro Yamasaki

### 公開鍵証明書の作成

```bash
openssl ca -in yamasaki.csr -out yamasaki.crt
```

### CAの公開鍵証明書

```bash
cp demoCA/cacert.pem ./
```

### 自分の公開鍵証明書の検証

```bash
openssl verify -CAfile cacert.pem yamasaki.crt
yamasaki.crt: OK
```

## S/MIME署名

署名作成

```bash
openssl smime -sign -in test.txt -signer yamasaki.crt -inkey yamasaki.key -out test.p7
```

署名検証

```bash
openssl smime -verify -in test.p7 -recip yamasaki.crt -CAfile cacert.pem
これはメッセージです

Verification successful
```

## CMS

署名作成

```bash
openssl cms -sign -in test.txt -signer yamasaki.crt -inkey yamasaki.key -out test.cms
```

署名検証

```bash
openssl cms -verify -in test.p7 -recip yamasaki.crt -CAfile cacet.pem
これはメッセージです

Verification successful
```

### Ruby

```
irb
require 'openssl'
```

証明書の取り込み

```ruby
cacert=OpenSSL::X509::Certificate.new(File.read('cacert.pem'))
yamasakicert=cacert=OpenSSL::X509::Certificate.new(File.read('cacert.pem'))

```

## JWS

## Ad

## JAdES
