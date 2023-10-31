CC=gcc
CFLAGS=-Wall -Wextra -Werror

all: s21_cat test

s21_cat: s21_cat.c s21_cat.h
	$(CC) $(CFLAGS) s21_cat.c s21_cat.h -o s21_cat

test: s21_cat test.c test.h
	$(CC) $(CFLAGS) test.c test.h -o test
	./test

clean:
	rm -f s21_cat test result_1 result_2

rebuild: clean all