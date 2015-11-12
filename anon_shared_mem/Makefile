SOURCE = shm_anon_bsd.c
EXE = run
CFLAGS = -Wall 
LIBS = -lpthread -lrt
OBJS = shm_anon_bsd.o
CC = gcc

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE) $(LIBS)

$(OBJS): $(SOURCE)
	$(CC) $(CFLAGS) -c $(SOURCE)

clean:
	rm -f *.o $(EXE)
