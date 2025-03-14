#include "helper.h"


int is_digit(const char* s) {
    const char* p = s;
    if (!*p) {
        return 0;
    }
    while (*p) {
        if (*p < 48 || *p > 57) {
            return 0;
        }
        p++;
    }
    return 1;
}