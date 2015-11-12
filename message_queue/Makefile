SOURCE = dropone.c takeone.c
EXE = dropone takeone
CFLAGS = -Wall 
LIBS = -lrt
OBJS = dropone.o takeone.o
CC = gcc

all: $(EXE)

dropone: dropone.o
	$(CC) dropone.o -o dropone $(LIBS)

takeone: takeone.o
	$(CC) takeone.o -o takeone $(LIBS)

dropone.o: dropone.c
	$(CC) $(CFLAGS) -c dropone.c

takeone.o: takeone.c
	$(CC) $(CFLAGS) -c takeone.c

clean:
	rm -f *.o $(EXE)
