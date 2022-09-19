#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  *result = value;
  s21_decimal one, mod_res;
  s21_from_int_to_decimal(1, &one);
  s21_mod(*result, one, &mod_res);
  s21_balancing(&mod_res, result);
  s21_ordinarySub(*result, mod_res, result);
  s21_bankRound(result, s21_getExp(*result));
  s21_setSign(result, s21_getSign(value));
  return OK;
}
