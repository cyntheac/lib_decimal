#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = s21_intDiv(value_1, value_2, result);
  if (!(status & 3)) {
    status >>= 2;
    s21_decimal mod_res = s21_initZeroDecimal();
    if (status) status = s21_addIntPrec(value_1, value_2, result, status);
    s21_truncate(*result, result);
    s21_mod(value_1, value_2, &mod_res);
    if (!s21_isZero(mod_res)) {
      s21_decimal div_res = s21_initZeroDecimal(), ten = {{10, 0, 0, 0}},
                  tmp = s21_initZeroDecimal();
      int exp = s21_getExp(mod_res);
      for (int sub_stat = 0; !sub_stat; tmp = s21_initZeroDecimal()) {
        sub_stat = s21_justMul(mod_res, ten, &tmp);
        if (!sub_stat) {
          mod_res = tmp;
          if (++exp < 29) s21_setExp(&mod_res, exp);
        }
      }
      if (!s21_getExp(mod_res)) s21_setExp(&mod_res, 28);
      while (s21_justLess(mod_res, value_2)) s21_bankRound(&value_2, 1);
      s21_intDiv(mod_res, value_2, &div_res);
      if (exp > 28) s21_setExp(&div_res, exp - s21_getExp(value_2));
      status = s21_add(div_res, *result, result);
    }
  } else {
    status &= 3;
  }
  if (status == 1 && s21_getSign(value_1) ^ s21_getSign(value_2)) ++status;
  return status;
}
