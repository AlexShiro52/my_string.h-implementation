#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size n) {
    s21_size res = 0;
    for (s21_size i = 0; i < n; i++) {
        if (str1[i] < str2[i]) {
            res = str1[i] - str2[i];
            return res;
        }
        else if (str1[i] > str2[i]) {
            res = str2[i] - str1[1];
            return res;
        }
    }
    return 0;
}

