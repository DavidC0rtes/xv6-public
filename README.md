# Proyecto Final Sistemas Operativos 2019-2

### Integrantes
* David Sanitiago Cortés - 1745133

### Objetivo
A partir del sistema operativo [xv6](https://github.com/mit-pdos/xv6-public) agregar/implementar tres comandos nuevos: 
* `date`: que imprime la fecha actual del sistema en el uso horario UTC-5.
* `uptime`: imprime el tiempo que lleva el sistema en ejecución.
* `count`: imprime cuantas veces se ha utilizado una determinada llamada al sistema, ó cuantas veces se han utilizado todas las llamadas al sistema.

### Implementando date
Para escribir el comando date, que en linux tiene una salida como `vie 05 jun 2020 22:51:18 -05` se tuvieron en cuenta el archivo de cabecera date.h
el cual define una estructura para la fecha (segundo, minuto,..., año) y el esqueleto del comando provisto por el profesor, en el cual
infiere que antes de crear el comando debe crearse una llamada al sistema que recibe como argumento un puntero a la estrucura definida en date.h.

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
