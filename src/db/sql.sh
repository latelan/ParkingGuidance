#!bin/sh

gcc -g -c  -o sql $(mysql_config --cflags) sql.c $(mysql_config --libs) datetime.o
