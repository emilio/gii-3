---
title: Práctica DLX
subtitle: Arquitectura de Computadores - 2016
numbersections: true
lang: es
babel-lang: spanish
author:
  - Emilio Cobos Álvarez
  - Héctor Gonzalo Andrés
---

# Problema a resolver

Esta práctica consistía en el desarrollo y optimización de un código que realice
el siguiente cálculo:

$$R = (A^{-1} \Theta B^{-1}) \cdot \frac{1}{|A|} \cdot \frac{1}{|B|}$$

Siendo $A$ y $B$ matrices de $2x2$ y:

$$\Theta(A, B) = \begin{pmatrix}
  A_{11} \cdot |B| & A_{12} \cdot |B| & B_{11} \cdot |B| & B_{12} \cdot |B| \\
  A_{21} \cdot |B| & A_{22} \cdot |B| & B_{21} \cdot |B| & B_{22} \cdot |B| \\
  B_{11} \cdot |A| & B_{12} \cdot |A| & A_{11} \cdot |A| & A_{12} \cdot |A| \\
  B_{21} \cdot |A| & B_{22} \cdot |A| & A_{21} \cdot |A| & A_{22} \cdot |A| \\
\end{pmatrix}$$

# Notas previas

Se adjuntan un total de **4 programas** (en vez de los dos que pedía el
enunciado).

El motivo para esto es que una de las técnicas de optimización que se ha
utilizado es [una aproximación muy precisa][opt] de la inversa de un número en
punto flotante en formato IEEE 754 (el formato que usa DLX y casi todos los
procesadores modernos), que **evita la división y la reemplaza por un simple
ajuste del exponente**, que en DLX son tres instrucciones (asumiendo que el
registro `f7` contiene el número a invertir, y `r31` contiene el número
`0x7EEEEEEE`):

```asm
movfp2i r1, f7
subu r1, r31, r1
movi2fp f7, r1
```

Esta estrategia implica que, además de no tener que gastar los 19 ciclos de la
división, la FPU queda potencialmente libre para otros cálculos[^free-fpu].

A pesar de que, en nuestra opinión, es una optimización válida, ante la
posibilidad de que no fuera considerada válida se ha mantenido la versión
original conservando las instrucciones `divf`.

Las optimizaciones aplicadas a ambas versiones son similares salvo la comentada.

[opt]: http://bits.stephan-brumme.com/inverse.html
[^free-fpu]: Esta ventaja, en la práctica, no se pudo aprovechar porque todos
los cálculos posteriores a estos dependían de las dos inversiones.

# Optimizaciones matemáticas

Lo primero que hicimos a la hora de afrontar esta práctica, es hacer los
cálculos correspondientes, para ver cuáles se podían simplificar. Esto implica
que nuestras versiones no optimizadas ya tienen todas las "optimizaciones"
relacionadas con eliminar cálculos duplicados aplicadas por lo general.

Los cálculos hechos son los siguientes:

$$A^{-1} = \frac{1}{|A|} \begin{pmatrix}
  a_4 & -a_2 \\
  -a_3 & a_1
\end{pmatrix}$$

$$B^{-1} = \frac{1}{|B|} \begin{pmatrix}
  b_4 & -b_2 \\
  -b_3 & b_1
\end{pmatrix}$$

$$|B^{-1}| = \frac{b_4}{|B|} \frac{b_1}{|B|} - \frac{b_2}{|B|} \frac{b_3}{|B|}
= \frac{1}{|B|^2} (|B|) = \frac{1}{|B|}$$
$$|A^{-1}| = \frac{1}{|A|}$$

Con eso, llegamos a la conclusión de que, tras desarrollar las cuentas:

$$\Theta(A^{-1}, B^{-1}) = \begin{pmatrix}
   a_4X & -a_2X &  b_4Y & -b_2Y \\
  -a_3X &  a_1X & -b_3Y &  b_1Y \\
   b_4X & -b_2X &  a_4Z & -a_2Z \\
  -b_3X &  b_1X & -a_3Z &  a_1Z \\
\end{pmatrix}$$

con:

$$X = \frac{1}{|B|^2} \cdot \frac{1}{|A|^2}$$
$$Y = \frac{1}{|B|^3} \cdot \frac{1}{|A|}$$
$$Z = \frac{1}{|A|^3} \cdot \frac{1}{|B|}$$

# Versión no optimizada

## Sin división (`src/opt.s`)

La versión sin división está anotada para que sea lo más legible posible.

### Estadísticas

#### Total

  * **N° de ciclos**: 172
  * **N° de instrucciones ejecutadas (IDs)**: 65

#### Stalls

  * **RAW stalls**: 21 (12.21%)
      * **LD stalls**: 1 (4.76%)
      * **Branch/Jump stalls**: 0
      * **Floating point stalls**: 20 (95%)
  * **WAW stalls**: 0
  * **Structural stalls**: 78 (45.35%)
  * **Control stalls**: 0
  * **Trap stalls**: 3 (1.74%)
  * **Total**:  102 (59.3%)

#### Conditional Branches

  * **Total**: 0
      * **Tomados**: 0
      * **No tomados**: 0

#### Instrucciones Load/Store

  * **Total**: 26
      * **Loads**: 10 (38.46%)
      * **Stores**: 16 (61.54%)

#### Instrucciones de punto flotante

  * **Total**: 32 (49.23%)
      * **Sumas**: 6 (18.75%)
      * **Multiplicaciones**: 26 (81.25%)
      * **Divisiones**: 0

