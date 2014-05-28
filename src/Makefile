
MAKEFILE	= Makefile
cc			= gcc

driver: dijkstra.o data.o datetime.o sql.o driver.o
	cc -o driver driver.o dijkstra.o data.o datetime.o $(mysql_config --libs)
driver.o: driver.c 
	cc -c driver.c
dijkstra.o: dijkstra.c dijkstra.h
	cc -c dijkstra.c dijkstra.h
data.o: data.c data.h
	cc -c data.c data.h
sql.o: sql.c sql.h
	cc -c sql.h $(mysql_config --cflags) sql.c $(mysql_config --libs) 
datetime.o: datetime.c datetime.h
	cc -c datetime.c datetime.h

demo:
	$(MAKE) -f Makefile_demo
show:
	sed -n '1,9p' ./data/data
	./demo_algorithm 
	cd ./data/ && sh showmap.sh && cd ..
clean:
	rm -f *.o
	rm -f *.h.gch
