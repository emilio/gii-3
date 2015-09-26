# OSI Layers

## Relevant links

* [OSI model in Wikipedia](https://en.wikipedia.org/wiki/OSI_model)
* [Flow Control in Wikipedia](https://en.wikipedia.org/wiki/Flow_control_%28data%29)

## Physical Layer

Raw transmission of bit streams.

Definition of the following characteristics:

* **Physical**: Pin type, shape...
* **Electrical**: Voltages, line inpedance, timing
* **Functional**: Simple, duplex, half-duplex
* **Network topology**

## Data Link Layer

Node-to-node data transfer. Synchronisation and **error correction** from the physical layer, flow control (overflow control).

## Network Layer

Routing data sequences (packet) from one nowde to another (provided they're connected to the same network).

## Transport layer

Reliable data transference, and verification of the data.

Eg: The **TCP/IP** protocol.

## Session layer

Synchronization state between to nodes (keeping state).

## Presentation layer

Conversion of the data to the application required format (**semantics**).
Compression, cyphering...

## Application layer

Interaction with the end user

# OSI Terminology

## Entity

An entity is an active element in any level, either software, hardware or both.

## Service

Interface that an inferior level provides to a superior one, regardless of the implementation.

The **provider** exposes the interface, the **user** consumes it.

## Tipos de servicios

* Con confirmación (send + recv)
* Sin confirmación (send)
* Servicios indicados por el proveedor

## Protocol

The way two entities talk to each other.
