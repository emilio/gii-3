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

### Implications for cryptography

If $n = p \cdot q$, with $p$ and $q$ being prime, and $p \neq q$, then:

$$\varphi(n) = \varphi(p) \cdot \varphi(q) = (p - 1) \cdot (q - 1)
= n - p - q + 1$$

We can see that ***if $n = p \cdot q$, computing $\varphi(n)$ is equivalent
(in complexity) to computing $p$ and $q$***. This has strong implications for
cryptography, since a lot of codes (like *RSA*) are strong based on the fact
that factorising large numbers is really hard computationally.

We can prove this rather easily.

#### General case

Given $\alpha$ and $\beta$, and knowing $a$ and $b$, which are two constants,
and given:

$$\left\{\begin{aligned}
a &= \alpha + \beta \\
b &= \alpha \cdot \beta
\end{aligned}\right.$$

We can construct a polynomial such that:

$$(x - \alpha) \cdot (x - \beta) = x^2 - (\alpha + \beta) x + \alpha \cdot
\beta = x^2 - ax + b$$

We can see that $\alpha$ and $\beta$ are the roots of that polynomial, and as
such we can compute it easily using:

$$\alpha, \beta = \frac{a \pm \sqrt{a^2 - 4b}}{2}$$


#### Particular case

Applying the above knowledge to what we know, if we get two constants that are
equal to $p + q$ and $p \cdot q$, we can easily find both $p$ and $q$.

We already have one of that constants ($n = p \cdot q$), so we just have to
find an expresion which is $p + q$, and we have that with $\varphi(n)$.

$$\varphi(n) = n - p - q + 1 \implies n - \varphi(n) + 1 = p + q$$

That is:

$$\left\{
\begin{aligned}
n - \varphi(n) + 1 &= p + q \\
n &= p \cdot q
\end{aligned}
\right.$$

That makes finding $p$ and $q$ knowing the totient function a matter of
solving:

$$p, q = \frac{n - \varphi(n) + 1 \pm \sqrt{(n - \varphi(n) + 1)^2 - 4n}}{2}$$

Which is computationally easy knowing that $p$ and $q$ are integers, so the
problem ends up being knowing a square root of an integer which also yields
another integer.

# $k[x]$

If $k$ is a body, we call $k[x]$ to the set of polynomials in a single variable
with coefficients belonging to $k$:

$$P(x) = a_nx^n + a_{n - 1}x^{n - 1} + \dots + a_0, a_i \in k$$
$$Q(x) = b_mx^m + b_{m - 1}x^{m - 1} + \dots + b_0, b_i \in k$$

## $k[x]$ is a ring

We can see that $k[x]$ is a **ring**, defining the product and sum
as follows:

$$P(x) + Q(x) = (a_0 + b_0) + (a_1 + b_1) x + \dots$$
$$P(x) \cdot Q(x) = (a_0 \cdot b_0) + (a_0b_1 + b_0a_1) x + \dots$$

## Division in $k[x]$

We can also see that **the algorithm for division holds**, that is:

Given $A(x), B(x) \in k[x]$, there exists $Q(x), R(x) \in k[x]$ such that:

$$A(x) = B(x) \cdot Q(x) + R(x), deg(R(x)) < deg(B(x))$$

## Congruency in $k[x]$

Given $P(x) \in k[x]$, $deg(P(x)) = n$, we say that $A(x)$ and $B(x)$ are
congruent modulo $P(x)$ if and only if $A(x) - B(x)$ is a multiple of $P(x)$.

We'll call $k[x]/P(x)$ to the set of classes of congruency mod. $P(x)$.

We can prove as we did with integers that this set is finite using the
algorithm of division:

$$A(x) = Q(x)P(x) + R(x)$$
$$deg(R(x)) < deg(P(x))$$

All of the above implies that:

$$A(x) \equiv R(x) (mod~P(x))$$

So we arrive to the conclusion that:

> $k[x]/P(x)$ is the set of polinomials $A(x) \in k[x]$ such that
> $deg(A(x)) < deg(P(x))$.

## Properties of $k[x]/P(x)$

We'll use $n = deg(P(x))$.

 * It's a $k$-vector space of base $\{1, x, x^2, \dots, x^{n - 1}\}$ and
     dimension $n$.
 * It's a **ring**:
     $$\bar{A} + \bar{B} = \overline{A + B}$$
     $$\bar{A} \cdot \bar{B} = \overline{A \cdot B}$$
 * It's a **field if and only if $P(x)$ is irreducible**. The proof is similar
     to that used for $\mathbb{Z}/p$, that is, we'll prove that in order to be
     a field, there must be a $B(x)$ for every $A(x)$ such that $A(x) \cdot B(x)
     = 1$. That ends up implying that for every $A(x)$, $mcd(A(x), P(x)) = 1$,
     and that can only happen if $P(x)$ is irreducible:

     $$\begin{array}{ll}
     \overline{A(x)} \cdot \overline{B(x)} = \bar{1} \iff \\
     A(x) \cdot B(x) \equiv 1 (mod~P(x)) \iff \\
     A(x) \cdot B(x) \equiv 1 + Q(x) \cdot P(x) \iff \\
     mcd(A(x), P(x)) = 1
     \end{array}$$

We won't prove this, at least yet, but **every finite field can be
expressed as $\mathbb{F}_p[x]/Q(x)$, being $p$ a prime**.

Remember that, if $p$ is prime, then $\mathbb{F}_p \equiv \mathbb{Z}/p$.

# Definition of *group*

We'll define a *group* as a set of elements and one product operation $(G,
\cdot)$, such that:

$$G \times G \xrightarrow{\cdot} G$$
$$a, b \xrightarrow{\cdot} c; a, b, c \in G$$

A group must verify that:

 * There must exist a *neutral factor* $e$:
     $a \cdot e = e \cdot a = a~\forall a \in G$
 * $a \cdot (b \cdot c) = (a \cdot b) \cdot c~\forall a, b, c \in G$
 * $\forall a \in G, \exists a^{-1}$, such that $a \cdot a^{-1} = e$

We'll call $G$ a *conmutative group* if and only if
$a \cdot b = b \cdot a \forall a, b \in G$.

We'll only see conmutative groups in this subject.

Being $k$ a field, you can verify that:

 * $(k, \cdot)$ is **not** a field (since 0 has no inverse).
 * $(k^*, \cdot)$, is a field.
 * $(k, +)$ is a field.

Remember that $k^* = k - \{0\}$

## Lagrange's theorem

> Being $G$ a finite group, with $n = |G|$, it's verified that
> $a^n = e~\forall a \in G$, being $e$ the *neutral factor* of the group.
