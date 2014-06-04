#include "data.h"


/*
 * 功能：更新每个停车区域的停车数量
 * 参数：无
 * 返回值：void
 */
void update_area_node()
{
	int i;
	for (i = 0; i < MAX_AREA_NODE; ++i)
	{
		switch(i)
		{  
			case A1: area_node[A1] = compute_single_node(0, 0, 2, 20); break;
			case A2: area_node[A2] = compute_single_node(2, 0, 2, 20); break;
			case A3: area_node[A3] = compute_single_node(4, 0, 2, 20); break;

			case B1: area_node[B1] = compute_single_node(0, 20, 2, 20); break;
			case B2: area_node[B2] = compute_single_node(2, 20, 2, 20); break;
			case B3: area_node[B3] = compute_single_node(4, 20, 2, 20); break;

			case C1: area_node[C1] = compute_single_node(0, 40, 2, 20); break;
			case C2: area_node[C2] = compute_single_node(2, 40, 2, 20); break;
			case C3: area_node[C3] = compute_single_node(4, 40, 2, 20); break;			
		}
	}
}

/*
 * 功能：计算某区域的停车数量
 * 参数：startr - 起始位置row值
 *		 startc - 起始位置cor值
 *		 rlen   - row长
 *		 clen   - cor长
 * 返回值：int 该区域停车数量 
 */
int compute_single_node(int startr,int startc, int rlen, int clen)
{
	int sum = 0;
	int i,j;
	for (i = startr; i < startr+rlen; ++i)
	{
		for (j = startc; j < startc+clen; ++j)
		{
			sum += parking_map[i][j] == '1' ? 1 : 0;
		}
	}

	return sum;	
}

/*
 * 功能：设置停车位信息
 * 参数：isparking - 0 为无车，1为有车
 *		 row - 停车位行坐标
 *		 cor - 停车位列坐标
 * 返回值：void
 */
void set_parking_map_node(int isparking, int row, int cor)
{
	parking_map[row][cor] = isparking;
	return;	
}

/*i
 * 功能：更新节点邻接矩阵值,设置node_adj
 * 参数：无
 * 返回值：void，
 */
void update_node_adj()
{
	int i,j;
	for (i = 0; i < MAX_KEY_NODE - 2; ++i)
	{
		for (j = 0; j < MAX_KEY_NODE - 2; ++j)
		{
			node_adj[i][j] = INF;
		}
	}	

	for (i = 0; i < MAX_KEY_NODE; ++i)
	{
		switch(i)
		{
			case NA1: 
			{
				node_adj[NA1][NA2] = compute_single_node(0,20,1,20) * 2;				
				node_adj[NA1][NB1] = compute_single_node(0,0,1,20) * 2;
				node_adj[NA1][NB2] = compute_single_node(0,10,2,20);
				break;
			}
			case NA2: 
			{
				node_adj[NA2][NA1] = compute_single_node(0,20,1,20) * 2;
				node_adj[NA2][NB3] = compute_single_node(0,30,2,20);
				node_adj[NA2][NB4] = compute_single_node(0,40,1,20) * 2;
				break;
			}
			case NB1: 
			{
				node_adj[NB1][NB2] = compute_single_node(0,0,2,20);
				node_adj[NB1][NC1] = compute_single_node(1,0,2,10) * 2;
				break;
			}
			case NB2: 
			{
				node_adj[NB2][NB1] = compute_single_node(0,0,2,20);
				node_adj[NB2][NB3] = compute_single_node(0,20,2,20);
				node_adj[NB2][NC2] = compute_single_node(1,10,2,20);
				break;
			}
			case NB3: 
			{
				node_adj[NB3][NB2] = compute_single_node(0,20,2,20);
				node_adj[NB3][NB4] = compute_single_node(0,40,2,20);
				node_adj[NB3][NC3] = compute_single_node(1,30,2,20);
				break;
			}
			case NB4: 
			{
				node_adj[NB4][NB3] = compute_single_node(0,40,2,20);
				node_adj[NB4][NC4] = compute_single_node(1,50,2,10) * 2;
				break;
			}
			case NC1: 
			{
				node_adj[NC1][NC2] = compute_single_node(2,0,2,20);
				node_adj[NC1][ND1] = compute_single_node(3,0,2,10) * 2;
				break;
			}
			case NC2: 
			{
				node_adj[NC2][NC1] = compute_single_node(2,0,2,20);
				node_adj[NC2][NC3] = compute_single_node(2,20,2,20);
				node_adj[NC2][ND2] = compute_single_node(3,10,2,20);
				break;
			}
			case NC3: 
			{
				node_adj[NC3][NC2] = compute_single_node(2,20,2,20);
				node_adj[NC3][NC4] = compute_single_node(2,40,2,20);
				node_adj[NC3][ND3] = compute_single_node(3,30,2,20);
				break;

			}
			case NC4: 
			{
				node_adj[NC4][NC3] = compute_single_node(2,40,2,20);
				node_adj[NC4][ND4] = compute_single_node(3,50,2,10) * 2;
				break;
			}
			case ND1: 
			{
				node_adj[ND1][ND2] = compute_single_node(4,0,2,20);
				break;
			}
			case ND2: 
			{
				node_adj[ND2][ND1] = compute_single_node(4,0,2,20);
				node_adj[ND2][ND3] = compute_single_node(4,20,2,20);
				break;
			}
			case ND3: 
			{
				node_adj[ND3][ND2] = compute_single_node(4,20,2,20);
				node_adj[ND3][ND4] = compute_single_node(4,40,2,20);
				break;
			}
			case ND4: 
			{
				node_adj[ND4][ND3] = compute_single_node(4,40,2,20);
				break;
			}
	
		}
	}
}

int is_full()
{
	int i;
	int sum = 0;
	for(i=0;i<MAX_KEY_NODE;i++)
	{
		sum += area_node[i];
	}

	if(sum == MAX_PARKING_NUM)
	{
		return 1;
	}

	return 0;
}
