#include "s21_string.h"
#include <stdarg.h>

int read_d(const char *str, int *address, int *j, int width, int was_star);

int read_c(const char *str, char *address, int *j, int width, int was_star);

int read_s(const char *str, char *address, int *j, int width, int was_star);

int read_u(const char *str, unsigned int *address, int *j, int width, int was_star);

int read_o(const char *str, unsigned int *address, int *j, int width, int was_star);

int read_x(const char *str, unsigned int *address, int *j, int width, int was_star);

int read_X(const char *str, unsigned int *address, int *j, int width, int was_star);

int all_read_func(va_list *args, int *j, const char *str, char read_spec, int width, int was_star);

int space_skip(const char *format, const char *str, int *i, int *j);

int specification_analysis(const char *format, int *i, char *read_spec, int *width, int *was_star);

int s21_sscanf(const char *str, const char *format, ...) {
    char read_spec;
    int was_star = 0;
    int width = 0;
    va_list args;
    int result = 0;
    int j = 0;
    va_start(args, format);
    for (int i = 0; format[i] != '\0'; i++) {
        if (space_skip(format, str, &i, &j) == 1) {
            break;
        }
        if (format[i] == '%') {
            int specification_analysis_res = specification_analysis(format, &i, &read_spec, &width, &was_star);
            if (specification_analysis_res == 1) {
                break;
            }
            int all_read_func_res = all_read_func(&args, &j, str, read_spec, width, was_star);
            if (all_read_func_res == 1) {
                break;
            }
            else if (all_read_func_res == 0 && was_star == 0) {
                result += 1;
            }
        }
        else if (str[j] != format[i] && format[i] != '\0') {
            break;
        }
        else if (str[j] == format[i] && format[i] != '\0') {
            j++;
        }
    }
    va_end(args);
    return result;
}




int space_skip(const char *format, const char *str, int *i, int *j) {
    if (format[*i] == ' ' || format[*i] == '\n' || format[*i] == '\t') {
        while (format[*i] == ' ' || format[*i] == '\n' || format[*i] == '\t') {
            (*i)++;
        }
        while (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t') {
            (*j)++;
        }
        if (format[*i] == '\0') {
            return 1;
        }
    }
    return 0;
}




int specification_analysis(const char *format, int *i, char *read_spec, int *width, int *was_star) {
    *width = 0;
    *was_star = 0;
    (*i)++;
    if (format[*i] == '\0') {
        return 1;
    }
    if (format[*i] != '\0') {
        if (format[*i] == '*') {
            *was_star = 1;
            (*i)++;
        }
        while (format[*i] <= '9' && format[*i] >= '0') {
            *width = *width * 10 + format[*i] - '0';
            (*i)++;
        }
        if (format[*i] == 'd') {
            *read_spec = 'd';
        }
        else if (format[*i] == 'c') {
            *read_spec = 'c';
        }
        else if (format[*i] == 's') {
            *read_spec = 's';
        }
        else if (format[*i] == 'u') {
            *read_spec = 'u';
        }
        else if (format[*i] == 'o') {
            *read_spec = 'o';
        }
        else if (format[*i] == 'X' || format[*i] == 'x') {
            *read_spec = 'x';
        }
        else {
            return 1;
        }
    }
    return 0;
}



int all_read_func(va_list *args, int *j, const char *str, char read_spec, int width, int was_star) {
    int read_d_res;
    int read_c_res;
    int read_s_res;
    int read_u_res;
    int read_o_res;
    int read_x_res;
    int read_X_res;
    if (read_spec == 'd') {
        int *address;
        if (was_star == 1) {
            address = S21_NULL;
        }
        else if (was_star == 0) {
            address = va_arg(*args, int*);
        }
        read_d_res = read_d(str, address, j, width, was_star);
        if (read_d_res == 1) {
            return 1;
        }
        else if (read_d_res == 0) {
            return 0;
        }
    }
    else if (read_spec == 'c') {
        char *address;
        if (was_star == 1) {
            address = S21_NULL;
        }
        else if (was_star == 0) {
            address = va_arg(*args, char*);
        }
        read_c_res = read_c(str, address, j, width, was_star);
        if (read_c_res == 0) {
            return 0;
        }
        else if (read_c_res == 1) {
            return 1;
        }
    }
    else if (read_spec == 's') {
        char *address;
        if (was_star == 1) {
            address = S21_NULL;
        }
        else if (was_star == 0) {
            address = va_arg(*args, char*);
        }
        read_s_res = read_s(str, address, j, width, was_star);
        if (read_s_res == 0) {
            return 0;
        }
        else if (read_s_res == 1) {
            return 1;
        }
    }
    else if (read_spec == 'u') {
        unsigned int *address;
        if (was_star == 1) {
            address = S21_NULL;
        }
        else if (was_star == 0) {
            address = va_arg(*args, unsigned int*);
        }
        read_u_res = read_u(str, address, j, width, was_star);
        if (read_u_res == 0) {
            return 0;
        }
        else if (read_u_res == 1) {
            return 1;
        }
    }
    else if (read_spec == 'o') {
        unsigned int *address;
        if (was_star == 1) {
            address = S21_NULL;
        }
        else if (was_star == 0) {
            address = va_arg(*args, unsigned int*);
        }
        read_o_res = read_o(str, address, j, width, was_star);
        if (read_o_res == 1) {
            return 1;
        }
        else if (read_o_res == 0) {
            return 0;
        }
    }
    else if (read_spec == 'x') {
        unsigned int *address;
        if (was_star == 1) {
            address = S21_NULL;
        }
        else if (was_star == 0) {
            address = va_arg(*args, unsigned int*);
        }
        read_x_res = read_x(str, address, j, width, was_star);
        if (read_x_res == 0) {
            return 0;
        }
        else if (read_x_res == 1) {
            return 1;
        }
    }
    return 1;
}




