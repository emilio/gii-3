---
title: Práctica de sockets
author:
  - Emilio Cobos Álvarez (70912324) <emiliocobos@usal.es>
lang: es-ES
toc-depth: 4
babel-lang: spanish
polyglossia-lang:
  options: []
  name: spanish
---

# Implementación

## Modelo de servidor

Se ha usado un modelo de servidor bastante diferente al propuesto, cono **un
sólo proceso**, un hilo por solicitud TCP, y un sólo hilo para todas las
solicitudes UDP.

## Mantenimiento del estado en conexiones UDP

Se ha creado un tipo de datos `vector_t` (ver los archivos `vector.h`
y `vector.c`), que sirve como un contenedor genérico de tamaño variable.

Esta estructura de datos se usa extensivamente a lo largo de la práctica.

En el caso concreto de las conexiones UDP, se crea un vector para el tipo de
datos `udp_cache_data`, inicialmente de tamaño 20:

```c
typedef struct udp_cache_data {
    connection_state_t state;
    struct sockaddr address;
    socklen_t address_len;
    time_t last_access;
} udp_cache_data_t;

// ...

vector_t connection_state_data;
vector_init(&connection_state_data, sizeof(udp_cache_data_t), 20);
```

Esta estructura de datos contiene el estado de la conexión (para el caso si el
usuario está logueado), la dirección remota (`struct sockaddr`), junto con su
longitud, y el último acceso a esta conexión, para poder implementar timeouts,
ya que UDP no sabe cuándo se ha desconectado la otra parte.

Cuando llega un mensaje nuevo, se busca el estado anterior en un bucle, o en su
defecto alguno que haya expirado y que se pueda reutilizar:

```c
// Find the state associated with this connection (if any)
time_t now = time(NULL);
ssize_t free_index = -1;
size_t i;
for (i = 0; i < vector_size(&connection_state_data); ++i) {
    vector_get(&connection_state_data, i, &data);
    // We've got our old state \o/
    if (sockaddr_cmp((struct sockaddr*)&src_addr, &data.address) == 0)
        break;
    else if (free_index == -1 &&
             now - data.last_access > SESSION_TIMEOUT)
        free_index = i;
}
```

Si no se encontrara ninguno, se añade un nuevo ítem al vector:

```c
// If there's none, push a new one, else reuse that one
if (free_index == -1) {
    LOG("udp: State vector entry not found, creating");
    vector_push(&connection_state_data, &data);
} else {
    LOG("udp: Reusing entry %zu", i);
    i = free_index;
}
```

## Implementación del protocolo

Todo lo relativo al protocolo está en los archivos `protocol.c` y `protocol.h`.

La estructura central del protocolo es el mensaje que se manda, aunque también
hay estructuras para representar un evento, una asistencia, una invitación, y un
usuario:

```c
typedef struct client_message {
    client_message_type_t type;
    union {
        protocol_uid_t uid; // HELLO/BYE
        struct {            // ASSISTANCE_LIST
            protocol_uid_t uid;
            protocol_event_id_t event_id;
        } assistance_list_info;
        struct { // ASSISTANCE
            protocol_uid_t uid;
            protocol_event_id_t event_id;
            struct tm datetime;
        } assistance_info;
    } content;
} client_message_t;

typedef struct protocol_event {
    protocol_event_id_t id;
    protocol_description_t description;
    struct tm starts_at;
    struct tm ends_at;
} protocol_event_t;

typedef struct protocol_assistance {
    protocol_uid_t uid;
    protocol_event_id_t event_id;
    struct tm at;
} protocol_assistance_t;

typedef struct protocol_invitation {
    protocol_uid_t uid;
    protocol_event_id_t event_id;
} protocol_invitation_t;

typedef struct protocol_user { protocol_uid_t id; } protocol_user_t;
```

La interfaz que estos archivos dan para parsear los datos del protocolo es la
siguiente:

```c
parse_error_t parse_client_message(const char* in_source,
                                   client_message_t* message);

parse_error_t parse_event(const char* in_source, protocol_event_t* event);

parse_error_t parse_assistance(const char* in_source,
                               protocol_assistance_t* assistance);

parse_error_t parse_invitation(const char* in_source,
                               protocol_invitation_t* invitation);

parse_error_t parse_user(const char* in_source, protocol_user_t* user);
```

La implementación de estas funciones está en el archivo `protocol.c`, y se
recomienda su lectura.

Existen pruebas para la lectura de los tipos de mensajes en `tests/tests.c`.

## Forma de actuar del servidor

El servidor guarda en memoria la representación de los datos desde su inicio, en
una estructura llamada `GLOBAL_DATA`.

Esta estructura tiene un mutex para proteger accesos concurrentes a memoria
mutable, y una serie de vectores donde se almacenan los datos recogidos al
inicio de los archivos.

**Es esta sección de memoria la que se lee y se escribe cuando se comunican con
los clientes**. Hay varias formas de hacer que los datos de las asistencias (que
en la práctica son los únicos mutables), se vuelquen en el archivo:

 * Matando al servidor limpiamente (via `SIGTERM` o `SIGINT`).
 * Mandando la señal `SIGUSR1` al servidor.

## Forma de desarrollo

Se ha seguido una forma de desarrollo orientada a tests, sobre todo en lo
referente al análisis de mensajes.

Los tests se pueden encontrar en el directorio `tests`, y se pueden ejecutar con
el comando:

```sh
$ make test
```

Esta práctica ha resultado tener un valor incalculable a la hora de ahorrar
tiempo de desarrollo.

## Problemas en Solaris y olivo

La máquina en la que la práctica tiene que ejecutarse es un Solaris, además
bastante antiguo, por lo que hubo alguna que otra complicación:

### Make

En olivo `make` no es el programa GNU Make, sino otra versión diferente, que no
soporta una gran cantidad de funcionalidad que yo usaba. Por suerte GNU Make
está en olivo como `gmake`.

### El compilador de C

En olivo el comando `cc` está asociado a un archivo al que por defecto no se
puede acceder, por lo tanto hay un if en el `Makefile` para solucionarlo,
comprobando el hostname:

```make
# Workaround for buggy C compiler in required targets (encina and olivo/SunOS)
ifeq ($(shell hostname), encina)
	CC := gcc
endif
ifeq ($(shell hostname), olivo)
	CC := gcc
endif
```

### Las librerías

Solaris requiere linkar más librerías que las que hacen falta en Linux. Solución
directa también:

```make
ifeq ($(UNAME), SunOS)
	CLINKFLAGS := $(CLINKFLAGS) -lsocket -lnsl
	CFLAGS := $(CFLAGS) -DSOLARIS
endif
```

### Timeouts

En un principio la implementación de los timeouts se hacía con un socket
bloqueante usando la opción `SO_RCVTIMEO`. Hasta que llegamos a solaris y...
resulta que no lo soporta[^solaris-recvtimeo].

Hubo que hacer una migración de `SO_RCVTIMEO` a `poll()`, aunque fueron cambios
menores.

[^solaris-recvtimeo]: Tras verlo con mis propios ojos encontré [esta issue de
Apache](https://issues.apache.org/jira/browse/THRIFT-1371), y [esta
página](http://www.pixelstech.net/article/1399694359-Socket-programming-tips-in-Solaris).

# Protocolo más adecuado para esta práctica

Sin duda, dado que es una conversación que mantiene estado entre dos clientes,
el protocolo más adecuado para esta práctica es **TCP**.


