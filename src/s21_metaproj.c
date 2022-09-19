#include "s21_decimal.h"

s21_decimal s21_divCore(s21_decimal value_1, s21_decimal value_2,
                     s21_decimal *result, int *status) {
  s21_decimal one;
  s21_from_int_to_decimal(1, &one);
  s21_decimal temp = s21_initZeroDecimal();
  if (s21_justEqual(value_1, value_2)) {
    *result = one;
  } else if (s21_justLess(value_1, value_2)) {
    *result = temp;
  } else {
    s21_decimal divcopy = value_2;
    while ((s21_justLess(value_2, value_1) || s21_justEqual(value_2, value_1)) &&
           !s21_getGbit(value_2, s21_ALL_BIT - 1)) {
      s21_leftShift(&value_2);
      s21_leftShift(result);
    }
    if (s21_justLess(value_1, value_2)) {
      s21_rightShift(&value_2);
      s21_rightShift(result);
    }
    s21_ordinarySub(value_1, value_2, &temp);
    one = s21_divCore(temp, divcopy, &one, status);
    *status = s21_checkAdd(*result, one, result);
  }

  return *result;
}

int s21_intDiv(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = OK;
  if (s21_isZero(value_2)) {
    status = DIV_ZERO;
  } else {
    s21_from_int_to_decimal(1, result);
    int sign = s21_getSign(value_1) ^ s21_getSign(value_2);
    s21_setSign(&value_1, 0);
    s21_setSign(&value_2, 0);
    int res_exp = s21_getExp(value_1) - s21_getExp(value_2);
    s21_setExp(&value_1, 0);
    s21_setExp(&value_2, 0);
    *result = s21_divCore(value_1, value_2, result, &status);
    if (res_exp < 0) {
      status = s21_shifting(result, -res_exp);
      status += -res_exp << 2;
      res_exp = 0;
    }
    s21_setExp(result, res_exp);
    s21_setSign(result, sign);
  }
  return status;
}

int s21_shifting(s21_decimal *a, int n) {
  s21_decimal ten;
  s21_from_int_to_decimal(10, &ten);
  s21_decimal tmp_result = s21_initZeroDecimal();
  int exp = s21_getExp(*a);
  int status = exp == s21_MAX_EXPONENT;
  while (!status && n--) {
    status = s21_justMul(*a, ten, &tmp_result);
    if (!status) {
      *a = tmp_result;
      tmp_result = s21_initZeroDecimal();
      status = (++exp == s21_MAX_EXPONENT);
    }
  }
  s21_setExp(a, exp);
  return status;
}

int s21_bankRound(s21_decimal *number, int n) {
  s21_decimal one, ten, hund;
  s21_from_int_to_decimal(1, &one);
  s21_from_int_to_decimal(10, &ten);
  s21_from_int_to_decimal(100, &hund);
  while (n--) {
    s21_decimal mod_res = s21_initZeroDecimal();
    s21_justMod(*number, hund, &mod_res);

    int exp = s21_getExp(*number) - 1;
    s21_intDiv(*number, ten, number);
    s21_setExp(number, exp);

    if (s21_bankRounding(mod_res.bits[0])) {
      s21_checkAdd(*number, one, number);
    }
  }
  return OK;
}

int s21_balancing(s21_decimal *a, s21_decimal *b) {
  int diff = s21_getExp(*a) - s21_getExp(*b);
  if (diff) {
    if (diff > 0)
      s21_shifting(b, diff);
    else
      s21_shifting(a, -diff);
    diff = s21_getExp(*a) - s21_getExp(*b);
    if (diff) {
      if (diff > 0)
        s21_bankRound(a, diff);
      else
        s21_bankRound(b, -diff);
    }
  }
  return OK;
}

int s21_justAdd(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int sign_1 = s21_getSign(value_1) == NEGATIVE;
  int sign_2 = s21_getSign(value_2) == NEGATIVE;
  int status;
  if (sign_1 && sign_2) {
    s21_setSign(&value_1, POSITIVE);
    s21_setSign(&value_2, POSITIVE);
    status = s21_checkAdd(value_1, value_2, result);
    s21_setSign(result, NEGATIVE);
  } else if (sign_1) {
    s21_setSign(&value_1, POSITIVE);
    status = s21_sub(value_2, value_1, result);
  } else if (sign_2) {
    s21_setSign(&value_2, POSITIVE);
    status = s21_sub(value_1, value_2, result);
  } else {
    status = s21_checkAdd(value_1, value_2, result);
  }
  return status;
}

int s21_addIntPrec(s21_decimal value_1, s21_decimal value_2,
                        s21_decimal *result, int status) {
  s21_setExp(&value_2, s21_getExp(value_2) - status);
  s21_decimal mod_res = s21_initZeroDecimal(), div_res = s21_initZeroDecimal();
  s21_mod(value_1, value_2, &mod_res);
  s21_shifting(&mod_res, -1);
  s21_setExp(&value_2, s21_getExp(value_2) + status);
  while (s21_justLess(mod_res, value_2)) s21_bankRound(&value_2, 1);
  s21_intDiv(mod_res, value_2, &div_res);
  status = s21_add(div_res, *result, result);
  return status;
}