int read_d(const char *str, int *address, int *j, int width, int was_star) {
    int result_char_d = 0;
    int otr_numb = 0;
    int was_number = 0;
    int was_znack = 0;
    int symb_count = 0;
    if (str[*j] == '\0') {
        return 1;
    }
    while (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t') {
        (*j)++;
    }
    for (; str[*j] != '\0'; (*j)++) {
        if (width != 0) {
            if (symb_count == width) {
                break;
            }
        }
        if (str[*j] == ' ') {
            break;
        }
        else if (str[*j] == '+' && was_number == 0 && was_znack == 0) {
            was_znack = 1;
            symb_count += 1;
        }
        else if (str[*j] == '-' && otr_numb == 0 && was_number == 0 && was_znack == 0) {
            otr_numb = 1;
            was_znack = 1;
            symb_count += 1;
        }
        else if (str[*j] < '0' || str[*j] > '9') {
            break;
        }
        else if (str[*j] >= '0' && str[*j] <= '9'){
            result_char_d = result_char_d * 10 + str[*j] - '0';
            was_number = 1;
            symb_count += 1;
        }
    }
    if (was_number == 1) {
        if (otr_numb == 1) {
            result_char_d = result_char_d * -1;
        }
        if (was_star == 0) {
            *address = result_char_d;
            return 0;
        }
        else if (was_star == 1) {
            return 0;
        }
    }
    else {
        return 1;
    }
    return 1;
}



int read_c(const char *str, char *address, int *j, int width, int was_star) {
    int symb_count = 0;
    int ind_pos = 0;
    if (str[*j] == '\0') {
        return 1;
    }
    else if (width != 0) {
        for (; str[*j] != '\0'; (*j)++) {
            if (symb_count == width) {
                break;
            }
            if (was_star == 0) {
                address[ind_pos] = str[*j];
                ind_pos++;
                symb_count += 1;
            }
            else if (was_star == 1) {
                symb_count += 1;
            }
        }
        return 0;
    }
    else {
        if (was_star == 0) {
            *address = str[*j];
            (*j)++;
            return 0;
        }
        else if (was_star == 1) {
            (*j)++;
            return 0;
        }
    }
    return 1;
}



int read_s(const char *str, char *address, int *j, int width, int was_star) {
    int was_string = 0;
    int pos_ind = 0;
    int symb_count = 0;
    while (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t') {
        (*j)++;
    }
    if (str[*j] == '\0') {
        return 1;
    }
    for (;str[*j] != '\0'; (*j)++) {
        if (width != 0) {
            if (symb_count == width) {
                break;
            }
        }
        if (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t') {
            break;
        }
        else {
            if (was_star == 1) {
                was_string = 1;
                symb_count += 1;
            }
            else if (was_star == 0) {
                address[pos_ind] = str[*j];
                was_string = 1;
                pos_ind++;
                symb_count += 1;
            }
        }
    }
    if (was_string == 1 && was_star == 0) {
        address[pos_ind] = '\0';
        return 0;
    }
    else if (was_string == 1 && was_star == 1) {
        return 0;
    }
    else if (was_string == 0) {
        return 1;
    }
    return 1;
}



