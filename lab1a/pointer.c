/*
 * CSE 351 Lab 1a (Data Lab - Pointers)
 *
 * Eric Boris - eboris91
 *
 * pointer.c - Source file with your solutions to the Lab.
 *             This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>. The included file, "common.c" contains a function declaration
 * that should prevent a compiler warning. In general, it's not good practice
 * to ignore compiler warnings, but in this case it's OK.
 */

#ifndef COMMON_H
#include "common.h"
#endif

/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

#if 0
You will provide your solution to this homework by
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
  3. Any operators listed as "allowed" at the top of the method you are writing.
  4. casting.

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc. (unless
     otherwise stated).
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions (unless otherwise stated).
  5. Use any operators not "allowed" at the top of the method you are writing.

  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has *undefined* behavior when shifting by a negative amount or an amount
     greater than or equal to the number of bits in the value being shifted.
     e.g. For x >> n, shifts by n < 0 or n >= *size of x* are undefined
     e.g. if x is a 32-bit int, shifts by >= 32 bits are undefined
     Undefined means you do not know what result you will get from the operation.
#endif

/*
 * STEP 2: Modify the following functions according the coding rules.
 */

/*
 * Return the size of an integer in bytes.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *
 *   Unary integer operators: !
 *   Shorthand operators based on the above: ex. +=, *=, ++, --, eFDIStc.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, <<, >>, ==, !=, ^, /, %
 *   Unary integer operators: ~, -
 */
int intSize() {
  int intArray[10];
  int * intPtr1;
  int * intPtr2;
  // Write code to compute size of an integer.
  // Create an integer with a dummy value at a location in memory.
  // Access the address and, using pointer arithmetic, increment 
  // it's value by one (the width of integer in bytes). Then
  // subtract that new address from the original address value to get the
  // width of an integer. 
  int x = 1;

  intPtr1 = &x;
  intPtr2 = &x +1;
 
  return (unsigned long)intPtr2 - (unsigned long)intPtr1;
}
/*
 * Return the size of a double in bytes.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *
 *   Unary integer operators: !
 *   Shorthand operators based on the above: ex. +=, *=, ++, --, etc.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, <<, >>, ==, !=, ^, /, %
 *   Unary integer operators: ~, -
 */
int doubleSize() {
  double doubArray[10];
  double * doubPtr1;
  double * doubPtr2;
  // Write code to compute size of a double.
  // Create a double with a dummy value to create an address.
  // Access the address and, using pointer arithmetic, increment
  // it's value by one. Store both this incremented address and 
  // the default address. Subtract the two to get the size
  // of the double in bytes. 
  double x = 1;

  doubPtr1 = &x;
  doubPtr2 = &x + 1; 

  return (unsigned long)doubPtr2 - (unsigned long)doubPtr1;
}

/*
 * Return the size of a pointer in bytes.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *
 *   Unary integer operators: !
 *   Shorthand operators based on the above: ex. +=, *=, ++, --, etc.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, <<, >>, ==, !=, ^, /, %
 *   Unary integer operators: ~, -
 */
int pointerSize() {
  double * ptrArray[10];
  double ** ptrPtr1;
  double ** ptrPtr2;
  // Write code to compute size of a pointer.
  // Create a dummy value and a dummy pointer. 
  // Increment the address of the dummy pointer and store
  // it and it's initial value in two new pointer variables.
  // Take the difference of these values to find the size
  // of the pointer.
  double x = 1;
  double * ptr1 = &x;
  
  ptrPtr1 = &ptr1;
  ptrPtr2 = &ptr1 + 1;

  return (unsigned long)ptrPtr2 - (unsigned long)ptrPtr1;
}

/*
 * Given pointers to two distinct variables write a function to swap the values
 * of said variables.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *
 *   Unary integer operators: !
 *   Shorthand operators based on the above: ex. +=, *=, ++, --, etc.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, <<, >>, ==, !=, ^, /, %
 *   Unary integer operators: ~, -
 */
