#!bin/sh

# 打印停车信息，和停车位信息，方便检测算法正确性

#gcc mapmaker.c -o mapmaker

#echo 100 | ./datamaker > data


#sh ./showmap.sh

echo "停车位图:"
sed -n '1,9p' data
