---
title: Prototipado en papel -- Google Play Music
subtitle: Interacción Persona Ordenador
date: 10 de Mayo de 2016
toc: true
fontsize: 12pt
lang: es
numbersections: true
babel-lang: spanish
polyglossia-lang:
  options: []
  name: spanish
author:
  - Emilio Cobos Álvarez (70912324N)
  - Héctor Gonzalo Andrés (71038384D)
  - Alberto González San Juan (70909715W)
  - Víctor Alonso Fraile (70909161T)
  - Ulises Herrero Sánchez (44414199A)
abstract: |
  En esta práctica se realiza un análisis crítico de *Google Play Music*,
  y se propone un rediseño basado en el usuario y poniendo la funcionalidad por
  delante de la apariencia, realizando un prototipado en papel como prueba de
  concepto.
---

# Introducción y fondo teórico

El principal concepto de diseño al que hemos dado vueltas en esta práctica es el
concepto de usabilidad. Pasamos a exponer mínimamente algunos conceptos de los
vistos en clase que hemos tratado durante la práctica.

Usabilidad:

  ~ Facilidad con que las personas pueden utilizar una herramienta particular
  con el fin de alcanzar un objetivo concreto.

Diseño centrado en el usuario:

  ~ Este es un concepto muy amplio, así que sólo se incluirá aquí una pequeña
  introducción al *UCD*.

    Esta filosofía de diseño trata de **resolver las necesidades concretas de los
  usuarios finales**, consiguiendo una mejor experiencia de uso.

    Obviamente, para resolver dichas necesidades es necesario conocerlas, por lo
  que el *UCD* conlleva habitualmente una **cantidad muy importante de
  investigación**.

Primero **analizaremos** cómo se podría mejorar la usabilidad de esta aplicación
desde nuestro punto de vista, para luego pasar a **investigar las necesidades de
otros posibles usuarios finales** mediante entrevistas en persona, para
finalmente hacer una **propuesta** de rediseño.

# Análisis de *Google Play Music*

Elegimos investigar sobre *Google Play Music*. El motivo es que consideramos
que, a pesar de que la aplicación tiene un diseño acorde al resto de
aplicaciones Android, su usabilidad es reducida respecto a la competencia,
**priorizando la publicidad y la venta de contenido respecto a la reproducción
de música**.

Otra posible crítica es la ausencia de carácter social, no teniendo capacidad de
compartir música con otros usuarios, una cracterística crucial según otros
competidores como *Spotify*:

