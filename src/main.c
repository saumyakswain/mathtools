#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "matsolve.h"


int main(int argc, char* argv[])
{
	struct rusage usage;
	
	module_generate (argc, argv);
	
	getrusage (RUSAGE_SELF, &usage);
	printf ("CPU time: %ld.%06ld sec user, %ld.%06ld sec system\n",
	usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,
	usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
	return 0;
}
