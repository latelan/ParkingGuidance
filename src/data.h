// 停车位映射矩阵的大小
#define WIDTH_OF_PARKING_MAP 61	//每个区域20车位共3区域，使用[1...61]
#define LENGTH_OF_PARKING_MAP 6 //共6个横排区域

// 关键节点矩阵的大小
#define COR_LENGTH 4 
#define ROW_LENGTH 5

char parking_map[LENGTH_OF_PARKING_MAP][WIDTH_OF_PARKINGL_MAP];	// 停车位映射举证
int  key_node[ROW_LENGTH][COR_LENGTH];	// 关键节点矩阵,值为已占车位数

/* 更新key_node矩阵 */
int update_key_node();

/* 计算指定node的值 */
int compute_single_node(int nrow, int ncor);


