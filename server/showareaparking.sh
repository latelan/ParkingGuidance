#!bin/sh

# 打印停车场区域停车信息

PATH_MAP=./map/map_2
PATH_PARKING_AREA=./parking_area.txt

a=(0)
i=1

while read line
do
	a[$i]=$line
	((i++))
done < $PATH_PARKING_AREA

echo "停车场区域停车信息统计："

sed -n '1p' $PATH_MAP 
sed -n '2p' $PATH_MAP 
sed -n '3p' $PATH_MAP 
sed -n '4p' $PATH_MAP 
sed -n '5p' $PATH_MAP 
sed -n '6p' $PATH_MAP 
sed -n '7p' $PATH_MAP | echo -ne "|  |   \033[32m${a[1]}\033[0m   |     |   \033[32m${a[2]}\033[0m   |     |   \033[32m${a[3]}\033[0m   |  |\n"
sed -n '8p' $PATH_MAP 
sed -n '9p' $PATH_MAP
sed -n '10p' $PATH_MAP 
sed -n '11p' $PATH_MAP 
sed -n '12p' $PATH_MAP 
sed -n '13p' $PATH_MAP 
sed -n '14p' $PATH_MAP 
sed -n '15p' $PATH_MAP | echo -ne "|  |   \033[32m${a[4]}\033[0m   |     |   \033[32m${a[5]}\033[0m   |     |   \033[32m${a[6]}\033[0m   |  |\n"
sed -n '16p' $PATH_MAP 
sed -n '17p' $PATH_MAP 
sed -n '18p' $PATH_MAP 
sed -n '19p' $PATH_MAP 
sed -n '20p' $PATH_MAP 
sed -n '21p' $PATH_MAP 
sed -n '22p' $PATH_MAP 
sed -n '23p' $PATH_MAP | echo -ne "|  |   \033[32m${a[7]}\033[0m   |     |   \033[32m${a[8]}\033[0m   |     |   \033[32m${a[9]}\033[0m   |  |\n"
sed -n '24p' $PATH_MAP
sed -n '25p' $PATH_MAP 
sed -n '26p' $PATH_MAP 
sed -n '27p' $PATH_MAP 
sed -n '28p' $PATH_MAP 
