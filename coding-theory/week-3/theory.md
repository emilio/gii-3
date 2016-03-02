---
title: Coding and Information Theory
subtitle: Week 3
author:
  - Emilio Cobos Álvarez (70912324N)
---

# Generator matrix

We call the generator matrix of a linear code to the matrix associated to the
code's transformation in vector space.

Let's suppose we have a code $C: A^m \rightarrow A^n$, with a generator matrix
$G$.

We know that any word $c$ can be generated multiplying a vector $s \in A^m$ by
the matrix $G$.

## Standard form

We say that matrix is in standard form if it has the form:

$$G = (Id_m | M)$$

## Example: Parity bit

*Disclaimer*: This example is somewhere in the previous bits.

A control matrix for the code of the parity bit, can be:

$$G = \begin{pmatrix}
1 & 0 & 1 \\
0 & 1 & 1 \\
\end{pmatrix}$$

So:

$$(a, b) \cdot G = (a, b, a + b)$$

We can see **$G$ is in standard form**, since it can be seen as:

$$G = (Id_2 | M)$$
$$M = \begin{pmatrix}1 \\ 1\end{pmatrix}$$

# Control matrix

In lineal codes, checking that a received word $c$ belongs to $C$ is a condition
of the type:

$$c \cdot H^T = 0$$

Where $H^T$ is the transpose of what we call the **control matrix** ($H$).

## Example: Triple control

We receive a word of the form $(a, b, c, x, y, z)$, where:

$$\begin{array}{ccc}
a + b = x  &          & a + b + x = 0 \\
a + c = y  & \implies & a + c + y = 0 \\
b + c = z  &          & b + c + z = 0
\end{array}$$

We can see that making that three equations consists in having three columns in
a matrix (one per equation), with six rows (one per received bit):

$$(a, b, c, x, y, z) \cdot \begin{pmatrix}
1 & 1 & 0 \\
1 & 0 & 1 \\
0 & 1 & 1 \\
1 & 0 & 0 \\
0 & 1 & 0 \\
0 & 0 & 1
\end{pmatrix} = (0, 0, 0)$$

Note that the first column is, for example, $(1, 1, 0, 1, 0, 0)$, so when
multiplying it ends up as $a + b + x$, which is the first of our equations.

In this case, the control matrix $H$ is:

$$H = \begin{pmatrix}
1 & 1 & 0 & 1 & 0 & 0 \\
1 & 0 & 1 & 0 & 1 & 0 \\
0 & 1 & 1 & 0 & 0 & 0 \\
\end{pmatrix} = (0, 0, 0)$$

## Creating a control matrix

A linear code **can have multiple control matrices**, but there's a simple
method to obtain one of them easily.

If we have a code $C: A^m \rightarrow A^n$, and it has a generator matrix $G$,
if $G$ is in standard form, then the control matrix will have the form:

$$G = (Id_m | M) \implies H = (M^T | Id_{n \cdot m})$$

### Proof

Since $G$ is a generator matrix, the words have the form:

$$c = (a_1, \dots, a_m) \cdot G$$

$H$ must have the property:

$$c \cdot H^T \iff c \in C$$

Then, substituting $c$ in the above formula, we reach the following equation:

$$(a_1, \dots, a_m) \cdot G \cdot H^T = 0 \implies G \cdot H^T = 0$$

Note that $G$ has $m$ rows, and $n$ columns, and $H^T$ has $n$ rows, which means
that $H$ has $n$ columns.

If we express $G$ as $(Id_m | M)$, we can see that the matrix $M$ has $n - m$
columns.

If we split the matrix $H$ horizontally in two matrices ($R$ and $S$), such as
$R$ has $m$ rows.

Then we've got:

$$\begin{array}{l}
G \cdot H = (Id_m | M) \cdot \begin{pmatrix}R \\\hline S\end{pmatrix}
= 0 \implies \\
\\
Id_m \cdot R + M \cdot S = 0
\end{array}$$

The easiest solution for that system is:
$$\begin{array}{l}
S = Id_{n - m} \\
R = - M
\end{array}$$

Which would mean:

$$\begin{array}{l}
H^T = \begin{pmatrix} -M \\ \hline Id_{n-m}\end{pmatrix} \implies \\
\\
H = (-M^T | Id_{n - m})
\end{array}$$


