---
title: Introducción
subtitle: Teoría de Códigos y Teoría de la Información
lang: es-ES
csl: acm-sig-proceedings.csl
babel-lang: spanish
numbersections: true
polyglossia-lang:
  options: []
  name: spanish
author:
  - Emilio Cobos Álvarez (70912324N)
bibliography: intro.bib
---

# Esquema de una transmisión

El esquema general de una transmision es el siguiente:

 * Tenemos una información que queremos transmitir a través de un canal
     **falible**.
 * Necesitamos **codificar** esa información para poder detectar posibles
     errores de transmisión, e incluso detectarlos.
 * La información debe ser **decodificada** fácilmente por el receptor.

Para codificar la información, usaremos una serie de códigos llamados **códigos de
bloques**.

Veremos las definiciones más adelante, preo este es el esquema general de una
transmisión tal y como está definida en [@Shannon:2001:MTC:584091.584093].

![Esquema de una transmisión](img/transmission-scheme.dot.png)

Una transmisión consistirá, por lo tanto, en:

1. Una *fuente de información* que produce un mensaje o secuencia de mensajes
para ser transmitidos al terminal receptor. El mensaje podrá ser de varios
tipos:

    a) Una secuencia de letras, como en un telégrafo o sistema similar.

    b) Una función dependiente del tiempo $f(t)$, como en la radio o el
    teléfono.

    c) Una función dependiente del tiempo y otras variables como en la
    televisión en blanco y negro, donde el mensaje puede ser visto como una
    función $f(x, y, t)$ de dos coordenadas y el tiempo, donde la salida de la
    función es la intensidad de la luz para la coordenada $(x, y)$ en el
    instante $t$.

2. Un *transmisor*, que modifica el mensaje de alguna manera para producir una
señal que pueda ser transmitida por el canal. En telefonía esta operación
consiste únicamente en convertir la presión del sonido en una corriente
eléctrica proporcional. En telegrafía tenemos una *codificación*, que produce
una señal de puntos, líneas y espacios en el canal que se corresponden con el
mensaje.

3. El *canal* es símplemente el medio usado para transmitir la señal del
transmisor al receptor. Puede ser un cable, una banda de frecuencias de radio,
etc. Durante la transmisión, o en uno de los terminales, la señal puede ser
perturbada por el ruido. Este ruido está representado por la línea discontinua
de la figura 1.

4. El *receptor* realiza la operación inversa a la que hizo el transmisor,
reconstruyendo el mensaje desde la señal.

5. El *destino* es la persona (o cosa) a la que el mensaje está dirigido.

## Ejemplo de un código de bloques y una transmisión: bit de paridad

El bit de paridad ya ha sido tratado extensivamente en otras asignaturas, así
que se usará como ejemplo por su simplicidad.

Usaremos el bit de paridad de tres bits de longitud, es decir, un bit de paridad
por cada dos bits de paridad.

Como ejemplo, supondremos que hay una probabilidad de fallo en la transmisión de
un bit de $p = \frac{1}{1000}$.

### Codificación

La función de codificación en este caso se puede definir:

$$CODIFICADA(INFO) \equiv INFO + XOR(INFO)$$

Info    Codificada
-----   ----------
00      000
01      011
10      101
11      110

### Probabilidad de error transmitiendo una palabra (3 bits)

Podemos calcular fácilmente la posibilidad de que ocurra un error al transmitir
una palabra de tres bits sabiendo la posiblidad de transmisión de un bit erróneo
en el canal ($p$):

Número de errores  Probabilidad
-----------------  ----------------------------------------------
0                  $(1 - p)^3$
1                  $(1 - p)^2 \cdot p \cdot 3$
2                  $(1 - p) \cdot p^2 \cdot 3$
3                  $p^3$

Nótese que la multiplicación que puede parecer "extra" significa: "¿Cuántas
combinaciones diferentes de bits con un error podría haber?", es decir:
$\binom{3}{0}$ en el caso sin errores, $\binom{3}{1}$ en el caso con un error,
etc...

Tras esta pequeña re-introducción a la teoría de códigos, pasaremos a ver una
serie de definiciones que nos servirán como base para todo el bloque.

# Definiciones básicas

Usaremos los siguientes términos de forma continuada durante la asignatura, por
lo que se recomienda estar familiarizado con ellos.

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

Llamaremos código de bloques a una aplicación que convierte palabras de longitud
$k$ pertenecientes un alfabeto a $\Sigma$ en palabras de longitud $n$, también
pertenecientes a dicho alfabeto.

$$C: \Sigma^k \rightarrow \Sigma^n$$

Usaremos diferentes métodos que nos servirán, entre otras cosas, para estimar la
"calidad" de un código de bloques, la **razón**, y la **distancia mínima**.

### Razón ($R$)

Definimos la razón como el ratio entre la longitud del código y la dimensión:

$$R = \frac{k}{n}$$

Se puede ver como una **forma de estimar el aprovechamiento del canal**.

### Distancia ($d$)

Definiremos la distancia o **distancia mínima** de un código de bloques $C$ como
el número mínimo de posiciones en las que dos palabras distintas de $C$
difieren.

$$d(C) = \{ min(d(w_1, w_2)) \forall w_1, w_2 \in \Sigma | w_1 \neq w_2 \}$$

La distancia mínima será de gran utilidad para **calcular la capacidad
correctora de un código**, como veremos más adelante.

## Canal

Este es un concepto bastante amplio, pero nosotros asumiermos canales binarios
**simétricos**, es decir, canales que transmiten el alfabeto $\{0, 1\}$ y cuya
probabilidad de error es la misma independientemente del símbolo transmitido.

# Propiedades de los códigos de bloques

