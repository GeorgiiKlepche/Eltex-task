CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LIB = libcalc.a
SRC = lib/add.c lib/div.c lib/mul.c lib/sub.c
OBJ = $(SRC:.c=.o)
MAIN = main.c
EXECUTABLE = calculate

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ) $(MAIN:.c=.o) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^

$(LIB): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(LIB) $(EXECUTABLE) $(MAIN:.c=.o) $(MAIN:.c=.o)

style:
	clang-format -style=Google -i $(SRC) $(MAIN) include/*.h