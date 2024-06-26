CC = gcc
CFLAGS = -Wall

SRCS = pool_client pool_server queue_client queue_client simple_client simple_server tcp_udp_client tcp_udp_server

OBJS = $(SRCS:.c=)

all: $(OBJS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(SRCS)

style:
	clang-format -style=Google -i *.c *.h