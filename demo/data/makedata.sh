#!bin/sh

PARKING_MAP=parking_map.txt
PARKING_AREA=parking_area.txt

if [ "$1" == "" ]; then
	echo "Need one argument."
	exit
fi

echo $1 | ./datamaker > data
sed -n '1,9p' data > $PARKING_MAP
sed -n 's/^\([0-9]\{2\}\) \([0-9]\{2\}\) \([0-9]\{2\}\)/\1\n\2\n\3/p' data >  $PARKING_AREA

cat parking_map.txt
cat parking_area.txt
