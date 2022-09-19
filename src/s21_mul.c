#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = s21_initZeroDecimal();
  int res = s21_justMul(value_1, value_2, result);
  while (res && (s21_getExp(value_1) || s21_getExp(value_2))) {
    int exp_check = s21_justLess(value_1, value_2) ? s21_getExp(value_2)
                                                  : s21_getExp(value_1);
    s21_decimal *value = s21_justLess(value_1, value_2) ? &value_2 : &value_1;
    s21_decimal *alt_value =
        !s21_justLess(value_1, value_2) ? &value_2 : &value_1;
    if (exp_check)
      s21_bankRound(value, 1);
    else
      s21_bankRound(alt_value, 1);
    res = s21_justMul(value_1, value_2, result);
  }
  int exp = s21_getExp(value_1) + s21_getExp(value_2);
  if (exp < 29)
    s21_setExp(result, exp);
  else
    *result = s21_initZeroDecimal();
  return res;
}