#### Traps

  * **Traps**: 1 (1.54%)

## Con división (`src/semi-opt.s`)

### Estadísticas

#### Total

  * **N° de ciclos**: 203
  * **N° de instrucciones ejecutadas (IDs)**: 61

#### Stalls

  * **RAW stalls**: 39 (19.21%)
      * **LD stalls**: 1 (2.56%)
      * **Branch/Jump stalls**: 0
      * **Floating point stalls**: 38(97.44%)
  * **WAW stalls**: 0
  * **Structural stalls**: 96 (47.29%)
  * **Control stalls**: 0
  * **Trap stalls**: 3 (1.48%)
  * **Total**:  138 (67.98%)

#### Conditional Branches

  * **Total**: 0
      * **Tomados**: 0
      * **No tomados**: 0

#### Instrucciones Load/Store

  * **Total**: 26
      * **Loads**: 10 (38.46%)
      * **Stores**: 16 (61.54%)

#### Instrucciones de punto flotante

  * **Total**: 34 (49.23%)
      * **Sumas**: 6 (17.65%)
      * **Multiplicaciones**: 26 (76.47%)
      * **Divisiones**: 2 (5.88%)

#### Traps

  * **Traps**: 1 (1.64%)

# Versión optimizada

## Sin división (`src/opt-reordered.s`)

### Estadísticas

#### Total

  * **N° de ciclos**: 146
  * **N° de instrucciones ejecutadas (IDs)**: 65

#### Stalls

  * **RAW stalls**: 5 (3.38%)
      * **LD stalls**: 1 (11.11%)
      * **Branch/Jump stalls**: 0
      * **Floating point stalls**: 8 (100%)
  * **WAW stalls**: 0
  * **Structural stalls**: 67 (45.89%)
  * **Control stalls**: 0
  * **Trap stalls**: 6 (4.11%)
  * **Total**:  81 (56.16%)

#### Conditional Branches

  * **Total**: 0
      * **Tomados**: 0
      * **No tomados**: 0

#### Instrucciones Load/Store

  * **Total**: 26
      * **Loads**: 10 (38.46%)
      * **Stores**: 16 (61.54%)

#### Instrucciones de punto flotante

  * **Total**: 32 (49.23%)
      * **Sumas**: 6 (18.75%)
      * **Multiplicaciones**: 26 (81.25%)
      * **Divisiones**: 0

#### Traps

  * **Traps**: 1 (1.54%)

## Con división (`src/semi-opt-reordered.s`)

### Estadísticas

#### Total

  * **N° de ciclos**: 177
  * **N° de instrucciones ejecutadas (IDs)**: 61

#### Stalls

  * **RAW stalls**: 26 (14.70%)
      * **LD stalls**: 2 (7.96%%)
      * **Branch/Jump stalls**: 0
      * **Floating point stalls**: 24 (92.31%)
  * **WAW stalls**: 0
  * **Structural stalls**: 86 (48.60%)
  * **Control stalls**: 0
  * **Trap stalls**: 6 (3.40%)
  * **Total**:  119 (66.67%)

#### Conditional Branches

  * **Total**: 0
      * **Tomados**: 0
      * **No tomados**: 0

#### Instrucciones Load/Store

  * **Total**: 26
      * **Loads**: 10 (38.46%)
      * **Stores**: 16 (61.54%)

#### Instrucciones de punto flotante

  * **Total**: 34 (55.74%)
      * **Sumas**: 6 (17.65%)
      * **Multiplicaciones**: 26 (76.47%)
      * **Divisiones**: 2 (5.88%)

#### Traps

  * **Traps**: 1 (1.64%)

# Optimizaciones realizadas

La mayoría de optimizaciones realizadas, aparte de la comentada anteriormente,
son en lo referente a **evitar dependencias innecesarias entre operaciones**,
y sobre todo, **mantener a la FPU ocupada siempre que se pueda**, ya que es el
cuello de botella de este programa.

Las reordenaciones se han realizado usando la lógica (y de vez en cuando prueba
y error).

Como ejemplo, las instrucciones de multiplicación y load de la versión no
optimizada:

```asm
  lw r31, INVERSE_CONSTANT(r0)
  lf f1, M10(r0)
  lf f2, M11(r0)
  lf f3, M12(r0)
  lf f4, M13(r0)
  subf f5, f0, f2
  subf f6, f0, f3
  multf f7, f1, f4
  multf f8, f2, f3
  subf  f7, f7, f8
```

Son reordenadas así:

```asm
  lf f1, M10(r0)
  lf f4, M13(r0)
  lw r31, INVERSE_CONSTANT(r0)
  multf f7, f1, f4
  lf f2, M11(r0)
  lf f3, M12(r0)
  multf f8, f2, f3
```

(con las otras dos instrucciones movidas más hacia abajo ya que no son de uso
inmediato).

Así se pone a funcionar la FPU lo más rápido posible (en cuanto `f1` y `f4`
están disponibles, módulo una instrucción para evitar un `RAW Stall`).

Ese tipo de optimizaciones, de tratar de abusar lo máximo posible del
paralelismo a nivel de instrucción y mantener tanto a la ALU como a la FPU
ocupada el máximo tiempo posible ha sido nuestra principal estrategia de
optimización, más allá de la optimización de la inversión, que obviamente es una
amplia ganancia de 30 ciclos prácticamente gratuita.


# Comparación de resultados

TODO
