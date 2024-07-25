require 'matrix'



# ハミング符号生成

## 1から 2^(m-1)
cs=->m{(0..(2**m-1)).map{|x|x.to_s(2).split('')}.map{|y|[0]*(m-y.size)+y.map{|z|z.to_i(2)}}}
cs1=->m{(1..(2**m-1)).map{|x|x.to_s(2).split('')}.map{|y|[0]*(m-y.size)+y.map{|z|z.to_i(2)}}}

## m=3 の場合
m=3
n=2**m-1
k=n-m

# 情報ベクトル 0 以外で右側に単位行列
iv=cs1[m].sort_by{|x|x.sum}.reverse

# パリティ検査行列 H の生成
h=Matrix.columns(iv)

# P行列
p=Matrix.columns(ha[0..(m+1)*(-1)]).t

# 生成行列 G
g=Matrix.I(k).hstack(p)

# 行列の積（GF(2))
mp=->a{->b{Matrix[(a*b).to_a.map{|x|(x.class==Integer ? x%2 : x.map{|y|y%2})}[0]]}}

# 生成行列とパリティ検査行列の積

mp[g][h.t]

# => Matrix[[0, 0, 0]]

# 情報符号
ca=cs[k]

# ランダムに情報符号を選ぶ(Matrix）
 a=Matrix[ca[rand(ca.size)]]


# 生成行列を使って通信路符号を生成
w=mp[a][g]

#=> [0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0]

# エラーを含む受信語
v=Matrix[[0,1,0,0,1,0,1]]

e=Matrix[[0,0,0,1,0,0,0]]

# 検査行列で検証

s=mp[v][h.t]

# => Matrix[[0, 1, 1]]


ca=cs[k]
a=Matrix[ca[rand(ca.size)]]
w=mp[a][g].to_a[0]
(0..6).map{|x| 
        e=[0,0,0,0,0,0,0]
        e[x]=1
        v=Matrix[w.zip(e).map{|x|x[0]^x[1]}]
        v.to_a[0].join
}

