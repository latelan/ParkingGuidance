/*
 * Dijkstra 单源最短距离
 */
#include <stdio.h>
#include <string.h>
#include "dijkstra.h"
#include "data.h"

// test 
void print_area_node(int array[], int len)
{
	int i;

	printf("区域停车统计：\n");
	for(i=0;i<len;i++)
	{
		switch(i)
		{
			case 0: printf("A1 B1 C1  "); break;
			case 3: printf("\nA2 B2 C3  "); break;
			case 6: printf("\nA3 B3 C3  "); break;
		}

//		if(i%3 == 0 && i != 0)
//			printf("\n");
		printf("%02d ",array[i]);
	}
	printf("\n");
}
void save_node_adj(int array[][20],int row,int cor)
{
	FILE *fp;
	fp = fopen("./data/node_adj.txt","w");
	if(!fp)
	{
		printf("Can not create node_adj.txt.\n");
	}

	fprintf(fp,"01 %02d\n",array[NA1][NB1]);
	fprintf(fp,"02 %02d\n",array[NA1][NA2]);
	fprintf(fp,"03 %02d\n",array[NA2][NB4]);
	fprintf(fp,"04 %02d\n",array[NA1][NB2]);
	fprintf(fp,"05 %02d\n",array[NA2][NB3]);
	fprintf(fp,"06 %02d\n",array[NB1][NB2]);
	fprintf(fp,"07 %02d\n",array[NB2][NB3]);
	fprintf(fp,"08 %02d\n",array[NB3][NB4]);
	fprintf(fp,"09 %02d\n",array[NB1][NC1]);
	fprintf(fp,"10 %02d\n",array[NB2][NC2]);
	fprintf(fp,"11 %02d\n",array[NB3][NC3]);
	fprintf(fp,"12 %02d\n",array[NB4][NC4]);
	fprintf(fp,"13 %02d\n",array[NC1][NC2]);
	fprintf(fp,"14 %02d\n",array[NC2][NC3]);
	fprintf(fp,"15 %02d\n",array[NC3][NC4]);
	fprintf(fp,"16 %02d\n",array[NC1][ND1]);
	fprintf(fp,"17 %02d\n",array[NC2][ND2]);
	fprintf(fp,"18 %02d\n",array[NC3][ND3]);
	fprintf(fp,"19 %02d\n",array[NC4][ND4]);
	fprintf(fp,"20 %02d\n",array[ND1][ND2]);
	fprintf(fp,"21 %02d\n",array[ND2][ND3]);
	fprintf(fp,"22 %02d\n",array[ND3][ND4]);
//	fprintf(fp,"23 %02d\n",array[ND2][NE1]);
//	fprintf(fp,"24 %d\n",array[ND3][NE2]);

	fclose(fp);
}
void print_node_adj(int array[][20],int row,int cor)
{
	int i,j;

	save_node_adj(array,row,cor);
	printf("节点邻接矩阵:\n");
	printf("   A1 A2 B1 B2 B3 B4 C1 C2 C3 C4 D1 D2 D3 D4\n");
	for(i=0;i<row;i++)
	{
		switch(i)
		{
			case 0: printf("A1 "); break;
			case 1: printf("A2 "); break;
			case 2: printf("B1 "); break;
			case 3: printf("B2 "); break;
			case 4: printf("B3 "); break;
			case 5: printf("B4 "); break;
			case 6: printf("C1 "); break;
			case 7: printf("C2 "); break;
			case 8: printf("C3 "); break;
			case 9: printf("C4 "); break;
			case 10: printf("D1 "); break;
			case 11: printf("D2 "); break;
			case 12: printf("D3 "); break;
			case 13: printf("D4 "); break;
		}
		for(j=0;j<cor;j++)
		{
			if(array[i][j] == INF)
				printf("-- ");
			else
				printf("%02d ",array[i][j]);
		}
		printf("\n");
	}

	return;
}
void print_char_array(char array[][60], int row, int cor)
{
	int i,j;
	for(i=0;i<row;i++)
	{
		for(j=0;j<cor;j++)
		{
			printf("%c",array[i][j]);
		}
		printf("\n");
	}
	return;
}
void print_dist(int array[], int len)
{
	int i;
	printf("%s到各节点最短路径距离：\n",node_id(start));
	printf("A1 A2 B1 B2 B3 B4 C1 C2 C3 C4 D1 D2 D3 D4\n");
	for(i=0;i<len;i++)
	{
		printf("%02d ",dist[i]);
	}
	printf("\n");
}
char *node_id(int id)
{
	switch(id)
	{
		case 0: return "A1";
		case 1: return "A2";   
		case 2: return "B1";   
		case 3: return "B2";   
		case 4: return "B3";   
		case 5: return "B4";   
		case 6: return "C1";   
		case 7: return "C2";   
		case 8: return "C3";   
		case 9: return "C4";   
		case 10: return "D1";   
		case 11: return "D2";   
		case 12: return "D3";   
		case 13: return "D4";
		case 14: return "E1";
		case 15: return "E2";
		default:return NULL;
	}
}
char *area_node_id(int id)
{
	switch(id)
	{
		case 0: return "A1";
		case 1: return "B1";
		case 2: return "C1";
		case 3: return "A2";
		case 4: return "B2";
		case 5: return "C2";
		case 6: return "A3";
		case 7: return "B3";
		case 8: return "C3";
		default: return NULL;
	}
}
int min(int a,int b)
{
	return a >b ? b:a;
}
int area_shortest_dist(int areaid)
{
	switch(areaid)
	{
		case 0: return min(dist[NB1],dist[NB2]);
		case 1: return min(dist[NB2],dist[NB3]);
		case 2: return min(dist[NB3],dist[NB4]);
		case 3: return min(dist[NC1],dist[NC2]);
		case 4: return min(dist[NC2],dist[NC3]);
		case 5: return min(dist[NC3],dist[NC4]);
		case 6: return min(dist[ND1],dist[ND2]);
		case 7: return min(dist[ND2],dist[ND3]);
		case 8: return min(dist[ND3],dist[ND4]);
		default: return INF;
	}
}
int node_to_area(int areaid)
{
	int idr,idl;
	switch(areaid)
	{
		case 0: idl = NB1; idr = NB2; break;
		case 1: idl = NB2; idr = NB3; break;
		case 2: idl = NB3; idr = NB4; break;
		case 3: idl = NC1; idr = NC2; break;
		case 4: idl = NC2; idr = NC3; break;
		case 5: idl = NC3; idr = NC4; break;
		case 6: idl = ND1; idr = ND2; break;
		case 7: idl = ND2; idr = ND3; break;
		case 8: idl = ND3; idr = ND4; break;
		default: return -1;
	}
	
	if(dist[idl] > dist[idr])
	{
		return idl;
	}
	else 
	{
		return idr;
	}
}
int findmin(  )
{
    int i;
    int min=INF;
    int minnode=-1;
    for( i=0;i<n;i++ ){
      if( min>dist[i] && ( !state[i] ) ){
          min=dist[i];
          minnode=i;
      }
    }
    return minnode;
}
/**
 * 功能：dijistra算法过程
 * 参数：s - 源节点
 * 返回值：到各节点的最短路径，结果保存在dist中
 */
