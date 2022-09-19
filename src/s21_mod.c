#include "s21_decimal.h"

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  s21_balancing(&value_1, &value_2);
  int exp = s21_getExp(value_1);
  int res = s21_justMod(value_1, value_2, result);
  s21_setExp(result, exp);
  return res;
}
