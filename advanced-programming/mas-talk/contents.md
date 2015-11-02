---
title: Influencia de los S.M.A. en sistemas concurrentes y nuevos lenguajes de programación
author:
  - Emilio Cobos Álvarez (70912324N) <emiliocobos@usal.es>
  - Víctor Barrueco Gutiérrez (53518348Q) <vbarruec@usal.es>
  - Arturo Balleros Albillo (70912222W) <arturoballeros@usal.es>
date: \today
lang: spanish
babel-lang: spanish
polyglossia-lang:
  options: []
  name: spanish
abstract: |
  En este papel se analizan las similitudes y diferencias de los sistemas
  multiagente con el desarrollo de nuevos lenguajes de programación, con un
  objetivo claro de maximizar la concurrencia sin perder seguridad.

  Concretamente nos centraremos en:

   * [Rust](https://www.rust-lang.org)
   * [Go](https://golang.org)
   * [Erlang](https://www.erlang.org) (si da tiempo)

  Como ejemplo de programa escrito en estos lenguajes con una arquitectura muy
  similar a los sistemas multi-agente usaremos
  [Servo](https://github.com/servo/servo), un motor de renderizado web cuyo
  desarrollo viene encabezado por Mozilla.
---

# Go

## Introducción

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

## Ejemplo: Productor y consumidor

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

## Wrap-up

Go es un lenguaje muy potente y competente para modelar sistemas distribuídos.

El paso de mensajes, básico en la arquitectura de agentes, ha influído de manera
enorme en su diseño.

El recolector de basura, aunque quita el problema de encargarse del manejo de
memoria manual, lo hace un lenguaje menos válido para sistemas hard
real-time, aunque genial para **soft real-time**.

## Recursos

 * [Página oficial](https://golang.org)
 * [Tutorial oficial](https://tour.golang.org)
 * [Pequeña introducción con
     vídeos](https://gist.github.com/honkskillet/bd1f72223dd8e06b5ce6)

# Rust

## Introducción

Rust es un lenguaje que **nace para competir en el nicho de Java y C/C++**.

Es un lenguaje que **usa el análisis estático en vez de un runtime** en tiempo
de compilación para garantizar la seguridad en cuanto a memoria. No obstante
puedes saltarte este check si estás muy seguro de lo que haces usando un bloque
`unsafe {}` (algo similar se ha propuesto para C++17).

Rust tuvo, al igual que go, canales como parte del lenguaje en versiones
preliminares. Para garantizar la seguridad se usaron traits, y ahora esos
canales están en la biblioteca estándar.

## Algunos elementos sintácticos

### Inmutabilidad por defecto e inferencia

En Rust todas las variables son inmutables por defecto, salvo que especifiques
lo contrario.

Aunque parezca poco intuitivo, tiene sus ventajas. **Es más difícil cometer
errores sin querer**, además de que el hecho de que el compilador sepa de
primera mano que una variable es inmutable le da **más campo para optimizar** el
programa en tiempo de compilación.

Otra de las ventajas de Rust es que tiene inferencia de tipos. Esto quiere decir
que el compilador hace el trabajo de escribir los tipos por tí en muchas
situaciones.

Algunos ejemplos:

```rust
fn main() {
    // Anotación de tipo explícita (opcional)
    let v1: Vec<i32> = vec![1, 2, 3];
    let vector = vec![1, 2, 3];

    // No tiene que saber el tipo completo en la inicialización
    // En este punto para el compilador la variable tiene el tipo
    // Vec<_>
    let mut mutable_vector = vec![];

    // No funcionaría, vector es inmutable
    // vector.push_back(3);

    // Aquí se le añade un i32, el compilador a partir de ahora
    // trata a `mutable_vector` como Vec<i32>
    mutable_vector.push_back(5);

    // Error: mutable_vector es Vec<i32> no Vec<f32> (float)
    // mutable_vector.push_back(0.4f32);

    // Imprime:
    // 1
    // 2
    // 3
    for i in vector {
        println!("{}", i);
    }
}
```

### Traits

Son un concepto similar al de una interfaz, pero el modo del que están diseñado
permite usar llamadas estáticas en vez de necesitar una vtable (tabla con
punteros a métodos).

```rust
// Este es un ejemplo bastante enrevesado:
// Implementar el trait `Sub` para la clase `Point2D`
// Si el tipo T es clonable y soporta el trait `Sub`
// para su propio tipo.
//
// Resumiendo: Implementa la resta para todo punto hecho
// de elementos que se puedan restar y clonar.
impl<T:Clone + Sub<T, Output=T>> Sub for Point2D<T> {
    type Output = Point2D<T>;
    fn sub(self, other: Point2D<T>) -> Point2D<T> {
        Point2D::new(self.x - other.x, self.y - other.y)
    }
}
```

### RAII

Modelo de programación muy conocido en C++, que Rust usa extensivamente.

Cuando un tipo quiere un destructor implementa el trait `Drop`.

### Enum y pattern matching

No (sólo) entendido como el `enum` de C/C++. Es equivalente a una *tagged union
de C*, pero segura.

El *pattern matching* viene dado por la palabra `match` y puede verse como un
`switch` de C ampliado de una manera enorme.

#### C/C++

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

#### Rust

```rust
use std::sync::mpsc::channel;
use std::thread;

enum Message {
    String(String),
    Integer(i32),
}

fn main() {
    let (tx, rx) = channel();

    thread::spawn(move || {
        tx.send(Message::String("Hola!"));
    });

    match rx.recv().unwrap() {
        Message::String(content) => {
            println!("{}", content);
        },
        Message::Integer(content) => {
          // hacer algo con el int (i32) `content`
        },
    }
}
```

Rust ya ha ejercido influencia en el diseño de C++17 (`std::optional<T>`,
`std::variant<T>`, propuestas para pattern matching...).

## Recursos

 * [Guía oficial](https://doc.rust-lang.org/book/)
 * [Guía oficial (avanzada)](https://doc.rust-lang.org/nightly/nomicon/)
 * [Rust by Example](http://rustbyexample.com/)
 * [Bugs fáciles para contribuir en
     Servo](https://github.com/servo/servo/labels/E-easy)

# Erlang

## Introducción

Erlang es un lenguaje de programación desarrollado inicialmente por Ericsson en
1986. [^erlangnote]

Es un lenguaje funcional diseñado precisamente para sistemas *soft real-time*
distribuídos.

La forma de programar concurrentemente con Erlang suele ser mediante paso de
mensajes.

Ejemplos de usuarios de Erlang:

 * WhatsApp
 * Amazon (SimpleDB)
 * Facebook (Chat)
 * T-Mobile, Motorola, Ericsson...

[^erlangnote]: Hay que tener en cuenta que en aquella época las compañías
telefónicas eran de las pocas que tenían la necesidad de tener un sistema
distribuido con esas necesidades en cuanto a tiempo de respuesta.

## Recursos

 * [Curso oficial](http://www.erlang.org/course/course.html)
 * [Ejemplo productor-consumidor
     distribuído](https://people.kth.se/~johanmon/dse/detector.pdf)
 * [Learn you some Erlang for Great Good!](http://learnyousomeerlang.com/)
     (gratis online)

# Conclusiones

La idea de este artículo es **ubicar los sistemas multi-agente** como parte
imprescindible dentro del **aumento de la relevancia de los mecanismos de paso
de mensajes** para sincronización y el desarrollo de sistemas distribuídos
complejos, e introducir a plataformas que a pesar de no estar específicamente
diseñadas para ello, responden a diferentes necesidades y son apropiadas para su
desarrollo, teniendo diferentes pros y contras.

También pretende ser una pequeña introducción a algunas de estas plataformas que
están tomando mucha relevancia en los últimos años.

## Comparativa de los sistemas presentados

### Go

**Tipos de sistemas más adecuados**:

  * Distribuído soft real-time.
  * Local soft real-time.

**Pros**:

  * Sintaxis parecida a C.
  * Recolector de basura.
  * Canales como tipo incluído en el sistema.
  * Compilado a código máquina.
  * Go-rutinas.
  * El scheduler, tareas ligeras.
  * Buenas herramientas (`gofmt`, etc..).
  * Muy buenas librerías estándar

**Contras**:

  * Recolector de basura.
  * Las go-rutinas siguen existiendo cuando se salen del scope, y si no se
      tratan con cuidado introducen memory leaks[^goroutines_leak].
  * Manejo de paquetes pobre.

[^goroutines_leak]:
[http://openmymind.net/Leaking-Goroutines/](http://openmymind.net/Leaking-Goroutines/)

### Rust

**Tipos de sistemas más adecuados**:

  * Tanto soft como hard real-time.

**Pros**:

  * Sistema de tipos relativamente novedoso y muy potente para ser un lenguaje
      compilado y de propósito general.
  * Multiparadigma.
  * Traits.
  * Borrow-checker.
  * Buena integración con librerías en C.
  * Sin runtime, lo que lo hace adecuado para sistemas hard real-time.
  * Muy buen manejo de paquetes vía `cargo`.
  * Compilador muy potente, con lints, plugins, etc...

**Contras**:

  * Menos librerías que el resto de lenguajes de los que hemos hablado (ha
      alcanzado la estabilidad hace relativamente poco).
  * Desarrollo de sistemas distribuídos más complicado que los dos anteriores
      (por falta de librerías).

### Erlang

**Tipos de sistemas más adecuados**:

  * Distribuídos soft real-time.

**Pros**:

  * Tareas ligeras, de forma similar a go (`go func() -> spawn(func)`).
  * Diseñado específicamente para sistemas distribuídos.
  * Puramente funcional (podría ser un contra!).
  * Estabilidad del lenguaje (mucho más antiguo que el resto).
  * Menos tiempo de desarrollo y probablemente muchos menos bugs de
      concurrencia, por el diseño del lenguaje.

**Contras**:

  * Sintaxis más complicada, especialmente si no se está acostumbrado a la
      programación funcional.
  * Corre sobre una máquina virtual, no sobre hardware directamente.

