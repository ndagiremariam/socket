#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int clientSocket, ret;
struct sockaddr_in serverAddr;
char buffer[1024];

//char *user = "guest";

//Function to make sure a string is terminated by a \0 not a \n
void makeString(char *commandLine)
{
	commandLine[strlen(commandLine) - 1] = '\0';
}

void connection()
{

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0)
	{
		printf("E... Failed to create socket.\n");
		exit(1);
	}
	printf(".... Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
	if (ret < 0)
	{
		printf("E... Error in connection.\n");
		exit(1);
	}
	printf(".... Connected to Server.\n");
}

int isCommandLegal(char *commandLine)
{
	int a = strncmp(commandLine, "Addpatient", 10);
	int b = strncmp(commandLine, "Check_status", 12);
	int c = strncmp(commandLine, "Addpatientlist", 14);
	int d = strncmp(commandLine, "Search", 6);
	int e = strncmp(commandLine, "Exit", 5);
	int f = strncmp(commandLine, "Clear", 5);
	if (!a || !b || !c || !d || !e || !f)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void welcome()
{
	printf("\n-----------------------------------------------------");
	printf("\n	COVID 19 CASE MANAGEMENT AND REPORTING SYSTEM");
	printf("\n                                            ");
	printf("\n	This is a commandline interface         ");
	printf("\n	of the covid 19 case management and     ");
	printf("\n	reporting system for uganda (CoCaRS Ug)\n");
	printf("\n	Here are the commands you will use throughout, please");
	printf("\n	use them the way they are.\n");
	
}
void listCommands(){
	printf("\n	Addpatient command. Usage");
	printf("\n	Addpatient patientName date gender Status");
	printf("\n	Addpatient file.txt");
	printf("\n	Check_status");
	printf("\n	Search query");
	printf("\n	Exit");
	printf("\n	Clear\n");
}

void main()
{
	system("clear");
	welcome();
	char hospitalName[50];
	char username[20];
	printf(">>> Username: ");
	bzero(username, sizeof(username));
	fgets(username, sizeof(username), stdin);
	makeString(username);
	printf(">>> District Name: ");
	fgets(hospitalName, sizeof(hospitalName), stdin);
	//welcome();

	while (1)
	{
		char command[50];
		char name[30];
		char date[30];
		char gender[10];
		char status[20];
	entry:

		bzero(command, sizeof(command));
		bzero(name, sizeof(name));
		bzero(date, sizeof(date));
		bzero(buffer, sizeof(buffer));
		printf("Enter command\n");
		printf(">>> ");

		fgets(buffer, 1024, stdin);
		
		buffer[strlen(buffer) - 1] = '\0';

		//check if the command entered is legal
		if (isCommandLegal(buffer))
		{
			sscanf(buffer, "%s %s %s %s %s", command, name, date, gender, status);
			//printf(">>> %s:~$ %s, %s, %s \n", user, command, name, date);
			//check if command is Addpatient
			if (strcmp(command, "Addpatient") == 0)
			{

				//Check if only name part has value and not date part
				//implying that it's not a patient name but a file name
				//Of course valid file name should be at least 4 characters a.txt

				if (strlen(name) > 4 && strlen(date) == 0)
				{
					printf("\n.... Reading file:  %s\n", name);
					char *txtExt, *b4null;

					//Get the file extension... It's meant to be a text file. ie .txt
					txtExt = strtok(buffer, ".");
					while (txtExt != NULL)
					{
						b4null = txtExt;
						txtExt = strtok(NULL, ".");
					}
					printf(".... The extension is %s\n", b4null);

					//If extension is not .txt It's a wrong  file.
					if (strcmp(b4null, "txt") == 0)
					{
						FILE *filePtr = fopen(name, "r+");
						if (filePtr == NULL)
						{
							printf("E... file read error!\n");
						}
						else
						{
							//char store[100];
							while (fgets(buffer, 100, filePtr) != NULL)
							{
								//Make sure the \n is replaced with the \0 to make it a string
								buffer[strlen(buffer) - 1] = '\0';
								//puts(store);
								connection();
								send(clientSocket, buffer, strlen(buffer), 0);
								bzero(buffer, sizeof(buffer));
							}
							printf(".... File sent successfully.\n");
						}
						
						goto entry;
					}
					else
					{
						printf("E... Wrong file format\n");
						goto entry;
					}
				}
				else if (strlen(status) > 0) //Check if all the require fields are entered.
				{
					printf(".... Sensible input\n");
					connection();
					send(clientSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
				else
				{
					printf(".... Input doesn't make sense\nIt's not a filename.!!!!\n");
					goto entry;
				}
			}
			else if (strcmp(command, "Addpatientlist") == 0)
			{
				//
			}
			else if (strcmp(command, "Search") == 0)
			{
				connection();
				send(clientSocket, buffer, strlen(buffer), 0);
				bzero(buffer, sizeof(buffer));
				printf("\n-------------Search Results-------------\n");
				for (; 1;)
				{
					ret = recv(clientSocket, buffer, 1024, 0);
					//printf("Things can go wrong");
					if (ret<0)
					{
						printf("Things can go wrong");
					}
					else if (strncmp(buffer, "End", 3)==0)
					{
						printf("\t%s\n", buffer);
						//break;
						connection();
						if (strstr(buffer, "End"))
							break;
					}
					
					bzero(buffer, sizeof(buffer));
				}
			}

			else if (strcmp(command, "Check_status") == 0)
			{
				connection();
				send(clientSocket, buffer, strlen(buffer), 0);
				bzero(buffer, sizeof(buffer));
			}
			else if (strcmp(buffer, "Exit") == 0)
			{
				close(clientSocket);
				printf("E... Exiting ....\n");
				exit(1);
			}
			else if (strcmp(command, "Clear") == 0)
			{
				system("clear");
				goto entry;
			}

			if (recv(clientSocket, buffer, 1024, 0) < 0)
			{
				printf("E... Error in receiving data.\n");
			}
			else
			{
				printf(">>> Server: %s\n", buffer);
			}
			goto entry;
			//close(clientSocket);
		}
		else
		{
			printf(">>> Illegal command\n Try using the correct spelling\n");
			goto entry;
		}
	}

	return;
}