/* Host.c */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <winsock.h>
#include <windows.h>

int
InitialiseWinSockDLL()
{

  int err;

  /* check the WinSock DLL */
  WORD wVersionRequested;
  
  WSADATA wsaData; /* structure of IPC Networkprogramming */
  
  /* The MAKEWORD macro creates an unsigned 16-bit integer
     by concatenating two given unsinged character values.
     wVersionRequested : The highest version of Wondows Socket
     API support that caller can use. The high order byte 
     specifies the minor version (revision) number; the low
     order byte specifies the major version number */

    wVersionRequested = MAKEWORD(2, 0);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
      printf("WSAStartup failed\n");
      return 0;
    }

    /* Confirm that the WinSock DLL supports 2.0.
       Note that if the DLL supports versions greater
       than 2.0 in addition to 2.0, it will return
       2.0 in wVersion since that is the version we
       requested */
     
    if ((LOBYTE(wsaData.wVersion) != 2) ||
        (HIBYTE(wsaData.wVersion) != 0)) {
      printf("wVersion mismatch\n");
      WSACleanup();
      return 0;
    }

    /* The WinSock DLL is acceptable. Proceed */
  
    printf("InitWinSockDLL successful\n");
  
  return 1;

}

char*
GetInetAddr(char *hostname) {

   int i;
   struct hostent *p_hostent;
   struct in_addr in;

   for(i=0; i<10; i++) {
     p_hostent = gethostbyname(hostname);  
     if (p_hostent != NULL) {
       memcpy(&in.s_addr, p_hostent->h_addr, sizeof(in.s_addr));
       return (char *)inet_ntoa(in);
     }
   }
   return NULL;

}

int main()
{


	char *hostname;
	char *ipAddr;

	InitialiseWinSockDLL();

	hostname = "bb1";
	ipAddr = GetInetAddr(hostname);
	if (ipAddr != NULL) {
		printf("hostname : %s, ip addr : %s\n", hostname, ipAddr);
	}
	else {
		printf("hostname : %s, ip addr = NULL\n", hostname);
	}

	hostname = "100.100.100.1";
	ipAddr = GetInetAddr(hostname);
	if (ipAddr != NULL) {
		printf("hostname : %s, ip addr : %s\n", hostname, ipAddr);
	}
	else {
		printf("hostname : %s, ip addr = NULL\n", hostname);
	}

	hostname = "bb4";
	ipAddr = GetInetAddr(hostname);
	if (ipAddr != NULL) {
		printf("hostname : %s, ip addr : %s\n", hostname, ipAddr);
	}
	else {
		printf("hostname : %s, ip addr = NULL\n", hostname);
	}

	hostname = "notknown";
	ipAddr = GetInetAddr(hostname);
	if (ipAddr != NULL) {
		printf("hostname : %s, ip addr : %s\n", hostname, ipAddr);
	}
	else {
		printf("hostname : %s, ip addr = NULL\n", hostname);
	}

  return 0;

}