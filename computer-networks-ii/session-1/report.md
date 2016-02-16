---
title: Analizadores de tráfico de red
subtitle: Práctica I - Redes II
lang: spanish
babel-lang: spanish
polyglossia-lang:
  options: []
  name: spanish
author:
  - Emilio Cobos Álvarez (70912324N)
---

# ICMP

## Órdenes ejecutadas

### `tcpdump`

Nótese que se realiza esta práctica desde el ordenador portátil personal, por
eso se puede ver la interfaz `wlan0`.

```
$ sudo tcpdump -vv -w session-1.cap
tcpdump: listening on wlan0, link-type EN10MB (Ethernet), capture size 262144 bytes
^C28 packets captured
28 packets received by filter
0 packets dropped by kernel
```

### `ping`

```sh
$ ping -c 1 informatica.usal.es
PING roble.fis.usal.es (212.128.144.90) 56(84) bytes of data.
64 bytes from roble.fis.usal.es (212.128.144.90): icmp_seq=1 ttl=63 time=2.32 ms

--- roble.fis.usal.es ping statistics ---
1 packets transmitted, 1 received, 0% packet loss, time 0ms
rtt min/avg/max/mdev = 2.322/2.322/2.322/0.000 ms
```

## Salida obtenida

```
19:07:22.358167 IP 10.200.24.43 > all-systems.mcast.net: igmp query v2
19:07:26.999643 IP 172.20.50.233 > 224.0.0.251: igmp v2 report 224.0.0.251
19:07:39.458941 IP 172.20.50.233.10384 > zafiro.usal.es.domain: 62115+ A? informatica.usal.es. (37)
19:07:39.458952 IP 172.20.50.233.10384 > agata.usal.es.domain: 62115+ A? informatica.usal.es. (37)
19:07:39.461944 IP zafiro.usal.es.domain > 172.20.50.233.10384: 62115* 2/3/3 CNAME roble.fis.usal.es., A 212.128.144.90 (188)
19:07:39.461961 IP agata.usal.es.domain > 172.20.50.233.10384: 62115* 2/3/3 CNAME roble.fis.usal.es., A 212.128.144.90 (188)
19:07:39.462159 IP 172.20.50.233 > roble.fis.usal.es: ICMP echo request, id 6832, seq 1, length 64
19:07:40.469770 IP 172.20.50.233 > roble.fis.usal.es: ICMP echo request, id 6832, seq 2, length 64
19:07:41.477819 IP 172.20.50.233 > roble.fis.usal.es: ICMP echo request, id 6832, seq 3, length 64
19:07:42.326294 IP 10.200.24.43 > all-systems.mcast.net: igmp query v2
19:07:42.485860 IP 172.20.50.233 > roble.fis.usal.es: ICMP echo request, id 6832, seq 4, length 64
19:07:43.831638 IP 172.20.50.233 > 224.0.0.251: igmp v2 report 224.0.0.251
19:07:44.471650 ARP, Request who-has 172.20.48.1 tell 172.20.50.233, length 28
19:07:44.475692 ARP, Reply 172.20.48.1 is-at 00:0a:42:38:ec:00 (oui Unknown), length 28
19:07:48.862206 IP 172.20.50.233.5805 > zafiro.usal.es.domain: 38398+ A? informatica.usal.es. (37)
19:07:48.864954 IP zafiro.usal.es.domain > 172.20.50.233.5805: 38398* 2/3/3 CNAME roble.fis.usal.es., A 212.128.144.90 (188)
19:08:02.294302 IP 10.200.24.43 > all-systems.mcast.net: igmp query v2
19:08:04.875771 IP 172.20.50.233.27284 > zafiro.usal.es.domain: 14661+ A? informatica.usal.es. (37)
19:08:04.875783 IP 172.20.50.233.27284 > agata.usal.es.domain: 14661+ A? informatica.usal.es. (37)
19:08:04.879873 IP zafiro.usal.es.domain > 172.20.50.233.27284: 14661* 2/3/3 CNAME roble.fis.usal.es., A 212.128.144.90 (188)
19:08:04.879889 IP agata.usal.es.domain > 172.20.50.233.27284: 14661* 2/3/3 CNAME roble.fis.usal.es., A 212.128.144.90 (188)
19:08:04.880028 IP 172.20.50.233 > roble.fis.usal.es: ICMP echo request, id 6856, seq 1, length 64
19:08:04.882345 IP roble.fis.usal.es > 172.20.50.233: ICMP echo reply, id 6856, seq 1, length 64
19:08:04.882575 IP 172.20.50.233.6487 > zafiro.usal.es.domain: 3340+ PTR? 90.144.128.212.in-addr.arpa. (45)
19:08:04.885414 IP zafiro.usal.es.domain > 172.20.50.233.6487: 3340* 1/5/6 PTR roble.fis.usal.es. (293)
19:08:11.367654 IP 172.20.50.233 > 224.0.0.251: igmp v2 report 224.0.0.251
19:08:22.364596 IP 10.200.24.43 > all-systems.mcast.net: igmp query v2
19:08:23.295652 IP 172.20.50.233 > 224.0.0.251: igmp v2 report 224.0.0.251
```

## Salida obtenida (relevante)

Estas son las únicas entradas relevantes referentes al ping.

