SOURCE = simple_mutex.c
EXE = run
CFLAGS = -Wall 
LIBS = -lpthread  
OBJS = simple_mutex.o
CC = gcc

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE) $(LIBS)

$(OBJS): $(SOURCE)
	$(CC) $(CFLAGS) -c $(SOURCE)

clean:
	rm -f *.o $(EXE)
