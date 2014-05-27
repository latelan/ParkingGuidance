parkingguidence: data.o dijkstra.o
	cc -o parkingquidence data.o dijkstra.o
data.o: data.c data.h
	cc -c  data.c
dijstra.o: dijkstra.h dijkstra.c
	cc -c  dijkstra.c

.PHONY : clean
clean:
	-rm -f *.o
