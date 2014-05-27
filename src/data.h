// 区域节点ID
#define A1 0
#define B1 1
#define C1 2
#define A2 3
#define B2 4
#define C2 5
#define A3 6
#define B3 7
#define C3 8

// 道路节点ID
#define NA1 0
#define NA2 1
#define NB1 2
#define NB2 3
#define NB3 4
#define NB4 5
#define NC1 6
#define NC2 7
#define NC3 8
#define NC4 9
#define ND1 10
#define ND2 11
#define ND3 12
#define ND4 13

// 停车位映射矩阵的大小
#define WIDTH_OF_PARKING_MAP 60	// 每个区域20车位共3区域
#define LENGTH_OF_PARKING_MAP 6 // 共6个横排区域
#define MAX_KEY_NODE 16			// 共十六个显示牌节点
#define MAX_AREA_NODE 9			// 共9个区
#define INF 0x3ffff				// 定义最大值
#define MAX_PARKING_NUM 360		// 最大停车位数量

// 全局变量
char parking_map[LENGTH_OF_PARKING_MAP][WIDTH_OF_PARKING_MAP];	// 停车位映射矩阵
int	area_node[MAX_AREA_NODE];	// 区域停车数量
int	node_adj[MAX_KEY_NODE][MAX_KEY_NODE]; // 节点的邻接矩阵

/* 更新area_node停车节点信息 */
void update_area_node();

/* 计算指定node的值 */
int compute_single_node(int startr,int startc, int rlen, int clen);

/* 设置停车位信息 */
void set_parking_map_node(int isparking, int row, int cor);

/* 更新node_adj矩阵 */
void update_node_adj();

/* 检查车库是否已满 */
int is_full();
