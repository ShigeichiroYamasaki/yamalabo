# javaScriptの暗号モジュールの利用

2024/05/10
Shigeichiro Yamasaki

## Cryptoモジュール

Node.js の暗号機能モジュール． OpenSSL のハッシュ関数，HMAC，暗号化・復号，署名生成・検証機能へのラッパー．
JavaScript から OpenSSLの暗号機能を利用できるようになります．

```js
> const crypto = require("crypto");
```

### 暗号学的ハッシュ関数

```js
//暗号学的ハッシュ関数の一覧
> crypto.getHashes()
[
  'RSA-MD5',
  'RSA-RIPEMD160',
  'RSA-SHA1',
  'RSA-SHA1-2',
  'RSA-SHA224',
  'RSA-SHA256',
  'RSA-SHA3-224',
    ...

```

### sha256 のハッシュ値を求める

```js
> msg="hello";
> crypto.createHash('sha256').update(msg).digest('hex');
'2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824'

// 以上を関数にする
> const sha256=(msg)=>{
    return crypto
      .createHash('sha256')
      .update(msg)
      .digest('hex');
  }
> sha256('hello')
'2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824'
```

先頭 40bit に制限した弱い暗号学的ハッシュ関数

```js
> const sha40=(msg)=>{
    return crypto
      .createHash('sha256')
      .update(msg)
      .digest('hex')
      .slice(0,9);
  }
> sha40('hello')
'2cf24dba5'
```

## ρ法による暗号学的ハッシュ関数の衝突ペアの探索

```js
const rho=(h0)=>{
  let kame=h0
  let usagi=h0
  do {
    kame = sha40(kame)
    usagi = sha40(sha40(usagi))
  } while(kame != usagi)
  let goryu=kame
  kame=h0
  let kame_p
  let goryu_p
  do {
    kame_p=kame
    goryu_p=goryu
    kame = sha40(kame)
    goryu = sha40(goryu)
  } while(goryu != kame)
  return [kame_p,goryu_p]
}
```