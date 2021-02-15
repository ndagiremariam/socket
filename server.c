#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int extract_command(char *);
void main()
{
	system("clear");
	printf("------------SERVER----------SERVER-----------SERVER-----------\n");
	printf("       COVID 19 CASE REPORTING AND MANAGEMENT SYSTEM\n");
	system("touch hello.txt");

	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[256];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("\n\t[-]Error in connection.\n");
		exit(1);
	}
	printf("\n\t[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (ret < 0)
	{
		printf("\t[-]Error in binding.\n");
		exit(1);
	}
	printf("\n\t[+]Bind to port %d\n", PORT);

	if (listen(sockfd, 10) == 0)
	{
		printf("\t[+]Listening....\n");
	}
	else
	{
		printf("\t[-]Error in binding.\n");
	}

	while (1)
	{
		newSocket = accept(sockfd, (struct sockaddr *)&newAddr, &addr_size);
		if (newSocket < 0)
		{
			exit(1);
		}
		printf("\n\t[+]Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if ((childpid = fork()) == 0)
		{
			close(sockfd);

			while (1)
			{
				bzero(buffer, sizeof(buffer));
				recv(newSocket, buffer, 1024, 0);
				int command = extract_command(buffer);

				switch (command)
				{
				case 1:
				{
					
					if (strcmp(buffer, ":exit") == 0)
					{
						printf("\t[-]Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					}
					else
					{

						FILE *filePtr = fopen("hello.txt", "a");
						printf("\t[+]Writing to file");
						fprintf(filePtr, "%s\n", buffer);
						printf("\t[+]wrote to file");
						printf("\t[+]Client: %s\n", buffer);
						send(newSocket, buffer, strlen(buffer), 0);
						//fputs(buffer,filePtr);
						bzero(buffer, sizeof(buffer));
					}
					return;
				}

				case 2:
				{
					FILE *filePtr = fopen("hello.txt", "r");
					int totalRecords = 0;
					while (fgets(buffer, 256, filePtr) != NULL)
					{
						totalRecords++;
						//if (strstr(buffer, searchString) != NULL)
						// {
						// 	puts(buffer);
						// 	available++;
						// }
					}
					bzero(buffer, sizeof(buffer));
					printf("\nTotal cases %d\n", totalRecords);
					sprintf(buffer, "Total cases are %d\n", totalRecords);
					send(newSocket, buffer, strlen(buffer), 0);
					return;
				}

				case 3:
				{
					FILE *filePtr = fopen("hello.txt", "r");
					// char search_name[50];
					char store[200];
					int records = 0;
					int total_records = 0;
					// char PatientName[20] = "PatientName";
					// char Date[20] = "Date";
					// char Gender[20] = "Gender";

					// char OfficerName[20] = "OfficerName";
					printf("\n");
					printf("\nThe search string is %s\n", buffer);
					//printf("%-20s\t\t%-15s\t%-2s\t%-5s\n", PatientName, Date, Gender, OfficerName);
					printf("\n");
					while (fgets(store, 100, filePtr) != NULL)
					{
						total_records++;

						if (strstr(store, buffer) != NULL)
						{
							//puts(store);
							//sprintf(buffer, "\nThis is from the server %s\n", store);
							send(newSocket, store, strlen(store), 0);
								bzero(store, sizeof(store));
							records++;
						}
						//send(newSocket, store, strlen(store), 0);
						//		bzero(store, sizeof(store));
					}
					bzero(store, sizeof(store));
					send(newSocket, store, strlen(store), 0);
					strcpy(store,"End");
					//send(newSocket, store, strlen(store), 0);
					bzero(buffer, sizeof(buffer));
					printf("\nWe found %d result(s)\n", records);
					sprintf(buffer, "%d Results found \n", records);
					send(newSocket, buffer, strlen(buffer), 0);
					return;
				}

				// case 4:
				// {
				// 	break;
				// }
				case 5:
				{if (strcmp(buffer, ":exit") == 0)
					{
						printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					}
					else
					{

						FILE *filePtr = fopen("hello.txt", "a");
						printf("[+]Writing to file");
						fprintf(filePtr, "%s\n", buffer);
						printf("[+]wrote to file");
						printf("[+]Client: %s\n", buffer);
						send(newSocket, buffer, strlen(buffer), 0);
						//fputs(buffer,filePtr);
						bzero(buffer, sizeof(buffer));
					}
					return;
				}
				}
			}
		}
	}

	close(newSocket);

	//return 0;
}

int extract_command(char *buffer)
{
	if (strncmp(buffer, "Addpatient", 10) == 0)
	{
		printf("\nBuffer before is %s\n", buffer);
		printf("\n\t[+]Addpatient command...");
		strcpy(buffer, buffer + 11);
		printf("\nBuffer now is %s\n", buffer);
		return 1;
	}
	else if (strncmp(buffer, "Check_status", 12) == 0)
	{
		printf("\n\t[+]Check_status command");
		return 2;
	}
	else if (strncmp(buffer, "Search", 6) == 0)
	{
		printf("\n\t[+]Search");
		strcpy(buffer, buffer + 7);
		return 3;
	}
	else if (strncmp(buffer, "Addpatientlist", 14) == 0)
	{
		printf("\n\t[+]Addpatientlist");
		return 4;
	}
	else
	{
		printf("\n\t[+]Adding from file");
		return 5;
	}
}