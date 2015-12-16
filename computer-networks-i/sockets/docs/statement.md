Below is the raw statement in spanish, just for reference:

---

## Entorno de desarrollo

 * Estación de trabajo SUN (olivo)
 * Sockets de Berkeley
 * Lenguaje de programación C

Durante el desarrollo de la práctica se puede trabajar en Linux pero es
obligatorio que funcione en la estación de trabajo SUN olivo.

## Requisitos

Se implementará un sistema de control de asistencia a eventos. De la siguiente
forma:

## Órdenes del cliente al servidor

    HOLA id_usuario (identificarse en el sistema)
    LISTAR EVENTOS (listará los eventos activos)
    LISTAR id_usuario id_evento (listará los fichajes de un usuario a un determinado evento)
    FICHAR id_evento id_usuario dia-hora (fichaje de la asistencia de un usuario a un evento
    ADIOS id_usuario (despedida para finalizar)

## Respuestas del servidor

Todas las órdenes del cliente se responderán:

    CORRECTO
    ERROR texto del error

La lista de eventos se devolverá de la siguiente forma:

    id_evento#descripción del evento#fecha y hora de inicio#fecha y hora de fin

A la orden de LISTAR id_evento id_usuario se devolverá un listado con los fichajes realizados por un usuario en un determinado evento de la siguiente forma:

    id_usuario#id_evento#dia-hora

## Otras consideraciones

 * La descripción del evento no podrán superar los 140 caracteres.
 * El id_usuario y el id_evento no podrán superar 20 caracteres.
 * El servidor verificará la correcta sintaxis de todas las órdenes recibidas.
 * El servidor leerá los eventos de un fichero llamado eventos.txt con la
     siguiente información:

    id_evento#descripción del evento#fecha y hora de inicio#fecha y hora de fin

 * Y los usuarios registrados en los eventos de un fichero llamado usuarios.txt con la siguiente información:

    id_usuario#id_evento

 * Los ficheros de eventos y usuarios se gestionarán manualmente no siendo necesario implementar órdenes para rellenar estos ficheros.
 * Los fichajes se almacenarán en un fichero de nombre fichajes.txt con la siguiente información:

    id_usuario#id_evento#dia y hora

## Ejemplo de dialogo:

```
cliente>FICHAR id_evento id_usuario 09/11/2015 11:11:30

servidor>ERROR No se ha identificado proporcione un id_usuario. Ej: HOLA id_usuario

cliente>HOLA id_usuario

servidor>CORRECTO

cliente>LISTAR EVENTOS

servidor>id_evento#descripción del evento#dia y hora de inicio#dia y hora de fin

cliente>FICHAR id_evento id_usuario 08/11/2015 11:11:30

servidor>ERROR Evento ya finalizado

cliente>FICHA id_evento id_usuario 08/11/2015 11:11:30

servidor>ERROR Orden no reconocida

cliente>FICHAR id_evento id_usuario_1 09/11/2015 11:11:30

servidor>ERROR Usuario no registrado en el evento

cliente>FICHAR id_evento id_usuario 09/11/2015 11:11:30

servidor>CORRECTO

cliente>LISTAR id_evento id_usuario

servidor>id_evento#id_usuario#fecha y hora

cliente>ADIOS id_usuario

servidor>CORRECTO
```

## Programa Servidor

 * Aceptará peticiones de sus clientes tanto en TCP como en UDP
 * Registrará todas las peticiones en un fichero de "log" llamado fichajes.log en el que anotará:
    * Conexiones realizadas: nombre del host, dirección IP, protocolo de
        transporte y nº de puerto efímero del cliente
    * Fecha y hora a la que se ha producido
    * Si el fichaje se ha realizado correctamente. Si no se ha realizado
        especificar la causa.

## Programa Cliente

  * Se conectará con el servidor bien con TCP o UDP.
  * Leerá por parámetros el nombre del servidor y si es TCP o UDP de la
      siguiente forma: `cliente nombre_del_servidor TCP`.
  * Realizará el fichaje como se ha indicado anteriormente.

Durante la fase de pruebas el cliente podrá ejecutarse como se muestra en el
ejemplo de diálogo anterior, pero **en la versión para entregar el cliente leerá
de un fichero las órdenes que ha de ejecutar (ordenes.txt)** y escribirá las
respuestas obtenidas del servidor **y los mensajes de error y/o depuración en un
fichero con nombre el número del puerto efímero y extensión .txt**.

Para verificar que esta práctica funciona correctamente y permite operar con
varios clientes, **se utilizará el script `lanzaServidor.sh` que han de
adjuntarse obligatoriamente** en el fichero de entrega de esta práctica. Este
script lanzan el servidor así como una serie de clientes de prueba.

El contenido de lanzaServidor.sh es el siguiente:

```sh
# lanzaServidor.sh
# Lanza el servidor que es un daemon y varios clientes
# las ordenes están en un fichero que se pasa como tercer parámetro
servidor
cliente olivo TCP ordenes.txt &
cliente olivo TCP ordenes1.txt &
cliente olivo TCP ordenes2.txt &
cliente olivo UDP ordenes.txt &
cliente olivo UDP ordenes1.txt &
cliente olivo UDP ordenes2.txt &
```

## Enlaces

 * Consultar en material práctico de la asignatura "Programas de Demostración
     API de Sockets Unix/Linux"
 * Scripts para lanzar los programas, ficheros ejemplos del sistema de fichaje
     y ficheros de ordenes (ordenes.zip)
 * Orden útil para ver quien utiliza un puerto: lsof
 * Transparencias utilizadas para explicar los ejemplos y esta práctica: 1 x Página | 2 x Página (Fecha: 16/11/215)


## Condiciones de entrega

Práctica opcional

Las primeras líneas de todos los ficheros fuente (.c y .h) contendrán unas
líneas de comentarios donde se especifique el nombre del fichero fuente y el
nombre completo y DNI de los autores. Ejemplo:

```c
/*
** Fichero: serie.c
** Autores:
** Antonio Pérez Puerto DNI 70876543M
** Araceli Sánchez Álvarez DNI 06876701A
*/
```

Es **obligatorio crear un fichero Makefile que genere todos los ejecutables
y adjuntarlo en la entrega de la práctica**.

Es obligatorio **adjuntar el scripts de ejecución, los ficheros del sistema de
fichaje y de ordenes puesto que sin ellos la práctica no será evaluada**.

Entregar un **informe en formato PDF que incluya los aspectos que se deseen
destacar de la implementación de la práctica y la justificación del protocolo de
transporte que el alumno considera como el más adecuado** para la implementación
de la misma.

Los ficheros fuente, Makefile, script de ejecución, ficheros del sistema de
fichaje, ficheros de ordenes y el informe se comprimirán en un fichero llamado
sockets.zip.

El incumplimiento de cualquiera de estas indicaciones, así como de cualquiera de
los requisitos especificados supondrá un suspenso en esta práctica.

 * Fecha límite de entrega: 17-diciembre-2015
 * Hora límite de entrega: 20:10

