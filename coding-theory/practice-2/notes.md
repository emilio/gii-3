# Estudio de códigos cíclicos de long. 7.

Recordar que código cíclico es un código $C$ lineal, binario, tal que al rotar
cualquier palabra de $C$ nos da otra palabra de $C$.

$$c = c_6 c_5 \dots c_0 \approx c_6 x^6 + \dots + c_1 x + c_0$$

Una palabra $c$ es correcta si es múltiplo de $g(x)$, siendo $g(x)$ divisor de
$x^7 + 1$.

La factorización de $x^7 + 1$ es:

$$x^7 + 1 = (x + 1) \cdot (x^3 + x + 1) \cdot (x^3 + x^2 + 1)$$

A los que llamaremos, respectivamente, $P_1$, $P_2$ y $P_3$.

Podemos hacerlo a mano, pero en un lenguaje como *Mathematica* sería algo así
como `Factor[x^7 - 1, Modulus -> 2]`.


Independientemente del polinomio generador la **la longitud será 7**.

La dimensión será **7 menos el grado de $g(x)$**.

g(x)                dim
----------          ------------
$P_1$               6
$P_2$               4
$P_3$               4
$P_1P_2$            3
$P_2P_3$            1
$P_1P_3$            3

# Decodificación BCH

No lo habíamos visto en clase. El caso es que en BCH al multiplicar por la
generadora la información queda entremezclada.

Se puede definir un polinomio generador de forma sistemática para que la
información quede en los $k$ primeros bits.
