/*
 * Dijkstra 单源最短距离
 */
#include <stdio.h>
#define N 20
#define INF 0x3f3f3f3f
                                                                                                        
int data[N][N]; // 邻接矩阵
int dist[N];    // 源节点到各节点的最短距离
int state[N];   // 各点访问状态
int n;          // 点个数
int start;      // 源节点
int fa[N];      // 父路径
                                                                                                        
int findmin(  )
{
    int i;
    int min=INF;
    int minnode=0;
    for( i=1;i<=n;i++ ){
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
    for( i=1;i<=n;i++ )
    {
      dist[i]=data[s][i];//算法开始前源节点到其他点的距离
      state[i]=0;
      fa[i]=s;
    }
                                                                                                        
    dist[s]=0;
    state[s]=1;
    fa[s]=0;
    done=1;
    //松弛
    while( done<n )//松弛n-1次
    {
        done++;
        node=findmin(  );
        if( node!=0 )
        {
            state[node]=1;
            for( i=1;i<=n;i++ ){//更新其他节点到源节点的距离
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
                                                                                                        
    printf( "%d-%d ",start,v );//源节点到目标节点
    len = 0;
    if( dist[v]==INF )//如果没有路，输出"no way"
        printf( ":can not arrive.");
    else{//如果有路，则输出路径
        // printf( "%d: ",dist[v] );
                                                                                                        
        while( v!=0 ){//将路径入栈
            s[++top]=v;
            v=fa[v];
        }
        path[len++]  = s[top--];                                                                      
        // printf( "%d",s[top--] );
                                                                                                        
        while( top>-1 ){//倒序输出
            v=s[top--];
            printf( " -> %d",v );
            path[len++] = v;
        }
    }
    return len;
}
/**
 * 功能：计算dijstra算法所需的邻接矩阵
 * 参数：parklots - 当前停车信息
 * 返回值；邻接矩阵结果保存在全局变量data中
 */
void compute_adj_matrix(char parklots[][])
{
    // TODO
    // 在这里加入路径权值的影响因素
    // 如：已占车位信息 - 停车数量作为该边权值
    //     主干道拥堵状况 - 如果该道路行进中车辆
    //                      数量达到上限，则认为
    //                      该边权值无穷大，即两
    //                      邻接点无法直达


}
/**
 * 功能：对dijstra计算出的dist作进一步考察，找到最终停车位
 * 参数：无
 * 返回值：stop - 最终停车节点
 */
int find_parking_place()
{
    int stop = 0;
    // TODO
    // 在这里加入多种考量最终停车位的方法，
    // 如：选择路径最短的节点 - 从dist中找出至最小的节点编号
    //     选择物理上最近的节点 - 出现到两个节点的距离是相同的，
    //                            由指示牌的物理编号大小选择最近

}
/**
 * 功能：由当前车库停车信息和车辆入库位置计算出该次停车的诱导方案
 * 参数：parklots - 当前停车信息
 *       start  - 起始位置
 *       path   - out类型，计算出来的最短路径
 *       len    - out类型，path长度
 * 返回值：计算结果保存在path和len中
 */
void get_shortest_path(char parklots[][], int start, int path[], int len)
{
    int stop = 0;
    
    // 生成新的邻接矩阵
    compute_adj_matrix(parklots);

    // dijstra 算法
    dijstra(start);

    // 确定终节点(最后停车位置)
    stop = find_parking_place();
    // 计算路径，放入path中
    len = find_path(stop,path)
    if (0 == len)
    {// 没有找到
        // 启用模糊算法，找另一停车位
        // TODO
    }
    else
    {
        // 返回
        return;
    }
}
int main(  )
{
    int i,j;
    int m;
    scanf( "%d",&n );
    for( i=1;i<=n;i++ ){
        for( j=1;j<=n;j++ ){
            scanf( "%d",&m );
            data[i][j]=(( i==j )?0:( m==0 ? INF : m) );//自身为零，没有路则为无穷大
        }
    }
    printf( "input the source point:" );
    scanf( "%d",&start );
                                                                                                        
    dijkstra( start );
                                                                                                        
    for( i=1;i<=n;i++ )//打印源节点到各节点的最短距离及路径
      if( i!=start )
        find_path( i );
                                                                                                        
    return 0;
}