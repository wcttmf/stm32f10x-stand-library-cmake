//
// Created by lsy on 17-4-14.
//

#ifndef FORMAT_H
#define FORMAT_H

#include <stdint.h>

class Format {
public:
    static int8_t convert(char *str, int32_t num) {
        char stack[20];
        uint8_t top = 0, len, sign = 0;
        if (num < 0) {
            num = -num;
            sign = 1;
        }
        if (num == 0) stack[top++] = '0';
        while (num) {
            stack[top++] = '0' + num % 10;
            num /= 10;
        }
        if (sign) stack[top++] = '-';
        len = top;
        while (top--) *(str++) = stack[top];
        *str = '\0';
        return len;
    };

    static int8_t convert(char *str, double num) {
        uint8_t len = 0;
        if (num < 0) {
            num = -num;
            str[len++] = '-';
        }
        len += convert(&(str[len]), num < 0 ? (int32_t) - num : (int32_t) num);
        str[len++] = '.';
        while (num) {
            num -= (int32_t) num;
            str[len++] = '0' + num * 10;
            num *= 10;
        }
        str[len] = '\0';
        return len;
    };
};


#endif
