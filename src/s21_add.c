#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = s21_initZeroDecimal();
  s21_balancing(&value_1, &value_2);
  int status = s21_justAdd(value_1, value_2, result);
  if (status && s21_getExp(value_1) && s21_getExp(value_2)) {
    s21_bankRound(&value_1, 1);
    s21_bankRound(&value_2, 1);
    status = s21_justAdd(value_1, value_2, result);
  }
  int exp = s21_getExp(value_1);
  s21_setExp(result, exp);
  return status;
}