void swapInts(int *ptr1, int *ptr2) {
  // Your code here
  // Temporarily store the value at the first pointer.
  // Set the first pointer to the value of the second.
  // Set the value of the second pointer to that of the
  // first which was stored in the temporary variable.
  int temp = *ptr1;
  *ptr1 = *ptr2;
  *ptr2 = temp;
} 

/*
 * Modify intArray[5] to be the value 351 using only intArray and pointer arithmetic.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *, <<, >>, ==, ^
 *   Unary integer operators: !, ~
 *   Shorthand operators based on the above: ex. +=, *=, ++, --, ~= etc.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, !=, /, %
 *   Unary integer operators: -
 */
int changeValue() {
  int intArray[10];
  int * intPtr1 = intArray;
  // Remember not to use constants greater than 255.
  // Remember to use * to dereference. You cannot use '[<index>]' syntax.
  // Create a new pointer variable at the fifth index of the array. 
  // Set the value of the variable at that index to 351 by adding 251 and 100.
  // Do the addition because values greater than 255 are illegal.
  // Return the result.  
  int * intPtr2 = intPtr1 + 5;
  *intPtr2 = 251 + 100;

  return *intPtr2;
}

/*
 * Return 1 if the addresses stored in ptr1 and ptr2 are within the
 * *same* 64-byte aligned  block of memory. Check the spec for examples if you are
 * confused about what this means. Return zero otherwise.
 * Operators / and % and loops are NOT allowed.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *, <<, >>, ==, ^
 *   Unary integer operators: !, ~
 *   Shorthand operators based on the above: ex. <<=, *=, ++, --, etc.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, !=, /, %
 *   Unary integer operators: -
 */
int withinSameBlock(int *ptr1, int *ptr2) {
  // The six rightmost bits of both pointers should not be compared
  // and can be discarded. Do so and compare what's left. They 
  // will be the same if they are both within the same 64 byte
  // block of memory and different otherwise. Return the result
  // of this comparison. 
  return ((unsigned long)ptr1 >> 6) == ((unsigned long)ptr2 >> 6);
}

/*
 * Return 1 if ptr points to an element within the specified intArray, 0 otherwise.
 * Pointing anywhere in the array is fair game, ptr does not have to
 * point to the beginning of an element. Check the spec for examples if you are 
 * confused about what this method is determining.
 * size is the size of intArray in number of ints. Can assume size != 0.
 * Operators / and % and loops are NOT allowed.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *, <<, >>, ==, ^
 *   Unary integer operators: !, ~
 *   Shorthand operators based on the above: ex. <<=, *=, ++, --, etc.
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, !=, /, %
 *   Unary integer operators: -
 */
int withinArray(int * intArray, int size, int * ptr) {
  // Your code here
  // First, find the address of the last array index and store it in
  // intArray 2. Then compare whether ptr is greater or less than the
  // array start index. The result of this comparison will be negative if
  // it is greater or positive otherwise. Right bitshift away all the
  // insignificant bits except this result leaving a zero if greater or one if 
  // less than. Do a similar comparison with the pointer and the end
  // array index. After bitshifting away the insignificant bits,
  // the result will be 0 if the pointer is less than the the end array 
  // index or 1 otherwise. Store both of these bitshifted results into
  // variables and compare the results. If they are equal, both
  // resultant bits were zero and the pointer was within the array.
  // They will be different otherwise.
  int * intArray2 = intArray + size;
  unsigned long val1 = ((unsigned long)(intArray - ptr)) >> 63;
  unsigned long val2 = ((unsigned long)(ptr - intArray2)) >> 63;
  return val1 == val2;
}

/*
 * In C characters are are terminated by the null character ('\0')
 * given a pointer to the start of the string return the length of this string.
 * (The null character is not counted as part of the string length.)
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *, ==, !=, <, >
 *   Unary integer operators: !
 *   Shorthand operators based on the above: ex. <<=, *=, ++, --, etc.
 *   Control constructs: for, while
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <<, >>, ^, /, %
 *   Unary integer operators: ~, -
 */
