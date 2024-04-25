CC = gcc
CFLAGS = -w
OBJ = server.o client.o
EXEC = server client

all: $(EXEC)

server: server.o
	$(CC) $(CFLAGS) -o server server.o

client: client.o
	$(CC) $(CFLAGS) -o client client.o

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

clean:
	rm -f $(OBJ) $(EXEC)