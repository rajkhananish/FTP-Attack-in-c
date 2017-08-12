#ifndef HANDLETCPSERVER_H_INCLUDED
#define HANDLETCPSERVER_H_INCLUDED
#define BUFSIZE 4096  /* Size of receive buffer */

char srvBuf[BUFSIZE]; /* Buffer for string */
char clntBuf[BUFSIZE];/* Buffer for string */
int port;             /* port Number for PASV connection (DATA Channel) */
int msgSize;          /* size of returned directory listing in chars */


void HandleTCPServer(int sock, char *sentUser, char *sentPass, int *success, int *sucindicator);

#endif // HANDLETCPSERVER_H_INCLUDED
