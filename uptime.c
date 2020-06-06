#include "types.h"
#include "user.h"

void
timef(uint time)
{
	int hours  = time/3600;
	
	int offset = time%3600;

	int mins = offset/60;

	int secs = offset%60;
	if (secs >= 1) {
		--secs;
	}

	printf(1, "up: %dh:%dm:%ds\n", hours, mins, secs);
}

int
main(void)
{
	if(uptime() < 0) {
		printf(2,"uptime failed.\n");
	}

	timef(uptime()/100);
	
	exit();
}
