CC = gcc
CFLAGS = -Wall

SRCS = TCPinet_client.c TCPinet_server.c TCPlocal_client.c TCPlocal_server.c UDPinet_client.c UDPinet_server.c UDPlocal_client.c UDPlocal_server.c

OBJS = $(SRCS:.c=)

all: $(OBJS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf TCPinet_client TCPinet_server TCPlocal_client TCPlocal_server UDPinet_client UDPinet_server UDPlocal_client UDPlocal_server

style:
	clang-format -style=Google -i *.c