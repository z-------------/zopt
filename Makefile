CC=gcc
SRC=src
OUT=build
TEST=test

.PHONY: default clean test

default: $(OUT)/zopt.o

$(OUT)/test: $(OUT)/test.o $(OUT)/zopt.o

$(OUT)/%.o: $(SRC)/%.c
	$(CC) -c -o $@ $^

test: $(OUT)/test
	$(OUT)/test

clean:
	rm build/*
