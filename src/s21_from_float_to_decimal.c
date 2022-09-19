#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
    s21_setNull(dst);
    s21_setSign(dst, src < 0 ? 1 : 0);
    int exp = 0;
    while (src != (int)src) {
        src *= 10;
        exp++;
    }
    dst->bits[0] = (int)src;
    s21_setExp(dst, exp);
    return OK;
}
