CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LIB = libcalc.so
SRC = lib/add.c lib/div.c lib/mul.c lib/sub.c
OBJ = $(SRC:.c=.o)
MAIN = main.c
EXECUTABLE = calculate

.PHONY: all clean run

all: $(EXECUTABLE)

$(EXECUTABLE): $(MAIN:.c=.o) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^

$(LIB): $(OBJ)
	$(CC) -shared -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

clean:
	rm -f $(OBJ) $(LIB) $(EXECUTABLE) $(MAIN:.c=.o) $(MAIN:.c=.o)

run:
	@LD_LIBRARY_PATH=$(CURDIR) ./$(EXECUTABLE)
