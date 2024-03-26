CC = gcc
CFLAGS = -Wall
OBJ = self_recovering_process.o
EXEC = srp

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

self_recovering_process.o:
	$(CC) $(CFLAGS) -c self_recovering_process.c

clean:
	rm -f $(OBJ) $(EXEC)

test: $(EXEC)
	./$(EXEC)
