---
title: Coding and Information Theory
subtitle: Week 2
author:
  - Emilio Cobos Álvarez (70912324N)
---

# Lineal codes

A block code is lineal if and ondly if the transformation it represents is
lineal.

Remember that a lineal transformation is a transformation $T$ such as:

 * $T(u + v) = T(u) + T(v)$
 * $a \cdot T(u) = T(a \cdot u)$

## Reasons to stick with linear codes

Theorical
: Gilbert-Varshamod bound implies that there exist linear codes within the
"good" code families.

Mathematical
: Easier to proof things ($+$, $-$, linear algebra).

Practical
: Easy methods to produce linear transformations.

## Injective transformation

Remember that a transformation is injective if and ony if for each different
element of $A^m$, the transformation produces a different element of $A^n$.

That, expressed in terms of the associated matrix, means that **the associated
matrix has maximum range**.

We're ovbiously interested in injective codes since **we won't be able to check
the received word is correct otherwise**.

## Proving a code is linear

### Triple Repetition

$$A = \mathbb{Z}/2$$
$$T: A \rightarrow A^3$$
$$T(a) \rightarrow (a, a, a)$$

$T$ is linear if and ony if both $A(a + b) = A(a) + A(b)$ and $A(a + b) = A(a)
+ A(b)$ are true.

There are only a few values so we can brute-force it and check it's true.

### Parity bit

TODO

# Modular arithmetic

Given a naturan number $n$, we'll call $\mathbb{Z}/n$ (read $\mathbb{Z}$ modulo
$n$):

$$\mathbb{Z}/n = \{0, \dots, n - 1\}$$

together with the operations:

Addition
: $(a_1 + a_2) \mod n$

Product
: $(a_1 \cdot a_2) \mod n$

For example:

$$\mathbb{Z}/2 = \{0, 1\}$$

We're interested specifically when the number of elements is $2^k$ for any $k$.

## Does it make sense to define division?

A number $n^{-1}$ such as $n \cdot n^{-1} = 1$ **doesn't have to exist** for
every $\mathbb{Z}/n$.

For example, it doesn't exist in $\mathbb{Z}/4$.

We can only define division in
[fields](https://en.wikipedia.org/wiki/Field_%28mathematics%29).

### Definition of *field*

A field is a set with two operations ($+$ and $\cdot$), such as for every
element $x$ which is not $0$ (the neutral element for the addition), another
number $x^{-1}$ (inverse of $x$) exists, such as $x \cdot x^{-1} = 0$.

Theorem
: $\mathbb{Z}/n$ is a field if and only if $n$ is prime.[^proof]

Problem
: $2^n$ is never prime (except with $n = 1$).

Solution
: We must be able to declare fields with $2^n$ elements. We'll use
$\mathbb{Z}/2^{[x]}/polinomial$ form.[^reed-shannon].


[^proof]: We won't prove it (at least right now).
[^reed-shannon]: We'll se them later, they're called Reed-Shannon codes.

## Fermat theorem

If $p$ is prime, and $a$ is not multiple of $p$, then $a^{p - 1} = 1 \mod p$.

Example
: Calculate $765^{1565} \mod 23$


$$765 \mathbin{\%} 23 = 6 \implies 765 = 6 \mod 23 \implies 765^{1565}
= 6^{1565} \mod 23$$

We know via Fermat's theorem that $6^{22} = 1 \mod 23$:

$$6^{22} = 1 \mod 23 \implies 6^{1565} = 6^{1565 \mathbin{\%} 22} = 6^3 \mod
23$$

# Matrix associated to a linear transformation

Given a linear transformation $T: A^m \rightarrow A^n$, you can associate
a matrix (respect a given base in $A^m$ and another in $A^n$), and express the
linear transformation as multiplications for that matrix.

This comes from the fact that, if $T$ is linear:

$$T(a) = T(a_1 \cdot e_1 + \dots + a_m \cdot e_m) = a_1 \cdot T(e_1) + \dots
+ a_n \cdot T(e_m)$$

If $\{e_1, \dots, e_m\}$ is a base of $A^m$, and If $\{f_1, \dots, f_n\}$ is
a base of $A^n$, then:

$$T(e_j) = \sum\limits_{i = 1}^{n} a_{ij} \cdot f_{i}$$

In binary, and assuming standard basis in both $A^m$ and $A^n$, we can simplify
it to:

> The term $(i, j)$ of the associated matrix is the $i$th bit of the image of $0
> \dots 1 \dots 0$ (where the 1 is in the $j$th position).

Or even:

> The $j$th row of the matrix is the image of the $j$th element of the standard
> base.

## Example: Parity bit

$$A = \mathbb{Z}/2$$
$$T: A^2 \rightarrow A^3$$
$$T((a, b)) \rightarrow (a, b, a + b)$$

Using the standard basis $\{(0, 1), (1, 0)\}$ for $A^2$, and $\{(0, 0, 1), (0,
1, 0), (1, 0, 0)\}$ in $A^3$.

We can construct the matrix using as rows $T((1, 0))$ and $T((0, 1))$:

$$ M =
\begin{array}{c}
  T((1, 0)) \rightarrow \\
  T((0, 1)) \rightarrow
\end{array}
\begin{pmatrix}
  1 & 0 & 1 \\
  0 & 1 & 1
\end{pmatrix}$$

We can prove it correct this way:

$$(a, b) \cdot M = (a \cdot 1 + b \cdot 0, a \cdot 0 + b \cdot 1, a \cdot
1 + b \cdot 1) = (a, b, a + b)$$

## Example: Triple repetition

$$A = \mathbb{Z}/2$$
$$T: A^1 \rightarrow A^3$$
$$T(a) \rightarrow (a, a, a)$$

Doing the same:

$$ M =
\begin{array}{c}
  T(1) \rightarrow
\end{array}
\begin{pmatrix}
  1 & 1 & 1
\end{pmatrix}$$

## Example: Triple control

$$A = \mathbb{Z}/2$$
$$T: A^3 \rightarrow A^6$$
$$T((a, b, c)) \rightarrow (a, b, c, a + b, a + c, b + c)$$

It's a bit more tedious, but it's the same:

$$ M =
\begin{array}{c}
  T((1, 0, 0)) \rightarrow \\
  T((0, 1, 0)) \rightarrow \\
  T((0, 0, 1)) \rightarrow
\end{array}
\begin{pmatrix}
  1 & 0 & 0 & 1 & 1 & 0 \\
  0 & 1 & 0 & 1 & 0 & 1 \\
  0 & 0 & 1 & 0 & 1 & 1 \\
\end{pmatrix}$$

**Note**: See that, to get the codified version of $101$, we could multiply it for the
matrix, but it's easier to express it as: $T(101) = T(100) + T(001)$, and just sum
the rows of the matrix.

# Control matrix

We'll dive into it a bit more later, but basically it's expressing the error
condition in the form of a matrix in order to check it.

## Example: Parity bit

A parity bit encoded word of the form $abc$ will be correct if and only if $a
+ b = c$. That's the same as saying: $a + b + c = 0$, so expressing that
constraint as a matrix we get:

$$ correct(abc) \iff \begin{pmatrix}
  a & b & c
\end{pmatrix} \cdot \begin{pmatrix}
  1 \\
  1 \\
  1
\end{pmatrix} = 0$$
