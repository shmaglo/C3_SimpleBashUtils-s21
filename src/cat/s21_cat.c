#include "./s21_cat.h"

int main(const int argc, char *argv[]) {
    flags f = {0};
    char *docs_list[100] = {'\0'};
    int result = 0;
    get_info(argc, argv, &f, docs_list);
    if (!f.flag_err) {
        if (f.total_docs) {
            for (int i = 0; i < f.total_docs; i++) {
                handle_info(docs_list[i], f);
            }
        }
    } else {
        fprintf(stderr, "usage: cat [-benstuv] [file ...]\n");
        result = 1;
    }
    return result;
}

void get_info(const int argc, char * const argv[], flags *f, char *docs_list[]) {
    const char * short_options = "beEnstTv";
    const struct option long_options[] = {
        { "squeeze-blank",      1, 0, 's'},
        { "number-nonblank",    1, 0, 'b'},
        { "number",             1, 0, 'n'},
        { NULL, 0, NULL, 0}
    };
    int ret;
    opterr = 1;
    while ((ret = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (ret) {
            case 'b':
                f->flag_b = 1;
            break;
            case 'e':
                f->flag_e = 1;
            break;
            case 'E':
                f->flag_E = 1;
            break;
            case 'n':
                f->flag_n = 1;
            break;
            case 's':
                f->flag_s = 1;
            break;
            case 't':
                f->flag_t = 1;
            break;
            case 'T':
                f->flag_T = 1;
            break;
            case 'v':
                f->flag_v = 1;
            break;
            case '?':
                f->flag_err = 1;
            break;
        }
    }
    if (optind < argc) {
        int l = 0;
        while (optind < argc) {
            docs_list[l++] = argv[optind++];
            f->total_docs++;
        }
    }
}

void handle_info(const char *pathL, const flags f) {
    FILE *fi;
    fi = fopen(pathL, "r");
    if (fi) {
        int new = 1;
        char *data = calloc(DATA_SIZE, sizeof(char));  //  = NULL;
        size_t lc = 0;
        ssize_t llen;
        while ((llen = getline(&data, &lc, fi)) > 0) {
            if (f.flag_b || f.flag_e || f.flag_E || f.flag_n ||
                f.flag_s || f.flag_t || f.flag_T || f.flag_v) {
                handle_all(data, llen, f, new);
                new = 0;
            } else {
                output2(data, llen, f);
            }
        }
        free(data);
        fclose(fi);
    } else {
        fprintf(stderr, "cat: %s: No such file or directory\n", pathL);
    }
}

void handle_all(const char *data, const ssize_t llen, const flags f, const int new) {
    int can_print = 0;
    if (f.flag_s) {
        static int prev_line_empty;
        if (llen == 1) {
            if (prev_line_empty == 0) {
                can_print = 1;
                prev_line_empty = 1;
            }
        } else {
            can_print = 1;
            prev_line_empty = 0;
        }
    } else {
        can_print = 1;
    }
    if (can_print) {
        if (f.flag_b) {
            static int n_1;
            if (new) {
                n_1 = 0;
            }
            if (llen > 1) {
                n_1++;
                printf("%6d\t", n_1);
            }
        }
        if (f.flag_n && !f.flag_b) {
            static int n_2;
            if (new) {
                n_2 = 0;
            }
            n_2++;
            printf("%6d\t", n_2);
        }
        output2(data, llen, f);
    }
}

void handle_symb(const char ch) {
    unsigned char c = (unsigned char)ch;
    int before = (c <= 31 && c != 9 && c != 10);
    int after = (c >= 128 && c <= 159);
    // int in_the_end = 0; //(c >= 160 && c <= 254);
    if (after) {
        printf("%s%c", "M-^", c - 64);
    } else if (before) {
        printf("%s%c", "^", c + 64);
    // } else if (in_the_end) { //     printf("%s%c", "M-^", c - 128);
    } else if (c == 127) {
        printf("^?");
    } else {
        printf("%c", ch);
    }
}

void output2(const char *data, const ssize_t llen, const flags f) {
    if (f.flag_t || f.flag_T || f.flag_E || f.flag_e || f.flag_v) {
        print_tev(data, llen, f);
    } else {
        for (int i = 0; i < llen; i++) {
            printf("%c", data[i]);
        }
    }
}

void print_tev(const char *data, const ssize_t llen, const flags f) {
    if (f.flag_t || f.flag_T || f.flag_E || f.flag_e || f.flag_v) {
        int data_len = llen;
        if (f.flag_E || f.flag_e) {
            data_len = llen - 1;
        }
        for (int i = 0; i < data_len; i++) {
            if ((f.flag_t || f.flag_T) && data[i] == '\t') {
                printf("%s", "^I");
            } else {
                if (f.flag_t || f.flag_e || f.flag_v) {
                    handle_symb(data[i]);
                } else {
                    printf("%c", data[i]);
                }
            }
        }

        if (f.flag_E || f.flag_e) {
            if (data[llen-1] == '\n' || data[llen-1] == '\0') {
                printf("%c%c", '$', data[llen-1]);
            } else {
                if (f.flag_e) {
                    handle_symb(data[llen-1]);
                } else {
                    printf("%c", '$');
                }
            }
        }
    }
}
