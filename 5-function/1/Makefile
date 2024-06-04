TARGET = main

CC = gcc
CFLAGS = -Wall

SRCS = main.c

OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@