CC=gcc
SRC=src
OUT=build

.PHONY: all clean

all: $(OUT)/zopt

clean:
	rm build/*

$(OUT)/zopt: $(OUT)/zopt.o
	$(CC) -o $@ $^

$(OUT)/%.o: $(SRC)/%.c
	$(CC) -c -o $@ $^
