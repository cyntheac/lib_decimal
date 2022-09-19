#ifndef SRC_S21_KERNEL_H_
#define SRC_S21_KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/////////////////////////////////////////////////////////////////////////////
// constants
/////////////////////////////////////////////////////////////////////////////
#define s21_BITS_IN_INT 32
#define s21_BITS_COUNT 3
#define s21_ALL_BIT (s21_BITS_IN_INT * s21_BITS_COUNT)
#define s21_DECIMAL_INFO 3
#define s21_START_EXP_BIT 16
#define s21_END_EXP_BIT 23
#define s21_SIGN_BIT 31
#define s21_MAX_EXPONENT 28

/////////////////////////////////////////////////////////////////////////////
// enum and struct
/////////////////////////////////////////////////////////////////////////////
#define mul2(bit) (1 << bit)
typedef enum { false, true } bool;
enum { OK = 0, CONVERTION_ERROR = 1, BIG_VALUE = 1, SMALL_VALUE, DIV_ZERO };
typedef enum { POSITIVE = 0, NEGATIVE = 1 } sign;
typedef struct {
  int bits[4];
} s21_decimal;

/////////////////////////////////////////////////////////////////////////////
// kernel functions 1
/////////////////////////////////////////////////////////////////////////////
s21_decimal s21_initZeroDecimal();
void s21_setNull(s21_decimal *my_struct);
int s21_onBit(int bits, int bit);
int s21_offBit(int bits, int bit);
int s21_getBit(int bits, int bit);
int s21_getGbit(s21_decimal a, int gbit);
int s21_setBit(int bits, int bit, int new_bit);
int s21_getBits(s21_decimal a, int global_ind);
int *p_s21_getBits(s21_decimal *a, int global_ind);
int s21_isZero(s21_decimal a);
sign s21_getSign(s21_decimal a);
int s21_setSign(s21_decimal *a, sign new_sign);
int s21_getExp(s21_decimal a);
int s21_setExp(s21_decimal *a, int new_exponent);
int s21_bankRounding(int a);

/////////////////////////////////////////////////////////////////////////////
// kernel functions 2
/////////////////////////////////////////////////////////////////////////////
int s21_leftShift(s21_decimal *a);
int s21_rightShift(s21_decimal *a);
int s21_justLess(s21_decimal a, s21_decimal b);
int s21_justEqual(s21_decimal a, s21_decimal b);
int s21_ordinarySub(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result);
int s21_checkAdd(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result);

/////////////////////////////////////////////////////////////////////////////
// kernel functions 3
/////////////////////////////////////////////////////////////////////////////
s21_decimal s21_fmodCore(s21_decimal value_1, s21_decimal value_2);
int s21_justMod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_justMul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

#endif  // SRC_S21_KERNEL_H_
