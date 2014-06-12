#define N 20 // 最大节点数
#define LIMITS_CASE_ONE_PARKING 15 // 情况1下停车候选区最大已停车数
#define MIN_CASE_ONE_PARKING 5 // 区域最少停车5辆

int data[N][N]; // 邻接矩阵
int dist[N];    // 源节点到各节点的最短距离
int state[N];   // 各点访问状态
int n;          // 点个数
int start;      // 源节点
int fa[N];      // 父路径

/* 打印各区域停车数量area_node */
void print_area_node(int array[], int len);

/* 保存节点邻接矩阵node_adj到./data/node_adj.txt文件中 */
void save_node_adj(int array[][20],int row,int cor);

/* 打印节点邻接矩阵node_adj */
void print_node_adj(int array[][20],int row,int cor);

/* 打印停车数据parking_map */
void print_parking_map(char array[][60],int row,int cor);

/* 打印最短距离dist */
void print_dist(int array[],int len);

/* 获取节点对应名称 */
char *node_id(int nodeid);

/* 获取区域节点对应名称 */
char *area_node_id(int id);

int min(int a,int b);

/* 计算出发点到区域areaid的最短距离 */
int area_shorest_dist(int areaid);

/* 求出区域areaid附近两个节点中路径最短的那个节点 */
int node_to_area(int areaid);

int findmin();

/* dijkstra算法过程*/
void dijkstra(int s);

/* 找出源节点到目的节点的路径 */
int find_path(int v, int path[]);

/* 计算dijstra算法所需的邻接矩阵 */
void compute_adj_matrix();

/* 对dijstra计算出的dist作进一步考察，找到最终停车位置 */
int find_parking_place(int start);

/* 停车诱导路径的计算函数 */
void get_shortest_path(int start, int path[], int len);

/* 计算最终停车位置（区域）*/
int get_final_stop(int start, int *stop);

/* 按指定方式设置parking_map */
void set_parking_map(const char *path_parking_data, int reset_tag);