int read_u(const char *str, unsigned int *address, int *j, int width, int was_star) {
    unsigned int result_char_u = 0;
    int otr_numb = 0;
    int was_number = 0;
    int was_znack = 0;
    int symb_count = 0;
    if (str[*j] == '\0') {
        return 1;
    }
    while (str[*j] == ' ' || str[*j] == '\n' || str[*j] == '\t') {
        (*j)++;
    }
    for (;str[*j] != '\0'; (*j)++) {
        if (width != 0) {
            if (symb_count == width) {
                break;
            }
        }
        if (str[*j] == '\0') {
            break;
        }
        else if (str[*j] >= '0' && str[*j] <= '9') {
            result_char_u = result_char_u * 10 + str[*j] - '0';
            was_number = 1;
            symb_count += 1;
        }
        else if ((str[*j] == '-' || str[*j] == '+') && was_number == 0 && was_znack == 0) {
            if (str[*j] == '-') {
                otr_numb = 1;
            }
            was_znack = 1;
            symb_count += 1;
        }
        else if (str[*j] < '0' || str[*j] > '9') {
            break;
        }
    }
    if (was_number == 1) {
        if (otr_numb == 1) {
            result_char_u = result_char_u * -1;
        }
        if (was_star == 1) {
            return 0;
        }
        else if (was_star == 0) {
            *address = result_char_u;
            return 0;
        }
    }
    else {
        return 1;
    }
    return 1;
}



int read_o(const char *str, unsigned int *address, int *j, int width, int was_star) {
    unsigned int result_char_o = 0;
    int otr_numb = 0;
    int was_number = 0;
    int was_znack = 0;
    int symb_count = 0;
    if (str[*j] == '\0') {
        return 1;
    }
    while (str[*j] == ' ' || str[*j] == '\t' || str[*j] == '\n') {
        (*j)++;
    }
    for (;str[*j] != '\0'; (*j)++) {
        if (width != 0) {
            if (symb_count == width) {
                break;
            }
        }
        if (str[*j] == '\0') {
            break;
        }
        else if (str[*j] >= '0' && str[*j] <= '7') {
            result_char_o = result_char_o * 8 + str[*j] - '0';
            was_number = 1;
            symb_count += 1;
        }
        else if ((str[*j] == '-' || str[*j] == '+') && was_number == 0 && was_znack == 0) {
            if (str[*j] == '-') {
                otr_numb = 1;
            }
            was_znack = 1;
            symb_count += 1;
        }
        else if (str[*j] < '0' || str[*j] > '7') {
            break;
        }
    }
    if (was_number == 1) {
        if (otr_numb == 1) {
            result_char_o = result_char_o * -1;
        }
        if (was_star == 1) {
            return 0;
        }
        else if (was_star == 0) {
            *address = result_char_o;
            return 0;
        }
    }
    else {
        return 1;
    }
    return 1;
}



int read_x(const char *str, unsigned int *address, int *j, int width, int was_star) {
    unsigned int result_char_x = 0;
    int otr_numb = 0;
    int was_number = 0;
    int was_znack = 0;
    int symb_count = 0;
    int was_prefix = 0;
    if (str[*j] == '\0') {
        return 1;
    }
    while (str[*j] == ' ' || str[*j] == '\t' || str[*j] == '\n') {
        (*j)++;
    }
    for (;str[*j] != '\0'; (*j)++) {
        if (width != 0) {
            if (symb_count == width) {
                break;
            }
        }
        if (str[*j] == '\0') {
            break;
        }
        else if (str[*j] == '0' && str[*j + 1] != '\0' && (str[*j + 1] == 'x' || str[*j + 1] == 'X') && was_prefix == 0 && was_number == 0) {
            if (width != 0) {
                if (width - symb_count >= 2) {
                    symb_count += 2;
                    (*j)++;
                    was_prefix = 1;
                }
                else if (width - symb_count == 1 && width > 0) {
                    symb_count += 1;
                    result_char_x = result_char_x * 16;
                    was_number = 1;
                }
            }
            else if (width == 0) {
                symb_count += 2;
                (*j)++;
                was_prefix = 1;
            }
        }
        else if (str[*j] >= '0' && str[*j] <= '9') {
            result_char_x = result_char_x * 16 + str[*j] - '0';
            was_number = 1;
            symb_count += 1;
        }
        else if (str[*j] >= 'a' && str[*j] <= 'f') {
            result_char_x = result_char_x * 16 + str[*j] - 'a' + 10;
            was_number = 1;
            symb_count += 1;
        }
        else if (str[*j] >= 'A' && str[*j] <= 'F') {
            result_char_x = result_char_x * 16 + str[*j] - 'A' + 10;
            was_number = 1;
            symb_count += 1;
        }
        else if ((str[*j] == '-' || str[*j] == '+') && was_number == 0 && was_znack == 0 && was_prefix == 0) {
            if (str[*j] == '-') {
                otr_numb = 1;
            }
            was_znack = 1;
            symb_count += 1;
        }
        else {
            break;
        }
    }
    if (was_number == 1) {
        if (otr_numb == 1) {
            result_char_x = result_char_x * -1;
        }
        if (was_star == 1) {
            return 0;
        }
        else if (was_star == 0) {
            *address = result_char_x;
            return 0;
        }
    }
    else {
        return 1;
    }
    return 1;
}



