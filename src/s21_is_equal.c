#include "s21_decimal.h"

int s21_is_equal(s21_decimal a, s21_decimal b) {
  s21_balancing(&a, &b);
  return s21_justEqual(a, b);
}