Veremos a continuación una serie de propiedades básicas de los códigos de
bloques.

## Distancia, detección y corrección de errores

Un bloque $C$ puede detectar $t$ errores y corregir $t + s$ errores si y sólo
si:

$$d(C) \geq 2t + s + 1$$

Esto se puede comprobar representando el alfabeto de forma gráfica (las palabras
serían puntos y dibujarías aristas de longitud uno entre palabras de distancia
uno), y viendo que no puedes corregir palabras a la misma distancia de otras
dos.

Podéis ver un ejemplo gráfico en [@britz:math3411-problems-22-23:unsw].

## Probabilidad de $k$ errores transmitiendo $n$ bits

Dado $p$, la probabilidad de error al transmitir 1 bit en un determinado canal:

$$p(k, n) = \binom{n}{k} \cdot p^k \cdot (1 - p)^{n-k}$$

Ésto se puede comprobar por inducción sobre $k$.

Recuerda que $\binom{n}{k}$ significa: "¿Cuántas posibles formas existen de
escoger $k$ elementos de un total de $n$?", es decir:

$$\binom{n}{k} = \frac{n!}{k! \cdot (n - k)!}$$

# Teorema de Shannon

En [@Shannon:2001:MTC:584091.584093], se define el concepto de *capacidad*, que
es la máxima cantidad de información capaz de ser transmitida por un canal
discreto en una cantidad finita de tiempo.

Así, un canal de $32$ símbolos distintos puede transmitir un total de $log_2(32)
= 5$ bits por símbolo, por lo que si transmite $n$ símbolos por segundo, su
capacidad será de $5n$ bits por segundo.

De forma genérica, para un canal binario simétrico y e infalible,  la capacidad
del canal $C$ es:

$$C = \lim\limits_{T\rightarrow\inf}\frac{log_2 N(T)}{T}$$

Donde $N(T)$ es el número de señales de duración $T$ permitidas.

Podríamos enunciar el teorema de Shannon como:

> Para cada canal simétrico existe una constante $C$ (la *capacidad*), de tal
> manera que existen códigos de bloques con una razón arbitrariamente cercana
> a $C$, y con una probabilidad de errores de transmisión prácticamente nula.

De laas conclusiones de ese paper de Shannon se puede extraer que esa capacidad
para un canal $C$ con una probabilidad de error $p$ en canales binarios
y simétricos es:

$$C(p) = 1 + log_2(p) + (1 - p) \cdot log_2(1 - p)$$

# Hamming

## Bola/esfera de Hamming

La bola o esfera de una palabra $c \in A^n$ de radio $r$ es el conjunto de
palabras tal que su distancia de Hamming con $c$ es igual o menor que $r$:

$$B(c, r) = \{c' ; d(c, c') \leq r\}$$

El número de elementos en B (para un código binario) es:

$$|B(c, r)| = 1 + n + n \cdot (n - 1) + ... + \binom{n}{r}
= \sum\limits^{r}_{i=0} \binom{n}{i}$$

Como antes, puedes generalizarlo a un código $q$-ario:

$$|B(c, r)| = 1 + n \cdot (q - 1) + \binom{n}{2} \cdot (q - 1)^2 + ...
+ \binom{n}{r} (q - 1)^r  = \sum\limits^{r}_{i=0} \binom{n}{i} \cdot (q - 1)^i$$

Es decir:

 * El número de palabras a distancia $0$ (sólo ella misma), más...
 * El número de palabras a distancia $1$ ($n \cdot (q - 1)$ palabras) más...
 * ...
 * El número de palabras a distancia $r$ ($\binom{n}{r} \cdot (q - 1)^r$).

Si el $(q - 1)$ no te resulta intuitivo, piensa que el número de
palabras a distancia $d$ de una palabra en un código $q$-ario es $\binom{n}{d}
\cdot (q - 1)$, es decir: habrá $\binom{n}{d}$ combinaciones de $d$ caracteres
cambiando a la vez, pero cada uno podrá variar de $(q - 1)$ formas distintas, es
decir, $(q - 1)^d$.

Así, las palabras a distancia $2$ de la palabra $000$ de un código ternario
serán $3 \cdot 2^2 = 12$, es decir, habrá tres formas de que dos caracteres a la
vez varíen ($\binom{3}{2}$), cada uno de ellos tendrá dos maneras distintas de
cambiar (todas las combinaciones menos la actual), es decir, $3 - 1$, todo ello
una vez por cada número ($(3 - 1)^2$).

# Cota de Hamming

> Si $C$ es un código binario de longitud $n$ y distancia mínima $2r + 1$,
> entonces $C$ tiene como mucho $\frac{2^n}{\sum\limits^{r}_{i=0} \binom{n}{i}}$
> palabras.

Esto se puede generalizar fácimente a códigos de bloques cuyo alfabeto es
$q$-arios, donde la cota se convertiría en:

$$\frac{q^n}{\sum\limits^{r}_{i=0} \binom{n}{i} \cdot (q - 1)^i}$$

Esto se puede probar imaginando un espacio circular llenado por las palabras de
$C$. Cada palabra tiene una bola de radio $r$ "a su alrededor". Todas las
palabras del alfabeto que estén dentro de esa bola las podremos corregir, por lo
que **las bolas no se solaparán**.

Con esto, podemos decir que **el número de palabras del código es como mucho
igual al número de palabras del alfabeto dividido entre el número de palabras
que contiene cada bola**.

Ya hemos visto que el número de palabras de una bola de radio $r$ es:

$$\sum\limits^{r}_{i=0} \binom{n}{i} \cdot (q - 1)^i$$

Así que la fórmula sale directamente.

Tambien se llama teorema del empaquetado esférico por su representación gráfica.

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

# Referencias
