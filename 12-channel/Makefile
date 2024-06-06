CC = gcc
CFLAGS = -Wall

SRCS = task1.c task3.c 

OBJS = $(SRCS:.c=)

all: $(OBJS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf main *.o task1 task3

style:
	clang-format -style=Google -i *.c