```
IP 172.20.50.233.27284 > zafiro.usal.es.domain: 14661+ A? informatica.usal.es.
(37)
IP 172.20.50.233.27284 > agata.usal.es.domain: 14661+ A? informatica.usal.es.
(37)
IP zafiro.usal.es.domain > 172.20.50.233.27284: 14661* 2/3/3 CNAME
roble.fis.usal.es., A 212.128.144.90 (188)
IP agata.usal.es.domain > 172.20.50.233.27284: 14661* 2/3/3 CNAME
roble.fis.usal.es., A 212.128.144.90 (188)
IP 172.20.50.233 > roble.fis.usal.es: ICMP echo request, id 6856, seq 1, length
64
IP roble.fis.usal.es > 172.20.50.233: ICMP echo reply, id 6856, seq 1, length 64
```

Aquí se pueden ver:

 * Las solicitudes DNS tanto a `zafiro` como a `agata`, preguntando por
     `informatica.usal.es`[^dns].
 * Las respuestas de ambos, diciéndonos que `informatica.usal.es` es un alias de
     `roble.fis.usal.es`, y que su dirección IP es `212.128.144.90`.
 * El

[^dns]: Se cree que el motivo por el que se lanzan las dos solicitudes DNS en
vez de una al primario y (si fallara) otra al secundario porque en Ubuntu opera
`dnsmasq` por defecto.

## Otra salida comentada

Por curiosidad.

```
IP 172.20.50.233 > roble.fis.usal.es: ICMP echo request, id 6832, seq 1, length
64
IP 172.20.50.233 > roble.fis.usal.es: ICMP echo request, id 6832, seq 2, length
64
IP 172.20.50.233 > roble.fis.usal.es: ICMP echo request, id 6832, seq 3, length
64
```

Tres echo request (ping) a `roble.fis.usal.es` (probablemente de otra solicitud
diferente, que se nos ha colado).

```
IP 10.200.24.43 > all-systems.mcast.net: igmp query v2
IP 172.20.50.233 > 224.0.0.251: igmp v2 report 224.0.0.251
```

Una query del router y su correspondiente respuesta que se ha colado.

```
ARP, Request who-has 172.20.48.1 tell 172.20.50.233, length 28
ARP, Reply 172.20.48.1 is-at 00:0a:42:38:ec:00 (oui Unknown), length 28
```

Unas ARP request reply nuestra preguntando qué dirección física tiene
`172.20.48.1` (desconozco el motivo).

# ARP

## Órdenes ejecutadas

### `tcpdump`

```
$ sudo tcpdump arp -w arp.cap
tcpdump: listening on wlan0, link-type EN10MB (Ethernet), capture size 262144
bytes
^C2 packets captured
2 packets received by filter
0 packets dropped by kernel
```

### `arp`

```
$ sudo arp --delete 172.20.48.1
$ arp 172.20.48.1
Address                  HWtype  HWaddress           Flags Mask            Iface
172.20.48.1                      (incomplete)                              wlan0
$ sudo arp 172.20.48.1
Address                  HWtype  HWaddress           Flags Mask            Iface
172.20.48.1              ether   00:0a:42:38:ec:00   C                     wlan0
```

La primera orden elimina la entrada de la caché ARP correspondiente a la puerta
de acceso.

La segunda busca en la tabla arp la misma dirección, pero no lanza la solicitud
por falta de privilegios (por lo que sale `(incomplete)`.

La tercera orden lanza la solicitud y obtiene la dirección usando un arp request
y un reply.

## Salida obtenida

```
ARP, Request who-has 172.20.48.1 tell 172.20.50.233, length 28
ARP, Reply 172.20.48.1 is-at 00:0a:42:38:ec:00 (oui Unknown), length 28
```

Bastante directa, un request y un reply.

# DNS

## Órdenes ejecutadas

### `tcpdump`

```
$ sudo tcpdump udp -vv -w dns.cap
tcpdump: listening on wlan0, link-type EN10MB (Ethernet), capture size 262144
bytes
^C4 packets captured
4 packets received by filter
0 packets dropped by kernel
```

### `nslookup`

```
$ nslookup emiliocobos.me
Server:   127.0.1.1
Address:  127.0.1.1#53

Non-authoritative answer:
Name: emiliocobos.me
Address: 192.30.252.153
Name: emiliocobos.me
Address: 192.30.252.154
```

## Salida obtenida

```
IP 172.20.50.233.32506 > zafiro.usal.es.domain: 38204+ A? emiliocobos.me. (32)
IP 172.20.50.233.32506 > agata.usal.es.domain: 38204+ A? emiliocobos.me. (32)
IP zafiro.usal.es.domain > 172.20.50.233.32506: 38204 2/5/5 A 192.30.252.153,
A 192.30.252.154 (260)
IP agata.usal.es.domain > 172.20.50.233.32506: 38204 2/5/5 A 192.30.252.153,
A 192.30.252.154 (260)
```

También bastante directo, dos solicitudes y dos respuestas via udp a los
servidores DNS de la USAL.

# HTTP

Se ha usado `elinks` en vez de Firefox para evitar todas las solicitudes extra
que se realizan al abrir un navegador moderno, desde peticiones de archivos
extra a autocompletados, etc...

## Órdenes ejecutadas

### `tcpdump`

Se usa la opción `-s 0` para poder ver toda la solicitud y la respuesta.

```
$ sudo tcpdump -vvv -s 0 -w http.cap
tcpdump: listening on wlan0, link-type EN10MB (Ethernet), capture size 262144
bytes
^C22 packets captured
22 packets received by filter
0 packets dropped by kernel
```

### `elinks`

```
elinks http://informatica.usal.es
```

## Salida obtenida

No se pega y comenta aquí por la longitud de la salida, pero se ha hecho dump
con el flag `-A` para obtener el contenido en ASCII, y se pueden tanto ver las
solicitudes HTTP, como las respuestas sin ningún problema en el archivo
`http.cap.txt`.

