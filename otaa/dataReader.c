#include <unistd.h>#include <stdio.h>#include <stdlib.h>#include <string.h>#include <wiringPi.h>#include <stdint.h>typedef uint8_t u1_t;static u1_t APPEUI[8];static u1_t  DEVEUI[8];static u1_t  APPKEY[16];int getVal(char c){    int num = (int) c;    if(num < 58 && num > 47)    {        return num - 48;    }    if(num < 71 && num > 64)    {        return num - 55;    }        return num;}void passingSettingToParameters(int count, char setting[256]){int counter;    switch(count)    {        case 0:            printf("line %s\n",setting);            counter = 0;            for (int i = 0;i<16;i=i+2)            {                printf("%c +",setting[i]);                printf(" %c =",setting[i+1]);                long int val = getVal(setting[i])<<4 | getVal(setting[i+1]);                printf("%d\n",val);                DEVEUI[7-counter]=val;                printf("%d + ",setting[i]);                printf(" %d = ",setting[i+1]);                printf("%c\n",val);                counter++;            }            printf("DEVEUI %d\n",DEVEUI);            return;        case 1:            printf("line %s\n",setting);            counter = 0;            for (int i = 0;i<16;i=i+2)            {                printf("%c +",setting[i]);                printf(" %c =",setting[i+1]);                long int val = getVal(setting[i])<<4 | getVal(setting[i+1]);                printf("%d\n",val);                APPEUI[7-counter]=val;                printf("%d + ",setting[i]);                printf(" %d = ",setting[i+1]);                printf("%c\n",val);                counter++;            }            printf("APPEUI %d\n",APPEUI);return;        case 2:            printf("line %s\n",setting);            counter = 0;            for (int i = 0;i<32;i=i+2)            {                printf("%c +",setting[i]);                printf(" %c =",setting[i+1]);                long int val = getVal(setting[i])<<4 | getVal(setting[i+1]);                printf("%d\n",val);                APPKEY[counter]=val;                printf("%d + ",setting[i]);                printf(" %d = ",setting[i+1]);                printf("%c\n",val);                counter++;            }            printf("APPKEY %d\n",APPKEY);            //printf("case %d\n", count);            printf("line %s\n", setting);            return;    }}void settingReader(){	FILE *fptr3;	fptr3 = fopen("setupfileC.txt","r");	int count=0;	char line[256];	while (fgets(line, sizeof line, fptr3) != NULL) /* read a line */	{	printf("line test %s\n",line);		switch(count)		{			case 0:				printf("Dev EUI %s\n",line);                passingSettingToParameters(0,line);                break ;			case 1:				printf("App EUI %s\n",line);                passingSettingToParameters(1,line);                break;			case 2:				printf("App key %s\n",line);				passingSettingToParameters(2,line);				break;			case 3:				return ;			default:				printf("Error settings not found");		}	count++;	}}int main(){	FILE *fptr1, *fptr2;	int linectr = 1;	char str[256];	char data[256],temp[]="../temp.txt";	//settingReader();	fptr1 = fopen("../dataFile.txt","r+");	if(!fptr1)	{		printf("UnaGblGGGe to open data file!!\n");		return 0;	}	fptr2 = fopen(temp,"w");	if (!fptr2)	{		printf("Unable to open a temporary file to write!!");		fclose(fptr1);		return 0;	}	if( fgets (data, 60, fptr1)!=NULL )	{		printf("Reading data from data file\n");		puts(data);	}	while (!feof(fptr1))	{		strcpy(str, "\0");		fgets(str, 256, fptr1);		if (!feof(fptr1))		{			linectr++;			fprintf(fptr2, "%s", str);		}	}	fclose(fptr1);	fclose(fptr2);	remove("../dataFile.txt");	rename(temp,"dataFile.txt");	printf("Removed the first line\n");	sleep(5);	printf("Sleep test\n");	return 0;}