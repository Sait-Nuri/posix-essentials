SOURCE = shm_msgclient.c shm_msgserver.c
EXE = shm_msgclient shm_msgserver
CFLAGS = -Wall 
LIBS = -lrt
OBJS = shm_msgclient.o shm_msgserver.o
CC = gcc

all: $(EXE)

shm_msgclient: shm_msgclient.o
	$(CC) shm_msgclient.o -o shm_msgclient $(LIBS)

shm_msgserver: shm_msgserver.o
	$(CC) shm_msgserver.o -o shm_msgserver $(LIBS)

shm_msgclient.o: shm_msgclient.c
	$(CC) $(CFLAGS) -c shm_msgclient.c

shm_msgserver.o: shm_msgserver.c
	$(CC) $(CFLAGS) -c shm_msgserver.c

clean:
	rm -f *.o $(EXE)
