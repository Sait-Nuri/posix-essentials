SOURCE = sig_purgechilds.c
EXE = run
CFLAGS = -Wall 
LIBS = 
OBJS = sig_purgechilds.o
CC = gcc

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE) $(LIBS)

$(OBJS): $(SOURCE)
	$(CC) $(CFLAGS) -c $(SOURCE)

clean:
	rm -f *.o $(EXE)
