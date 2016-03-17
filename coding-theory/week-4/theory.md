---
title: Coding and Information Theory
subtitle: Week 4
numbersections: true
author:
  - Emilio Cobos Álvarez (70912324N)
---

# Hamming codes

A $k$-Hamming code ($Ham(k)$) is a code whose control matrix is the binary
representation of every non-null word of $k$ bits.

$$H = \begin{pmatrix}
0 & 0 & 0 & \dots & 1 \\
0 & 0 & 0 & \dots & 1 \\
\vdots & \vdots & \vdots &   & \vdots \\
0 & 1 & 1 & \dots & 1 \\
1 & 0 & 1 & \dots & 1 \\
\end{pmatrix}$$

Note that interchanging rows in $H$ (or columns in $H^T$) doesn't modify the
code, but doing that in columns in $H$ (rows in $H^T$), implies that the
generator matrix will be different, and the code too.

You can change columns and later undo that changes if you just want to find the
generator matrix though.

## Properties

Longitude
: The longitude will be the number of columns in $H$, that is $2^{k - 1}$.

Dimension
: That's the number of rows in the generator matrix, that is $2^k - 1 - k$.

Minimum distance
: The minimum distance in a Hamming code is always $3$.

Here is a table with a few examples:

k        Longitude    Dimension    Min. Distance
----     ---------    ---------    -------------
2        3            1            3
3        7            4            3
4        15           11           3

## Alternative representation

Hamming codes are sometimes parameterized in terms of the longitude and
dimension, instead of the value $k$:

$$Ham(k) \equiv Ham(2^k - 1, 2^k - 1 - k)$$

## Example: $Ham(3)$

Well analyse the $Ham(3)$ or $Ham(4, 7)$ code.

We'll start constructing the control matrix $H$:

$$H = \begin{pmatrix}
0 & 0 & 0 & 1 & 1 & 1 & 1 \\
0 & 1 & 1 & 0 & 0 & 1 & 1 \\
1 & 0 & 1 & 0 & 1 & 0 & 1 \\
\end{pmatrix}$$

### Calculating the generator matrix

We proved before, while talking about the syndrome, that $G \cdot H^T = 0$, and
that for any correct word $c$, $c \cdot H^T = 0$.

We'll use that, with the fact that the longitude of this code is $7$, to build
a system of equations and get the rows of the generator matrix.

$$(m_1, \dots, m_7) \cdot H^T = (0, 0, 0) \implies
\left\{\begin{array}{l}
  m_4 + m_5 + m_6 + m_7 = 0 \\
  m_2 + m_3 + m_6 + m_7 = 0 \\
  m_1 + m_3 + m_5 + m_7 = 0 \\
\end{array}\right.
$$

There are multiple solutions for this system. All the resulting rows must not
only make the system true, but also be linearly independent.

The easiest of the solutions by far is the images of the elements of the
standard base.
