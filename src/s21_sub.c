#include "s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = s21_initZeroDecimal();
  s21_balancing(&value_1, &value_2);
  int exp = s21_getExp(value_1);
  s21_setExp(result, exp);
  int sign_1 = s21_getSign(value_1);
  int sign_2 = s21_getSign(value_2);
  int status = 0;
  if (sign_1 && sign_2) {
    s21_setSign(&value_1, !sign_1);
    s21_setSign(&value_2, !sign_2);
    status = s21_sub(value_2, value_1, result);
  } else if (sign_1) {
    s21_setSign(&value_1, !sign_1);
    status = s21_checkAdd(value_1, value_2, result);
    s21_setSign(result, NEGATIVE);
  } else if (sign_2) {
    s21_setSign(&value_2, !sign_2);
    status = s21_checkAdd(value_1, value_2, result);
  } else if (s21_is_less(value_1, value_2)) {
    status = s21_ordinarySub(value_2, value_1, result);
    s21_setSign(result, NEGATIVE);
  } else {
    s21_ordinarySub(value_1, value_2, result);
  }
  return status;
}
