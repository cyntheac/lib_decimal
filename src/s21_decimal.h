#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include "s21_kernel.h"

/////////////////////////////////////////////////////////////////////////////
// decimal
/////////////////////////////////////////////////////////////////////////////
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_intDiv(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

/////////////////////////////////////////////////////////////////////////////
// metaproj
/////////////////////////////////////////////////////////////////////////////
s21_decimal s21_divCore(s21_decimal value_1, s21_decimal value_2,
                     s21_decimal *result, int *status);
int s21_intDiv(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_shifting(s21_decimal *a, int n);
int s21_bankRound(s21_decimal *a, int n);
int s21_balancing(s21_decimal *a, s21_decimal *b);
int s21_justAdd(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_addIntPrec(s21_decimal value_1, s21_decimal value_2,
                        s21_decimal *result, int status);

#endif  // SRC_S21_DECIMAL_H_
