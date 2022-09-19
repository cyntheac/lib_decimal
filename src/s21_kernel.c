#include "s21_kernel.h"

// & (AND) -> if 1&1 == 1 else 0
// | (OR) -> if 0|0 == 0 else 1
// ^ (XOR) -> if (0^0) || (1^1) == 0 else 1
// ~ (NOT) -> reverse bits

/////////////////////////////////////////////////////////////////////////////
// kernel functions 1
/////////////////////////////////////////////////////////////////////////////
s21_decimal s21_initZeroDecimal() {
  s21_decimal new_decimal = {{0, 0, 0, 0}};
  return new_decimal;
}

void s21_setNull(s21_decimal *my_struct) {
  for (int i = 0; i < 4; i++) {
    my_struct->bits[i] = 0;
  }
}

int s21_onBit(int bits, int bit) {
  return bits | mul2(bit % s21_BITS_IN_INT);
}

int s21_offBit(int bits, int bit) {
  return bits & ~(mul2(bit % s21_BITS_IN_INT));
}

int s21_getBit(int bits, int bit) {
    return (bits & (mul2(bit % s21_BITS_IN_INT))) != 0;
}

int s21_getGbit(s21_decimal a, int gbit) {
  return s21_getBit(s21_getBits(a, gbit), gbit);
}

int s21_setBit(int bits, int bit, int new_bit) {
  if (new_bit) {
    bits = s21_onBit(bits, bit);
  } else {
    bits = s21_offBit(bits, bit);
  }
  return bits;
}

int s21_getBits(s21_decimal a, int global_ind) {
  return a.bits[(global_ind % s21_ALL_BIT) / s21_BITS_IN_INT];
}

int* p_s21_getBits(s21_decimal* a, int global_ind) {
  return a->bits + ((global_ind % s21_ALL_BIT) / s21_BITS_IN_INT);
}

int s21_isZero(s21_decimal a) { return !a.bits[0] && !a.bits[1] && !a.bits[2]; }

sign s21_getSign(s21_decimal a) {
    return s21_getBit(a.bits[s21_DECIMAL_INFO], s21_SIGN_BIT);
}

int s21_setSign(s21_decimal *a, sign new_sign) {
    a->bits[s21_DECIMAL_INFO] = s21_setBit(a->bits[s21_DECIMAL_INFO], s21_SIGN_BIT, new_sign);
    return 0;
}

int s21_getExp(s21_decimal a) {
  int exp = 0;
  for (int i = s21_START_EXP_BIT; i <= s21_END_EXP_BIT; i++) {
    if (s21_getBit(a.bits[s21_DECIMAL_INFO], i)) exp += (1 << (i - s21_START_EXP_BIT));
  }
  return exp;
}

int s21_setExp(s21_decimal *a, int new_exponent) {
  for (int i = s21_START_EXP_BIT; i <= s21_END_EXP_BIT; i++) {
    a->bits[s21_DECIMAL_INFO] = s21_setBit(a->bits[s21_DECIMAL_INFO], i, new_exponent & 1);
    new_exponent >>= 1;
  }

  return 0;
}

int s21_bankRounding(int a) {
  return (a % 10 > 5 || (a % 10 == 5 && (a / 10) % 2));
}

/////////////////////////////////////////////////////////////////////////////
// kernel functions 2
/////////////////////////////////////////////////////////////////////////////
int s21_leftShift(s21_decimal *a) {
  int n = 0;
  for (int i = 0; i < s21_BITS_COUNT; i++) {
    int tmp = s21_BITS_IN_INT * i;
    int old = s21_getGbit(*a, tmp + s21_BITS_IN_INT - 1);
    *p_s21_getBits(a, tmp) = s21_setBit(s21_getBits(*a, tmp) << 1, 0, n);
    n = old;
  }
  return 0;
}

int s21_rightShift(s21_decimal *a) {
  int n = 0;
  for (int i = s21_BITS_COUNT - 1; i >= 0; i--) {
    int tmp = s21_BITS_IN_INT * i;
    int old = s21_getGbit(*a, tmp);
    *p_s21_getBits(a, tmp) = s21_setBit(s21_getBits(*a, tmp) >> 1, tmp - 1, n);
    n = old;
  }
  return 0;
}

