#include "types.h"
#include "stat.h"
#include "user.h"
#include "count.h"

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


int
main(int argc, char *argv[])
{	
	int nsyscall = -1;
	
	if (argc == 2) {
		nsyscall = atoi(argv[1]);
	}

	if (nsyscall == -1) {

		for(int i=1; i <= 23; i++) {
			printf(1,"%s called %d times.\n", names[i-1], count(i));
		}
	
	} else {
		printf(1, "%s called %d times.\n", names[nsyscall-1], count(nsyscall));
	}

 	exit();
}
