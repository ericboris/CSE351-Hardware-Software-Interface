/* 
 * CS:APP Data Lab 
 * 
 * Eric Boris
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#ifndef COMMON_H
#include "common.h"
#endif

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
     not allowed to use big constants such as 0xffffffff.
     However, you are allowed to combine constants to values greater
     than 255 or less than 0. e.g. 250 + 250 = 500, so long as
     the operator you are using to combine the constants is listed as
     "legal" at the top of the method you are writing.
  2. Function arguments and local variables (no global variables).
  3. Any operators listed as "legal" at the top of the method you are writing.
  4. Shorthand versions of "legal" operators (ex. you can use ++ and += if + is legal)
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 */
#endif

/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  // We use DeMorgans law to solve for an alternate way to represent & 
  // without using an &. That is, the negation of the disjunction of 
  // negated x and y.
  return ~(~x | ~y);
}
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  // Similar to bitAnd, we can use DeMorgans to represent the equivalent
  // of XOR using only ~ and &.  
  return ~(x & y) & ~(~x & ~y);
}
/* 
 * thirdBits - return word with every third bit (starting from the LSB) set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int thirdBits(void) {
  /* 0x49249249 */
  /* [0100][1001][0010][0100][1001][0010][0100][1001] */
  // We make the largest representation allowable and store it in x.
  // Then we repeated shift left and add, shift and add to fill out the 
  // the signature. 
  int x = 0x49;
  x <<= 9;
  x += 0x49;
  x <<= 9;
  x += 0x49;
  x <<= 6;
  x += 0x09;
  return x;
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (least significant) to 3 (most significant)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  // Use a mask to retrieve only the two bytes from x at location n.
  return (0xFF) & (x >> (n << 3));
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  int xShift = x >> n;
  int bitMask = 0x1; // Create mask with of all zeros with LSB of 1
  bitMask <<= 0x1F;  // Shift mask to left by 32-1=31 making 1 MSB and rest 0
  bitMask = ~bitMask; // Negate to make all to 1 except for MSB = 0
  bitMask >>= n; // Shift right to make n 0s on left 
  bitMask <<= 1; // Shift back left by 1 becase right shift was 1 too many
  bitMask += 1; // Add 1 on right to fill 0 made by left shift
  return xShift & bitMask;
}
/*
 * invert - Return x with the n bits that begin at position p inverted
 *          (i.e., turn 0 into 1 and vice versa) and the rest left
 *          unchanged. Consider the indices of x to begin with the low-order
 *          bit numbered as 0.
 *   Can assume that 0 <= n <= 31 and 0 <= p <= 31
 *   Example: invert(0x80000000, 0, 1) = 0x80000001,
 *            invert(0x0000008e, 3, 3) = 0x000000b6,
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
*/
int invert(int x, int p, int n) {
  // Create a bitmask of the form, [0011 1100] where the ones will
  // will be used to flip the values in x and the zeroes will leave
  // those values alone. Ones begin at p and extend n bits to the left.
  int bitMask = 0x0;
  bitMask = ~bitMask;
  bitMask <<= n;
  bitMask = ~bitMask;
  bitMask <<= p;

  return x ^ bitMask;
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  return 2;
}
/* 
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 *  Examples: sign(130) = 1
 *            sign(-23) = -1
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 2
 */
int sign(int x) {
  // "Flatten" x to a 1 or zero and store it in y. When we or that 
  // with the rightshifted MSB from x we have three possible results, 
  // -1 if shifted x is -1 and y is 1, 0 if both are zero, and 1 if one
  // is 1 and the other is zero. 
  int y = !!x;
  x >>= 31;
  return x | y;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  // Shift the bits in x to the left and if potentially drop values off
  // the left edge. If we lose values this way the comparison will return
  // zero because the original is no longer equivalent with the modified
  // showing that it didn't fit in n. 
  int y = x;
  int shift = 32 + (~n + 1);
  x = ((x << shift) >> shift);
  return !(x ^ y);
}
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  // Check for three test cases. If any pass then we can safely add.
  // If x or y are zero then the result won't change. Adding zero makes
  // no difference. 
  int a = !x;
  int b = !y;
  int c = a | b;

  // Prepare the values for the other test cases. 
  int z = x + y;
  int signX = ((x>>31)<<31);
  int signY = ((y>>31)<<31);
  int signZ = ((z>>31)<<31);

  // If either sign value is negative then the sum of those will reduce
  // the value and not risk overflow. 
  // Similarly, if the signs on both x and y are 1 but they don't affect
  // the sign of the sum then they haven't overflowed.  
  int w = !!(signX ^ signY);
  int k = !((signX & signY) ^ signZ);

  return c | w | k;
}
/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
  return 2;
}
/* 
 * floatNegate - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatNegate(unsigned uf) {
  // There are two cases we need to check for. 1. We know that if 
  // The exponent is all 1 and the mantissa is not zero then the the
  // number is nan, return the input. Otherwise, the number is not nan
  // so just flip the sign bit. 
  int sign = 1 << 31;
  int exp = (uf << 1) >> 24;
  int man = uf << 9;
  int result;
  if (exp == 0xFF & man != 0) {
    result = uf;
  } else {
    result = sign ^ uf;
  }
  return result;
}
/* 
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 2
 */
int floatIsEqual(unsigned uf, unsigned ug) {
  // There are four cases to consider. Iff they're all true, then 
  // the floats are equivalent. 
  // First store the sign, exponent, and mantissa of each of the 
  // inputs. 
  int signA = uf >> 31;
  int expA = (uf << 1) >> 24;
  int manA = uf << 9;

  int signB = ug >>  31;
  int expB = (ug << 1) >> 24;
  int manB = ug << 9;
  
  // Also, create and store the decision variables. 
  int nan = 1; // exp all 1 and man != 0
  int sign = 0;
  int exp = 0;
  int man = 0;

  // Check if the value is nan. 
  if ((expA == 0xFF && manA) || (expB == 0xFF && manB)) {
    nan = 0;
  } // logical shift for unsigned int

  // Check whether the signs are equivalent and we aren't comparing
  // positive and negative 0.
  if ((!expA && !expB && !manA && !manB) || (signA == signB)) {
    sign = 1;
  }

  // Finally confirm the exponents and mantissa are equivalent. 
  if (expA == expB) {
    exp = 1;
  }
  if (manA == manB) {
    man = 1;
  } 

  // If they're all equivalent return 1. 
  return nan && sign && exp && man;
}
/* 
 * floatInt2Float - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatInt2Float(int x) {
  return 2;
}