int stringLength(char * s) {
  // Your code here
  // Simply initalize a counter to zero. Examine each element of the array
  // by incrementing the pointer address using pointer arithmetic and the 
  // sum of the pointer and counter. Continue incrementing this way until
  // the null character, '0', is encountered. Return the number of times 
  // counter was incremented.
  int counter = 0;

  while(*(s + counter) != 0) {
    counter++;
  }

  return counter;
}

/*
 * Change the value pointed to by ptr byte-by-byte so that when returned as an integer
 * the value is 351351.
 *
 * Hint: Remember that an int is 4 bytes.
 *
 * Hint: Remember how little endian works for data storage, how is it different between an multiple bytes(int) and a single byte?
 *
 * Hint: It will be easiest to start convert 351351 into binary form and starting seeing how the endian works from there.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *
 *   Shorthand operators based on the above: ex. +=, *=, ++, --, etc.
 *   Unary integer operators: !
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <, >, <<, >>, ==, !=, ^, /, %
 *   Unary integer operators: ~, -
 */
int endianExperiment(int* ptr) {
  char *bytePtr;
  // Your code here
  // First, find the byte value representation of 351351, i.e. 0x00055C77.
  // Then convert that to its little endian representation: 0x775C0500.
  // Break that value into four bytes: 0x77, 0x5C, 0x05, and 0x00.
  // Using pointer arithmetic, access each byte stored in the four byte
  // pointer and set its value to one of those found for the four bytes
  // above. 
  *((unsigned char *)ptr + 0) = 0x77;
  *((unsigned char *)ptr + 1) = 0x5C;
  *((unsigned char *)ptr + 2) = 0x05;
  *((unsigned char *)ptr + 3) = 0x00; 
  return *ptr;
}

/*
 * Selection sort is a sorting algorithm that works by partitioning the array into
 * a sorted section and unsorted section. Then it repeatedly selects the minimum element
 * from the unsorted section and moves it to the end of the sorted section.
 *
 * So the pseudo-code might look something like this:
 * arr - an array
 * n - the length of arr
 *
 * for i = 0 to n - 1
 *   minIndex = i
 *   for  j = i + 1 to n
 *       if arr[minIndex] > arr[j]
 *           minIndex = j
 *       end if
 *   end for
 *   Swap(arr[i], arr[minIndex])
 * end for
 *
 * Implement selection sort below, it might be helpful to use the swapInts function you
 * defined earlier.
 *
 * ALLOWED:
 *   Pointer operators: *, &
 *   Binary integer operators: -, +, *, ==, !=, <, >
 *   Unary integer operators: !
 *   Shorthand operators based on the above: ex. +=, *=, ++, --, etc.
 *   Control constructs: for, while, if
 *   Function calls: swapInt()
 *
 * DISALLOWED:
 *   Pointer operators: [] (Array Indexing Operator)
 *   Binary integer operators: &, &&, |, ||, <<, >>, ^, /
 *   Unary integer operators: ~, -
 */
void selectionSort(int arr[], int arrLength) {
  int i, j, min_index;
  // Your code here
  // Translate the above pseudocode into functional c. 
  // Create two nested loops. Each time the inner loop finds
  // a value that is greater than that maintained by the index
  // of the outer loop, increase the minimum index value to that
  // newly found smaller value. At the end of each inner loop 
  // iteration, swap the outer loop index with the current
  // least value in the array. Continue until the end of array
  // is reached and therefore sorted from least to greatest.
  for(i = 0; i < arrLength - 1; i++) {
    min_index = i;
    for(j = i + 1; j < arrLength; j++) {
      if (*(arr + i) > *(arr + j)) {
        min_index = j;
      }
    }
    swapInts((arr + i), (arr + min_index));    
  }
}
