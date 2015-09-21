# Notes for Computer Networks I - Session 1

## Material
* Serial cable (7 threads) (2m)
* 2 female 9 pins connectors

## Emulators
* [com0com](http://com0com.sourceforge.net)

## Intro
* rs232c standard

History...

### Films
* "Juegos de guerra"

## Serial port

### Async serial port transmission
1. High bit start
2. Charo (5-8 bits)
3. [Optional] parity bit
4. Low bit stop
5. `goto 1`

---
**NOTE**: Inverted frecuency (1 ~= 15V; 0 ~= -15V). See the RS-232-C standard for more info.

#### Communication example
* `DTR`: Data terminal ready. The sender pings the receiver.
* `DSR`: Data set ready. The receiver pings back.
* `RTS`: Request To Send. Self-descriptive.
* `CTS`: Clear to send. The receiver tells the sender it can send.
* `RI`: Ring Indicator. The receiver tells the sender that it has *received a call*.

---
**NOTES**:
s/sender/etd (ETD)
s/receiver/módem (ETCD)

#### Hardware
##### `UART`: Universal Asyncronous Receptor/Transmissor
Integrated circuit to translate parallel data to serial and vice-versa

Transmission:
* *Eg.*: From CPU (x32/x64) to characters (x8).
* Construct the Serial Data Unit (*SDU*): (start + char + parity + stop)
* Serialize that data unit (parallel -> serial)
* Send

Reception:
* Detect start, parity and stop bits.
* Detect transmission errors.
* Deserialize (serial -> parallel).
* Deliver the character to the reception.

---
**NOTE**: The first bit sent is the less significative: That means that if we sent the char `00000001`, the receiver will receive `10000000`.

#### Configuration
* Word: 5, 6, 7 and 8 bits.
* Stop bit: 1, 1.5 and 2.
* Parity enabled or not, odd or even

#### Programming
##### Windows
...
##### UNIX
`struct termios` + `cfsetospeed` + `cfsetispeed` + `tcflush` + `tcsetattr`

