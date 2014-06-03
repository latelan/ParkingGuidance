
# C_SOURCES = $(shell find .. -name "dijkstra.c")
demo: dijkstra.o data.o 
	cc demo_algorithm.c dijkstra.o data.o -I ../include/ -o demo
dijkstra.o: ../guidence/dijkstra.c ../include/dijkstra.h
	cc -c ../guidence/dijkstra.c -I ../include/
data.o: ../guidence/data.c ../include/data.h
	cc -c ../guidence/data.c -I ../include/ 

.PHONY:clean
clean:
	rm -f *.o
