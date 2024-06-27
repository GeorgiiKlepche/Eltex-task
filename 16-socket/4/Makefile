CC = gcc
CFLAGS = -g
TARGETS = server ip_rawsock ether_rawsock udp_rawsock

all: $(TARGETS)

server: server.c functions.c functions.h
	$(CC) $(CFLAGS) server.c functions.c -o server

ip_rawsock: ip_rawsock.c functions.c functions.h
	$(CC) $(CFLAGS) ip_rawsock.c functions.c -o ip_rawsock

ether_rawsock: ether_rawsock.c functions.c functions.h
	$(CC) $(CFLAGS) ether_rawsock.c functions.c -o ether_rawsock

udp_rawsock: udp_rawsock.c functions.c functions.h
	$(CC) $(CFLAGS) udp_rawsock.c functions.c -o udp_rawsock

clean:
	rm -f $(TARGETS)

style:
	clang-format -style=Google -i *.c *.h