void dijkstra( int s )
{
    int i,j;
    int done;
    int node;
    //初始化
    for( i=0;i<n;i++ )
    {
      dist[i]=data[s][i];//算法开始前源节点到其他点的距离
      state[i]=0;
      fa[i]=s;
    }
     
	// test
	//printf("Debug in dijkstra function.\n");
	//print_dist(dist,14);

    dist[s]=0;
    state[s]=1;
    fa[s]=-1;
    done=1;
    //松弛
    while( done<n )//松弛n-1次
    {
        done++;
        node = findmin();
        if( node!=-1 )
        {
            state[node]=1;
            for( i=0;i<n;i++ ){//更新其他节点到源节点的距离
                if( dist[i]>dist[node]+data[node][i] && ( !state[i] ) ){
                  dist[i]=dist[node]+data[node][i];
                  fa[i]=node;//记录父节点
                }
            }
        }
        else break;
    }
}
/**
 * 功能：找出源节点到节点v的路径
 * 参数：v - 终结点
 * 返回值；len - 路径长度，0表示没有找到
 */
int find_path( int v, int path[])
{
    int i;
    int len;
    int s[N],top,p;
    top=-1;
    for( i=0;i<n;i++ )
      s[i]=0;
                                                                                                        
    printf( "%s-%s: %s -> ",node_id(start),node_id(v),node_id(start) );//源节点到目标节点
    len = 0;
    if( dist[v]==INF )//如果没有路，输出"no way"
        printf( ":can not arrive.");
    else{//如果有路，则输出路径
        // printf( "%d: ",dist[v] );
                                                                                                        
        while( v != -1 ){//将路径入栈
            s[++top]=v;
            v=fa[v];
        }
        path[len++]  = s[top--];                                                                      
        printf( "%s",node_id(s[top--]) );
                                                                                                        
        while( top>-1 ){//倒序输出
            v=s[top--];
            printf( " -> %s",node_id(v) );
            path[len++] = v;
        }
    }
	printf("\n");
    return len;
}
/**
 * 功能：计算dijkstra算法所需的邻接矩阵
 * 参数：无
 * 返回值；邻接矩阵结果保存在全局变量data中
 */
