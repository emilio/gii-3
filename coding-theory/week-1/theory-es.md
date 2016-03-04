---
title: Introducción
subtitle: Teoría de Códigos y Teoría de la Información
lang: spanish
babel-lang: spanish
polyglossia-lang:
  options: []
  name: spanish
author:
  - Emilio Cobos Álvarez (70912324N)
---

# Esquema de una transmisión

El esquema general de una transmision es el siguiente:

 * Tenemos una información que queremos transmitir a través de un canal
     **falible**.
 * Necesitamos **codificar** esa información para poder detectar posibles
     errores de transmisión, e incluso detectarlos.
 * La información debe ser **decodificada** fácilmente por el receptor.

Para codificar la información, usaremos una serie de códigos llamados **códigos de
bloques**. Veremos las definiciones más adelante, preo este es el esquema
general de una transmisión:

![Esquema de una transmisión](img/transmission-scheme-es.dot.png)

## Ejemplo de un código de bloques y una transmisión: bit de paridad

El bit de paridad ya ha sido tratado extensivamente en otras asignaturas, así
que se usará como ejemplo por su simplicidad.

Recordemos que el código de paridad consiste en 2 bits de información, y un bit
de paridad.

Asumiremos que hay una probabilidad de fallo en la transmisión de un bit de $p
= \frac{1}{1000}$.

### Codificación

La función de codificación se puede definir:

$$CODIFICADA(INFO) \equiv INFO + XOR(INFO)$$

Info    Codificada
-----   ----------
00      000
01      011
10      101
11      110

### Probabilidad de error transmitiendo una palabra (3 bits)

Número de errores  Probabilidad
-----------------  ----------------------------------------------
0                  $(1 - p)^3$
1                  $(1 - p)^2 \cdot p \cdot 3$
2                  $(1 - p) \cdot p^2 \cdot 3$
3                  $p^3$

Nótese que la multiplicación "extra" significa: "¿Cuántas combinaciones
diferentes de bits con un error podría haber?", es decir: $\binom{3}{0}$ en el
caso sin errores, $\binom{3}{1}$ en el caso con un error, etc...

# Definiciones básicas

## Alfabeto ($\Sigma$)

Conjunto de símbolos que representan la información a ser transmitida.

Por ejemplo: $\{0, 1\}$

## Palabra ($w$) de longitud $k$

Sucesión de $k$ símbolos pertenecientes a un alfabeto $\Sigma$.

Por ejemplo, la palabra $001$, es una palabra de longitud $3$ perteneciente al
alfabeto $\Sigma = \{0, 1\}$

### Peso de una palabra

El peso de una palabra es el número de símbolos no nulos de esa palabra, es
decir, el número de símbolos que no son $0$.

Por ejemplo, el peso de la palabra $101$ es $2$.

## Distancia entre dos palabras ($d$)

La distancia entre dos palabras de la misma longitud, $w_1$ and $w_2$, es el
número de posiciones en las que difieren.

Por ejemplo, $d(001, 101) = 1$, porque sólo difieren en el primer símbolo.

## Código de bloques de alfabeto $\Sigma$, longitud $k$ y dimensión $n$

Aplicación que convierte palabras de longitud $k$ pertenecientes a $\Sigma$ en
palabras de longitud $n$, también pertenecientes a $\Sigma$.

$$C: \Sigma^k \rightarrow \Sigma^n$$

### Razón ($R$)

Definimos la razón como el ratio entre la longitud del código y la dimensión:

$$R = \frac{k}{n}$$

Se puede ver como una **forma de estimar el aprovechamiento del canal**.

### Distancia ($d$)

La distancia o **distancia mínima** de un código de bloques $C$ es el número
mínimo de posiciones en las que dos palabras distintas de $C$ difieren.

$$d(C) = \{ min(d(w_1, w_2)) \forall w_1, w_2 \in \Sigma | w_1 \neq w_2 \}$$

