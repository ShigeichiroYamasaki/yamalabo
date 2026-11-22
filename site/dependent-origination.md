---
title: 仏教の縁起は数学的に証明できるか
description: 因果連鎖と相互依存ネットワークを比較し、縁起の一側面を数理モデルにすると何が証明できるかを考える。
head:
  - [meta, { property: 'og:type', content: 'article' }]
  - [meta, { property: 'og:locale', content: 'ja_JP' }]
  - [meta, { property: 'og:title', content: '仏教の縁起は数学的に証明できるか' }]
  - [meta, { property: 'og:description', content: '因果連鎖と相互依存ネットワークを比較し、縁起の一側面を数理モデルにすると何が証明できるかを考える。' }]
  - [meta, { property: 'og:url', content: 'https://shigeichiroyamasaki.github.io/yamalabo/dependent-origination.html' }]
  - [meta, { property: 'og:image', content: 'https://shigeichiroyamasaki.github.io/yamalabo/dependent-origination-og.png' }]
  - [meta, { property: 'og:image:width', content: '1200' }]
  - [meta, { property: 'og:image:height', content: '630' }]
  - [meta, { property: 'og:image:alt', content: '因果連鎖と相互依存ネットワークの比較図' }]
---

# 仏教の縁起は数学的に証明できるか

縁起の一側面を「ある状態が複数の条件に依存して成り立つ」と捉えると、その依存関係を数学で表し、仮定から何が従うかを証明できます。たとえば、互いに影響し合う変数の安定状態は存在するか。ある条件の変化はネットワークのどこまで及ぶか。こうした問いには、モデルを明示したうえで答えを与えられます。

## 因果連鎖から相互依存へ

単純な因果連鎖 $A\to B\to C$ では、影響を上流から下流へ順にたどれます。依存関係をネットワークとして捉えると、一つの状態に複数の条件が関わり、ある変化が複数の経路を通って戻ってくる場合も表せます。この図では、各矢印はモデルの中で仮定した直接の依存を表します。

![因果連鎖では A から B、B から C へ矢印が伸びる。相互依存ネットワークでは複数の経路と循環がある。](/dependent-origination.svg)

この比較は縁起の数理的な読み方の一例です。仏教で説かれる条件関係には、順序をもつものも含まれます。ネットワークに循環があることは、縁起を表すための必須条件ではありません。

## 相互依存する状態は一意に定まるか

二つの状態 $x$ と $y$ に、外部から与えられる条件 $a$ と $b$ があるとします。それぞれが相手の状態にも依存する最も単純な線形モデルは、次のように書けます。

$$
x=a+\alpha y,\qquad y=b+\beta x.
$$

ここで $\alpha$ は $y$ から $x$ への影響の強さ、$\beta$ は逆方向の影響の強さです。第2式を第1式に代入すると $(1-\alpha\beta)x=a+\alpha b$ を得ます。したがって $\alpha\beta\ne 1$ なら、平衡状態は一意に定まります。

$$
x^*=\frac{a+\alpha b}{1-\alpha\beta},
\qquad y^*=\frac{b+\beta a}{1-\alpha\beta}.
$$

同じ式を時間発展の規則として読み、右辺に前の時刻の値を使うと、平衡への収束も調べられます。

$$
x_{t+1}=a+\alpha y_t,\qquad y_{t+1}=b+\beta x_t.
$$

2回更新すると $x_{t+2}-x^*=\alpha\beta(x_t-x^*)$ です。$y$ についても同じ関係が成り立ちます。よって $|\alpha\beta|<1$ なら誤差は2ステップごとに縮小し、どの初期状態からも平衡へ収束します。相互依存があっても状態は定まり得ますが、その安定性は影響の強さに依存します。

## 一つの条件を変えたとき、何が変わるか

外部条件 $a$ だけを $\delta$ 変えると、上のモデルの平衡状態は次のように変化します。

$$
\Delta x^*=\frac{\delta}{1-\alpha\beta},
\qquad
\Delta y^*=\frac{\beta\delta}{1-\alpha\beta}.
$$

$|\alpha\beta|<1$ のとき、$x$ の変化は $\delta\sum_{k=0}^{\infty}(\alpha\beta)^k$ とも書けます。最初の項は $a$ の直接の効果、次の項は $x$ から $y$ を経て $x$ に戻った効果です。その後の項は同じ経路をさらに回った効果に対応します。条件の変化がどのように全体の状態へ現れるかを、経路ごとに読み解けます。

より多くの状態を扱うなら、状態ベクトルを $\mathbf{x}$、外部条件を $\mathbf{u}$、結びつきを行列 $W$ として $\mathbf{x}=\mathbf{u}+W\mathbf{x}$ と書けます。$W$ のスペクトル半径が1未満なら、平衡は $\mathbf{x}^*=\sum_{k=0}^{\infty}W^k\mathbf{u}$ です。$W^k$ の各成分には、長さ $k$ の経路に沿った重みの積が足し合わされています。複数の経路が重なり、符号によっては互いの効果を打ち消すこともわかります。

## 証明の射程

数学で証明できるのは、依存関係や更新規則を仮定したときに従う性質です。どの変数を選ぶか、実際にどの依存関係が存在するか、介入がほかの条件を保ったまま実施できるかは、モデルの外で確かめる必要があります。また、縁起という教説全体の意味を平衡やグラフで尽くすことはできません。

縁起を手がかりに数学的な問いを立てるなら、「何が何に依存するか」に加えて、「どの条件を変えれば、どの関係を通って、何が変わるか」を問うことができます。グラフは関係の構造を、力学系は変化の過程を記述します。証明によって明らかになるのは、その構造と規則のもとで成り立つことです。

## 参考資料

- [Paccayasutta（相応部 12.20、Bhikkhu Sujato 英訳）](https://suttacentral.net/sn12.20/en/sujato)