void compute_adj_matrix()
{
    // TODO
    // 在这里加入路径权值的影响因素
    // 如：已占车位信息 - 停车数量作为该边权值
    //     主干道拥堵状况 - 如果该道路行进中车辆
    //                      数量达到上限，则认为
    //                      该边权值无穷大，即两
    //                      邻接点无法直达

    int i,j;
    n = MAX_KEY_NODE - 2; // 设置节点个数,去掉两个出口节点
	
	update_node_adj();
	update_area_node();

    // 将node_adj 拷贝到 data中
    for (i = 0; i < MAX_KEY_NODE - 2; ++i)
    {
        for (j = 0; j < MAX_KEY_NODE - 2; ++j)
        {
            data[i][j] = node_adj[i][j];
        }
    }

    // 对部分边权做修改
    //TODO
}
/**
 * 功能：对dijkstra计算出的dist作进一步考察，找到最终停车位
 * 参数：无
 * 返回值：stop - 最终停车节点
 */
int find_parking_place(int start)
{
    int stop = 0;
    // TODO
    // 在这里加入多种考量最终停车位的方法，
    // 如：选择路径最短的节点 - 从dist中找出至最小的节点编号
    //     选择物理上最近的节点 - 出现到两个节点的距离是相同的，
    //                            由指示牌的物理编号大小选择最近
	int i;
	int min = INF;
	int tmin;
	int allparking = 0;
	
	// 统计全车停车数量
	for(i=0;i<MAX_AREA_NODE;i++)
	{
		allparking += area_node[i];
	}
	
	if(allparking <= 135) 
	{// 情况1,停车场较空时
		// 统计停车数小于15辆的最少区域
		for(i=0;i<MAX_AREA_NODE;i++)
		{
			if(area_node[i] < LIMITS_CASE_ONE_PARKING
					&& min > area_node[i])
			{
				if(area_node[i] < MIN_CASE_ONE_PARKING)
				{// 如果该区域少于5 MIN_CASE_ONE_PARKING 辆车，直接选择
					stop = i;
					return stop;
				}
				stop = i;
				min = area_node[i];
			}
		}

		for(i=0;i<MAX_AREA_NODE;i++)
		{
			if(stop != i && area_node[stop] == area_node[i])
			{
				if(area_shortest_dist(stop)> area_shortest_dist(i))
				{
					stop = i;
				}
			}
		}

		return stop;
	}
	else if(allparking <= 320)
	{// 情况2,停车场均匀分布
		// 找到已停车数最少的区域
		for(i=0;i<MAX_AREA_NODE;i++)
		{
			if(min > area_node[i])
			{
				stop = i;
				min = area_node[i];
			}
		}
		// 如果出现多个最少区域，则选择路径最短的区域
		for(i=0;i<MAX_AREA_NODE;i++)
		{
			if(stop != i && area_node[stop] == area_node[i])
			{
				if(area_shortest_dist(stop)> area_shortest_dist(i))
				{
					stop = i;
				}
			}
		}

		return stop;
	}
	else
	{// 情况3,停车场较满
		tmin = 0;
		min = start;
		for(i=start;i<MAX_AREA_NODE;)
		{
			// 记录靠近入口的两个区的最少停车区域,如从A1入库，则考察A区和B区
			tmin = area_node[i] >= area_node[i+1] ? i+1 : i;
			min = area_node[min] > area_node[tmin] ? tmin : min;
			i += 3;
		}

		stop = min;

		return stop;
	}

	return 0;
}
/**
 * 功能：由当前车库停车信息和车辆入库位置计算出该次停车的诱导方案
 * 参数：
 *       start  - 起始位置
 *       path   - out类型，计算出来的最短路径
 *       len    - out类型，path长度
 * 返回值：计算结果保存在path和len中
 */
