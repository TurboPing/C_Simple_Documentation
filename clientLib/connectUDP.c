/* connectUDP.c - connectUDP */

/*------------------------------------------------------------------------
 * connectUDP - connect to a specified UDP service on a specified host
 *------------------------------------------------------------------------
 */
int
connectUDP( host, service )
char	*host;		/* name of host to which connection is desired	*/
char	*service;	/* service associated with the desired port	*/
{
	return connectsock(host, service, "udp");
}
