CC	= gcc

sql.o: sql.c sql.h datetime.o
	cc -c sql.h $(mysql_config --cflags) sql.c  $(mysql_config --libs) 
datetime.o: datetime.c datetime.h
	cc -c datetime.c datetime.h 

.PHONY: clean
clean:
	rm -f *.o
