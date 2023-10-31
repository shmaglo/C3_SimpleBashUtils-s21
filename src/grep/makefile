CC=gcc
CFLAGS=-Wall -Wextra -Werror

all: s21_grep test

s21_grep: s21_grep.c s21_grep.h
	$(CC) $(CFLAGS) s21_grep.c s21_grep.h -o s21_grep

test: s21_grep grep.sh
	./grep.sh

clean:
	rm -f s21_grep log.txt

rebuild: clean all