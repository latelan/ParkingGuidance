#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	int i,j,n;
	int rrow,rcor,r,c;
	int sum;
	int data[6][60] = {0};
	FILE *fp;
//	printf("input current parked cars:\n");
	scanf("%d",&n);

	srand(time(NULL));

	for(i=0;i<n;)
	{
		// get row random
		rrow = random()%6;
		// get cor random
		rcor = random()%60;
		
		if(data[rrow][rcor] != 1)
		{
			data[rrow][rcor] = 1;
			i++;
		}
	}

	// save to file
	fp = fopen("parking_lots.txt","w");
	if(fp)
	{
		for(i=0;i<6;i++)
		{
			for(j=0;j<60;j++)
				fprintf(fp,"%d",data[i][j]);
			fprintf(fp,"\n");
		}
	}
	fclose(fp);

	// show
	for(i=0;i<6;i++)
	{
		for(j=0;j<60;j++)
		{
			if(j%10 == 0 && j != 0)
			{
				if(j%20 == 0) printf("\t");
				else printf(" ");
			}

			if(data[i][j] == 1)
				printf("\033[32m%d\033[0m",data[i][j]);
			else
				printf("%d",data[i][j]);
		}

		printf("\n");
		if(i%2 == 0) printf("\n");
	}

	// show area sum
	for(n=0;n<9;n++)
	{
		sum = 0;
		r = (n/3)*2;
		c = (n%3)*20;
		for(i=r; i<r + 2; i++)
		{
			for(j=c;j<c+20;j++)
			{
				sum += data[i][j];
			}
		}
		if(n%3 == 0 && n != 0)
			printf("\n");
		printf("%02d ",sum);
	}
	printf("\n");
}
