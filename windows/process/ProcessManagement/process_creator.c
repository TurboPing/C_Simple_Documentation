/* ------------------------------------------------------
 * Testapplication for Process Management under Windows
 * ------------------------------------------------------
 * Wavetek - Wandel und Goltermann
 * Date: 10.5.2000
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
 * Main -- Creates a new process and waits until it terminates
 *		   then print the ExitCode (ReturnCode) from the new process
 *
 * Arguments: none
 *
 * Returns: none
 *
 * Error Messages: none
 *
 */
  
void main( VOID )
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
	LPDWORD ExitStatus;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process. 
    if( !CreateProcess( NULL, // No module name (use command line). 
        "ChildAppl.exe blabla uuullla",			  // Command line. 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        0,                // No creation flags. 
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
    ) 
    {
		printf("mainthread: Couldn't create new process\n");
    }



    /* Wait until child process exits. */
	printf("mainthread: Waiting for Single Object!\n");
    WaitForSingleObject( pi.hProcess, INFINITE );
	printf("mainthread: Single Object Terminated ... \n");

	/* Get Exit Code from the terminating process */

   	GetExitCodeProcess(pi.hProcess,	(LPDWORD)&ExitStatus); 

	printf("mainthread: ExitCode: %u\n", ExitStatus);
	
	// Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

/* End of Main */