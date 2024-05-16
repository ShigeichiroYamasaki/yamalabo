// 階乗
const fact=(n)=>{
  if(n==0) {
    return 1;
  } else {
    return n*fact(n-1);
  }
}
// Bigint 階乗
const factB=(n)=>{
  if(n==0n) {
    return 1n;
  } else {
    return n*factB(n-1n);
  }
}
// 範囲
const range = ((from,to) =>{return [...Array(to-from+1)].map((_,i)=>i+from)})
// イテレータ階乗
const factR=(n)=>{return range(1,n).reduce((x,y)=>x*y,1);}

// ユークリッド互除法
const gcd=(a,b)=>{
    if (a%b==0) {
      return b;
    } else {
      return gcd(b,a%b);
    }
}

module.exports = {fact, factB, range, factR, gcd};

