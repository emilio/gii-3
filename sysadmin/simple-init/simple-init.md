---
title: Adecuación de un nivel de ejecución a nuestras necesidades (B2)
subtitle: Administración de sistemas
lang: spanish
babel-lang: spanish
polyglossia-lang:
  options: []
  name: spanish
author:
  - Emilio Cobos Álvarez (70912324N)
---

# Enunciado

Se pide ejecutar un comando (en este caso será un simple `echo`), cuando se
entre en un nivel de ejecución o *runlevel* particular (el 3), y otro que se
ejecute cuando se sale de ese nivel a cualquier otro.

# Solución

Usaremos la capacidad de los init scripts de LSB, en este caso en un Debian.

Para ello necesitaremos un script que cuando se inicialize (`start`) ejecute
nuestro comando, y cuando se pare (`stop`) ejecute el otro.

Aparte de ello, se tiene que inicializar cuando se entra en el nivel 3, y parar
cuando se sale. Esto es algo que por suerte tiene una solución directa usando
los init scripts.

Lo primero que haremos será copiar la plantilla de init script por defecto
al nuevo script, en este caso:

```
# cp /etc/init.d/skeleton /etc/init.d/hello-runlevel-3
# chmod +x $_
```

El script final queda de la siguiente manera[^note-approximated]:

```bash
#!/bin/sh
### BEGIN INIT INFO
# Provides:          skeleton
# Required-Start:    $all
# Required-Stop:
# Default-Start:     3
# Default-Stop:      0 1 2 4 5 6
# Short-Description: Hello runlevel 3!
# Description:       This script starts when the runlevel 3 is chosen, and
#                    stops when you're no longer there.
### END INIT INFO

# Author: Emilio Cobos Álvarez <emiliocobos@usal.es>

DESC="Hello runlevel 3"

case $1 in
  start)
    echo "HE ENTRADO PERFECTAMENTE EN EL NIVEL 3" ;;
  stop)
    echo "ESTOY SALIENDO DEL NIVEL 3: ADIOS" ;;
esac
```

Para instalarlo, habría que ejecutar:

```
# insserv hello-runlevel-3
```

Si se quisiera deshabilitar el mensaje temporalmente (segunda parte del
ejercicio), se podría eliminar de todos los runlevels usando:

```
# insserv -r hello-runlevel-3
```

Esto permitiría volver a activarlo con `insserv` otra vez casi instantáneamente.


[^note-approximated]: Nótese que es posible que tenga errores, ya que no lo
guardé en clase, y sólo tenía un sistema Arch Linux con systemd (que tiene una
arquitectura completamente diferente y no permite hacer "stop" del servicio
cuando se cambia de "runlevel"). Aparte, el output de un comando de systemd no
va directo a la consola, sino a syslog.

