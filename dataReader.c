#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>

int main()
{
	FILE *fptr1, *fptr2;
	int linectr = 1;
	char str[256];
	char data[256],temp[]="temp.txt";


	fptr1 = fopen("dataFile.txt","r+");

	if(!fptr1)
	{
		printf("Unable to open data file!!\n");
		return 0;
	}

	fptr2 = fopen(temp,"w");
	if (!fptr2)
	{
		printf("Unable to open a temporary file to write!!");
		fclose(fptr1);
		return 0;
	}

	if( fgets (data, 60, fptr1)!=NULL )
	{
		printf("Reading data from data file\n");
		puts(data);

	}

	while (!feof(fptr1))
	{
		strcpy(str, "\0");
		fgets(str, 256, fptr1);
		if (!feof(fptr1))
		{
			linectr++;
			fprintf(fptr2, "%s", str);
		}
	}
	fclose(fptr1);
	fclose(fptr2);
	remove("dataFile.txt");
	rename(temp,"dataFile.txt");
	printf("Removed the first line\n");
	sleep(5);
	printf("Sleep test\n");
	return 0;
}
