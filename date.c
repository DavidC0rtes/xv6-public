#include "types.h"
#include "user.h"
#include "date.h"

char*
month(uint i)
{
	char* months[12] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "junio", "julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
	return months[--i];
}

uint
fhour(uint hour)
{
	uint time = hour < 5 ? 24 - (5 - hour) : hour - 5;
	return time;
}

int
main(void)
{
  struct rtcdate r;

  if (date(&r)) {
    printf(2, "date failed\n");
    exit();
  }

  	printf(1,"%d de %s del %d  %d:%d\n", r.day, month(r.month), r.year, fhour(r.hour), r.minute);

  exit();
}
