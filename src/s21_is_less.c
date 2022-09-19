#include "s21_decimal.h"

int s21_is_less(s21_decimal a, s21_decimal b) {
  int res = false;
  if (s21_isZero(a) && s21_isZero(b)) return res;

  if (s21_getSign(a) != s21_getSign(b)) {
    res = s21_getSign(a);
  } else {
    s21_balancing(&a, &b);
    res = s21_justLess(a, b);
    if (s21_getSign(a)) res ^= 1;
  }

  return res;
}
