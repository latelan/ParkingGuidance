#!bin/sh

if [ "$1" == "" ]; then
	echo "Need one argument."
	exit
fi

echo $1 | ./datamaker > data
