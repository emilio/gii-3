# Influencia de los sistemas multi-agente en sistemas concurrentes y nuevos
lenguajes de programación

## Resumen

En este papel se analizan las similitudes y diferencias de los sistemas
multiagente con el desarrollo de nuevos lenguajes de programación, con un
objetivo claro de maximizar la concurrencia sin perder seguridad.

Concretamente nos centraremos en:
 * [Rust](https://www.rust-lang.org)
 * [Go](https://golang.org)

Como ejemplo de programa escrito en estos lenguajes con una arquitectura muy
similar a los sistemas multi-agente usaremos
[Servo](https://github.com/servo/servo), un motor de renderizado web cuyo
desarrollo viene encabezado por Mozilla.

## Similitudes con Go

Go es un lenguaje de programación diseñado por Google para ser un lenguaje
adecuado a sus necesidades de escalado y velocidad, centrado en:
 * Permitir de forma fácil la programación distribuída.
 * Maximizar la concurrencia.
 * Seguridad en cuanto a memoria.
 * Velocidad.

Para conseguir la seguridad en cuanto a memoria tiene un **recolector de
basura**, y tiene **canales (`chan`) como tipo de datos nativo del lenguaje**.

Para maximizar la concurrencia y permitir la programación distribuida tiene,
además de los canales que hemos comentado, la idea estrella del lenguaje, las
*go-rutinas* (*goroutines*), una especie de **tarea asíncrona**. Las gorutinas
se diferencian de los hilos en que **son muy ligeras** (usan 2kb de pila por
defecto, y el scheduler se encarga de repartirlas entre todos los hilos/procesos
disponibles). **Es viable tener varios miles de go-rutinas ejecutándose a la vez
en el mismo programa**.

La arquitectura de un programa en go consiste en:
* El scheduler (que se encarga de repartir las gorutinas entre procesos/hilos
    para asegurarse de que son livianas).
* El recolector de basura (paralelo desde hace relativamente poco).
* El programa principal y las go-rutinas del usuario.

### Ejemplo: Productor y consumidor

Las funciones `produce` y `consume` son similares al comportamiento de un
agente.

```go
package main
/* producer-consumer problem in Go */

import ("fmt")

var done = make(chan bool)
var msgs = make(chan int)

func produce () {
  for i := 0; i < 10; i++ {
    msgs <- i
  }
  done <- true
}

func consume () {
  for {
    msg := <-msgs
    fmt.Println(msg)
  }
}

func main () {
  go produce()
  go consume()
  <- done
}
```

### Wrap-up

Go es un lenguaje muy potente y competente para modelar sistemas distribuídos.

El paso de mensajes, básico en la arquitectura de agentes, ha influído de manera
enorme en su diseño.

El recolector de basura, aunque quita el problema de encargarse del manejo de
memoria manual, lo hace un lenguaje menos válido para sistemas **hard
real-time**, aunque genial para **soft real-time**.

## Similitudes con Rust

Rust es un lenguaje que **nace para competir en el nicho de Java y C/C++**.

Es un lenguaje que **usa el análisis estático en vez de un runtime** en tiempo
de compilación para garantizar la seguridad en cuanto a memoria. No obstante
puedes saltarte este check si estás muy seguro de lo que haces usando un bloque
`unsafe {}` (algo similar se ha propuesto para C++17).

Rust tuvo, al igual que go, canales como parte del lenguaje en versiones
preliminares. Para garantizar la seguridad se usaron traits, y ahora esos
canales están en la biblioteca estándar.

### Algunos elementos sintácticos

#### Inmutabilidad por defecto

// TODO

#### Traits

Son un concepto similar al de una interfaz, pero el modo del que están diseñado
permite usar llamadas estáticas en vez de necesitar una vtable (tabla con
punteros a métodos).

```rust
impl<T:Clone + Sub<T, Output=T>> Sub for Point2D<T> {
    type Output = Point2D<T>;
    fn sub(self, other: Point2D<T>) -> Point2D<T> {
        Point2D::new(self.x - other.x, self.y - other.y)
    }
}
```

#### RAII

Modelo de programación muy conocido en C++, que Rust usa extensivamente.

Cuando un tipo quiere un destructor implementa el trait `Drop`.

#### Enum y pattern matching

No (sólo) entendido como el `enum` de C/C++. Es equivalente a una *tagged union
de C*, pero segura.

El *pattern matching* viene dado por la palabra `match` y puede verse como un
`switch` de C ampliado de una manera enorme.

##### C/C++

```c
typedef enum message_type {
  MESSAGE_TYPE_STRING,
  MESSAGE_TYPE_INTEGER
} message_type_t;

struct message {
  message_type_t type;
  union {
    char* string;
    int integer;
  } u;
}

// ...

switch ( message.type ) {
  case MESSAGE_TYPE_INTEGER:
    int content = message.u.integer;
    // hacer algo con `content`
    break;
  case MESSAGE_TYPE_STRING:
    char* content = message.u.string;
    // hacer algo con `content`
    break;
}
```

##### Rust

```rust
enum Message {
  String(String),
  Integer(i32),
}

/// ...

match message {
  Message::String(content) => {
    // hacer algo con el string `content`
  },
  Message::Integer(content) => {
    // hacer algo con el int (i32) `content`
  },
}
```

Rust ya ha ejercido influencia en el diseño de C++17 (`std::optional<T>`,
`std::variant<T>`, propuestas para pattern matching...).