void get_shortest_path(int start, int path[], int len)
{
    int stop = 0;
   	int i; 

    // 生成新的邻接矩阵
    compute_adj_matrix();

	// test 
	//print_char_array(parking_map,6,60);
	print_node_adj(data,MAX_KEY_NODE-2,MAX_KEY_NODE-2);
	print_area_node(area_node,9);


	n = MAX_KEY_NODE - 2; // 设置dijkstra算法所需节点数

    // dijkstra 算法
    dijkstra(start);
	
	// test
	print_dist(dist,14);
	
    // 确定终节点(最后停车位置)
    stop = find_parking_place(start);

	printf("建议停车区域: %s\n",area_node_id(stop));
	printf("该区停车数量: %d\n",area_node[stop]);

    //test
	/*
    for( i=0;i<n;i++ )//打印源节点到各节点的最短距离及路径
      if( i!=start )
	  {
        find_path( i,path);
	  }
	*/

	// test
	//printf("诱导路线: ");
    // 计算路径，放入path中
    //len = find_path(node_to_area(stop),path);
    //if (0 == len)
    //{// 没有找到
        // 启用模糊算法，找另一停车位
        // TODO
	//	printf("Find it.\n");
    //}
    //else
    //{
        // 返回
    //    return;
    //}
}

/**
 * 功能：确定最终停车区域，结果放在stop中
 * 参数：start - 入口位置A1 or A2
 * 		 stop  - 停车位置
 * 返回值：正确计算出返回1，否则返回0
 */
int get_final_stop(int start, int *stop)
{
	compute_adj_matrix();

	n = MAX_KEY_NODE - 2; // 去掉两个出口节点

	dijkstra(start);

	*stop = find_parking_place(start);

	if(*stop >= 0 && *stop <= 9)
		return 1; // 找到返回1
	else return 0;

}
/**
 * 功能：按指定方式设置parking_map
 * 参数：path_parking_data - 停车数据文件路径
 * 		 reset_tag - 重设标志，为1时按指定数据文件设置，为0时全设为0
 * 返回值：无
 */
void set_parking_map(const char *path_parking_data, int reset_tag)
{
	int i,j;
	FILE *fp;
	char ch;
	char abs_path[200];

	// 获取path的绝对路径
	// TODO
	strcpy(abs_path,path_parking_data);		
	if(reset_tag == 1)
	{// 按path_parking_data所指文件初始化parking_map
		fp = fopen(abs_path,"r");
		if(!fp)
		{
			printf("Can not open file %s.\n",path_parking_data);
			return ;
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

		return;
	}	

	// 将parking_map设置为0
	for(i=0;i<LENGTH_OF_PARKING_MAP;i++)
	{
		for(j=0;j<WIDTH_OF_PARKING_MAP;j++)
		{
			parking_map[i][j] = 0;
		}
	}

	return;
}
/*
int main(  )
{
    int path[20];
	int i,j;
    FILE *fp = NULL;
	char ch;
	int stop;
	fp = fopen("./data/parking_lots.txt","r");
	if(!fp)
	{
		printf("Can not open parking_lots.txt.\n");
		return 0;
	}
	
	// test.read parking data
	for(i=0; i<6; i++)
	{
		for(j=0; j<60; j++)
		{
			fscanf(fp,"%c",&parking_map[i][j]);
		}
		fscanf(fp,"%c",&ch);
	}
	fclose(fp);

	// show
	// get start
	printf("input start point(1/2):");
	scanf("%d",&start);
	
	start = start - 1; // set start point
	
	// dijkstra algorithm
  // 	get_shortest_path(start,path,20);
  get_final_stop(start,&stop);

  printf("Final parking: %s\n",area_node_id(stop));
                                                                                                        
    return 0;
}
*/
