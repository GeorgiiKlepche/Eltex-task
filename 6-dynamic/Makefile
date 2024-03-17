CC = gcc
CFLAGS = -Wall -Wextra

SRCS = main.c handbook.c
OBJS = $(SRCS:.c=.o)
TARGET = handbook

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

style:
	clang-format -style=Google -i *.c *.h