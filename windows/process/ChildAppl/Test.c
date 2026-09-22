/* ------------------------------------------------------
 * Testapplication for Process Management under Windows
 * ------------------------------------------------------
 * Wavetek - Wandel und Goltermann
 * Date:23.5.2000
 * Description: For testing Process Management 
 *		under Windows NT
 * Autor: Kai Keong Ng
 * ------------------------------------------------------*/

/* ------------------------------------------- INCLUDES -*/

#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------- DEFINES */

/* ------------------------------------------- TYPES */

/* ------------------------------------------- GLOBAL VARS */

/* ------------------------------------------- LOCAL VARS */

/* ------------------------------------------- PROCEDURES */

/* -------------------------------------------------------
 * Main -- Counts from 10 down (step: one second) then exit
 *
 * Arguments: none
 *
 * Returns: yes - testvalue 5
 *
 * Error Messages: none
 *
 */

int main(int argc, char *argv[]) {
	
	DWORD interval = 1000;
	int i;

	printf("newproc: printing out commandline arguments: \n");
	for (i = 0; i < argc; i++) {
		printf("newproc: argument %i: %s\n", i, argv[i]);
	}

	printf("newproc: Count ten seconds!\n");
	Sleep(interval);
	printf("newproc: 10\n");	Sleep(interval);
	printf("newproc: 9\n");	Sleep(interval);
	printf("newproc: 8\n");	Sleep(interval);
	printf("newproc: 7\n");	Sleep(interval);
	printf("newproc: 6\n");	Sleep(interval);
	printf("newproc: 5\n");	Sleep(interval);
	printf("newproc: 4\n");	Sleep(interval);
	printf("newproc: 3\n");	Sleep(interval);
	printf("newproc: 2\n");	Sleep(interval);
	printf("newproc: 1\n");	Sleep(interval);
	printf("newproc: Game over!\n");

	return(2);
}

/* End of Main */