---
title: Cryptography
author:
  - Emilio Cobos Álvarez (70912324N)
numbersections: true
---

# Bibliography

 * A course in Number Theory and Cryptography (Springer), by N. Koblitz

# Congruences in integer numbers

Read more: http://mathworld.wolfram.com/Congruence.html

This shouldn't be necessary but, let's remember what the integer set
($\mathbb{Z}$) is:

$$\mathbb{Z} = \{ \dots, -2, -1, 0, 1, 2, \dots \}$$

Let's fix  a number $n \in \mathbb{Z} (n > 0)$.

We'll say that **two numbers $a, b \in \mathbb{Z}$ are congruent modulo $n$ if
and only if $a \cdot b$ is multiple of $n$**.

We'll denote that as:

$$a \equiv b (mod~n)$$

For any number $a \in \mathbb{Z}$, we'll call $\bar{a}$ to the set of numbers
congruent with $a$ modulo $n$, and we'll call that **the congruent class of
$a (mod~n)$**.

$$\bar{a} = \{ a, a \pm n, a \pm 2n, \dots \}$$

Note that the congruent class of $0 (mod~n)$ are the multiples of $n$:

$$\bar{0} = \{ 0, \pm n, \pm 2n, \dots \}$$

We'll denote $\mathbb{Z} / (n)$ to the set of **all congruence classes modulo
n**.

We'll prove that this set **is finite**:

$$a \in \mathbb{Z}$$

$$a = q \cdot n + r \implies
a \equiv r (mod~n) \implies
\bar{a} \equiv \bar{n}$$

It's obvious now, that:

$$\mathbb{Z}/(n) = \{ \bar{0}, \bar{1}, \dots, \overline{n - 1} \}$$

Let's define the sum and product inside $\mathbb{Z}/(n)$:

$$\bar{a} + \bar{b} = \overline{a + b} \\
\bar{a} \cdot \bar{b} = \overline{a \cdot b}$$

For example, in $\mathbb{Z}/(2) = \{ \bar{0}, \bar{1} \}$:

$$\begin{aligned}
\bar{0} + \bar{0} &= \bar{0} \\
\bar{0} + \bar{1} &= \bar{1} \\
\bar{1} + \bar{1} &= \bar{2} = \bar{0} \\
\bar{0} \cdot \bar{0} &= \bar{0} \\
\bar{1} \cdot \bar{0} &= \bar{0} \\
\bar{1} \cdot \bar{1} &= \bar{1} \\
\end{aligned}$$

$(\mathbb{Z}/(n), +, \cdot)$ is
a [**ring**](http://mathworld.wolfram.com/Ring.html).

We'll define $(\mathbb{Z}/(n))^*$ as the **invertible elements of
$\mathbb{Z}/(n)$**.

$$\bar{a} \in (\mathbb{Z}/(n))^* \iff \bar{a} \cdot \bar{a}^{-1} = \bar{1}$$

Let's suppose the existence of $\bar{b} = \bar{a}^{-1}$. Then:

$$\begin{aligned}
\bar{a} \cdot \bar{b} &= \bar{1} \iff a \cdot b \equiv 1 (mod~n) \\
a \cdot b &= \lambda n + 1 \\
a \cdot b - \lambda n &= 1 \implies mcd(n, a) = 1 \\
\end{aligned}$$

The last implication can be deduced from the Euclides algorithm, that is:

$$mcd(m, n) = d \iff \exists \lambda, \mu$$
$$\mu \cdot m + \lambda n = d$$

That means that $(\mathbb{Z}/(n))^*$ is the **set of integers between $0$ and
$n - 1$ that are coprime with $n$**.

We'll denote $|(\mathbb{Z}/(n))^*|$ as $\varphi(n)$, and we'll call it the
**Euler indicator of $n$**, that is, [*Euler's totient
function*](https://en.wikipedia.org/wiki/Euler's_totient_function).

A *field* is a ring in which every element is invertible. That means that:

$$\mathbb{Z}/(n)~\textrm{is a field} \iff n~\textrm{is prime} \iff
|(\mathbb{Z}/(n))^*| = n - 1$$

**Example:** $\mathbb{Z}/(4)$ is **not** a field, since $2$ is not coprime with
$4$, so it's not invertible:

$$(\mathbb{Z}/(n))^* = \{ \bar{1}, \bar{3} \}$$

## Calculating Euler's totient ($\varphi(n)$)

### If $n$ is prime

If $n$ is prime, we have already proved that $\varphi(n) = n-1$.

### If $p$ is prime, and $n$ is $p^k$

In this case, $\varphi(p^k) = p^k - p^{k - 1} = p^{k - 1} \cdot (p - 1)$.

The intuitive thing for seeing it is that the only number that is not coprime
with $p^n$ is $p$ and it's multiples, that is: $\{ 0, p, 2p, \dots, p^{k - 1}
\cdot p = p^k \}$. Those numbers are $p^{n - 1}$, so the total number of
coprimes with $p$ is the total number ($p^k$) minus the number which are not
coprime with it ($p^{k -1}$).

### If $n = a \cdot b$, and $mcd(a, b) = 1$

In this case, $\varphi(a \cdot b) = \varphi(a) \cdot \varphi(b)$.

This can be proven as a direct consequence of the [*Chinese remainder
theorem*](https://en.wikipedia.org/wiki/Chinese_remainder_theorem).

### Otherwise

We can always factorise $n$, and then:
$$n = p_1^{r_1} + p_2^{r_2} + \dots + p_h^{r_h}$$
$$\varphi(n) = \varphi(p_1^{r_1}) + \varphi(p_2^{r_2}) + \dots
+ \varphi(p_h^{r_h})$$
