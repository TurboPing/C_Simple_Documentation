/*##################################################################################################
#
#	NAME - dateconv.c
#
#	FUNCTIONS - char *ux2sys(time_t tc)  converts the time tc (in Unix time format)
#			to the spezial time format (YYMMDDWHHMMSS
#							YY: last two chars of year 
#							    if >90 then 19xx else
#							    if <90 then 20xx
#							MM: Month of the year
#							DD: Day of the month
#							W:  Weekday (0-6)
#							HH: Hours
#							MM: Minutes
#							SS: Seconds)
#		    time_t sys2ux(char *st) converts a spezial system time st (format: YYMMDDWHHMMSS)
#			to the unix time format. 
#
#####################################################################################################*/



#include<stdio.h>
#include<time.h>
#include<strings.h>

char *ux2sys(time_t tc);
time_t sys2ux(char *st);

main()
{
	time_t act_time, time_now;
	char *sys_time;
	int unixtime;
	char dat1[]="9909201105959";
        char dat2[]="0001011030303";
        char dat3[]="1302281234856";
        char dat4[]="3607171171311";

	printf("1st: 20.Sep 1999 10:59:59\n");
	printf("Input:       9909201105959\n");
	printf("Input -> unix: %i\n", sys2ux(dat1));
	printf("unix -> sys: %s", ux2sys(sys2ux(dat1)));
	time_now = (time_t)sys2ux(dat1);
	printf("unix -> nor: %s\n", ctime(&time_now));

        printf("2nd: 1.Jan 2000 03:03:03\n");
        printf("Input:       0001011030303\n");
        printf("Input -> unix: %i\n", sys2ux(dat2));
        printf("unix -> sys: %s", ux2sys(sys2ux(dat2)));
        time_now = (time_t)sys2ux(dat2);
        printf("unix -> nor: %s\n", ctime(&time_now));

        printf("3th: 28.Feb 2013 23:48:56\n");
        printf("Input:       1302281234856\n");
        printf("Input -> unix: %i\n", sys2ux(dat3));
        printf("unix -> sys: %s", ux2sys(sys2ux(dat3)));
        time_now = (time_t)sys2ux(dat3);
        printf("unix -> nor: %s\n", ctime(&time_now));

        printf("4th: 17.Jul 2036 17:13:11\n");
        printf("Input:       3607171171311\n");
        printf("Input -> unix: %i\n", sys2ux(dat4));
        printf("unix -> sys: %s", ux2sys(sys2ux(dat4)));
        time_now = (time_t)sys2ux(dat4);
        printf("unix -> nor: %s\n", ctime(&time_now));
}

char *ux2sys(time_t tc)
{
	struct tm *localTime;
	char *asci_time;
	static char spec_time[20];

	localTime = localtime(&tc);
	asci_time = asctime(localTime);

	strftime(spec_time, 18, "%y%m%d%w%H%M%S\n", localTime);
	
	printf("%s", localTime);

	return spec_time;
}

time_t sys2ux(char *st)
{
	struct tm ts;
	static time_t unixtime;
	char dummy;

	sscanf(st, "%2d%2d%2d%*c%2d%2d%2d", &ts.tm_year, &ts.tm_mon, &ts.tm_mday, &ts.tm_hour, &ts.tm_min, &ts.tm_sec);
	ts.tm_mon--;
	if (ts.tm_year < 90)
		ts.tm_year =  ts.tm_year + 100;

	unixtime=mktime(&ts);

	return unixtime;
} 
