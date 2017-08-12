#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdio.h>           /* for perror() */
#include <stdlib.h>          /* for exit() */
#include <sys/socket.h>      /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>       /* for sockaddr_in and inet_addr() */
#endif
#include <stdlib.h>          /* for atoi() and exit() */
#include <string.h>          /* for memset() */
#include <unistd.h>          /* for close() */

int createAndConnectSocket(char * servIP, unsigned short servPort);
int receiveMessage(int s, char *buf, int maxLen);
int receiveData(int dataSocket, char * buffer, int maxSize);
int returnCode(char * buf);
void sendMessage(int sock, char *buf, int msgLen);
void processPASV(char * buf, char * addr, int * port);
void lineSort(char * srvBuf);
void errorMessage(char * message);

//#endif // UTILS_H_INCLUDED


