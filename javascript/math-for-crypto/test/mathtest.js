const chai = async ()=>{return await import('chai')};
const math = require('../math.js');

// 階乗関数
describe('fact関数のテスト', function () {
  describe('#fact', function () {
        it('fact(0)= 1', async function () {
		let c = await chai();
		let assert = c.assert;
                assert.equal(math.fact(0) ,1);
        });
        it('fact(3)= 6', async function () {
                let c = await chai();
                let assert = c.assert;
                assert.equal(math.fact(3) ,6);
        });
  });
});

// Bigint の階乗関数
describe('factB関数のテスト', function () {
  describe('#factB', function () {
        it('factB(0n)= 1n', async function () {
                let c = await chai();
                let assert = c.assert;
                assert.equal(math.factB(0n) ,1n);
        });
        it('factB(3n)= 6n', async function () {
                let c = await chai();
                let assert = c.assert;
                assert.equal(math.factB(3n) ,6n);
        });
  });
});

// 範囲オブジェクト
describe('range関数のテスト', function () {
  describe('#range', function () {
        it('range(1,3)= [1,2,3]', async function () {
                let c = await chai();
                let assert = c.assert;
                assert.equal(math.range(1,3).toString ,[1,2,3].toString);
        });
  });
});  

// イテレータ階乗関数
describe('factR関数のテスト', function () {
  describe('#factR', function () {
        it('factR(0)= 1', async function () {
                let c = await chai();
                let assert = c.assert;
                assert.equal(math.factR(0) ,1);
        });
        it('factR(3)= 6', async function () {
                let c = await chai();
                let assert = c.assert;
                assert.equal(math.factR(3) ,6);
        });
  });
});

// ユークリッド互除法
describe('gcd関数のテスト', function () {
	// 素数
	let p=11;
	let q=17;
	let r=23;
  describe('#dcg', function () {
	it('a%b==0 のとき、最大公約数は b', async function () {
                let c = await chai();
                let assert = c.assert;
      		assert(math.gcd(p*q,q) == q);
	});
        it('p*q と p*r の最大公約数は p', async function () {
                let c = await chai();
                let assert = c.assert;
                assert(math.gcd(p*q,p*r) == p);
        });
        it('a b が互いに素のとき最大公約数は 1', async function () {
                let c = await chai();
                let assert = c.assert;
                assert(math.gcd(p,q) == 1);         
        });

  });
});

