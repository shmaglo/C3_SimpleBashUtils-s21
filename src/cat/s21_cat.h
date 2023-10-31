#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PATH_SIZE 500000
#define DATA_SIZE 500000

typedef struct format_struct {
    int flag_b;
    int flag_e;
    int flag_E;
    int flag_n;
    int flag_s;
    int flag_t;
    int flag_T;
    int flag_v;
    int flag_err;
    int total_docs;
} flags;

void get_info(const int argc, char * const argv[], flags *f, char *docs_list[]);
void handle_info(const char *pathL, const flags f);
void handle_all(const char *data, const ssize_t llen, const flags f, const int new);
void handle_symb(const char ch);
void output2(const char *data, const ssize_t llen, const flags f);
void print_tev(const char *data, const ssize_t llen, const flags f);

#endif  // SRC_CAT_S21_CAT_H_
