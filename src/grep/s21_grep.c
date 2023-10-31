#include "./s21_grep.h"

int main(const int argc, char *argv[]) {
    flags f = {0};
    char *docs_list[200] = {'\0'};
    int result = 0;
    get_info(argc, argv, &f, docs_list);
    if (!f.flag_err) {
        if (f.total_docs) {
            char *template[500] = {'\0'};
            if (f.flag_f) {
                handle_template(docs_list[0], template, &f);
            } else {
                template[0] = docs_list[0];
                f.total_template = 1;
            }
            if (f.total_template == 1 && strlen(template[0]) == 0) {
                f.not_temp = 1;
            }
            if (f.flag_v && !f.flag_c && f.not_temp) {} else {
                for (int i = 1; i < f.total_docs && !f.stop_print; i++) {
                    handle_info(docs_list[i], &f, template);
                }
            }
        }
    } else {
        if (!f.flag_s) {
            fprintf(stderr, "usage: grep [-ivclnhso] [-e pattern] [-f file] template [file ...]\n");
        }
        result = 1;
    }
    return result;
}

void get_info(const int argc, char *const argv[], flags *f, char *docs_list[]) {
    const char *short_options = "eivclnhsfo";
    const struct option long_options[] = {
        {"--file", 1, 0, 'f'},
        {NULL, 0, NULL, 0}};
    int ret;
    opterr = 1;
    while ((ret = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (ret) {
            case 'e':
                f->flag_e = 1;
                break;
            case 'i':
                f->flag_i = 1;
                break;
            case 'v':
                f->flag_v = 1;
                break;
            case 'c':
                f->flag_c = 1;
                break;
            case 'l':
                f->flag_l = 1;
                break;
            case 'n':
                f->flag_n = 1;
                break;
            case 'h':
                f->flag_h = 1;
                break;
            case 's':
                f->flag_s = 1;
                break;
            case 'f':
                f->flag_f = 1;
                break;
            case 'o':
                f->flag_o = 1;
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

void handle_info(const char *pathL, flags *f, char *template[]) {
f->not_empty_doc = 0;
    FILE *fi;
    fi = fopen(pathL, "r");
    if (fi) {
        int new = 1;
        char *data = calloc(DATA_SIZE, sizeof(char));
        size_t lc = 0;
        ssize_t llen;
        f->print_once++;
        f->total_found = 0;
        while ((llen = getline(&data, &lc, fi)) > 0) {
            f->not_empty_doc++;
            f->same_line = 0;
            for (int i = 0; i < f->total_template; i++) {
                handle_e(data, template[i], llen, f, pathL, new);
                new = 0;
            }
        }
        if (f->not_temp && !f->not_empty_doc) {
            f->stop_print = 1;
        } else {
            if (f->flag_c) {
                if (!f->flag_h && f->total_docs > 2) {
                    printf("%s:", pathL);
                }
                if (f->flag_v && f->not_temp) {
                    printf("%d\n", 0);
                } else if (f->flag_l && f->not_temp) {
                    printf("%d\n", 1);
                } else {
                    printf("%d\n", f->total_found);
                }
            }
            if (f->flag_l) {
                if (f->total_found > 0) {
                    printf("%s\n", pathL);
                }
            }
        }
        free(data);
    } else {
        if (!f->flag_s) {
            fprintf(stderr, "grep: %s: No such file or directory\n", pathL);
        }
    }
    fclose(fi);
}

void handle_e(char *data, char *template, const ssize_t llen,
                flags *f, const char *path, const int new) {
    static int n;
    if (f->flag_n || f->flag_c || f->flag_l) {
        if (new) {
            n = 0;
        }
        if ((f->flag_n && !f->flag_c && !f->same_line) || f->empty_template) {
            n++;
        }
    }
    if (strlen(template)) {
        regex_t regex;
        int reti = 0;
        char msgbuf[100] = {'\0'};
        regmatch_t w_found[50] = {'\0'};
        reti = regcomp(&regex, template, REG_EXTENDED | REG_ICASE * f->flag_i);
        if (reti) {
            fprintf(stderr, "Could not compile regex\n");
            exit(1);
        }
        reti = regexec(&regex, data, 50, w_found, 0);
        if (f->flag_v) {
            if (!reti) {
                // puts("Match\n");
            } else if (reti == REG_NOMATCH) {
                n = to_output(data, llen, f, path, n);
            } else {
                regerror(reti, &regex, msgbuf, sizeof(msgbuf));
                fprintf(stderr, "Regex match failed: %s\n", msgbuf);
                exit(1);
            }
        } else {
            if (!reti) {
                if (f->flag_o && !f->flag_c && !f->flag_l) {
                    multifound(data, template, llen, f, path, w_found, 0, n);
                } else {
                    if (f->flag_c && *template == '.' && strlen(data) == 1) {
                    } else {
                        n = to_output(data, llen, f, path, n);
                    }
                }
            } else if (reti == REG_NOMATCH) {
                // puts("No match");
            } else {
                regerror(reti, &regex, msgbuf, sizeof(msgbuf));
                fprintf(stderr, "Regex match failed: %s\n", msgbuf);
                exit(1);
            }
        }
        regfree(&regex);
    } else if (f->print_once == 1) {
        to_output(data, llen, f, path, n);
        f->empty_template++;
    }
}

int to_output(const char *data, const ssize_t llen,
              flags *f, const char *path, int n) {
    if (f->flag_c || f->flag_l) {
        n++;
        f->total_found = n;
    } else {
        int numb = f->flag_n ? n : 0;
        output(data, llen, f, path, numb);
    }
    return n;
}

void handle_template(char *docs_list, char *template[], flags *f) {
    FILE *fi;
    fi = fopen(docs_list, "r");
    if (fi) {
        int i = 0;
        char tmp[200][200] = {'\0'};
        while (fgets(tmp[i], 200, fi) != NULL) {
            template[i] = tmp[i];

            template[i][strlen(tmp[i])-1] = '\0';

            i++;
            f->total_template++;
        }
    } else {
        if (!f->flag_s) {
            fprintf(stderr, "grep: %s: No such file or directory\n", docs_list);
        }
    }
    fclose(fi);
}

void multifound(char *data, char *template, ssize_t llen, flags *f,
                const char *path, regmatch_t *w_found, const int new, int n) {
    int start = w_found[0].rm_so;
    int finish = w_found[0].rm_eo;
    int nn = n;
    if (f->same_line) {
        nn = 0;
    }
    if (!f->flag_c) {
        output(data + start, (finish - start), f, path, nn);
    }
    f->same_line = 1;
    data += finish;
    llen -= finish;
    handle_e(data, template, llen, f, path, new);
}

void output(const char *data, ssize_t llen, const flags *f,
            const char *path, const int numb) {
    if (!f->same_line) {
        if (f->total_docs > 2 && !f->flag_h) {
            printf("%s:", path);
        }
    }
    if (numb) {
        printf("%d:", numb);
    }
    for (int i = 0; i < llen; i++) {
        printf("%c", data[i]);
    }
    if (data[llen - 1] != '\n') {
        printf("\n");
    }
}
