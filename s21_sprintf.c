#include <stdarg.h>
#include <stdio.h>

#include "s21_string.h"

static void widthprint(char** ps, int w, char c) {
    for (int j = 0; j < w; j++) {
        **ps = c;
        (*ps)++;
    }
}

static void prbuf(char** ps, char* buf, int* flags, int i) {
    
    widthprint(ps, flags[5] - i, '0');
    
    for (i--; i >= 0; i--) {
        **ps = buf[i];
        (*ps)++;
    }
    
    if (flags[0] == 1) {
        widthprint(ps, flags[3], ' ');
    }
}

static void arrmult(int* num_len, char* buf, int x) {
    unsigned char carry = 0;
    for (int i = 0; i < *num_len; i++) {
        unsigned char cur = (buf[i] * x) + carry;
        buf[i] = cur % 10;
        carry = cur / 10;
    }
    if (carry) {
        buf[*num_len] = carry;
        (*num_len)++;
    }
}

static void printeger(long long n, char** ps, int* flags) {
    char buf[32] = {' '};
    unsigned long long num;
    int i = 0;
    if (n < 0) {
        **ps = '-';
        flags[3]--;
        (*ps)++;
        num = (unsigned long long)(-n);
    } else {
        if (flags[1] == 1) {
            **ps = '+';
            flags[3]--;
            (*ps)++;
            
        } else if (flags[6] == 1) {
            **ps = ' ';
            flags[3]--;
            (*ps)++;
            
        }
        num = (unsigned long long)(n);
    }
    if (!(n == 0 && flags[4] == 1 && flags[5] == 0)) {
        do {
            buf[i] = (char)((num % 10) + 48);
            num = num / 10;
            i++;
        } while (num != 0);
    }

    flags[3] -= i;
    flags[3] -= flags[5];
    char fc = ' ';
    if (flags[7] == 1 && flags[4] != 1) {
        fc = '0';
    }

    if (flags[0] == 0) {
        widthprint(ps, flags[3], fc);
    }

    prbuf(ps, buf, flags, i);
} 

static void prfloat(float f, char** ps, int* flags) {
    static union Doublebits {
        double value;
        unsigned long long bits;
    } dbits;

    dbits.value = f;

    unsigned long long mantis = dbits.bits & 0x7FFFFFULL; 

    mantis |= 0x800000ULL;

    long long exp = (long long)((dbits.bits >> 23) & 0xFFULL) - 127 - 23; 

    unsigned char buf[800] = {0};
    int num_len = 0;
    int dec_ecsp;
    
    do {
        buf[num_len] = (unsigned char)((mantis % 10));
        mantis = mantis / 10;
        num_len++;
    } while (mantis != 0);
    printf("%lld", exp);
    if (exp > 0){
        for (int j = 0; j < exp; j++) {
            arrmult(&num_len, buf, 2);
        } 
    } else if (exp < 0) {
        for (int j = 0; j > exp; j--) {
            unsigned char carry = 0;
            for (int i = 0; i < num_len; i++) {
                unsigned char cur = (buf[i] * 5) + carry;
                buf[i] = cur % 10;
                carry = cur / 10;
            }
            if (carry > 0) {
                buf[num_len] = carry;
                num_len++;
            }
        } 
    }
    for (int i = num_len - 1; i >= 0; i--) {
        **ps = buf[i] + '0';
        (*ps)++;
    }
} 

static void prdouble(double f, char** ps, int* flags) {
    static union Doublebits {
        double value;
        unsigned long long bits;
    } dbits;

    dbits.value = f;

    unsigned long long mantis = dbits.bits & 0xFFFFFFFFFFFFFULL; 

    mantis |= 0x10000000000000ULL;

    long long exp = (long long)((dbits.bits >> 52) & 0x7FFULL) - 1023 - 52; 
    printf("%lld", exp + 52);
    unsigned char buf[800] = {0};
    int num_len = 0;
    

  //  printf(" %d", dec_exp);
    
    do {
        buf[num_len] = (unsigned char)((mantis % 10));
        mantis = mantis / 10;
        num_len++;
    } while (mantis != 0);

    if (exp > 0){
        for (int j = 0; j < exp; j++) {
            arrmult(&num_len, buf, 2);
        } **ps = '0';
        (*ps)++;
    } else if (exp <= 0) {
        for (int j = 0; j > exp; j--) {
            arrmult(&num_len, buf, 5);
        } 
    }

    printf(" %lld", num_len);
    int dec_exp;
    if (exp + 52 <= 0) {
        dec_exp = (int)(((double)(52 + exp) * 30103) / 100000 + 0.5);
        flags[5] -= dec_exp;
        **ps = '0';
        (*ps)++;
        **ps = '.';
        (*ps)++;
        widthprint(ps, -dec_exp, '0'); 
    } else {
        dec_exp = (int)(((double)(52 + exp) * 30103) / 100000);
    }

    for (int i = num_len - 1; i >= 0; i--) {
        **ps = buf[i] + '0';
        (*ps)++;
        if (i == num_len - dec_exp - 1 && exp + 52 > 0) {
            **ps = '.';
            (*ps)++;
        }
    }
    
    
} 

