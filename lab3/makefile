CC = gcc
CFLAGS = -Wall
OBJ = main.o text_utils.o
EXEC = invert_filter

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c text_utils.h
	$(CC) $(CFLAGS) -c main.c

text_utils.o: text_utils.c text_utils.h
	$(CC) $(CFLAGS) -c text_utils.c

clean:
	rm -f $(OBJ) $(EXEC)

test: $(EXEC)
	./$(EXEC) input.txt output.txt
