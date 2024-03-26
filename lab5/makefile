CC = gcc
CFLAGS = -Wall
OBJ = threaded_sort.o
EXEC = threaded_sort

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

main.o:
	$(CC) $(CFLAGS) -c threaded_sort.c

clean:
	rm -f $(OBJ) $(EXEC)

test: $(EXEC)
	./$(EXEC)
