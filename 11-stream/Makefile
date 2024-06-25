TARGET = main

CC = gcc

SRCS = main.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lcurses

clean:
	rm -f $(TARGET) *.o

style:
	clang-format -style=Google -i *.c