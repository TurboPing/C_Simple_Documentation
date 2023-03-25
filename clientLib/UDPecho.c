/* UDPecho.c - main, UDPecho */

#include <stdio.h>

extern int	errno;
extern char	*sys_errlist[];

#define	LINELEN		128

/*------------------------------------------------------------------------
 * main - UDP client for ECHO service
 *------------------------------------------------------------------------
 */
int
main(argc, argv)
int	argc;
char	*argv[];
{
	char	*host = "localhost";
	char	*service = "echo";

	switch (argc) {
	case 1:
		host = "localhost";
		break;
	case 3:
		service = argv[2];
		/* FALL THROUGH */
	case 2:
		host = argv[1];
		break;
	default:
		fprintf(stderr, "usage: UDPecho [host [port]]\n");
		exit(1);
	}
	UDPecho(host, service);
	exit(0);
}

/*------------------------------------------------------------------------
 * UDPecho - send input to ECHO service on specified host and print reply
 *------------------------------------------------------------------------
 */
int
UDPecho(host, service)
char	*host;
char	*service;
{
	char	buf[LINELEN+1];		/* buffer for one line of text	*/
	int	s, nchars;		/* socket descriptor, read count*/

	s = connectUDP(host, service);

	while (fgets(buf, sizeof(buf), stdin)) {
		buf[LINELEN] = '\0';	/* insure null-terminated */
		nchars = strlen(buf);
		(void) write(s, buf, nchars);

		if (read(s, buf, nchars) < 0)
			errexit("socket read failed: %s\n",
					sys_errlist[errno]);
		fputs(buf, stdout);
	}
}
