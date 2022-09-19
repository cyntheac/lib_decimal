#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  *result = value;
  s21_setSign(result, !s21_getSign(value));
  return OK;
}
