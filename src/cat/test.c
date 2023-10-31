#include "test.h"

int main(void) {
    test();
    return 0;
}

void test() {
    int len = 64;
    char buff[64][32];
    for (int i = 0; i < len; i++) {
        memset(buff[i], '\0', 32);
        if (i >> 0 & 1) strcat(buff[i], flags[0]);
        if (i >> 1 & 1) strcat(buff[i], flags[1]);
        if (i >> 2 & 1) strcat(buff[i], flags[2]);
        if (i >> 3 & 1) strcat(buff[i], flags[3]);
        if (i >> 4 & 1) strcat(buff[i], flags[4]);
        if (i >> 5 & 1) strcat(buff[i], flags[5]);
    }
    for (int i = 0; i < 64; i++) {
        char create_1[512] = {'\0'};
        char create_2[512] = {'\0'};
        strcat(create_1, "cat");
        strcat(create_2, "./s21_cat");
        strcat(create_1, buff[i]);
        strcat(create_2, buff[i]);
        strcat(create_1, " file1 > result_1");
        strcat(create_2, " file2 > result_2");
        printf("%s\n", create_1);
        printf("%s\n", create_2);
        system(create_1);
        system(create_2);
        if (system("diff result_1 result_2")) {
            printf("%sFAIL%s\n", RED, RESET);
        } else {
            printf("%sOK!%s\n", GRN, RESET);
        }
    }
}

