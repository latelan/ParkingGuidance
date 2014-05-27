#include <stdio.h>
#include "dijkstra.h"
#include "sql.h"

int parking_check(const char *id_card,char *emsg)
{
	int can_stop;
	if(is_full() == 1)
	{// 车库是否已满
		emsg = (char *)malloc(siezof(char)*100);
		strcpy(emsg,"The parking lots is full.");
		return 0;
	}
	
	if(is_valid_user(id_card) == 0)
	{
		emsg = (char *)malloc(sizeof(char)*);
		strcpy(emsg,"Valid user")
	}



}
void init()
{
	set_parking_map(path_parking_data,1);
}
int main()
{
	char path_parking_data[] = "./data/parking_lots.txt";
	int stop;
	set_parking_map(path_parking_data,1);

	get_final_stop(2,&stop);

	printf("Suggest stop area: %s\n",area_node_id(stop));
}
