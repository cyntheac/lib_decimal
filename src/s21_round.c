#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  s21_decimal one, zeroDotFive, mod_res;
  s21_from_int_to_decimal(1, &one);
  s21_from_int_to_decimal(5, &zeroDotFive);
  s21_setExp(&zeroDotFive, 1);
  s21_mod(value, one, &mod_res);
  s21_setSign(&mod_res, 0);
  s21_truncate(value, result);
  if (!s21_is_less(mod_res, zeroDotFive)) s21_checkAdd(*result, one, result);
  return OK;
}
