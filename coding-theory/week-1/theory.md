---
title: Coding and Information Theory
subtitle: Week 1
author:
  - Emilio Cobos Álvarez (70912324N)
---

# Transmission scheme

The general scheme of a transmission is that:

 * We have some information we want to transmit over a fallible channel.
 * We need to encode that information so we can detect errors, or even correct
   them.
 * The information must be decoded by the receiver easily.

To encode the information we'll use what we call a block dode. We'll se a few
definitions later, but this is the general diagram for a transmission:

![Transmission scheme diagram](img/transmission-scheme.dot.png)

## Example of a code and a transmission: parity bit

 * 2 information bits, 1 parity bit
 * Probability of error in the transmission of 1 bit: $p = 1 / 1000$

### Codification

$$ENCODED(INFO) \equiv INFO + XOR(INFO)$$

Info    Encoded
-----   --------
00      000
01      011
10      101
11      110

### Error probability transmitting a word (3 bits)

Number of errors   Probability
-----------------  ----------------------------------------------
0                  $(1 - p)^3$
1                  $(1 - p)^2 \cdot p \cdot 3$
2                  $(1 - p) \cdot p^2 \cdot 3$
3                  $p^3$

Note that the extra multiplication by three means "how many different
combinations of 1 error might be?", that is: $\binom{3}{1}$ in the 1 error
case, $\binom{3}{2}$ in the next, etc...

# Some vocabulary

## Alphabet ($\Sigma$)

Set of symbols that represent the information to be transmitted, e.g: $\{0, 1\}$

## Word ($w$) of length $k$

Succession of $k$ symbols belonging to an alphabet $\Sigma$.

### Weight

The weight of a world is the number of symbols of that word which are not $0$.

## Distance between two words ($d$)

The distance between two words of the same length, $w_1$ and $w_2$, is the
number of positions in which they differ.

## Block code of alphabet $\Sigma$, message length $k$ and block length $n$

Application which converts words of length $k$ belonging to $\Sigma$ into words
of length $n$, also belonging to $\Sigma$.

$$C: \Sigma^k \rightarrow \Sigma^n$$

### Rate ($R$)

We define the rate as the ratio between the message length and the block length.

$$R = \frac{k}{n}$$

### Distance ($d$)

The distance or **minimum distance** of a block code is the minimum number of
positions in which  any two distinct words of $C$ differ.

$$d(C) = min(d(w_1, w_2)) \forall w_1, w_2 \in \Sigma | w_1 \neq w_2$$

## Channel

This is a rather broad concept, but we'll be assuming **symmetric** binary
channels, that is, codes which use the **alphabet $\{0, 1\}$** and in which
**error probability is the same regardless of the symbol transmitted**.

# Properties of block codes

## Distance, detection and correction

A block code $C$ can detect $t$ errors and correct $t + s$ errors if and only
if:

$$d(C) \geq 2t + s + 1$$

You can prove this representing a code graphically, and seeing that you can't
have words to correct that overlap.

## Probabilty of $k$ errors ocurring while transmitting $n$ bits

Given $p$ as the error probability when transmitting 1 bit:

$$p(k, n) = \binom{n}{k} \cdot p^k \cdot (1 - p)^{n-k}$$

Remember that $\binom{n}{k}$ means "how many ways of choosing $k$ elements out
of $n$ are possible?", that is:

$$\binom{n}{k} = \frac{n!}{k! \cdot (n - k)!}$$

# Shannon theorem

> For each symmetric channel there exists a constant $C$ (read *capacity*), so
> that block codes of smaller-but-arbitrarily-next to $C$ rate, and with
> near-to-perfect correct transmission rate.

That capacity can be defined (in binary channels) as:

$$C(p) = 1 + log_2(p) + (1 - p) \cdot log_2(1 - p)$$

# Hamming bound

> If $C$ is a binary code of block length $n$ and minimum distance $2r + 1$,
> then $C$ has at most $\frac{2^n}{\sum\limits^{r}_{i=0} \binom{n}{i}}$ words.

This can easily be expanded to $q$-ary block codes, where the bound will become:

$$\frac{q^n}{\sum\limits^{r}_{i=0} \binom{n}{i} \cdot (q - 1)^i}$$

It's called sphere packing bound due to it's visual representation.

## Hamming ball/sphere

The ball or sphere of a word $c \in A^n$ of radius $r$ is the set of words such
as its hamming distance with $c$ is less than or equal to $r$:

$$B(c, r) = \{c' ; d(c, c') \leq r\}$$

The number of elements in B (for a binary block) is:

$$|B(c, r)| = 1 + n + n \cdot (n - 1) + ... + \binom{n}{r}
= \sum\limits^{r}_{i=0} \binom{n}{i}$$

As before, you can generalise to a $q$ block:

$$|B(c, r)| = 1 + (q - 1) \cdot n + (q - 1)^2 \cdot n \cdot (n - 1) + ... + (q
- 1)^r \cdot \binom{n}{r} = \sum\limits^{r}_{i=0} \binom{n}{i} \cdot (q - 1)^i$$

## Perfect code

We call perfect code a code that reaches the Hamming bound.

# Singleton bound

> If $C$ is a q-ary block code of length $n$ and minimum distance $d$, then
> $A_q(n, d) \leq q^{n - d + 1}$.

Here $A_q(n, d)$ means the maximum number of words in a $q$-ary code of length
$n$ and minimum distance $d$.

This is easy to see, since we can remove $d - 1$ elements of a code of distance
$d$, and they will still be different words (by the definition of distance).
Thus, we've obtained a code with as many words as the previous, and this code
has at most $n - (d - 1)$ symbols, which means that it has at most $q^{n
- d + 1} words.

For linear codes, $A_q(n, d)$, the number of codewords can't be more than the
number of words of the original alphabet $q^k$ (where $k$ is the dimension).
That implies that:

$$q^k \leq q^{n - d + 1} \implies k \leq n - d + 1$$

# Gilbert-Varshamod bound

$$|C| = A_q(n, d) \geq \frac{q^n}{\sum\limits^{d - 1}_{i = 0} \binom{n}{i} \cdot
(q - 1)^i}$$