> College radio stations might not hold the sway they once did, now that dorm
> rooms are connected to all of the music on the internet. But it’s somewhat
> telling that Napster, the file-sharing service that fundamentally altered the
> course of music history, was created in a dorm room to help students share
> music with each other.
>
> --- [*Spotify Insights: How U.S. college students listen to
music*](https://insights.spotify.com/es/2015/10/09/how-us-college-students-listen/)

Estos factores son, en nuestra opinión, cruciales para explicar por qué la
adopción de esta aplicación ha sido peor tanto frente a reproductores de música
"clásicos" (como el de stock de algunos dispositivos Android), como frente
a otros servicios on-line de reproducción y venta de música, como *Spotify*.

# Investigación

Hemos investigado preguntando a un total de 10 compañeros sobre por qué o por
qué no usan esta aplicación, y hemos obtenido algunas conclusiones que respaldan
nuestra crítica inicial, y que proponen otra funcionalidad extra que no está
presente en esta aplicación.

El perfil de los usuarios es el siguiente:

  * La mayoría tienen la aplicación instalada, pero **sólo uno lo usa como
  servicio de reproducción de música principal**.

  * Sólo un usuario ha realizado pagos en la aplicación, pero sólo para el
  servicio premium, no para comprar ninguna canción o álbum.

Hemos dividido esta sección en base a la experiencia de los usuarios tanto con
esta aplicación como con aplicaciones similares. No obstante, las respuestas
a una de las preguntas realizadas no se pueden clasificar en base a esto.

Cuando se preguntó a los entrevistados acerca de qué funcionalidad extra les
gustaría tener que no tuvieran en su reproductor habitual, la mayoría no supo
qué contestar, pero uno de ellos comentó que el reproductor por defecto en su
móvil Android tenía un tema claro y que, aunque le gusta, preferiría poder tener
una forma de cambiarlo porque le resulta molesto a la vista por la noche.

## Usuarios no familiares con la aplicación

Algunos de los comentarios más reveladores sobre la aplicación por parte de
usuarios no familiares con ella:

  * La colección de música no se muestra al arrancar, sino que se muestra una
  pantalla con actividad reciente y recomendaciones.

    Esto muestra el interés de Google en priorizar la venta de contenidos, lo
  cual vemos razonable, aunque consideramos que tiene que haber un balance
  adecuado teniendo en cuenta que se trata de la aplicación de música por
  defecto en el S.O. Android.

  * El reproductor que aparece en la primera pantalla no permite leer el título
  de la última canción que estabas reproduciendo si es de más de 6 caracteres
  (este dato depende del ancho de la pantalla, pero es cierto en un móvil de
  5 pulgadas).

## Usuario habitual de la aplicación

El usuario más familiar con la aplicación (el que lo usa como servicio de
reproductor de música habitual), también expresó su descontento con la pantalla
de inicio.

Además, como mejoras extra, propuso:

  * Mostrar las letras de la canción mientras se escucha sin tener que cambiar
  de aplicación.

    Comentó que utilizaba una aplicación llamada *Genius* que le muestra una
  notificación cuando se detecta la letra de la canción que se está
  reproduciendo, por tanto esto crea una dinámica de salto de aplicación en
  aplicación que le parecía bastante mejorable.

  * Un ecualizador. Nos comentó que es aficionado a los ecualizadores de música
  y cuenta con varios dispositivos en los que utiliza *Google Play Music*. Esto
  le crea un problema ya que, aunque utiliza los mismos auriculares en los tres
  dispositivos pero en dos de ellos, al ser de diferente marca, tienen un
  ecualizador diferente, y el tercero ni siquiera cuenta con ecualizador si no
  se utiliza una aplicación externa.

  * Un widget para poder pausar y reanudar la música sin tener que abrir la
  aplicación completamente.

Dicho usuario también comentó acerca de porqué prefiere usar la aplicación en
vez de otras de la competencia:

  * La mayor razón por la que la usa es porque *Google Play Music* cuenta con
  una aplicación de escritorio que *"sube a la cuenta de Google"* tu colección
  de música, y esto permite utilizar un dispositivo Android como si se tratara
  de un iPod+iTunes, con el pro de que en su dispositivo puede sincronizar los
  álbumes que quiera offline, y el resto escucharlos en streaming.

## Usuarios habituales de *Spotify*

Como dato bastante curioso y revelador, de los 5 usuarios habituales de
*Spotify* a los que entrevistamos, cuando se les preguntó acerca de razones por
las que usar *Spotify* en vez de *Google Play Music*, cuatro de ellos
mencionaron la capacidad de compartir música.

## Notas acerca del proceso de investigación

Somos conscientes de que nuestro proceso de investigación tiene un fallo
importante, y es que, a pesar de que los datos son extremadamente valiosos,
**sólo hemos preguntado a compañeros de facultad**, por lo que nos estamos
perdiendo una cantidad enorme de puntos de vista de otros usuarios potenciales,
que en un proceso de investigación más exhaustivo deberíamos de tener en cuenta.

\clearpage

# Propuesta

Teniendo en cuenta los datos expuestos anteriormente, proponemos un rediseño de
la aplicación para tratar de solventar los problemas de usabilidad reportados
por los usuarios potenciales.

Da **prevalencia a la colección de música en la primera pantalla**, ofreciendo
la compra únicamente durante la reproducción, y de tal manera que no sea
intrusiva para el usuario.

Proponemos mostrar las **letras de la canción durante la reproducción** si el
usuario hace *swipe* hacia arriba sobre la carátula del álbum en la vista de
reproducción, una acción que ahora mismo no se utiliza para nada.

Integra un **modo noche en el menú principal** (que en la aplicación actual
tiene mucho espacio libre), de tal manera que cambiar de un modo a otro sólo
estaría a dos toques de distancia.

También se añade el **perfil social**, en el cual podremos tener nuestras
playlist públicas, recomendar canciones (dando like o no mientras se reproducen)
o ver la música que están escuchando nuestros amigos.

Además, proponemos incluir un ecualizador en la aplicación, el cual tendrá
capacidad de sincronizarse entre tus dispositivos, aprovechando que los puede
asociar a tu cuenta de Google.

Finalmente, el rediseño también añade un **widget flotante** totalmente
personalizable para controlar la reproducción desde cualquier lugar, que
permite:

  * Mostrar la información de la canción actual.
  * Pausar y reanudar la canción con un sólo toque.
  * Pasar a la canción anterior o siguiente deslizando el dedo hacia la
  izquierda o hacia la derecha.
  * Decir si te gusta la canción o no deslizando el dedo hacia arriba o hacia
  abajo haciendo así que aparezcan tus recomendaciones en el feed de tus amigos.
  * Cambiar a una canción completamente aleatoria de tu colección realizando un
  movimiento circular.

# Conclusiones

Creemos que los cambios mencionados en la propuesta producirían una mejor
experiencia de usuario en general[^on-beliefs], y una mayor utilización del
producto en particular.

[^on-beliefs]: Sabemos que "creer" no proporciona ninguna evidencia. Nos
gustaría haber tenido el tiempo suficiente para hacer algunas pruebas de
concepto con usuarios (por ejemplo, probar cuánto tarda un usuario completamente
ajeno a la aplicación en encontrar una canción en la aplicación original y en el
rediseño) y reportar resultados.

Consideramos que en este caso **la aplicación original no hizo todo lo posible
a la hora de hacer un diseño centrado en el usuario, y que esto ha traído
consecuencias importantes en cuanto a la cantidad de usuarios que utilizan la
aplicación**.
