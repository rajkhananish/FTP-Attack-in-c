#include <stdio.h>         /* for perror() */
#include <stdlib.h>        /* for exit() */
#include <sys/socket.h>    /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>     /* for sockaddr_in and inet_addr() */
#include <stdlib.h>        /* for atoi() and exit() */
#include <string.h>        /* for memset() */
#include <unistd.h>        /* for close() */
#include "Utils.h"
#define BUFSIZE 4096

/* Should have a procedure to allow buffered file storage rather than depend on a 4K Buffer */

int createAndConnectSocket(char * servIP, unsigned short servPort)
{

	int sock = 0;
	struct sockaddr_in servAddr;    /* server address */

	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		errorMessage("socket() failed");
	}

	/* Construct the server address structure */
	memset(&servAddr, 0, sizeof(servAddr));                         /* Zero out structure */
	servAddr.sin_family      = AF_INET;                             /* Internet address family */
	servAddr.sin_addr.s_addr = inet_addr(servIP);                   /* Server IP address */
	servAddr.sin_port        = htons(servPort);                     /* Server port */

	/* Establish the connection to the server */
	if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
	{
		errorMessage("connect() failed");
	}
	return sock;
};

/* Receive Message over command stream */
int receiveMessage(int sock, char *buf, int maxLen)
{
	int received = 0;
	int rv = 0;
	memset(buf,0,maxLen);

	rv = recv(sock, buf+received, 1, 0);
	while ((received < maxLen) && (rv > 0) && *(buf+received) != '\n')
	{
		received += rv;
		rv = recv(sock, buf+received, 1, 0);
	}
	if (rv < 0)
	{
		errorMessage("recv() failed");
	}
	return received;
};

/* Send message over command stream */
void sendMessage(int sock, char *buf, int msgLen)
{
	if (send(sock, buf, msgLen, 0) != msgLen)
	{
		errorMessage("sendMessage Error");
	}
};

/* process PASV return string to get IP Address and Port number for Data Channel */
void processPASV(char * buf, char *addr, int * port)
{
	char messageSent[BUFSIZE];
	strcpy(messageSent, buf);
	char delim[] = "(,)";
	char *token;
	int i = 0;
	int portA, portB;
	char *array[6];
	token = strtok(messageSent, delim);
	while(token != NULL)
	{
		array[i] = strdup(token);
		token = strtok(NULL, delim);
		i++;
	}
	sprintf(addr, "%s.%s.%s.%s", array[1], array[2], array[3], array[4]);
	portA = atoi(array[5]);
	portB = atoi(array[6]);
	*port = (portA*256)+ portB;
};

void lineSort(char *srvBuf)
{
	char* lineLast;
	char* columnLast;
	char* lineToken;
	char* columnToken;

	lineToken = strtok_r(srvBuf, "\r\n", &lineLast);
	int columnTokenCounter;
	int isdir;
	int filesizetotal = 0;

	printf("Type \tSize \t\tName\n");
	while(lineToken != NULL)
	{
		columnToken = strtok_r(lineToken, " ",  &columnLast);
		columnTokenCounter = 0;
		while(columnToken != NULL)
		{
			if(columnTokenCounter < 8)
			{
				switch(columnTokenCounter)
				{
					case 0:
						if(*columnToken == 'd')
						{
							isdir = 1;
							printf("Dir \t");
						}
						else
						{
							isdir = 0;
							printf("File \t");
						}
						break;
					case 4:
						printf("%s \t\t", columnToken);
						if(isdir != 1)
						{
							filesizetotal += atoi(columnToken);
						}
						break;
				}
			}
			else if(columnTokenCounter >= 8)
			{
				printf("%s ", columnToken);
			}
			columnToken = strtok_r(NULL, " ", &columnLast);
			columnTokenCounter++;
		}
		printf("\n");
		lineToken = strtok_r(NULL, "\r\n", &lineLast);
	}
	printf("\n Total Size of FIles: %d Bytes\n", filesizetotal);
	sleep(5);
}


/* Receive data from data stream */
int receiveData(int dataSocket, char * buffer, int maxSize)
{

	int byteCount = 0;
	/* Get Data from the server, theres a 4K buffer there for this but you could use a temp file or dynamic memory */
	int rv = 0;
	memset(buffer,0,maxSize);
	rv = recv(dataSocket, buffer+byteCount, 1, 0);
	while((byteCount < maxSize) && (rv > 0))
	{
		byteCount += rv;
		rv = recv(dataSocket, buffer+byteCount, 1, 0);
	}
	if(rv < 0)
	{
		errorMessage("Function: reciveData failed! ");
	}
	return byteCount;
};

/* Extract Return Code (First three characters) */
int returnCode(char * buf)
{
	return atoi(buf);
};

void errorMessage(char * message) {
	fprintf(stderr, "ERROR: %s\n", message);
	exit(1);
};
