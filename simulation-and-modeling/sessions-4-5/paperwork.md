---
title: Modelado de sistemas continuos
subtitle: Segunda entrega (Modelado y Simulación)
lang: spanish
babel-lang: spanish
polyglossia-lang:
  options: []
  name: spanish
author:
  - Emilio Cobos Álvarez (70912324N)
---

# Modelos matemáticos

## Masa con resorte

Tratamos de obtener una función $x(t)$, donde $x$ es el desplazamiento del
muelle, que depende de $F_a$, la fuerza aplicada sobre la masa.

Tenemos en cuenta la fuerza de rozamiento viscoso, que denotaremos como $F_f$,
con el correspondiente coeficiente de rozamiento que llamaremos $b$.

Sabemos que:

$$F_d = -k \cdot x$$

$$F_f = b \cdot v$$

$$\sum \vec{F} = m \cdot a$$

A partir de ahí obtenemos:

$$\sum \vec{F}(t) = F_a(t) - F_d(t) - F_f(t) = m \cdot a(t)$$

Sustituyendo $a(t)$ por $\frac{\partial v(t)}{\partial t}$, y después $v$ por
$\frac{\partial x(t)}{\partial t}$:

$$ F_a(t) - k \cdot x(t) - b \cdot \frac{\partial x(t)}{\partial t} = m \cdot
\frac{\partial^2 x(t)}{\partial t^2}$$

Despejando:

$$F_a = m \cdot \frac{\partial^2 x(t)}{\partial t^2} + k \cdot x(t) - b \cdot
\frac{\partial x(t)}{\partial t}$$

Aplicando la transformada de laplace (asumiendo $x(0) = 0$):

$$F_a(s) = m \cdot s^2 X(s) + k \cdot X(s) - b \cdot s \cdot X(s)$$
Con lo que llegamos a:

$$G(s) = \frac{X(s)}{F_a(s)} = \frac{1}{m \cdot s^2 + k - b \cdot s}$$

### Sin rozamiento

Para hallar la función de transferencia correspondiente sin rozamiento basta con
igualar $b$ a $0$.

## Circuito RC

Tomaremos como entrada la variable $V_{al}(t)$ y como salida $i(t)$.

Partimos de la ecuación del voltaje del circuito en el dominio del tiempo, que
es:

$$V_{al}(t) = V_r(t) + V_c(t)$$

Sabemos por la ley de Ohm que:

$$ V_r(t) = R \cdot i(t)$$

Siendo un condensador ideal:

$$i(t) = C \frac{\partial V_c(t)}{\partial t} \implies V_c(t) = \frac{1}{C}
\int^t_0 i(t) \, \partial t$$  {#eq:capacitorintensity}

Sustituyendo en la ecuación del circuito:

$$V_{al}(t) = R \cdot i(t) + \frac{1}{C} \int^t_0 i(t) \, \partial t$$

Aplicando la transformada de laplace:

$$V_{al}(s) = R \cdot I(s) + \frac{1}{C \cdot s} \cdot I(s)$$

Sacando factor común a $I(s)$:

$$V_{al}(s) = I(s) \cdot (R + \frac{1}{C \cdot s}) = I(s) \cdot \frac{R \cdot
C \cdot s + 1}{C \cdot s}$$ {#eq:capacitorintensitylaplace}

Podemos por lo tanto obtener la función de transferencia:

$$G(s) = \frac{I(s)}{V_{al}(s)} = \frac{C \cdot s}{R \cdot C \cdot s + 1}$$

### Salida alternativa

Es trivial cambiar la variable de salida a $V_c(t)$:

Aplicando la transformada de laplace a (@eq:capacitorintensity) obtenemos que:

$$V_c(s) = \frac{1}{C \cdot s} I(s) \implies I(s) = V_c(s) \cdot C \cdot s$$

Podemos entonces sustituir en (@eq:capacitorintensitylaplace) $I(s)$ por el
valor obtenido, resultando en:

$$V_{al}(s) = V_c(s) \cdot C \cdot s \cdot \frac{R \cdot C \cdot s + 1}{C \cdot
s} = V_c(s) \cdot R \cdot C \cdot s + 1$$

$$G(s) = \frac{V_c(s)}{V_{al}(s)} = \frac{1}{R \cdot C \cdot s + 1}$$

## Circuto LRC

Tomaremos como entrada $i(t)$, y como salida $V_R(t)$.

### Ecuaciones

Podemos usar las siguientes ecuaciones:

 * Ecuación del circuito:
  $$V_{al}(t) = V_c(t) + V_L(t) + V_R(t)$$
 * Tensión en los extremos del condensador (ver @eq:capacitorintensity):
  $$V_c(t) = \frac{1}{C} \int^t_0 i(t) \, \partial t$$
 * Tensión en los extremos de la bobina [^notreally]:
  $$V_L(t) = L \cdot \frac{\partial i(t)}{\partial t}$$
 * Tensión en los extremos de la resistencia (ley de Ohm):
  $$V_R(t) = R \cdot i_R(t)$$

[^notreally]: Lo cierto es que es $- L \cdot \frac{\partial i(t)}{\partial t}$,
pero para el caso el criterio de signos que cojamos no importa

### Deducción

Sustituyendo en la primera ecuación:

$$V_{al}(t) = L \cdot \frac{\partial i(t)}{\partial t} + R \cdot i(t) + \frac{1}{C} \int^t_0 i(t) \, \partial t$$

`TODO`

## Depósito simple

La entrada que escogemos es $Q_{in}(t)$, y como salida hemos escogido $H(t)$, la
altura del depósito.

Por la ley de conservación de la materia sabemos que:

$$\partial V = Q_{in} \partial t - Q_{out} \partial t$$

Ese diferencial de volumen también se puede expresar como:

$$\partial V = A \partial H$$

Siendo $A$ el área de la base del depósito, y $H$ la altura del mismo.

Igualando ambas expresiones:

$$A \frac{\partial H}{\partial t} = Q_{in} - Q_{out}$$ {#eq:equalityareaflow}

De la teoría hidrodinámica sabemos que:

$$Q_{out} = B \sqrt{2 \cdot g \cdot H} = k \sqrt{H}$$

Lo cual implica, sustituyendo en (@eq:equalityareaflow):

$$A \frac{\partial H}{\partial t} = Q_{in} - k \sqrt{H} \implies \frac{\partial
H}{\partial t} = \frac{Q_{in} - k \sqrt{H}}{A}$$

Es decir, que nuestra salida depende de ella misma, lo cual implica que **el
modelo no es lineal**. Por lo que **sacar el modelo en el dominio de Laplace es
imposible** (a no ser que se linearice en torno a un punto).

# Simulaciones

## Muelle

 * Matlab: Ver el archivo adjunto `muelle.m`.

 * Simulink: Ver el archivo adjunto `muelle_simulink.slx`.

## Circuito RC

 * Matlab: Ver el archivo adjunto `circuit.m`.

## Circuito RLC

 * Matlab: Ver el archivo adjunto `circuit2.m`.

 * Simulink: Ver el archivo adjunto `circuit2_simulink.slx`.

