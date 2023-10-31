#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

#define PATH_SIZE 500000
#define DATA_SIZE 500000

typedef struct format_struct {
    int flag_e;
    int flag_i;
    int flag_v;
    int flag_c;
    int flag_l;
    int flag_n;
    int flag_h;
    int flag_s;
    int flag_f;
    int flag_o;
    int flag_err;
    int total_docs;
    int total_found;
    int total_template;
    int empty_template;
    int not_empty_doc;
    int not_temp;
    int same_line;
    int print_once;
    int stop_print;
} flags;

void get_info(const int argc, char * const argv[], flags *f, char *docs_list[]);
void handle_info(const char *pathL, flags *f, char *template[]);
void handle_e(char *data, char *template, const ssize_t llen, flags *f, const char *path, const int new);
void handle_template(char *docs_list, char *template[], flags *f);
void multifound(char *data, char *template, ssize_t llen, flags *f,
                const char *path, regmatch_t *w_found, const int new, int n);
void output(const char *data, ssize_t llen, const flags *f, const char *path, const int numb);
int to_output(const char *data, const ssize_t llen, flags *f, const char *path, int n);

#endif  // SRC_GREP_S21_GREP_H_
