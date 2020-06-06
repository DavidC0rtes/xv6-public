# Proyecto Final Sistemas Operativos 2019-2

### Integrantes
* David Santiago Cortés - 1745133

### Objetivo
A partir del sistema operativo [xv6](https://github.com/mit-pdos/xv6-public) agregar/implementar tres comandos nuevos: 
* `date`: que imprime la fecha actual del sistema en el uso horario UTC-5.
* `uptime`: imprime el tiempo que lleva el sistema en ejecución.
* `count`: imprime cuantas veces se ha utilizado una determinada llamada al sistema, ó cuantas veces se han utilizado todas las llamadas al sistema.

### Contenido
1. [Implementando date](#implementando-date)
2. [Implementando uptime](#implementando-uptime)
3. [Implementando count](#implementando-count)
4. [Modo de uso](#uso)

### Implementando date
Para escribir el comando date, que en linux tiene una salida como `vie 05 jun 2020 22:51:18 -05` se tuvo en cuenta el archivo de cabecera date.h
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
```c
printf(1,"%d de %s del %d  %d:%d\n", r.day, month(r.month), r.year, fhour(r.hour), r.minute);
```

### Implementando uptime
Se utilizó la llamada al sistema `uptime` que devuelve el numero de ticks del reloj que han sucedido hasta el momento actual, en el sistema xv6 se hacen 100 ticks por
cada segundo, por lo tanto en el método principal de uptime.c se hace un llamado a uptime y a otra función encargada de formatear la salida de uptime:
```c
if(uptime() < 0) {
	printf(2, "uptime failed\n");
}

timef(uptime() / 100);
exit();
```
Se divide en 100 porque la médida de tiempo más pequeña a tomar en cuenta van a ser los segundos. Para saber cuantas horas han trancurrido
se hace la división por 3600, para calcular los minutos se divide entre 60 el resto de la división anterior, y en el caso de los segundos
estos se obtienen del resto de la división anterior.
```c
int hours = time/3600;
int mins = (time%3600) / 60;
int secs = (time%3600) % 60;
```
Un dato interesante que sucedió es que a pesar de que los cálculos estén bien planteados
y funcionen teoricamente, durante la ejecución de `date` muchas veces se observó que se calcula entre 1 y 3 segundos de más, probablemente 
debido a errores de redondeo.

### Implementando count
La función `syscall()` en syscall.c es el pilar fundamental en esta implementación, especialmente este fragmento:
```c
num = curproc->tf->eax;
  if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
    curproc->tf->eax = syscalls[num]();
...
```
De allí se infiere que la variable `num` contiene el número que identifica la llamada al sistema que se va a realizar. Lo siguiente consistió entonces 
en declarar un arreglo `calls[]` en el archivo de cabecera count.h, definirlo como variable global de 24 puestos en syscall.c y dentro de la función
antes mencionada, incrementar la posición correspondiente en 1: `calls[num] += 1;`

Posteriormente se creó una llamada al sistema `count`:
```c
int sys_count(void)
{
  int n;
  if(argint(0, &n) < 0) {
	  return -1;
  } else {
	  return calls[n];
  }
}
```
Con el objetivo de que acceda a las posiciones del arreglo `calls`, tal posición se le pasa como un puntero a `argint`, una función encargada
de captar los argumentos de las llamadas al sistema, de lo que se pudo observar, los argumentos de entrada de las llamadas al sistema
primero se pasan como punteros a funciones del tipo `argint` o `argptr` antes de ser usadas en otros cálculos o procedimientos.

Por último, en count.c se definió el arreglo `names` previamente declarado en count.h, allí se van a almacenar los nombres de las llamadas
al sistema, esto con el objetivo de que a la hora de imprimir el número de veces que se ha usado una llamada al sistema el usuario vea su 
nombre y no solo un número.
```c
char* names[23] = {
        "fork",
        "exit",
        "wait",
        "pipe",
        "read",
        "kill",
        "exec",
        "fstat",
        "chdir",
        "dup",
        "getpid",
        "sbrk",
        "sleep",
        "uptime",
        "open",
        "write",
        "mknod",
        "unlink",
        "link",
        "mkdir",
        "close",
        "date",
        "count"
};
```
La función `main` sigue el esqueleto planteado por el profesor, si se pasa un argumento por línea de comandos entonces se llama a `count` con
el número indicado, de lo contrario se llama a `count` 23 veces, cada vez por llamada al sistema.

### Uso
* `$ uptime`
* `$ date`
* `$ count <numero-llamada-al-sistema>` ó `$ count`
