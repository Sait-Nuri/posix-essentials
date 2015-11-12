SOURCE = semaphore.c
EXE = run
CFLAGS = -Wall 
LIBS = -lpthread
OBJS = semaphore.o
CC = gcc

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE) $(LIBS)

$(OBJS): $(SOURCE)
	$(CC) $(CFLAGS) -c $(SOURCE)

clean:
	rm -f *.o $(EXE)
