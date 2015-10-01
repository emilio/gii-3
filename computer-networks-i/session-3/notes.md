# The link level

Notes:
* Block == packet, meaning **LPDU** (*Link Protocol Data Unit*)

## Data links

Set of data terminal equipment, with their associated circuits that **allow
information exchange**.

The data link implies:

* Physical level (OSI level 1)
* Logical level (OSI level 2)

### Phases to exchange information

1. Connection of the circuit (physical level).
1. Logical link establishment

## Suministrated services

### Main service

Data transfer from the network layer from the origin machine to the destination.

Block transmission between directly-connected machines.

### Service types

#### No connection, no confirmation

The sender just sends. Convenient if there's a low error rate.

Used in RTA (Real Time Applications).

#### No connection, confirmation

No connection prelude, but each data sent is confirmed.

#### Connection oriented service

There's a connection prelude.

The flow is something like:

1. Estabilishing a connection.
1. Data transmission (checked).
1. Freeing the connection.

### Functions of the link level

1.  Well-defined interface with the network layer.
1.  Syncronisation of data blocks/packets.
1.  Error control.
1.  Flow control.
    * Regulation o the packet flow to avoid saturation of the receivers.
1.  Moderate the access to the link layer when it's shared between multiple
    systems.

## Techniques for packet control

### Begin-end

A few characters indicate the beginning of a group and the end of the group.

### TODO translate: Guión

A group of bits to identify **both** the beginning and the end of the block.

In this and the previous technique **to prevent special characters in the data
segments extra bits might be inserted**. For example, given we have a protocol
where `01111110` is the marker for the begin and end.

To transmit a `01111110` character, always that a string of 5 ones is detected,
**a zero is inserted after them** (and later trimmed by the receiver).

### Begin and length

A few characters identify the beginning and after it a counter to indicate its
length.

### Code violation

For example, marking the beginning of the block as a plain signal, and using
Manchester for the rest of the communication.

## Error control

### Error rates

See in the class presentation error rates of different mediums.

### Techniques

#### FEC: Forward Error control

Allows to **detect and correct** the errors in place.

Hamming distance, etc...

Not used in data transmission due to high redundancy.

#### ARQ: Automatic Repeat reQuest

A payload is added to the packet to the end, in order to allow just the
**detection** of the error. If it's detected, it requests the packet again.

Each packet must be numbered or identified in order to be requested again.

## Protocol types

## Protocol examples

