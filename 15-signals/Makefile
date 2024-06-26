CC = gcc
CFLAGS = -Wall

SRCS = 1-task.c 2-task.c 3-task.c

OBJS = $(SRCS:.c=)

all: $(OBJS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf main *.o 1-task 2-task 3-task

style:
	clang-format -style=Google -i *.c