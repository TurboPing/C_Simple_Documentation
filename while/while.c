/*/ time.c */

#include <stdio.h>
#include <sys/time.h>


int main()
{


	time_t actualTime;
	time_t gmTimeOffset;
	struct tm *gmTime;
	
while (1) {
	/* set time for sync the TRU */

	actualTime = time(NULL);
	gmTime = gmtime(&actualTime);
	gmTimeOffset = actualTime - (mktime(gmTime));
	
	printf("actualTime : %d Offset : %d\n",
                actualTime, gmTimeOffset);

}

}
