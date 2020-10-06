#include <unistd.h>#include <stdio.h>#include <stdlib.h>#include <string.h>#include <wiringPi.h>#include <stdint.h>typedef uint8_t u1_t;static u1_t APPEUI[8];static u1_t  DEVEUI[8];static u1_t  APPKEY[16];void passingSettingToParameters(int count, char setting[256]){    switch(count)    {        case 0:            //printf("case %d\n", count);            printf("line %s\n",setting);            int counter = 0;            for (int i = 2;i<47;i=i+6)            {               DEVEUI[counter]= setting[i+0] ^ setting[i+1] ^ setting[i+2] ^ setting[i+3];               // if(setting[i] != ",");               //{               //    DEVEUI[counter] = setting[i];               //  counter++;               // }               counter++;            }            printf("DEVEUI %s\n",DEVEUI);            return;        case 1:            //printf("case %d\n", count);            printf("line %s\n", setting);            return;        case 2:            //printf("case %d\n", count);            printf("line %s\n", setting);            return;    }}void settingReader(){	FILE *fptr3;	fptr3 = fopen("setupfileC.txt","r");    /*	char a [256] ;	char b [256] ;	char c [256] ;   	int a1 = 0;	int b1 = 1;	int c1 = 2;*/	int count=0;	char line[256];	while (fgets(line, sizeof line, fptr3) != NULL) /* read a line */	{	 //   printf("line test %s\n",line);		switch(count)		{			case 0:				//fscanf(fptr3,"%[^\n]",a);				//a = line;				//printf("Dev EUI %s\n",a);				//printf("count test %d\n",count);                printf("Dev EUI %s\n",line);                passingSettingToParameters(0,line);                break ;			case 1:				//fscanf(fptr3,"%[^\n]",b);				//b = line;                //printf("App EUI %s\n",b);                //printf("count test %d\n",count);                printf("App EUI %s\n",line);                passingSettingToParameters(1,line);                break;			case 2:				//fscanf(fptr3,"%[^\n]",c);				//c = line;				//printf("App key %s\n",c);				//printf("count test %d\n",count);				printf("App key %s\n",line);				passingSettingToParameters(2,line);				break;			case 3:				return ;			default:				printf("Error settings not found");		}	count++;	}}int main(){	FILE *fptr1, *fptr2;	int linectr = 1;	char str[256];	char data[256],temp[]="temp.txt";	settingReader();/*	 if (count == a1)        {	 fscanf(fptr3,"%[^\n]",a);	  printf("Dev key%s\n",line);            //use line or in a function return it            //in case of a return first close the file with "fclose(file);"        }        else        {            count++;        }    }/*   	fscanf(fptr3,"%[^\n]",a);	fscanf(fptr3,"%[^\n]",b);    fscanf(fptr3,"%[^\n]",c);    printf("Dev key%s\n",a);	printf("App eui%s\n",b);    printf("App key%s\n",c);    fclose(fptr3);/*    for (int i = 0;i=7;i++){        APPEUI[i] =    }*/	fptr1 = fopen("dataFile.txt","r+");	if(!fptr1)	{		printf("Unable to open data file!!\n");		return 0;	}	fptr2 = fopen(temp,"w");	if (!fptr2)	{		printf("Unable to open a temporary file to write!!");		fclose(fptr1);		return 0;	}	if( fgets (data, 60, fptr1)!=NULL )	{		printf("Reading data from data file\n");		puts(data);	}	while (!feof(fptr1))	{		strcpy(str, "\0");		fgets(str, 256, fptr1);		if (!feof(fptr1))		{			linectr++;			fprintf(fptr2, "%s", str);		}	}	fclose(fptr1);	fclose(fptr2);	remove("dataFile.txt");	rename(temp,"dataFile.txt");	printf("Removed the first line\n");	sleep(5);	printf("Sleep test\n");	return 0;}