SOURCE = udpserv.c unixcli.c
EXE = udpserv unixcli
CFLAGS = -Wall 
LIBS = 
OBJS = udpserv.o unixcli.o
CC = gcc

all: $(EXE)

udpserv: udpserv.o
	$(CC) udpserv.o -o udpserv $(LIBS)

unixcli: unixcli.o
	$(CC) unixcli.o -o unixcli $(LIBS)

udpserv.o: udpserv.c
	$(CC) $(CFLAGS) -c udpserv.c

unixcli.o: unixcli.c
	$(CC) $(CFLAGS) -c unixcli.c

clean:
	rm -f *.o $(EXE)