static void prfltostr(double f, char** ps, int* flags) {
    double base = 1.0;

    if (f < 0) {
        **ps = '-';
        (*ps)++;
        f = -f;
    }

    if (flags[4] == 0) {
        flags[5] = 6;
    }

    double round = 0.5;
    for(int i = flags[5]; i > 0; i--) {
        round /= 10.0;
    }
    f += round;

    while (f / base >= 10.0) {
        printf("%lf\n", base);
        base *= 10.0;
    } 

    while (base >= 1.0) {
        int digit = (int)(f / base);
        printf("%d\n", digit);
        **ps = (char)(digit + '0');
        (*ps)++;

        f -= (double)digit * base;
        printf("%lf\n", f);
        base /= 10.0;
    }

    **ps = '.';
    (*ps)++;

    printf("%d\n", flags[5]);

    for (int i = flags[5]; i > 0; i--) {
        f *= 10.0;
        int digit = (int)f;

        printf("%lf\n", f);

        **ps = (char)(digit + '0');
        (*ps)++;
        f -= (double)digit;
    }



    // long long n = (long long)f;
    // int flogs[6] = {0};
    // pritostr(n, ps, flogs);
    // **ps = '.';
    // (*ps)++;
    // f -= n;
    // if (f < 0) {
    //     f = -f;
    // }
    // long long drob = (long long)((f * 1000000) + 0.5);
    
    // printf("%lld\n", drob);
    // char buf[32] = {0};
    // for (int i = 0; i < 6; i++) {
    //     buf[i] = (char)((drob % 10) + 48);
    //     drob = drob / 10;
    // }
    // for (int i = 5; i >= 0; i--) {
    //     **ps = buf[i];
    //     (*ps)++;
    // }

}

static void prstr(char* s, char** ps, int* flags) {
    int l = 0;
    // if (s == NULL) {
    //     s = "(null)";
    // }
    // непонятки с зазными системами, где-то должно выводит нулл, где-то нет
    
    if (s != NULL) {
        char* t = s;
        while (*t != '\0') {
            l++;
            t++;
        }
    }
    
    if (l > flags[5] && flags[4] == 1) {
        l = flags[5];
    }

    flags[3] -= l;
    if (flags[0] == 0) {
        widthprint(ps, flags[3], ' ');
    }

    for (int i = 0; i < l; i++) {
        **ps = s[i];
        (*ps)++;
    }

    if (flags[0] == 1) {
        widthprint(ps, flags[3], ' ');
    }
}

static void prch(char c, char** ps, int* flags) {
    if (flags[0] == 0) {
        widthprint(ps, flags[3] - 1, ' ');
    }

    **ps = c;
    (*ps)++;

    if (flags[0] == 1) {
        widthprint(ps, flags[3] - 1, ' ');
    }

}

static void proct(unsigned int n, char** ps, int* flags){
    int digit = 0;
    char buf[32] = {0};
    int i = 0;

    if (!(n == 0 && flags[4] == 1 && flags[5] == 0)) {
        do {
            digit = n & 0x7;
            printf("%d", n);
            buf[i] = (char)(digit + 48);
            i++;

            n = n >> 3;
        } while (n != 0);

    }

    flags[3] -= i;
    flags[3] -= flags[5];
    char fc = ' ';
    if (flags[7] == 1 && flags[4] != 1) {
        fc = '0';
    }

    if (flags[0] == 0) {
        widthprint(ps, flags[3], fc);
    }
 
    if (n != 0 && flags[2]) {
        buf[i] = '0';
        i++;
        
    }
    
    prbuf(ps, buf, flags, i);
}

