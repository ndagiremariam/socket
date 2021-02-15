#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//typedef struct Patients
//{
	// char patientName[100];
	// char patientStatus[30];
	// char patientGender[10];
	// char date[50];

//} patient;

//patient patients[100];

int main()
{
	char *patientName;
	patientName = (char*)malloc(sizeof(char)*100);
	char *patientStatus;
	patientStatus = (char*)malloc(sizeof(char)*100);
	char *patientGender;
	patientGender = (char*)malloc(sizeof(char)*100);
	char *date;
	date = (char*)malloc(sizeof(char)*100);
	printf("Starting...");
	MYSQL *con = mysql_init(NULL);

	if (con == NULL)
	{
		//fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_real_connect(con, "127.0.0.1", "root", "", "test", 0, NULL, 0) == NULL)
	{
		//printf("Failed to connect to database");
	}

	FILE *serverFile;
	serverFile = fopen("/home/code-lord/Desktop/cron_job_working/hello.txt", "r");
	//puts("\nConnected to database server\n");

	char *buffer, *dtm, *sqlStatement;
	buffer = (char*)malloc(sizeof(char)*256);
	dtm =  (char*)malloc(sizeof(char)*256);
	sqlStatement = (char*)malloc(sizeof(char)*256);
	char  *secondName;
	secondName = (char*)malloc(sizeof(char)*100);
	int index = 0;
	while (fgets(buffer, 255, serverFile) != NULL)
	{
		//puts(buffer);
		//printf(buffer);
		strcpy(dtm, buffer);
		sscanf(dtm, "%s %s %s %s %s\n", patientName, secondName, date,
			   patientGender, patientStatus);

		//concatenate first to patients.name
		strcat(patientName, "  ");
		strcat(patientName, secondName);

		snprintf(sqlStatement, 300, "INSERT INTO patients(name,date,gender,officer) VALUES('%s','%s','%s','%s')",
				 patientName, date, patientGender,
				 patientStatus);

		if (mysql_query(con, sqlStatement)==NULL)
		{
			fprintf(stderr, "%s\n", mysql_error(con));
		}
		//printf("Name: %s \nDate: %s \nGender: %s\nStatus: %s\n",
		//patients[index].patientName, patients[index].date, patients[index].patientGender,
		//patients[index].patientStatus);
		
	}
	fclose(fopen("/home/code-lord/Desktop/cron_job_working/hello.txt", "w+"));

	mysql_close(con);
	exit(0);
}