int s21_justLess(s21_decimal value_1, s21_decimal value_2) {
  int res = 0;
  for (int i = s21_ALL_BIT - 1; i >= 0; i--) {
    if (s21_getGbit(value_1, i) == s21_getGbit(value_2, i))
      continue;
    else if (s21_getGbit(value_1, i) < s21_getGbit(value_2, i))
      res = 1;
    break;
  }
  return res;
}

int s21_justEqual(s21_decimal a, s21_decimal b) {
  bool res = s21_getSign(a) == s21_getSign(b);
  bool alt_res = s21_isZero(a) && s21_isZero(b);
  res = res && s21_getExp(a) == s21_getExp(b);
  res = res && a.bits[0] == b.bits[0];
  res = res && a.bits[1] == b.bits[1];
  res = res && a.bits[2] == b.bits[2];

  return res || alt_res;
}

int s21_ordinarySub(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result) {
  int status = 0;

  for (int i = 0; i < s21_ALL_BIT; i++) {
    int a = s21_getGbit(value_1, i);
    int b = s21_getGbit(value_2, i);
    if (b && !a) {
      int j;
      for (j = i + 1; !s21_getGbit(value_1, j) && j < s21_ALL_BIT;) j++;

      *p_s21_getBits(&value_1, j) = s21_setBit(s21_getBits(value_1, j), j, 0);
      for (j--; j != i; j--) {
        *p_s21_getBits(&value_1, j) = s21_setBit(s21_getBits(value_1, j), j, 1);
      }
      *p_s21_getBits(result, i) = s21_setBit(s21_getBits(*result, i), i, 1);
    } else {
      *p_s21_getBits(result, i) = s21_setBit(s21_getBits(*result, i), i, a ^ b);
    }
  }
  return status;
}

int s21_checkAdd(s21_decimal value_1, s21_decimal value_2,
                    s21_decimal *result) {
  int res = OK;

  int additional_bit = 0;
  for (int i = 0; i < s21_ALL_BIT; i++) {
    int a = s21_getBit(s21_getBits(value_1, i), i);
    int b = s21_getBit(s21_getBits(value_2, i), i);

    *p_s21_getBits(result, i) =
        s21_setBit(s21_getBits(*result, i), i, a ^ b ^ additional_bit);
    additional_bit = (a && b) || (b && additional_bit) || (a && additional_bit);
  }

  if (additional_bit) res = BIG_VALUE;

  return res;
}

/////////////////////////////////////////////////////////////////////////////
// kernel functions 3
/////////////////////////////////////////////////////////////////////////////
s21_decimal s21_fmodCore(s21_decimal value_1, s21_decimal value_2) {
  s21_decimal divcopy = value_2;
  s21_decimal res;
  s21_decimal temp = {{0, 0, 0, 0}};
  if (s21_justEqual(value_1, value_2))
    return temp;
  else if (s21_justLess(value_1, value_2))
    return value_1;

  while ((s21_justLess(value_2, value_1) || s21_justEqual(value_2, value_1)) &&
         !s21_getGbit(value_2, s21_ALL_BIT - 1)) {
    s21_leftShift(&value_2);
  }
  if (s21_justLess(value_1, value_2)) {
    s21_rightShift(&value_2);
  }
  s21_ordinarySub(value_1, value_2, &temp);
  res = s21_fmodCore(temp, divcopy);
  return res;
}

int s21_justMod(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  if (s21_isZero(value_2)) return DIV_ZERO;
  result->bits[0] = 0;
  result->bits[1] = 0;
  result->bits[2] = 0;
  result->bits[3] = 0;
  int sign = s21_getSign(value_1);
  s21_setSign(&value_1, 0);
  s21_setSign(&value_2, 0);
  *result = s21_fmodCore(value_1, value_2);
  s21_setSign(result, sign);
  return 0;
}

int s21_justMul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int status = 0;
  s21_decimal tmp = *result;
  s21_setSign(&tmp, s21_getSign(value_1) ^ s21_getSign(value_2));
  while (!s21_isZero(value_2) && !status) {
    if (s21_getGbit(value_2, 0)) {
      status = s21_checkAdd(tmp, value_1, &tmp);
    }
    if (s21_getGbit(value_1, s21_ALL_BIT - 1)) status = 1;
    s21_rightShift(&value_2);
    s21_leftShift(&value_1);
  }
  if (!status)
    *result = tmp;
  else if (s21_getSign(value_1) ^ s21_getSign(value_2))
    ++status;
  return status;
}