static void prhex(unsigned int n, char** ps, int mode, int* flags){
    int digit = 0;
    char buf[32] = {0};
    int i = 0;
    int nn = n;


    if (!(n == 0 && flags[4] == 1 && flags[5] == 0)) {
        do {
            digit = n & 0xf;
            printf("%d", n);
            if (digit < 10) {
                buf[i] = (char)(digit + 48);
            } else {
                buf[i] = (char)(digit + mode);
            }
            i++;
            n = n >> 4;
        } while (n != 0);


    }

    flags[3] -= i;
    flags[3] -= flags[5];
    if (nn != 0 && flags[2]) {
        flags[3] -= 2;
    }
    char fc = ' ';
    if (flags[7] == 1 && flags[4] != 1) {
        fc = '0';
    }

    if (flags[0] == 0) {
        widthprint(ps, flags[3], fc);
    }

    if (nn != 0 && flags[2]) {
        **ps = '0';
        (*ps)++;
        **ps = (char)(mode + 33);
        (*ps)++;
        
    }
    
    prbuf(ps, buf, flags, i);
    
}

int s21_sprintf(char *str, const char* format, ...) {
    char* ps = str;
    const char* pf = format;
    const char* specstart;
    va_list args;
    va_start(args, format);
    
    
// flags[0] - 1 если минус(-), 2 если нуль(0)
// flags[1] - 1 если плюс(+), 2 если пробел( )
// flags[2] - решётка(#)
// flags[3] - значение ширины
// flags[4] - точность
// flags[5] - значение точности

    int state = 0;
    int forn = 0;

    while (*pf != '\0') {
        if (state == 0 && *pf == '%') {
            state = 1;
            specstart = pf;
        } else if (state == 1) {
            int flags[8] = {0};
            //printf("DEBUb:'%c'\n", *pf);
            int fl = 1;
            while (fl) {
                if (*pf == '-')  {
                    flags[0] = 1;
                    pf++;
                } else if (*pf == '0') {
                    flags[7] = 1;
                    pf++;
                } else if (*pf == '+')  {
                    flags[1] = 1;
                    pf++;
                } else if (*pf == ' ') {
                    flags[6] = 1; 
                    pf++;
                } else if (*pf == '#') {
                    flags[2] = 1; 
                    pf++;
                } else {
                    fl = 0;
                }
            }

            //printf("DEBUb:'%c'\n", *pf);

            if (*pf == '*') {
                flags[3] = va_arg(args, int);
                pf++;
            } else {
                while (*pf >= '0' && *pf <= '9') {
                    flags[3] *= 10;
                    flags[3] += (int)(*pf - '0'); 
                    pf++;
                }
            }
            //printf("DEBUb:'%c'\n", *pf);
            if (*pf == '.') {
                flags[4] = 1;
                pf++;
                if (*pf == '*') {
                    flags[5] = va_arg(args, int);
                    pf++;
                } else {
                    while (*pf >= '0' && *pf <= '9') {
                        flags[5] *= 10;
                        flags[5] += (int)(*pf - '0'); 
                        pf++;
                    }
                }
            }

            //printf("DEBUb:'%c'\n", *pf);

            if (*pf == 'c') {
                prch((char)va_arg(args, int), &ps, flags);
                
                state = 0;
            } else if (*pf == 'd') {
                printeger(va_arg(args, int), &ps, flags);
                state = 0;
            } else if (*pf == 'f') {
               // prfltostr(va_arg(args, double), &ps, flags);
                // prfloat((float)(va_arg(args, double)), &ps, flags);
                prdouble((va_arg(args, double)), &ps, flags);
                state = 0;
            } else if (*pf == 's') {
                state = 0;
                prstr(va_arg(args, char*), &ps, flags);
            } else if (*pf == 'u') {
                state = 0;
            } else if (*pf == 'o') {
                state = 0;
                proct(va_arg(args, int), &ps, flags);
            } else if (*pf == 'x') {
                state = 0;
                prhex(va_arg(args, int), &ps, 87, flags);
            } else if (*pf == 'X') {
                state = 0;
                prhex(va_arg(args, int), &ps, 55, flags);
            } else if (*pf == 'p') {
                state = 0;
                prstr(va_arg(args, char*), &ps, flags);
            } else if (*pf == 'n') {
                state = 0;
                printeger((long long)forn, &ps, flags);
            } else if (*pf == '%') {
                state = 0;
                *ps = '%';
                ps++;
            } else {
                pf = specstart;
                state = 0;
                *ps = '%';
                ps++;
            }
        } else {
            *ps = *pf;
            ps++;
            forn++;
        }


        //printf("DEBUG: char='%c', state=%d, ps_dist=%ld\n", *pf, state, (long)(ps - str));

        pf++;
    }

    *ps = '\0';
    va_end(args);
    return 1;
}