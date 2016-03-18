---
title: Perfil Personal
subtitle: Interacción Persona-Ordenador
numbersections: true
toc: true
links-as-notes: true
author:
  - Emilio Cobos Álvarez (70912324-N)
lang: spanish
babel-lang: spanish
polyglossia-lang:
  options: []
  name: spanish
---

# Introducción

En esta práctica hemos hecho una página personal usando tecnologías web, con la
intención de variar el contenido lo mínimo, obteniendo tres diseños diferentes,
con características diferentes.

# Modo de desarrollo y herramientas

El código fuente debería ir adjunto con este informe. Para escribir todo el
código fuente se ha usado el editor [`vim`](http://www.vim.org/), y ha sido
llevado a cabo exclusivamente con software libre en un sistema *GNU/Linux*.

Como preprocesador de CSS se ha usado [`sass`](http://sass-lang.com/), con el
objetivo de hacer más liviana la sintaxis y poder hacer cálculos con variables
que de otra forma se harían más tediosos.

Como herramienta para asegurarse de que la página final siempre tenía los
últimos cambios se ha usado [`make`](https://www.gnu.org/software/make/).

Como procesador automático de imágenes se ha usado
[`ImageMagick`](http://www.imagemagick.org/script/index.php), y como editor de
las mismas, para cambios manuales, [`GIMP`](https://www.gimp.org).

Como generador de PDF para el informe se ha usado [`pandoc`](http://pandoc.org),
que internamente usa [`xelatex`](https://en.wikipedia.org/wiki/XeTeX).

Para testear el software se ha usado
[Firefox](https://www.mozilla.org/en-US/firefox/new) como navegador principal,
pero también Chromium para asegurar compatibilidad con los motores basados en
WebKit/Blink[^compatibility-notice].

[^compatibility-notice]: Probablemente el único diseño que no sea compatible con
IE$\leq 9$ sea el tercero, que usa constantemente *viewport units* en vez de
unidades fijas.

# Estilo "minimal"

El primer estilo que hice fue el que he convenido en llamar "minimal". No es
nada excesivamente complejo, pero aún así creo que consigue alcanzar un balance
interesante entre profesionalidad (o algo así) e informalidad.

## Color

He usado como color predominante en el fondo, y como color para resaltar los
links el azul claro, que da una impresión de profesionalidad y es muy utilizado
en el sector tecnológico.

Como color de texto, nunca me gusta usar un color negro total, sino una versión
un poco más grisácea, que hace que el contraste con el blanco de fondo no sea
tan agresivo para la vista. Este detalle también aplica a los otros diseños.

## Tipografía

He usado una fuente *sans serif* para el texto, concretamente **Open Sans**, que
es una de mis favoritas para usar tanto en títulos, como en texto plano.

He tratado de ajustar el interlineado o `line-height` de tal manera que el texto
sea legible, pero no demasiado espaciado como para que los párrafos quedaran
irreconocibles.

## Pequeño *Easter Egg*

Hay un pequeño *Easter Egg* en este diseño. No es muy difícil de encontrar si
usas el ratón. Solución[^solution].

[^solution]: Mi foto (antigua como ella sola, por cierto) no se estará quieta si
pasas el ratón por encima.

# Estilo "rusty"

Este estilo trata de reflejar el contenido como un pequeño fragmento de código
en [rust](http://rust-lang.org).

Fue sido un doble intento. Por un lado quería hacerlo personalmente, porque se
me ocurrió y me pareció original. Por otro lado, tiene pequeños detalles
técnicos que son no triviales para conseguir que el HTML siga siendo semántico.

## Color

El esquema de colores usado no lo he inventado yo, se llama
[*solarized*](http://ethanschoonover.com/solarized), y es un esquema de color
diseñado para ser agradable a la vista (aparte de tener otras [propiedades
interesantes](http://ethanschoonover.com/solarized#features)).

De hecho, es el esquema de color que uso habitualmente para programar (alterno
ambas variantes dependiendo de la hora del día que sea).

## Tipografía

La tipografía, de nuevo, está basada en mi experiencia personal. Al principio
iba a arriesgarme a usar la fuente `monospace` por defecto, pero luego me
imaginé cuál sería en Windows y se me pasó.

Se llama **Fira Mono**, y es una tipografía con la que he programado bastante
tiempo (a pesar de no ser la que uso actualmente).

## Algunos detalles técnicos

No se ha usado ni una pizca de javascript específica para realizar el diseño,
todo está hecho con pseudo-elementos CSS y animaciones.

# Estilo "flat"

Este diseño usa la imagen de fondo para conseguir dar profundidad al texto
superior, y la imagen superior para conseguir contraste.

## Color

He querido usar un color oscuro de fondo, para que fuera más fácil hacer legible
el texto frontal. Como color para resaltar los links he utilizado el color más
presente en la imagen superior ligeramente modificado.

## Imagen

La imagen de fondo está hecha con una panorámica encontrada en internet,
convertida a blanco y negro por ImageMagick, y con un degradado en el borde
inferior derecho hecho con GIMP, para evitar problemas con pantallas muy altas.

La imagen frontal es una foto mía (también de hace ~2 años, como la primera) con
una chica cuya identidad no ha querido que sea revelada en este informe.

El color verde del paisaje es el que hace que resalte esa parte de la página
especialmente, transmitiendo una sensación mucho más alegre que el fondo.

## Proporciones

Los anchos y del recuadro y la relación con la imagen no son casualidad, sino
que se usa la proporción áurea para conseguirlos.

Si se tiene más interés en los detalles, se puede consultar el código fuente
adjunto.
