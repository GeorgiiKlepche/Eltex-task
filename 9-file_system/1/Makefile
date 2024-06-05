TARGET = main

CC = gcc
CFLAGS = -Wall

SRCS = main.c

OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f main *.o

style:
	clang-format -style=Google -i *.c