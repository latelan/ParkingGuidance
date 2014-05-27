#include <stdio.h>
#include "data.h"
#include "dijkstra.h"

#define MAX_PATH_LENGTH 16
int main()
{
	FILE *fp;
	char ch;
	int i,j;
	int start,stop;
	int path[MAX_PATH_LENGTH];

	fp = fopen("./data/parking_lots.txt","r");
	if(!fp)
	{
		printf("Can not open the file parking_lots.txt.\n");
		return 0;
	}

	for(i=0;i<LENGTH_OF_PARKING_MAP;i++)
	{
		for(j=0;j<WIDTH_OF_PARKING_MAP;j++)
		{
			fscanf(fp,"%c",&parking_map[i][j]);
		}
		fscanf(fp,"%c",&ch);
	}
	fclose(fp);

	printf("Set start point(1/2):");
	scanf("%d",&start);

	start = start - 1; // set start point
	
	get_shortest_path(start,path,MAX_PATH_LENGTH);

	return 0;
}
