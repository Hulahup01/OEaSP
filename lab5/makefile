CC = gcc
CFLAGS = -Wall
OBJ = threaded_merge_sort.o
EXEC = threaded_merge_sort

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

main.o:
	$(CC) $(CFLAGS) -c threaded_merge_sort.c

clean:
	rm -f $(OBJ) $(EXEC)

test: $(EXEC)
	./$(EXEC)
