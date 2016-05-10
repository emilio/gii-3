---
title: Prototipado en papel -- Google Play Music
subtitle: Interacción Persona Ordenador
date: 10 de Mayo de 2016
toc: true
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

Se pasa a definir algunos de los conceptos dados durante la asignatura que se
han utilizado a lo largo del desarrollo de la práctica.

Usabilidad
: facilidad con que las personas pueden utilizar una herramienta particular
con el fin de alcanzar un objetivo concreto.

TODO: more are missing here.

# Crítica a *Google Play Music*

Elegimos investigar sobre *Google Play Music*. El motivo es que consideramos
que, a pesar de que la aplicación tiene un diseño acorde al resto de
aplicaciones Android, su usabilidad es mucho menor que otro tipo de aplicaciones
de reproducción de música, y **da a la propia reproducción de música un papel
secundario**, priorizando la publicidad y la venta de contenido.

Otra posible crítica es que no tiene la capacidad de compartir música con otros
usuarios, una capacidad que es crucial según otros competidores como *Spotify*:

> College radio stations might not hold the sway they once did, now that dorm
> rooms are connected to all of the music on the internet. But it’s somewhat
> telling that Napster, the file-sharing service that fundamentally altered the
> course of music history, was created in a dorm room to help students share
> music with each other.
>
> --- [*Spotify Insights: How U.S. college students listen to
music*](https://insights.spotify.com/es/2015/10/09/how-us-college-students-listen/)

Estos factores son, en nuestra opinión, cruciales en porqué la adopción de esta
aplicación ha sido peor tanto frente a reproductores de música "clásicos", como
el de stock de Android, como frente a otros servicios on-line de reproducción
y venta de música, como *Spotify*.

## Opinión de usuarios

Hemos investigado preguntando a un total de 10 compañeros sobre porqué o porqué
no usan esta aplicación, y hemos obtenido algunas conclusiones que respaldan
nuestra crítica inicial, y que proponen otra funcionalidad extra que no está
presente en esta aplicación:

 * La mayoría tienen la aplicación instalada, pero sólo uno lo usa como servicio
 de reproducción de música principal.
 * Sólo un usuario ha realizado pagos en la aplicación, pero sólo para el
 servicio premium, no para comprar ninguna canción o álbum.

Algunos de los comentarios más reveladores sobre la aplicación por parte de
usuarios no familiares con ella:

 * La colección de música no se muestra al arrancar, sino que se muestra una
 pantalla con actividad reciente y recomendaciones.
 * El reproductor que aparece en la primera pantalla no permite leer el título
 de la última canción que estabas reproduciendo si es de más de 6 caracteres
 (este dato depende del ancho de la pantalla, pero es cierto en un móvil de
 5 pulgadas).

El usuario más familiar con la aplicación (el que lo usa como servicio de
reproductor de música habitual), también expresó su descontento con la pantalla
de inicio. Como mejoras extra, propuso:

 * Mostrar las letras de la canción mientras se escucha sin tener que cambiar de
 aplicación.
 * Un ecualizador, que tienen la mayoría de los otros reproductores.
 * Un widget para poder pausar y reanudar la música sin tener que abrir la
 aplicación completamente.

Dicho usuario también comentó acerca de porqué prefiere usar la aplicación en
vez de otras de la competencia cuando se le preguntó. Acerca de esto dijo que la
mayor razón por la que la usa es porque tiene otra aplicación de escritorio
desde la que puede tener la colección de música sincronizada, y puede
sincronizar selectivamente álbums.

De los 5 usuarios habituales de *Spotify* a los que entrevistamos, cuando se les
preguntó acerca de razones por las que usar *Spotify* en vez de *Google Play
Music*, cuatro de ellos mencionaron la capacidad de compartir listas de
reproducción.

Cuando se preguntó a los entrevistados acerca de qué funcionalidad extra les
gustaría tener que no tuvieran en su reproductor habitual, la respuesta por
parte de la mayoría fue silencio, pero uno de ellos comentó que el reproductor
por defecto en su móvil Android tenía un tema claro y que, aunque le gusta,
preferiría poder tener una forma de cambiarlo porque le resulta molesto a la
vista por la noche.

# Propuesta

Teniendo en cuenta los datos expuestos anteriormente, proponemos un rediseño de
la aplicación que solventaría estos problemas de forma centrada en el usuario.

El rediseño trata de solucionar los problemas de usabilidad reportados por los
usuarios.

Da prevalencia a la colección de música en la primera pantalla, ofreciendo la
compra únicamente durante la reproducción, y de tal manera que no sea intrusiva
para el usuario.

Proponemos mostrar las letras de la canción durante la reproducción si el
usuario hace swipe hacia arriba, una acción que ahora mismo no se utiliza para
nada.

Integra un modo noche en el menú principal (que en la aplicación actual tiene
mucho espacio libre), de tal manera que cambiar de un modo a otro
sólo estaría a dos toques de distancia.

También se añade la opción para compartir playlists desde el menú de listas de
reproducción, donde se pueden marcar como públicas para que aparezcan en el feed
de otros usuarios que están en tus contactos de Google.

Finalmente, el rediseño también añade un widget totalmente personalizable para
los usuarios más impacientes, que permite:

 * Mostrar la información de la canción actual.
 * Pausar y reanudar la canción con un sólo toque.
 * Pasar a la canción anterior o siguiente deslizando el dedo hacia la izquierda
 o hacia la derecha.
 * Decir si te gusta la canción o no deslizando el dedo hacia arriba o hacia
 abajo.
 * Cambiar a una canción completamente aleatoria de tu colección realizando un
 movimiento circular.

Creemos que estos cambios conllevarían una mejor experiencia de usuario, y en
general una mejor adopción del producto.
