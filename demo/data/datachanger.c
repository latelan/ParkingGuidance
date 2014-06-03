#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ROW_LEN 6
#define COR_LEN 60
int main(int argc,char const *argv[])
{
	FILE *fp;
	char path[100];
	char ch;
	int data[ROW_LEN][COR_LEN];
	int i,j;
	srand(time(NULL));
	
	if(argc != 2)
	{
		printf("Too few arguments,need parkingdata path.\n");
		return 0;
	}
	strcpy(path,argv[2]);
	fp = fopen(path,"r+");
	if(!fp)
	{
		printf("Can not open file at %s.",path);
		return 0;
	}

	for(i=0;i<ROW_LEN;i++)
	{
		for(j=0;j<COR_LEN;i++)
			fscanf(fp,"%d",&data[i][j]);
		fscanf(fp,"\n",ch);
	}

	for(i=0;i<ROW_LEN;i++)
	{
		for(j=0;j<COR_LEN;j++)
			printf("%d",data[i][j]);
		printf("\n");
	}
}
