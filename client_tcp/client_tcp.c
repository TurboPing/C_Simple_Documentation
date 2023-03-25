/* httpget.c
 * Demoprogramm zur Programmierung von Netzwerkclients
 * Es wird ein GET request via http an einen Webserver 
 * gesendet und das Ergebnis auf der Konsole ausgegeben. */

/*Compile with: gcc -o httpget httpget.c -lnsl -lsocket*/

#include <stdio.h>
#include <errno.h>

#ifdef _WIN32
/* Headerfiles für Windows */
#include <winsock.h>
#include <io.h>

#else
/* Headerfiles für Unix/Linux */ 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket(s) close(s)
#endif

/* http requests werden normalerweise auf Port 7
 * vom Server entgegengenommen */
#define TCP 7
#define INADDR_NONE 0xffffffff

/****************** MAIN *********************/
int main( int argc, char **argv)
{
    struct sockaddr_in server;
    struct hostent *host_info;
    unsigned long addr;
    int sock;
    char buffer[8192];
    int count;
    int argument;
    
#ifdef _WIN32  
    /* Initialisiere TCP für Windows ("winsock") */
    short wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD (1, 1);
    if (WSAStartup (wVersionRequested, &wsaData) != 0) {
        fprintf( stderr, "Gescheitert mit Windows\n");
        exit(1);
    }
#endif

    /* Sind die erforderlichen Kommandozeilenargumente vorhanden? */
    if (argc != 3) {
        fprintf( stderr, "Zu wenig Argumente fuer das Programm client_test angegeben\n");
        exit(1);
    }
    /* Erzeuge das Socket */
    sock = socket( PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror( "Das kreiren vom Socket ist gescheitert");
        exit(1);
    }

    /* Erzeuge die Socketadresse des Servers 
     * Sie besteht aus Typ, IP-Adresse und Portnummer */
    memset( &server, 0, sizeof (server));
    if ((addr = inet_addr( argv[1])) != INADDR_NONE) {
        /* argv[1] ist eine numerische IP-Adresse */
        memcpy( (char *)&server.sin_addr, &addr, sizeof(addr));
    }
    else {
        /* Wandle den Servernamen in eine IP-Adresse um */
        host_info = gethostbyname( argv[1]);
        if (NULL == host_info) {
            fprintf( stderr, "Unbekannter server: %s\n", argv[1]);
            exit(1);
        }
        memcpy( (char *)&server.sin_addr, host_info->h_addr, host_info->h_length);
    }

    argument = atoi(argv[2]);
    server.sin_family = AF_INET;
    server.sin_port = htons(argument);

    /* Baue die Verbindung zum Server auf */
    if ( connect( sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror( "Kann nicht zum Server verbinden \n");
        exit(1);
    }
    
    /* Erzeuge und sende den http GET request */
    sprintf( buffer, "GET %s TCP/1.0", argv[2]);
    printf("send : %s\n", buffer);
    send(sock, buffer, strlen(buffer), 0);
    
    /* Hole die Serverantwort und gib sie auf Konsole aus */
    count = recv( sock, &buffer, strlen(buffer), 0);
    printf("recv : %s\n", buffer);

    /* Schließe Verbindung und Socket */
    closesocket(sock);
    printf("Socket geschlossen\n");
    return (count);
}

