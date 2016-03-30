---
title: Instalación y configuración de Postfix
subtitle: Seminario - Administración de sistemas
lang: spanish
babel-lang: spanish
polyglossia-lang:
  options: []
  name: spanish
author:
  - Emilio Cobos Álvarez (70912324N)
---

# Entorno

Se ha usado un droplet Ubuntu de [DigitalOcean](https://digitalocean.com)
recién creado para la instalación.

Para hacer la demostración y enlazarlo a un DNS normal se ha usado el dominio
`emiliocobos.me`.

Se intentará configurar el correo `ec@emiliocobos.me` como primer paso, y se
tratará de crear cuentas virtuales posteriormente.

## Setup inicial

Para hacer el setup inicial iniciaremos una sesión ssh en el droplet.

```
$ ssh root@188.166.145.122
```

Una vez dentro, instalaremos tanto `postfix` como `mailutils`:

```
# apt-get install mailutils postfix
```

Saldrá una serie de diálogos para la configuración de `postfix`.

Estos pasos se pueden realizar posteriormente usando `dpkg-reconfigure`, pero
lo configuraremos como **Internet Site**, con el nombre de dominio apropiado, en
este caso `emiliocobos.me`.


## El MX record

Tenemos que asegurarnos que hay un MX record en la configuración de DNS del
dominio, apuntando a nuestro servidor.

El MX record sólo funciona con otros nombres de dominio, así que tenemos que
crear uno con un A record.

Se ha utilizado el subdominio `mail.emiliocobos.me` como servidor de correo,
apuntando a la dirección IP de nuestro droplet.

![Ejemplo de configuración en NameCheap](screenshots/3-dns-mx-config.png)

Podemos comprobar que funciona usando los comandos `dig` y `nslookup`:

```
$ dig mx emiliocobos.me
; <<>> DiG 9.9.5-3ubuntu0.7-Ubuntu <<>> mx emiliocobos.me
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 50018
;; flags: qr rd ra; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 1

;; OPT PSEUDOSECTION:
; EDNS: version: 0, flags:; udp: 512
;; QUESTION SECTION:
;emiliocobos.me.      IN  MX

;; ANSWER SECTION:
emiliocobos.me.   1798  IN  MX  10 mail.emiliocobos.me.

;; Query time: 30 msec
;; SERVER: 8.8.8.8#53(8.8.8.8)
;; WHEN: Thu Feb 18 06:21:40 EST 2016
;; MSG SIZE  rcvd: 64
```

```
$ nslookup mail.emiliocobos.me
Server:   127.0.1.1
Address:  127.0.1.1#53

Non-authoritative answer:
Name: mail.emiliocobos.me
Address: 188.166.145.122 # <- Nuestra dirección IP
```

## Crear el usuario y asegurarse de que la configuración de postfix funciona

Desde el servidor tenemos que crear ahora el usuario `me`:

```
# useradd -m -s /bin/bash me
```

Comprobamos que `postfix` está ejecutándose:

```
# postfix status
postfix/postfix-script: the Postfix mail system is running: PID: 2504
```

Podemos enviarnos un correo conectándonos al puerto 25 localmente via `telnet`:

```
# telnet localhost 25
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
220 postfix-mail-server ESMTP Postfix (Ubuntu)
ehlo localhost
250-postfix-mail-server
250-PIPELINING
250-SIZE 10240000
250-VRFY
250-ETRN
250-STARTTLS
250-ENHANCEDSTATUSCODES
250-8BITMIME
250 DSN
mail from:root@localhost
250 2.1.0 Ok
rcpt to:me@localhost
250 2.1.5 Ok
data
354 End data with <CR><LF>.<CR><LF>
Subject: Hola desde postfix!

Hey, esto es sólo una prueba.
.
250 2.0.0 Ok: queued as EAAE8120337
quit
221 2.0.0 Bye
```

Para comprobar que está, podemos entrar como `me`, y comprobar que hay correo en
`/var/mail/me`.

Usando `cat`:

```
# su -l me

me:~$ cat /var/mail/me
From root@localhost  Thu Feb 18 06:34:16 2016
Return-Path: <root@localhost>
X-Original-To: me@localhost
Delivered-To: me@localhost
Received: from localhost (localhost [127.0.0.1])
by postfix-mail-server (Postfix) with ESMTP id EAAE8120337
for <me@localhost>; Thu, 18 Feb 2016 06:31:41 -0500 (EST)
Subject: Hola desde postfix!
Message-Id: <20160218113332.EAAE8120337@postfix-mail-server>
Date: Thu, 18 Feb 2016 06:31:41 -0500 (EST)
From: root@localhost

Hey, esto es sólo una prueba.
```

Usando `mail`:

```
# su -l me
me:~$ mail
"/var/mail/me": 1 message 1 new
>N   1 root@localhost     Thu Feb 18 06:34  12/440   Hola desde postfix!
?
Return-Path: <root@localhost>
X-Original-To: me@localhost
Delivered-To: me@localhost
Received: from localhost (localhost [127.0.0.1])
by postfix-mail-server (Postfix) with ESMTP id EAAE8120337
for <me@localhost>; Thu, 18 Feb 2016 06:31:41 -0500 (EST)
Subject: Hola desde postfix!
Message-Id: <20160218113332.EAAE8120337@postfix-mail-server>
Date: Thu, 18 Feb 2016 06:31:41 -0500 (EST)
From: root@localhost

Hey, esto es sólo una prueba.
?
Saved 1 message in /home/me/mbox
Held 0 messages in /var/mail/me
```

# Instalar el servidor imap y pop

Usaremos courier como servidor IMAP y POP.

Courier usa el estilo de inbox con `Maildir`s en vez de `/var/mail`
[^maildir-note], así que es necesario configurar postfix para que funcione así:

[^maildir-note]: La ventaja de esto es que usando `Maildir` cada correo se
guarda en un archivo diferente, así que no es necesario bloquear el fichero.

```
# postconf -e "home_mailbox = Maildir/"
# postconf -e "mailbox_command = "
```

Y posteriormente instalar courier.

```
# apt-get install courier-imap courier-pop
```

# Dejar a `postfix` recibir mail desde internet

Para ello tenemos que decirle que escuche a todas las interfaces:

```
# postconf -e "inet_interfaces = all"
# postconf -e "inet_protocols = all"
```

```
# postconf -e myhostname=mail.emiliocobos.me
```

Y reiniciar `postfix`:

```
# service postfix restart
 * Stopping Postfix Mail Transport Agent postfix  [ OK ]
 * Starting Postfix Mail Transport Agent postfix  [ OK ]
```

# Mandar nuestro primer correo desde nuestro ordenador personal

Podríamos usar `telnet`, pero está bloqueado por eduroam.

Mandando un correo de ejemplo a `me@emiliocobos.me`, nos llega correctamente:

```
:~$ cat Maildir/new/1455799542.Vfd01I120a80M324168.mail.emiliocobos.me
Return-Path: <ecoal95@gmail.com>
X-Original-To: me@emiliocobos.me
Delivered-To: me@emiliocobos.me
...
```

# Configurando imap con mutt y msmtp

Asignaremos una contraseña a nuestro usuario `me`:

```
# passwd me
```

Y configuraremos `mutt` tal que así:

```
set from = "me@emiliocobos.me"
set sendmail="/usr/bin/msmtp -v -a personal"

set folder      = imap://mail.emiliocobos.me/
set imap_user   = me
set imap_pass   = xxx
set spoolfile   = +INBOX

mailboxes       = +INBOX

set mbox = +INBOX.All
set postponed = +INBOX.Drafts
set record = +INBOX.Sent
```

con una cuenta en `.msmtprc` definida así:

```
account personal
host mail.emiliocobos.me
from "me@emiliocobos.me"
port 25
auth on
user "me"
password "xxx"
```

**Ahora podemos enviar y recibir mails desde nuestro propio correo!**

# Forzando TLS con SASL

Uno de los problemas que tenemos actualmente es que puede que nuestros datos
vayan **en texto plano** directamente al servidor.

Esto es un problema muy serio de seguridad, así que vamos a forzar el uso de SSL
o TLS para conectarnos a nuestro servidor.

Para ello deberíamos usar un certificado creado por nosotros mismos.

```
# openssl req -sha256 -x509 -newkey rsa:2048 \
  -keyout key.pem -out cert.pem -days 365 -nodes
```

Este comando genera un certificado y una clave válidos para 365 días.

Pero... por suerte **en los SO basados en Debian ya existe un certificado y una
clave a tal efecto**: `ssl-cert-snakeoil.pem`.

Estas opciones deberían estar ya configuradas en postfix, pero si no lo
estuvieran, o quisieras cambiar el certificado, podrías poner las opciones:

```
# postconf -e "smtpd_tls_cert_file=/etc/ssl/certs/ssl-cert-snakeoil.pem"
# postconf -e "smtpd_tls_key_file=/etc/ssl/private/ssl-cert-snakeoil.key"
```

## Forzando TLS con SASL en postfix

Para permitir mandar correo via TLS usaremos:

```
# postconf -e "smtpd_tls_auth_only=yes"
# postconf -e "smtpd_tls_security_level=encrypt"
# postconf -e "smtp_tls_security_level=may"
```

Pero ahora nuestro servidor no soporta ningún modo de autenticación, así
que tenemos que activar **SASL**:

```
# postconf -e "smtpd_sasl_auth_enable=yes"
# postconf -e "smtpd_sasl_tls_security_options=noanonymous"
```

Si sois curiosos y queréis saber lo que es SASL, significa *Simple
Authentication Security Layer*. Mi conocimiento no pasa de ahí, está explicado
en el [RFC 2222](http://ftp.ietf.org/rfc/rfc2222.txt), que no me he leído.

No obstante, citando a [la documentación de
`cyrus`](http://www.sendmail.org/~ca/email/cyrus2/programming.html#what_is_sasl):

> That document is very difficult to understand however and it should be
unnecessary to consult it.

El caso es que para soportar SASL, hay que usar una de las librerías que proveen
este sistema (`cyrus` o `dovecot`).

Usaremos `cyrus`, para lo que seguiremos [esta guía de
Debian](https://wiki.debian.org/PostfixAndSASL#SASL_authentication_in_the_Postfix_SMTP_server).

Instalaremos `libsasl2`:

```
# apt-get install libsasl2-modules sasl2-bin
```

Crearemos `/etc/postfix/sasl/smtpd.conf`, con los contenidos:

```
pwcheck_method: saslauthd
mech_list: PLAIN LOGIN
```

Y seguimos con la guía. En resumen, tenemos que crear un daemon nuevo para
postfix ya que se ejecuta con `chroot` en Debian.

```
# cp /etc/default/saslauthd /etc/default/saslauthd-postfix
# # Editarlo (ver la guía)
# dpkg-statoverride --add root sasl 710 /var/spool/postfix/var/run/saslauthd
# adduser postfix sasl
# postconf -e 'smtpd_sasl_local_domain = $myhostname'
# postconf -e 'smtpd_sasl_auth_enable = yes'
# postconf -e 'broken_sasl_auth_clients = yes'
# postconf -e 'smtpd_sasl_security_options = noanonymous'
# postconf -e 'smtpd_recipient_restrictions = permit_sasl_authenticated, \
permit_mynetworks, reject_unauth_destination'
# saslfinger -s
# service postfix restart
```

Y listo! **Ya podemos loguearnos correctamente otra vez!**

## Aceptar el certificado

Ahora mismo, al intentar mandar un e-mail, el cliente de correo no lo envía
porque no confía en el certificado mandado por el servidor (ya que no está
firmado por ninguna autoridad).

Para solucionar esto, tenemos que decirle a nuestro cliente que "confíe" en ese
certificado.

Para hacerlo con `msmtp`, podremos hacer:

```
# openssl x509 -fingerprint -in /etc/ssl/certs/ssl-cert-snakeoil.pem
SHA1 Fingerprint=xxxxx
```

Copiando ese fingerprint en `.msmtprc`, con la línea:

```
tls_fingerprint "xxxx"
```

Comprobará el fingerprint del certificado cuando se conecte.

# Usar SSL en el servidor IMAP

Para usar SSL en el servidor imap, deberemos instalar el paquete
`courier-imap-ssl`.

```
# apt-get install courier-imap-ssl
```

Esto generará un certificado en `/usr/lib/courier/imapd.pem`, pero nosotros, por
comodidad, usaremos el mismo que para `smtpd`, es decir: `ssl-cert-snakeoil`.

Courier imap usa un formato un poco diferente, así que tenemos que concatenar el
certificado y la clave.

```
# cat /etc/ssl/certs/ssl-cert-snakeoil.pem \
      /etc/ssl/private/ssl-cert-snakeoil.key \
      > /etc/courier/ssl-cert-snakeoil.pem
```

Hay que editar la variable `TLS_CERTFILE` del archivo `/etc/courier/imapd-ssl`
para apuntar a este nuevo archivo.

Tras eso reiniciaremos `courier-imap`:

```
# service courier-imap-ssl restart
```

# Extras

## Aliases

Especificar aliases es extremadamente simple, sólo hay que alterar el archivo:
`/etc/aliases`.

Puedes camiarlo a uno específico de postfix de todas formas con las claves de
configuración `alias_maps` y `alias_database`.

Por ejemplo, supongamos que queremos que `admin@emiliocobos.me`
vaya a `me@emiliocobos.me`:

```
# echo "admin: me" >> /etc/aliases
# newaliases
```

## Cliente web

Se puede acceder al correo con cualquier cliente, llámese `mutt`, `thunderbird`,
Outlook, etc...

Pero vivimos en un mundo en el que si no puedes tener acceso siempre al correo
no eres nadie, así que vamos a añadir rápidamente `roundcube` como ejemplo.

Para eso necesitaremos un servidor web (`apache2` funciona bien, y se configura
automáticamente), y `mysql`.

```
# apt-get install apache2 mysql-client-5.6 mysql-server-5.6
# mysql_secure_installation
# apt-get install roundcube roundcube-mysql aspell-es
```

Resulta que la instalación de mysql falla porque nuestro servidor es demasiado
pequeño, así que usaremos `postgresql`, que esperemos que se porte mejor con
nuestra memoria.

```
# apt-get install apache2 postgresql
# pg_createcluster 9.3 main --start
# apt-get install roundcube roundcube-pgsql aspell-es
```

Como no tenemos hosts virtuales, activaremos roundcube en el directorio
principal, descomentando las siguientes líneas del archivo
`/etc/apache2/conf-enabled/roundcube.conf`:

```
Alias /roundcube/program/js/tiny_mce/ /usr/share/tinymce/www/
Alias /roundcube /var/lib/roundcube
```

Posteriormente:

```
# service apache2 restart
```

Deberíamos de ser capaces de ver roundcube en el directorio `/roundcube` de
nuestro servidor:

![roundcube](screenshots/4-roundcube.png)

Ahora sólo tenemos que instalar la base de datos, para eso usaremos el comando
`createuser` de postgresql, que con la opción `-d` también crea una BBDD con los
permisos adecuados.

```
# su -l postgres
postgres:~$ createuser -d -W roundcube
Password:
```

Finalmente usamos:

```
# dpkg-reconfigure roundcube-core
```

Que nos guiará a lo largo del resto de la instalación.

Si aún así tenéis problemas al entrar, mirad `/var/lib/roundcube/logs/errors`,
comprobad que la configuración de la base de datos es correcta, y que el módulo
`mcrypt` de PHP está instalado.

Si no lo estuviera, podríais hacer:

```
# php5enmod mcrypt
# service apache2 restart
```

## El SPF record

El record SPF (*Sender Policy Framework*) es un DNS record del tipo TXT que
indica o puede indicar, entre otras cosas, la dirección IP de tu servidor de
correo.

Usarlo ayuda, por ejemplo, a que clientes de correo muy utilizados no marquen tu
correo como spam, y a que marquen como spam el email de gente que te está
tratando de impersonar.

La [sintaxis detallada](http://www.openspf.org/SPF_Record_Syntax) es
relativamente compleja, pero un buen intento puede ser el siguiente:

```
v=spf1 a mx -all
```

o (más estricto):

```
v=spf1 a mx ip4:188.166.145.122/32 -all
```

# Cambiando el puerto por defecto de SMTP

La red eduroam bloquea el puerto 25 para evitar envío de SPAM.

Este es el puerto por defecto del daemon SMTP, lo que implica que desde la usal
no podemos enviar correo.

Para cambiar esta configuración, hay que descomentar la configuración del
proceso `master` de postfix (`/etc/postfix/master.cf`) referente a `submission`.

```
submission inet  n       -       -       -       -       smtpd
```

`submission` es equivalente al puerto 587, que es estándar, y al ser también el
puerto que utiliza GMail es probable que nunca esté bloqueado.

Podríamos descomentar la línea `smtps` en vez de `submission`. `submission`
garantiza que el mail esté encriptado, pero nuestra configuración de `smtp(s)`
también, así que no creo que haya una diferencia práctica.

Como siempre que se cambia la configuración, hay que reiniciar postfix:

```
# service postfix restart
```

# Instalando spamassassin

Spamassassin es un servicio creado por Apache para el filtrado de correos. Es el
programa anti-spam más utilizado con diferencia.

Para ello instalaremos los siguientes paquetes:

```
# apt-get install spamassassin spamc
```

Posteriormente editaremos la configuración en `/etc/postfix/master.cf`.

Lo primero es añadir el servicio `spamassassin` al final del fichero:

```
spamassassin unix -     n       n       -       -       pipe
  user=debian-spamd argv=/usr/bin/spamc -f -e /usr/sbin/sendmail -oi -f ${sender} ${recipient}
```

Lo siguiente es poner el filtro en todas las líneas que reciban correos (todas
las líneas que acaben en `smtpd`):

```
submission inet n       -       -       -       -       smtpd
   -o content_filter=spamassassin
```

Y posteriormente reiniciar postfix:

```
# service postfix restart
```

## Configuración de spamassassin

Spamassassin tiene una gran cantidad de configuración, que se puede editar en
`/etc/spamassassin/local.cf`.

Yo sólo he descomentado la línea correspondiente a `rewrite_header Subject` y la
he modificado para que sea:

```
rewrite_header Subject [SPAM]
```

El resto de opciones están documentadas en la [documentación
oficial](https://spamassassin.apache.org/full/3.1.x/doc/Mail_SpamAssassin_Conf.html).

## Comprobando que funciona

Para comprobar que spamassassin está correctamente instalado puedes usar
[GTUBE](http://spamassassin.apache.org/gtube/). En la URL anterior hay un
mensaje de ejemplo que puedes enviar desde otra cuenta.

# Direcciones virtuales con postgresql

Postfix permite usar fácilmente direcciones virtuales (que son diferentes a los
alias).

Lo primero es tener el paquete correspondiente instalado:

```
# apt-get install postfix-pgsql courier-authlib-postgresql libsasl2-modules-sql
```

Con cuentas virtuales un sólo servidor de correo puede hostear cuentas para
múltiples dominios diferentes.

Para almacenar los usuarios usaremos postgresql, aunque se puede usar cualquier
otra configuración, incluso archivos de texto.

Habrá que tener un usuario que sea el propietario de todos los mailboxes
virtuales. Puede ser cualquiera, incluído uno ya existente, pero nosotros
crearemos uno llamado `virtual`. Le daremos un UID específico para la
configuración de postfix:

```
# useradd -u 5000 -m virtual
# su -l postgres
$ createuser -W virtual
$ psql
postgres=# CREATE DATABASE virtual WITH OWNER virtual;
CREATE DATABASE
postgres=# \c virtual
CREATE TABLE mailbox WITH OWNER virtual (
  id serial NOT NULL,
  username character varying(64) NOT NULL,
  "domain" character varying(64) NOT NULL,
  "password" character varying(64),
  maildir character varying(256),
  alias character varying(32)
);
ALTER TABLE mailbox OWNER TO virtual;
ALTER ROLE virtual WITH PASSWORD 'virtual';
```

La configuración de postfix necesaria es la siguiente:

```
# postconf -e "virtual_mailbox_domains = pgsql:/etc/postfix/virtual-domains.cf"
# postconf -e "virtual_mailbox_maps = pgsql:/etc/postfix/virtual-mailbox-maps.cf"
# postconf -e "virtual_alias_maps = pgsql:/etc/postfix/virtual-alias-maps.cf"
# postconf -e "virtual_mailbox_base = /var/spool/mail"
# postconf -e "virtual_minimum_uid = 100"
# postconf -e "virtual_uid_maps = static:5000"
# postconf -e "virtual_gid_maps = static:5000"
# cat /etc/postfix/virtual-domains.cf
user = virtual
password = virtual
dbname = virtual
table = mailbox
select_field = domain
where_field = domain
# cat /etc/postfix/virtual-mailbox-maps.cf
user = virtual
password = virtual
dbname = virtual
table = mailbox
select_field = maildir
where_field = username
# cat /etc/postfix/virtual-alias-maps.cf
user = virtual
password = virtual
dbname = virtual
table = mailbox
select_field = alias
where_field = username
# service postfix restart
```

También habrá que asegurarse de que ninguno de los dominios virtuales esté
también en `mydestination`.

Tenemos que crear los directorios correspondientes y asociarlos al usuario
`virtual`:

```
# mkdir /var/spool/mail/emiliocobos.me
# chown virtual:virtual /var/spool/mail/emiliocobos.me
```

## Configuración de SASL

Tenemos que decirle a SASL cómo obtener la información de login a partir de
ahora:

```
# cat /etc/postfix/sasl/smtpd.conf
mech_list: PLAIN
allowanonymouslogin: no
allowplaintext: no

pwcheck_method: auxprop
auxprop_plugin: sql
sql_hostnames: localhost
sql_engine: pgsql
sql_database: virtual
sql_passwd: virtual
sql_user: virtual
sql_select: SELECT password FROM mailbox WHERE username = '%u' AND domain = '%r'
```

## Activando cuentas virtuales en courier-imap

Para ello tenemos que añadir `authpgsql` a la lista de módulos de autenticación.
Esto permitirá a `courier-imap` tener una lista de usuarios aparte de la del
sistema. Tendremos que editar el archivo `/etc/courier/authdaemonrc`, y editar
la variable `authmodulelist`:

```
authmodulelist="authpgsql"
```

Tendremos que crear el archivo correspondiente a `authpgsqlrc`,
`/etc/courier/authpgsqlrc`:

```
# cat /etc/courier/authpgsqlrc
PGSQL_HOST           localhost
PGSQL_PORT           5432
PGSQL_USERNAME       virtual
PGSQL_PASSWORD       virtual
PGSQL_DATABASE       virtual
# Fields
PGSQL_USER_TABLE     mailbox
PGSQL_CLEAR_PWFIELD  password
PGSQL_LOGIN_FIELD    username || '@' || domain
PGSQL_MAILDIR_FIELD  maildir
# Makes postgres select a constant
PGSQL_UID_FIELD      5000
PGSQL_GID_FIELD      5000
PGSQL_HOME_FIELD     '/var/spool/mail/'
```

Posteriormente reiniciaremos `courier-authdaemon`:

```
# service courier-authdaemon restart
```


## Crear un usuario virtual

Para crear un usuario virtual (vamos a usar como ejemplo `pepe`) sólo tendríamos
que añadirlo a la BBDD, para lo que haremos:

```
# su - l virtual
$ psql
INSERT INTO mailbox (username, "domain", "password", maildir, alias) VALUES
('pepe', 'emiliocobos.me', 'pass', 'emiliocobos.me/pepe/', NULL);
```

**Nota**: Postfix sólo usa el estilo `maildir` si el path acaba en
`/` (es decir, `emiliocobos/pepe/` en vez de `emiliocobos/pepe`).

Esto es importante porque imap sólo funcionará con el estilo `maildir`.

## Cambio de configuración del cliente

Nótese que antes, en nuestro cliente de correo, nos autenticábamos con el
usuario `me` directamente.

Tendremos que cambiarlo a `me@emiliocobos.me`, ya que ahora podemos
potencialmente hostear correo para más dominios.

Lo mismo pasará con `msmpt`.

## Prevenir acceso ssh al usuario `virtual`

Para evitar sustos, sobre todo si has escogido una contraseña fácil como la mía
(`virtual`), puedes cambiar `/etc/ssh/sshd_config` y añadir:

```
Match User virtual
  PasswordAuthentication no
```

Asegurándose de usar después:

```
# service ssh restart
```

# DKIM

Comentábamos antes, en la sección del SPF, que otras opciones existían, como
`DKIM`.

Configuraremos `DKIM` en el dominio para evitar que nuestros correos vayan
a bandejas de spam de algunos servicios como GMail.

```
# apt-get install opendkim opendkim-tools
# opendkim-genkey -s mail -d mail.emiliocobos.me
# postconf -e "milter_protocol = 2"
# postconf -e "milter_default_action = accept"
# postconf -e "smtpd_milters = inet:localhost:12301"
# postconf -e "non_smtpd_milters = inet:localhost:12301"
# mkdir /etc/opendkim
# cat /etc/opendkim.conf
AutoRestart             Yes
AutoRestartRate         10/1h
UMask                   002
Syslog                  yes

OversignHeaders         From

InternalHosts           refile:/etc/opendkim/TrustedHosts
ExternalIgnoreList      refile:/etc/opendkim/TrustedHosts
KeyTable                refile:/etc/opendkim/KeyTable
SigningTable            refile:/etc/opendkim/SigningTable

Mode                    sv
PidFile                 /var/run/opendkim/opendkim.pid

Socket                  inet:12301@localhost
```

Cada dominio que use DKIM necesitará una clave y un *selector*. El selector se
puede escoger, así que nosotros usaremos `mail` (por lo que nuestro record
acabará siendo `mail._domainkey.emiliocobos.me`).

Como queremos permitir dominios virtuales, en vez de las opciones `Domain`,
`KeyFile` y `Selector`, puedes ver que hemos utilizado `TrustedHosts`,
`KeyTable` y `SigningTable`, que tienen una línea por dominio.

El puerto en el que opendkim está funcionando es el `12301`, pero podría ser
cualquier otro, siempre que adecuemos la configuración de postfix.

### TrustedHosts (`/etc/opendkim/TrustedHosts`)

Nuestro archivo `TrustedHosts` contendrá la lista de hostnames que de la que nos
fiamos, y cuyos emails firmamos. Serán los host locales y `emiliocobos.me`, en
este caso:

```
127.0.0.1
localhost
192.168.0.1/24

emiliocobos.me
```

### KeyTable (`/etc/opendkim/KeyTable`)

La KeyTable nos dirá con qué clave tendrá que firmar para según qué dominio
y que selector.

En este caso sólo tendrá la línea correspondiente a `emiliocobos.me` con el
selector `mail` (nótese que no hemos creado aún esa clave):

```
mail._domainkey.emiliocobos.me emiliocobos.me:mail:/etc/opendkim/keys/emiliocobos.me/mail.private
```

### SigningTable (`/etc/opendkim/SigningTable`)

En este archivo especificaremos con qué selector se firmará para qué dirección.
Como vamos a firmar todas con el mismo selector, podemos usar un wildcard (`*`):

```
*@emiliocobos.me mail._domainkey.emiliocobos.me
```

## Creando una key para un dominio y un selector

Hemos visto antes que apuntábamos en el `KeyTable` al archivo
`/etc/opendkim/keys/emiliocobos.me/mail.private`. Veremos cómo generar ese
archivo ahora mismo.

Para generar una clave DKIM usaremos la utilidad `opendkim-genkey`.

```
# mkdir -p /etc/opendkim/keys/emiliocobos.me
# cd $_
# opendkim-genkey -r -s mail -d emiliocobos.me
```

La opción `-r` restringe el uso al firmado de correo, `mail` es el selector,
y `emiliocobos.me` el dominio.

Esto habrá creado en el directorio **dos archivos**: `mail.private`
y `mail.txt`. El primero es la clave privada referenciada en el `SigningKey`, el
segundo es la clave pública que tendremos que poner en nuestro registro TXT.

### El registro TXT

Con nuestro archivo `mail.txt`, tendremos que ir al manejo de nuestras DNS
y crear el registro `TXT` correspondiente (es copiar y pegar). En este caso
sería al subdominio `mail._domainkey`.

### Finalizando la configuración

Para finalizar la configuración tendremos que dar los permisos adecuados
a `/etc/opendkim`, y reiniciar postfix y opendkim:

```
# chown opendkim:mail -R /etc/opendkim
# service opendkim restart
# service postfix restart
```

# DMARC

Lo primero es instalar OpenDMARC:

```
# apt-get install opendmarc
# cat /etc/opendmarc.conf
AuthservID mail.emiliocobos.me
PidFile /var/run/opendmarc.pid #Debian default
RejectFailures false
Syslog true
TrustedAuthservIDs mail.emiliocobos.me
UMask 0002
UserID opendmarc:opendmarc
IgnoreHosts /etc/opendmarc/ignore.hosts
HistoryFile /var/run/opendmarc/opendmarc.dat
Socket inet:54321@localhost
# cp /etc/opendkim/TrustedHosts /etc/opendmarc/ignore.hosts
# chown opendmarc:mail -R /etc/opendmarc
```

Ahora habrá que añadir los nuevos filtros a postfix, nótese que se lo añadimos
al anterior (opendkim).

```
# postconf -e "smtpd_milters = inet:localhost:12301,inet:localhost:54321"
# postconf -e "non_smtpd_milters = inet:localhost:12301,inet:localhost:54321"
# service opendmark restart
# service postfix restart
```

# Eliminar datos sensibles de las cabeceras de correo

Las cabeceras de correo por defecto contienen determinados datos que podrían ser
sensibles, como el hostname y la IP del ordenador que lo envía.

Postfix permite filtrar cabeceras de correo para evitar que se envíen esas
cabeceras `Received` y `X-Origin-IP`:

```
# cat /etc/postfix/header_checks
/^Received:.*with ESMTPSA/              IGNORE
/^X-Originating-IP:/    IGNORE
# postconf -e "header_checks=regexp:/etc/postfix/header_checks"
# postmap /etc/postfix/header_checks
# service postfix restart
```

# Testeando la configuración de correo

Para comprobar que todo está correcto existen una gran variedad de recursos.
Personalmente he utilizado:

 * http://www.allaboutspam.com/email-server-test/
 * https://mxtoolbox.com
 * https://www.skelleton.net/2015/03/21/how-to-eliminate-spam-and-protect-your-name-with-dmarc/
