#!bin/sh

row=0
FILE='./map/map2'
a=(0)
i=1

while read line
do
	a[$i]=`echo $line | cut -d ' ' -f 2`
	((i++))
done < node_adj.txt

#cat $(FILE) | grep '11'  && echo 1
sed -n '1p' $FILE | echo -ne "       \033[32m\033[05m${a[1]}             ${a[2]}              ${a[3]}\n\033[0m"
sed -n '2p' $FILE 
sed -n '3p' $FILE 
sed -n '4p' $FILE | echo -ne "|   |        | |\033[32m\033[05m${a[4]}\033[0m |        | |\033[32m\033[05m${a[5]}\033[0m |        | |\n"
sed -n '5p' $FILE 
sed -n '6p' $FILE | echo -ne "|       \033[32m\033[05m${a[6]}\033[0m     |       \033[32m\033[05m${a[7]}\033[0m     |       \033[32m\033[05m${a[8]}\033[0m     |\n"
sed -n '7p' $FILE 
sed -n '8p' $FILE 
sed -n '9p' $FILE 
sed -n '10p' $FILE | echo -ne "|\033[32m\033[05m${a[9]}\033[0m |________| |\033[32m\033[05m${a[10]}\033[0m |________| |\033[32m\033[05m${a[11]}\033[0m |________| |\033[32m\033[05m${a[12]}\033[0m\n"
sed -n '11p' $FILE 
sed -n '12p' $FILE 
sed -n '13p' $FILE | echo -ne "|       \033[32m\033[05m${a[13]}\033[0m     |       \033[32m\033[05m${a[14]}\033[0m     |       \033[32m\033[05m${a[15]}\033[0m     |\n"
sed -n '14p' $FILE 
sed -n '15p' $FILE 
sed -n '16p' $FILE 
sed -n '17p' $FILE | echo -ne "|\033[32m\033[05m${a[16]}\033[0m |________| |\033[32m\033[05m${a[17]}\033[0m |________| |\033[32m\033[05m${a[18]}\033[0m |________| |\033[32m\033[05m${a[19]}\033[0m\n"
sed -n '18p' $FILE 
sed -n '19p' $FILE 
sed -n '20p' $FILE | echo -ne "|       \033[32m\033[05m${a[20]}\033[0m     |       \033[32m\033[05m${a[21]}\033[0m     |       \033[32m\033[05m${a[22]}\033[0m     |\n"
sed -n '21p' $FILE 
sed -n '22p' $FILE 
sed -n '23p' $FILE 
sed -n '24p' $FILE 
sed -n '25p' $FILE 
sed -n '26p' $FILE 
