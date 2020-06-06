# Proyecto Final Sistemas Operativos 2019-2

### Integrantes
* David Santiago Cortés - 1745133

### Objetivo
A partir del sistema operativo [xv6](https://github.com/mit-pdos/xv6-public) agregar/implementar tres comandos nuevos: 
* `date`: que imprime la fecha actual del sistema en el uso horario UTC-5.
* `uptime`: imprime el tiempo que lleva el sistema en ejecución.
* `count`: imprime cuantas veces se ha utilizado una determinada llamada al sistema, ó cuantas veces se han utilizado todas las llamadas al sistema.

### Implementando date
Para escribir el comando date, que en linux tiene una salida como `vie 05 jun 2020 22:51:18 -05` se tuvieron en cuenta el archivo de cabecera date.h
el cual define una estructura para la fecha (segundo, minuto,..., año) y el esqueleto del comando provisto por el profesor, en el cual
se infiere que antes de crear el comando debe crearse una llamada al sistema que recibe como argumento un puntero a la estrucura definida en date.h.

La llamada al sistema en cuestión es:
```c
int
sys_date(void)
{
	struct rtcdate *r;
	if(argptr(0, (void*)&r, sizeof(r)) < 0) {
		return -1;
	}

	cmostime(r);
	return 0;
}
```

`argptr` se usa para "sanear" el contenido de la estructura rtcdate, si todo va bien se llama a cmostime con el puntero a rtcdate como
parametro para "traducir" el contenido de rtcdate a una forma más "amigable". 

Para agregar la llamada al sistema fue necesario añadirla en user.h, syscall.h, usys.S y syscall.c 
Una vez hecho el llamado a `date` dentro de date.c para acceder al día, mes o año tan sólo es necesario acceder a los elementos de la estructura
rtcdate (`r.year`, `r.month`, `r.day`, etc). La hora proporcionada hasta este momento está en el uso horario UTC, por tanto se hizo
una función auxiliar `uint fhour(uint hour)` que hace la conversión a UTC-5.

Para hacer el output un poco más similar al de los sistemas linux se utilizó otra función `char* month(uint i)` la cual simplemente devuelve
el nombre del mes correspondiente al número i.

Por último se imprime la fecha actual en el formato dd-MM-YY HH:mm.

### Implementando uptime
Se utilizó la llamada al sistema `uptime` que devuelve el numero de ticks del reloj que han sucedido, en el sistema xv6 se hacen 100 ticks por
cada segundo, por lo tanto en el método principal de uptime.c se hace un llamado a uptime y a otra función que formatee la salida de uptime:
```c
if(uptime() < 0) {
	printf(2, "uptime failed\n");
}

timef(uptime() / 100);
exit();
```
Se divide en 100 porque la médida de tiempo más pequeña a tomar en cuenta van a ser los segundos. Para saber cuantas horas han trancurrido
se hace la división por 3600, para calcular los minutos se divide entre 60 el resto de la división anterior, y en el caso de los segundos
estos se obtienen del resto de la división anterior. Un dato interesante que sucedió es que a pesar de que los cálculos estén bien planteados
y funcionen teoricamente, durante la ejecución de `date` muchas veces se observó que se calcula entre 1 y 3 segundos de más, probablemente 
debido a errores de redondeo.


### Implementando count
