CC = gcc
CFLAGS = -Wall

SRCS = task1.c task2.c task3.c

OBJS = $(SRCS:.c=)

all: $(OBJS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f main *.o task1 task2 task3

style:
	clang-format -style=Google -i *.c