## Canal

Este es un concepto bastante amplio, pero nosotros asumiermos canales binarios
**simétricos**, es decir, canales que transmiten el alfabeto $\{0, 1\}$ y cuya
probabilidad de error es la misma independientemente del símbolo transmitido.

# Propiedades de los códigos de bloques

## Distancia, detección y corrección de errores

Un bloque $C$ puede detectar $t$ errores y corregir $t + s$ errores si y sólo
si:

$$d(C) \geq 2t + s + 1$$

Esto se puede comprobar representando el alfabeto de forma gráfica (las palabras
serían puntos y dibujarías aristas de longitud uno entre palabras de distancia
uno), y viendo que no puedes corregir palabras a la misma distancia de otras
dos.

## Probabilidad de $k$ errores transmitiendo $n$ bits

Dado $p$, la probabilidad de error al transmitir 1 bit en un determinado canal:

$$p(k, n) = \binom{n}{k} \cdot p^k \cdot (1 - p)^{n-k}$$

Recuerda que $\binom{n}{k}$ significa: "¿Cuántas posibles formas existen de
escoger $k$ elementos de un total de $n$?", es decir:

$$\binom{n}{k} = \frac{n!}{k! - (n - k)!}$$

# Teorema de Shannon

> Para cada canal simétrico existe una constante $C$ (la *capacidad*), de tal
> manera que existen códigos de bloques con una razón arbitrariamente cercana
> a $C$, y con una probabilidad de errores de transmisión prácticamente nula.

Esa capacidad puede ser definida (en canales binarios) como:

$$C(p) = 1 + log_2(p) + (1 - p) \cdot log_2(1 - p)$$

# Cota de Hamming

> Si $C$ es un código binario de longitud $n$ y distancia mínima $2r + 1$,
> entonces $C$ tiene como mucho $\frac{2^n}{\sum\limits^{r}_{i=0} \binom{n}{i}}$
> palabras.

Esto se puede generalizar fácimente a códigos de bloques cuyo alfabeto es
$q$-arios, donde la cota se convertiría en:

$$\frac{q^n}{\sum\limits^{r}_{i=0} \binom{n}{i} \cdot (q - 1)^i}$$

Tambien se llama teorema del empaquetado esférico por su representación gráfica.

## Bola/esfera de Hamming

La bola o esfera de una palabra $c \in A^n$ de radio $r$ es el conjunto de
palabras tal que su distancia de Hamming con $c$ es igual o menor que $r$:

$$B(c, r) = \{c' ; d(c, c') \leq r\}$$

El número de elementos en B (para un código binario) es:

$$|B(c, r)| = 1 + n + n \cdot (n - 1) + ... + \binom{n}{r}
= \sum\limits^{r}_{i=0} \binom{n}{i}$$

Como antes, puedes generalizarlo a un código $q$-ario:

$$|B(c, r)| = 1 + (q - 1) \cdot n + (q - 1)^2 \cdot n \cdot (n - 1) + ... + (q
- 1)^r \cdot \binom{n}{r} = \sum\limits^{r}_{i=0} \binom{n}{i} \cdot (q - 1)^i$$

## Código perfecto

Llamamos código perfecto a aquel código que alcanza la cota de Hamming.

# Cota de Singleton

> Si $C$ es un código $q$-ario de longitud $n$ y distancia mínima $d$, entonces
> $A_q(n, d) \leq q^{n - d + 1}$.

Aquí $A_q(n, d)$ representa el máximo numero de palabras en un código $q$-ario
de longitud $n$ y distancia mínima $d$.

Esto, para códigos lineales, implica que:

$$dim(C) \leq n - d + 1$$

# Cota de Gilbert-Varshamov

$$|C| = A_q(n, d) \geq \frac{q^n}{\sum\limits^{d - 1}_{i = 0} \binom{n}{i} \cdot
(q - 1)^i}$$
