#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "unity.h"
#include "code.h"

/*
 * Every test below is a skeleton that already compiles and FAILS on purpose.
 * Replace the TEST_FAIL_MESSAGE line with your assertions.
 *
 * HOW A UNITY TEST LOOKS
 *   void test_something(void)
 *   {
 *       TEST_ASSERT_EQUAL_INT_MESSAGE(5, (int)strlen("hello"), "strlen(\"hello\") should be 5");
 *   }
 *   The FIRST argument is the value you EXPECT, the SECOND is the value the
 *   code under test ACTUALLY produces, the LAST is the text shown on failure.
 *   The first assertion that fails stops the test, so put the simple cases first.
 *
 * RULES
 *  - Only the standard library and Unity. No setUp()/tearDown().
 *  - Every value you assert must be written as a literal in your test - do NOT
 *    compute the expected value with the same function you are testing.
 *  - Compile with:
 *      gcc -std=c99 -g -Wall -Wextra -fsanitize=address,undefined main.c code.c unity.c -lm -o main
 *    (adjust the file names to your repository layout).
 */

/* ======================================================================
 * HELPERS (not tests)
 * ====================================================================== */

/* Given: the pattern for the other comparators. */
int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

/*
 * HELPER cmp_char
 *
 * Compare two chars ascending. Cast the pointers to const unsigned char *
 * and return (x > y) - (x < y).
 */

int cmp_char(const void *a, const void *b)
{
    const unsigned char x = *(const unsigned char *)a;
    const unsigned char y = *(const unsigned char *)b;
    return (x > y) - (x < y);
}

/*
 * HELPER cmp_strlen
 *
 * Each element is a char *, so a points to a char *:
 *     const char *x = *(const char * const *)a;
 * Return the comparison of strlen(x) and strlen(y), shorter first.
 */

int cmp_strlen(const void *a, const void *b)
{
    const char *x = *(const char * const *)a;
    const char *y = *(const char * const *)b;
    size_t len_x = strlen(x);
    size_t len_y = strlen(y);
    return (len_x > len_y) - (len_x < len_y);
}

/*
 * HELPER cmp_str
 *
 * Each element is a char *. Get both strings out of the elements (see
 * cmp_strlen) and return strcmp(x, y).
 */

int cmp_str(const void *a, const void *b)
{
    const char *x = *(const char * const *)a;
    const char *y = *(const char * const *)b;
    return strcmp(x, y);
}

/*
 * HELPER is_anagram
 *
 * Return 1 if a and b are anagrams, 0 otherwise.
 *  - If strlen(a) != strlen(b), or the length is 63 or more, return 0.
 *  - Copy both strings into char first[64] and char second[64] (strcpy).
 *  - Sort the characters of each copy: qsort(first, strlen(first), sizeof(char), cmp_char);
 *  - Return 1 if strcmp(first, second) == 0.
 */

int is_anagram(const char *a, const char *b)
{
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);

    if (len_a != len_b || len_a >= 63) {
        return 0;
    }

    char first[64];
    char second[64];

    strcpy(first, a);
    strcpy(second, b);

    qsort(first, len_a, sizeof(char), cmp_char);
    qsort(second, len_b, sizeof(char), cmp_char);

    return strcmp(first, second) == 0 ? 1 : 0;
}

/* ======================================================================
 * PART 1: Text and numbers (every case is listed - just turn them into assertions)
 * ====================================================================== */

/*
 * STEP 1: test_strlen_basic
 *
 * Verify strlen() on ordinary strings. Check that strlen("hello") is 5,
 * strlen("a") is 1, strlen("Hello, World!") is 13 and strlen("  x ") is 4
 * (spaces count). strlen() returns size_t, so cast the result to int before
 * you compare it.
 *
 * HINT: One assertion per string:  TEST_ASSERT_EQUAL_INT_MESSAGE(5, (int)strlen("hello"), "strlen(\"hello\") should be 5");
 *       The first argument is the EXPECTED value, the second is the ACTUAL value.
 */

void test_strlen_basic(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, (int)strlen("hello"), "strlen(\"hello\") should be 5");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, (int)strlen("a"), "strlen(\"a\") should be 1");
    TEST_ASSERT_EQUAL_INT_MESSAGE(13, (int)strlen("Hello, World!"), "strlen(\"Hello, World!\") should be 13");
    TEST_ASSERT_EQUAL_INT_MESSAGE(4, (int)strlen("  x "), "strlen(\"  x \") should be 4");
}

/*
 * STEP 2: test_strlen_edge_cases
 *
 * Verify the edge cases of strlen(). The empty string has length 0. For char
 * buf[10] = "abc"; strlen(buf) is 3 although sizeof(buf) is 10 (assert both
 * numbers). A string with a terminator inside, such as "ab\0cd", has length 2
 * because strlen() stops at the first '\0'. A string ending in a newline,
 * "line\n", has length 5.
 *
 * HINT: Declare the array and the embedded string as variables first:  const char embedded[] = "ab\0cd";
 */

void test_strlen_edge_cases(void)
{
    const char embedded[] = "ab\0cd";
    char buf[10] = "abc";
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, (int)strlen(""), "strlen(\"\") should be 0");
    TEST_ASSERT_EQUAL_INT_MESSAGE(3, (int)strlen(buf), "strlen(buf) should be 3");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, (int)strlen(embedded), "strlen(\"ab\\0cd\") should be 2");
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, (int)strlen("line\n"), "strlen(\"line\\n\") should be 5");
}

/*
 * STEP 3: test_strcmp_signs
 *
 * strcmp() returns a negative number, zero or a positive number. Only the SIGN
 * is specified, so never compare the result with -1 or 1. Assert that
 * strcmp("same", "same") is exactly 0. Assert every other result by checking a
 * comparison: for example TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("apple",
 * "banana") < 0, ...). Check: "apple" < "banana"; "banana" > "apple"; a prefix
 * is smaller ("app" < "apple" and "apple" > "app"); uppercase comes before
 * lowercase ("Zebra" < "apple" and "a" > "A"); the empty string is smaller
 * than any other string ("" < "a").
 *
 * HINT: A comparison in C gives 1 when it is true and 0 when it is false, so  (strcmp(a, b) < 0)  can be compared with the number 1.
 */

void test_strcmp_signs(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, strcmp("same", "same"), "strcmp(\"same\", \"same\") should be zero");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("apple", "banana") < 0, "strcmp(\"apple\", \"banana\") should be negative");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("banana", "apple") > 0, "strcmp(\"banana\", \"apple\") should be positive");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("app", "apple") < 0, "strcmp(\"app\", \"apple\") should be negative");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("apple", "app") > 0, "strcmp(\"apple\", \"app\") should be positive");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("Zebra", "apple") < 0, "strcmp(\"Zebra\", \"apple\") should be negative");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("a", "A") > 0, "strcmp(\"a\", \"A\") should be positive");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("", "a") < 0, "strcmp(\"\", \"a\") should be negative");
}

/*
 * STEP 4: test_atoi_valid
 *
 * Verify atoi() on valid input. Check: "123" gives 123, "0" gives 0, "-45"
 * gives -45, "+7" gives 7, "  42" (leading spaces are skipped) gives 42, "007"
 * gives 7 and "2147483647" gives 2147483647.
 *
 * HINT: atoi() returns int, so TEST_ASSERT_EQUAL_INT_MESSAGE can compare it directly.
 */

void test_atoi_valid(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(123, atoi("123"), "atoi(\"123\") should be 123");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, atoi("0"), "atoi(\"0\") should be 0");
    TEST_ASSERT_EQUAL_INT_MESSAGE(-45, atoi("-45"), "atoi(\"-45\") should be -45");
    TEST_ASSERT_EQUAL_INT_MESSAGE(7, atoi("+7"), "atoi(\"+7\") should be 7");
    TEST_ASSERT_EQUAL_INT_MESSAGE(42, atoi("  42"), "atoi(\"  42\") should be 42");
    TEST_ASSERT_EQUAL_INT_MESSAGE(7, atoi("007"), "atoi(\"007\") should be 7");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2147483647, atoi("2147483647"), "atoi(\"2147483647\") should be 2147483647");
}

/*
 * STEP 5: test_atoi_partial_and_invalid
 *
 * Verify what atoi() does with text that is not a clean number. Check: "12abc"
 * gives 12 (it reads the digits and stops), "abc" gives 0, "" gives 0, "-"
 * gives 0, "3.9" gives 3 (the fraction is ignored) and "1 2" gives 1 (it stops
 * at the space).
 *
 * HINT: atoi() has no way to report an error - it simply returns 0. That is why serious programs use strtol().
 */

void test_atoi_partial_and_invalid(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(12, atoi("12abc"), "atoi(\"12abc\") should be 12");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, atoi("abc"), "atoi(\"abc\") should be 0");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, atoi(""), "atoi(\"\") should be 0");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, atoi("-"), "atoi(\"-\") should be 0");
    TEST_ASSERT_EQUAL_INT_MESSAGE(3, atoi("3.9"), "atoi(\"3.9\") should be 3");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, atoi("1 2"), "atoi(\"1 2\") should be 1");
}

/*
 * STEP 6: test_atol
 *
 * Verify atol(). Check: "123456" gives 123456, "2000000000" gives 2000000000,
 * "-2000000000" gives -2000000000, "  -99" gives -99, "42xyz" gives 42 and "x"
 * gives 0. Keep all values within +-2,000,000,000: on a 32-bit system a long
 * has only 32 bits.
 *
 * HINT: atol() returns long. Cast it to int before the comparison:  (int)atol("123456")
 */

void test_atol(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(123456, (int)atol("123456"), "atol(\"123456\") should be 123456");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2000000000, (int)atol("2000000000"), "atol(\"2000000000\") should be 2000000000");
    TEST_ASSERT_EQUAL_INT_MESSAGE(-2000000000, (int)atol("-2000000000"), "atol(\"-2000000000\") should be -2000000000");
    TEST_ASSERT_EQUAL_INT_MESSAGE(-99, (int)atol("  -99"), "atol(\"  -99\") should be -99");
    TEST_ASSERT_EQUAL_INT_MESSAGE(42, (int)atol("42xyz"), "atol(\"42xyz\") should be 42");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, (int)atol("x"), "atol(\"x\") should be 0");
}

/*
 * STEP 7: test_atof
 *
 * Verify atof(). Check: "3.5" gives 3.5, "-0.25" gives -0.25, "1e3" gives
 * 1000.0 (scientific notation), "  2.5xyz" gives 2.5, "abc" gives 0.0, ".5"
 * gives 0.5 and "42" gives 42.0.
 *
 * HINT: Use TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(expected, actual, "message"). Unity compares doubles with a very small tolerance, because most decimal numbers cannot be stored exactly in binary.
 */

void test_atof(void)
{
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(3.5, atof("3.5"), "atof(\"3.5\") should be 3.5");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-0.25, atof("-0.25"), "atof(\"-0.25\") should be -0.25");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1e3, atof("1e3"), "atof(\"1e3\") should be 1000.0");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(2.5, atof("  2.5xyz"), "atof(\"  2.5xyz\") should be 2.5");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, atof("abc"), "atof(\"abc\") should be 0.0");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(.5, atof(".5"), "atof(\".5\") should be 0.5");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(42, atof("42"), "atof(\"42\") should be 42.0");
}

/* ======================================================================
 * PART 2: Math (cases are listed; choose the right assertion)
 * ====================================================================== */

/*
 * STEP 8: test_sqrt
 *
 * Verify sqrt(). Check: sqrt(0) is 0, sqrt(1) is 1, sqrt(16) is 4, sqrt(144)
 * is 12, sqrt(0.25) is 0.5, sqrt(1000000) is 1000 and sqrt(2) is
 * 1.4142135623730951.
 *
 * HINT: sqrt(2) is irrational. TEST_ASSERT_EQUAL_DOUBLE_MESSAGE still works, because of the tolerance.
 */

void test_sqrt(void)
{
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, sqrt(0), "sqrt(0) should be 0");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, sqrt(1), "sqrt(1) should be 1");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(4.0, sqrt(16), "sqrt(16) should be 4");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(12.0, sqrt(144), "sqrt(144) should be 12");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.5, sqrt(0.25), "sqrt(0.25) should be 0.5");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1000.0, sqrt(1000000), "sqrt(1000000) should be 1000");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.4142135623730951, sqrt(2), "sqrt(2) should be approximately 1.4142135623730951");
}

/*
 * STEP 9: test_pow
 *
 * Verify pow(). Check: pow(2, 10) is 1024; pow(5, 0) is 1; pow(0, 0) is 1;
 * pow(2, -1) is 0.5 (negative exponent); pow(-2, 3) is -8 and pow(-2, 2) is 4
 * (negative base); pow(9, 0.5) is 3 (fractional exponent); pow(10, 2) is 100;
 * pow(1, 1000) is 1.
 *
 * HINT: All arguments of pow() are doubles: write 2.0 and 10.0.
 */

void test_pow(void)
{
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1024.0, pow(2.0, 10.0), "pow(2, 10) should be 1024");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, pow(5.0, 0.0), "pow(5, 0) should be 1");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, pow(0.0, 0.0), "pow(0, 0) should be 1");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.5, pow(2.0, -1.0), "pow(2, -1) should be 0.5");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-8.0, pow(-2.0, 3.0), "pow(-2, 3) should be -8");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(4.0, pow(-2.0, 2.0), "pow(-2, 2) should be 4");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(3.0, pow(9.0, 0.5), "pow(9, 0.5) should be 3");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(100.0, pow(10.0, 2.0), "pow(10, 2) should be 100");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, pow(1.0, 1000.0), "pow(1, 1000) should be 1");
}

/*
 * STEP 10: test_floor
 *
 * Verify floor(), which rounds DOWN. Check: floor(2.7) is 2, floor(2.0) is 2,
 * floor(0.5) is 0, floor(-2.1) is -3, floor(-2.0) is -2 and floor(-0.5) is -1.
 * The negative numbers are the interesting part: down means toward minus
 * infinity, not toward zero.
 *
 * HINT: The result is a double: use TEST_ASSERT_EQUAL_DOUBLE_MESSAGE with 2.0, -3.0, ...
 */

void test_floor(void)
{
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(2.0, floor(2.7), "floor(2.7) should be 2");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(2.0, floor(2.0), "floor(2.0) should be 2");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, floor(0.5), "floor(0.5) should be 0");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-3.0, floor(-2.1), "floor(-2.1) should be -3");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-2.0, floor(-2.0), "floor(-2.0) should be -2");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-1.0, floor(-0.5), "floor(-0.5) should be -1");
}

/*
 * STEP 11: test_ceil
 *
 * Verify ceil(), which rounds UP. Check: ceil(2.1) is 3, ceil(2.0) is 2,
 * ceil(0.5) is 1, ceil(-2.7) is -2, ceil(-2.0) is -2 and ceil(5.0) is 5. A
 * value that is already an integer must stay unchanged.
 *
 * HINT: Compare with floor() from the previous step: they differ for 2.1 and agree for 2.0.
 */

void test_ceil(void)
{
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(3.0, ceil(2.1), "ceil(2.1) should be 3");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(2.0, ceil(2.0), "ceil(2.0) should be 2");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, ceil(0.5), "ceil(0.5) should be 1");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-2.0, ceil(-2.7), "ceil(-2.7) should be -2");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-2.0, ceil(-2.0), "ceil(-2.0) should be -2");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5.0, ceil(5.0), "ceil(5.0) should be 5");
}

/*
 * STEP 12: test_round
 *
 * Verify round(), which rounds to the NEAREST integer, with halves going AWAY
 * from zero. Check: round(2.4) is 2, round(2.5) is 3, round(2.6) is 3,
 * round(-2.4) is -2, round(-2.5) is -3, round(-2.6) is -3, round(0.0) is 0 and
 * round(7.0) is 7.
 *
 * HINT: The negative halves (-2.5) are where a wrong rounding rule shows up.
 */

void test_round(void)
{
    TEST_ASSERT_EQUAL_MESSAGE(2, round(2.4), "round(2.4) should be 2");
    TEST_ASSERT_EQUAL_MESSAGE(3, round(2.5), "round(2.5) should be 3");
    TEST_ASSERT_EQUAL_MESSAGE(3, round(2.6), "round(2.6) should be 3");
    TEST_ASSERT_EQUAL_MESSAGE(-2, round(-2.4), "round(-2.4) should be -2");
    TEST_ASSERT_EQUAL_MESSAGE(-3, round(-2.5), "round(-2.5) should be -3");
    TEST_ASSERT_EQUAL_MESSAGE(-3, round(-2.6), "round(-2.6) should be -3");
    TEST_ASSERT_EQUAL_MESSAGE(0, round(0.0), "round(0.0) should be 0");
    TEST_ASSERT_EQUAL_MESSAGE(7, round(7.0), "round(7.0) should be 7");
}

/*
 * STEP 13: test_fmod
 *
 * Verify fmod(), the remainder of a floating point division. Check: fmod(7.5,
 * 2) is 1.5; fmod(10, 3) is 1; fmod(9, 3) is 0; fmod(-7.5, 2) is -1.5 and
 * fmod(7.5, -2) is 1.5 (the result has the sign of the FIRST argument);
 * fmod(2, 5) is 2.
 *
 * HINT: This is the operator % for doubles, which C does not have.
 */

void test_fmod(void)
{
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.5, fmod(7.5, 2.0), "fmod(7.5, 2) should be 1.5");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, fmod(10.0, 3.0), "fmod(10, 3) should be 1");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, fmod(9.0, 3.0), "fmod(9, 3) should be 0");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-1.5, fmod(-7.5, 2.0), "fmod(-7.5, 2) should be -1.5");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.5, fmod(7.5, -2.0), "fmod(7.5, -2) should be 1.5");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(2.0, fmod(2.0, 5.0), "fmod(2, 5) should be 2");
}

/*
 * STEP 14: test_floor_ceil_round_relation
 *
 * Combine floor(), ceil() and round() in one test, using loops over arrays.
 * For every value of {0.1, 2.5, -2.5, -0.9, 100.99, -100.01} check that
 * ceil(x) - floor(x) is exactly 1, that floor(x) <= x, that x <= ceil(x), and
 * that round(x) lies between floor(x) and ceil(x). For every value of {-3.0,
 * 0.0, 8.0} check that floor(x), ceil(x) and round(x) all equal x.
 *
 * HINT: Declare const double arrays and loop over them. Turn the comparisons into numbers with TEST_ASSERT_EQUAL_INT_MESSAGE(1, floor(x) <= x, ...). A relation that must hold for MANY values is a stronger test than a few single examples.
 */

void test_floor_ceil_round_relation(void)
{
    const double test_values[] = {0.1, 2.5, -2.5, -0.9, 100.99, -100.01};
    const size_t num_test_values = sizeof(test_values) / sizeof(test_values[0]);
    const double exact_values[] = {-3.0, 0.0, 8.0};
    const size_t num_exact_values = sizeof(exact_values) / sizeof(exact_values[0]);

    // Test the relations for the first set of values
    for (size_t i = 0; i < num_test_values; i++) {
        double x = test_values[i];
        double floored = floor(x);
        double ceiled = ceil(x);
        double rounded = round(x);

        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(ceiled - floored, 1.0, "ceil(x) - floor(x) should be 1");
        TEST_ASSERT_TRUE_MESSAGE(floored <= x, "floor(x) should be less than or equal to x");
        TEST_ASSERT_TRUE_MESSAGE(x <= ceiled, "x should be less than or equal to ceil(x)");
        TEST_ASSERT_TRUE_MESSAGE(floored <= rounded && rounded <= ceiled, "round(x) should lie between floor(x) and ceil(x)");
    }

    // Test the exact values
    for (size_t i = 0; i < num_exact_values; i++) {
        double x = exact_values[i];
        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(floor(x), x, "floor(x) should equal x");
        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(ceil(x), x, "ceil(x) should equal x");
        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(round(x), x, "round(x) should equal x");
    }
}

/* ======================================================================
 * PART 3: Memory, strings and formatting
 * ====================================================================== */

/*
 * STEP 15: test_memcpy_int_array
 *
 * Verify memcpy() on an int array. Copy int src[5] = {1, 2, 3, 4, -1} into a
 * zeroed int dest[5] with memcpy(dest, src, sizeof src) and check that dest
 * equals src. Check that memcpy() returns the destination pointer. Then copy
 * only the first 3 ints (3 * sizeof(int) bytes) into a freshly zeroed array
 * and check that the result is {1, 2, 3, 0, 0}.
 *
 * HINT: TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected, actual, number_of_elements, "message") compares two int arrays element by element. The size of memcpy() is in BYTES, the number of elements is not.
 *       Why -1 as the last value? The int -1 has ALL its bytes non-zero. If memcpy() copied one byte too few, the missing byte would be 0 in a zeroed destination and a value like 5 would still look correct - a test that cannot see the bug.
 *       A pointer comparison such as  (returned == dest)  gives 1 or 0.
 */

void test_memcpy_int_array(void)
{
    int src[5] = {1, 2, 3, 4, -1};
    int dest[5] = {0};
    int dest2[5] = {0};

    // Test memcpy() on the entire array
    void *returned = memcpy(dest, src, sizeof src);
    TEST_ASSERT_EQUAL_PTR_MESSAGE(dest, returned, "memcpy() should return the destination pointer");
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(src, dest, 5, "memcpy() should copy the entire array");

    // Test memcpy() on a portion of the array
    memset(dest2, 0, sizeof dest2);
    memcpy(dest2, src, 3 * sizeof(int));
    int expected2[5] = {1, 2, 3, 0, 0};
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected2, dest2, 5, "memcpy() should copy only the specified number of bytes");
}

/*
 * STEP 16: test_memcpy_string_and_zero_bytes
 *
 * Verify memcpy() on text. Prepare char buf[10] with the first 9 bytes set to
 * 'X' (use memset) and the last byte set to '\0'. (1) memcpy(buf, "hello", 6)
 * copies the text and its terminator: buf must be the string "hello" and
 * buf[6] must still be 'X'. (2) Refill buf with 'X', copy only 5 bytes, and
 * check that the first 9 bytes are "helloXXXX" (no terminator was copied). (3)
 * Refill buf and copy 0 bytes: buf[0] must still be 'X'.
 *
 * HINT: Pre-filling the buffer is what makes the test meaningful: a bug that writes too much or too little would be invisible in a buffer full of zeros.
 *       TEST_ASSERT_EQUAL_MEMORY_MESSAGE(expected, actual, number_of_bytes, "message") compares raw bytes. Single characters can be compared as numbers with TEST_ASSERT_EQUAL_INT_MESSAGE('X', buf[6], ...).
 */

void test_memcpy_string_and_zero_bytes(void)
{
    char buf[10];
    memset(buf, 'X', 9);
    buf[9] = '\0';

    // Test memcpy() on a string with terminator
    memcpy(buf, "hello", 6);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("hello", buf, "memcpy() should copy the string and its terminator");
    TEST_ASSERT_EQUAL_INT_MESSAGE('X', buf[6], "The byte after the copied string should remain 'X'");

    // Test memcpy() on a string without terminator
    memset(buf, 'X', 10);
    buf[9] = '\0';
    memcpy(buf, "hello", 5);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("hello", buf, "memcpy() should copy the string without the terminator");
    TEST_ASSERT_EQUAL_INT_MESSAGE('X', buf[5], "The byte after the copied string should remain 'X'");

    // Test memcpy() with zero bytes
    memset(buf, 'X', 10);
    buf[9] = '\0';
    memcpy(buf, "hello", 0);
    TEST_ASSERT_EQUAL_INT_MESSAGE('X', buf[0], "Copying zero bytes should not change the buffer");
}

/*
 * STEP 17: test_memset_and_memcmp
 *
 * Verify memset() and memcmp(). With unsigned char buf[8]: memset(buf, 'A', 8)
 * must fill every byte (check the last one); memset(buf, 0, 8) must give 8
 * zero bytes (compare with an array of zeros); after memset(buf, 'B', 4) the
 * 4th byte is 'B' and the 5th byte is still 0. Then compare the strings a =
 * "abcdX" and b = "abcdY": memcmp(a, a, 5) is 0; memcmp(a, b, 4) is 0 (the
 * difference is outside the first 4 bytes); memcmp(a, b, 5) is negative and
 * memcmp(b, a, 5) is positive.
 *
 * HINT: Like strcmp(), memcmp() promises only the sign of its result. The last byte decides here - a buggy memcmp() that looks only at the first byte would not notice it.
 */

void test_memset_and_memcmp(void)
{
    unsigned char buf[8];
    unsigned char expected[8];

    // Test memset() with 'A'
    memset(buf, 'A', 8);
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE('A', buf[i], "memset() should fill every byte with the specified value");
    }

    // Test memset() with 0
    memset(buf, 0, 8);
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE(0, buf[i], "memset() should fill every byte with zero");
    }

    // Test memset() with 'B' for the first 4 bytes
    memset(buf, 'B', 4);
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE('B', buf[i], "memset() should fill the specified number of bytes");
    }
    for (int i = 4; i < 8; i++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE(0, buf[i], "memset() should not change bytes outside the specified range");
    }

    // Test memcmp() with identical strings
    char a[] = "abcdX";
    char b[] = "abcdY";
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, memcmp(a, a, 5), "memcmp() should return 0 for identical strings");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, memcmp(a, b, 4), "memcmp() should return 0 for the first n bytes of different strings");
    TEST_ASSERT_TRUE_MESSAGE(memcmp(a, b, 5) < 0 && memcmp(b, a, 5) > 0, "memcmp() should return negative for a < b and positive for a > b");
}

/*
 * STEP 18: test_strcpy_strcat
 *
 * Verify strcpy() and strcat(). Prepare char buf[20] with the first 19 bytes
 * set to 'X' and the last byte set to '\0'. strcpy(buf, "abc") must give the
 * string "abc", buf[3] must be '\0' (the terminator is copied), buf[4] must
 * still be 'X', and the function must return buf itself. strcat(buf, "def")
 * must give "abcdef" and return buf. strcat(buf, "") must change nothing.
 * strcat(buf, "!") must give "abcdef!", whose strlen() is 7.
 *
 * HINT: A pre-filled buffer shows whether the terminator was really copied: if strcpy() forgot it, the 'X' characters after "abc" would still be part of the string.
 */

void test_strcpy_strcat(void)
{
    char buf[20];
    memset(buf, 'X', 19);
    buf[19] = '\0';

    // Test strcpy()
    char *returned = strcpy(buf, "abc");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("abc", buf, "strcpy() should copy the string and its terminator");
    TEST_ASSERT_EQUAL_INT_MESSAGE('\0', buf[3], "The byte after the copied string should be the null terminator");
    TEST_ASSERT_EQUAL_INT_MESSAGE('X', buf[4], "The byte after the null terminator should remain 'X'");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(buf, returned, "strcpy() should return the destination pointer");

    // Test strcat()
    returned = strcat(buf, "def");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("abcdef", buf, "strcat() should concatenate the strings");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(buf, returned, "strcat() should return the destination pointer");

    // Test strcat() with an empty string
    returned = strcat(buf, "");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("abcdef", buf, "strcat() with an empty string should not change the buffer");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(buf, returned, "strcat() should return the destination pointer");

    // Test strcat() with a single character
    returned = strcat(buf, "!");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("abcdef!", buf, "strcat() should concatenate a single character");
    TEST_ASSERT_EQUAL_INT_MESSAGE(7, (int)strlen(buf), "The length of the concatenated string should be 7");
}

/*
 * STEP 19: test_strchr
 *
 * Verify strchr(). In s = "hello": the first 'l' is at index 2, 'h' is at
 * index 0 and 'o' is at index 4. A character that does not occur ('z') gives
 * NULL. The terminator can be searched for too: strchr(s, '\0') points to
 * index 5. In an empty string nothing is found: strchr("", 'a') is NULL.
 *
 * HINT: strchr() returns a POINTER. The index is the difference of two pointers:  (int)(found - s).
 *       Compare a pointer with NULL like this:  TEST_ASSERT_EQUAL_INT_MESSAGE(1, found == NULL, "...");
 */

void test_strchr(void)
{
    char s[] = "hello";
    char *found;

    // Test for a character that exists
    found = strchr(s, 'l');
    TEST_ASSERT_NOT_EQUAL_PTR_MESSAGE(NULL, found, "strchr() should return a pointer to the first occurrence of the character");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, (int)(found - s), "The index of the first 'l' should be 2");

    // Test for a character that does not exist
    found = strchr(s, 'z');
    TEST_ASSERT_EQUAL_PTR_MESSAGE(NULL, found, "strchr() should return NULL for a character that does not occur");

    // Test for the terminator
    found = strchr(s, '\0');
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, (int)(found - s), "The index of the null terminator should be 5");

    // Test with an empty string
    char empty[] = "";
    found = strchr(empty, 'a');
    TEST_ASSERT_EQUAL_PTR_MESSAGE(NULL, found, "strchr() should return NULL for an empty string");
}

/*
 * STEP 20: test_sprintf_integers
 *
 * Verify sprintf() with integer formats, using char buf[64]. Check the text
 * produced and, for the first two calls, also the return value (the number of
 * characters written): "%d" of 42 gives "42" (returns 2); "%d" of -7 gives
 * "-7" (returns 2); "%05d" of 42 gives "00042"; "%5d" of 42 gives "   42";
 * "%-5d|" of 42 gives "42   |"; "%x" of 255 gives "ff"; "%X" of 255 gives
 * "FF"; "%o" of 8 gives "10"; "%+d" of 5 gives "+5".
 *
 * HINT: TEST_ASSERT_EQUAL_STRING_MESSAGE(expected, actual, "message") compares two strings with strcmp().
 */

void test_sprintf_integers(void)
{
    char buf[64];
    int result;

    // Test "%d" of 42
    result = sprintf(buf, "%d", 42);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("42", buf, "sprintf() with \"%d\" should produce the correct string");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, result, "sprintf() with \"%d\" should return the correct number of characters");

    // Test "%d" of -7
    result = sprintf(buf, "%d", -7);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("-7", buf, "sprintf() with \"%d\" should produce the correct string");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, result, "sprintf() with \"%d\" should return the correct number of characters");

    // Test "%05d" of 42
    result = sprintf(buf, "%05d", 42);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("00042", buf, "sprintf() with \"%05d\" should produce the correct string");
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, result, "sprintf() with \"%05d\" should return the correct number of characters");

    // Test "%5d" of 42
    result = sprintf(buf, "%5d", 42);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("   42", buf, "sprintf() with \"%5d\" should produce the correct string");
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, result, "sprintf() with \"%5d\" should return the correct number of characters");

    // Test "%-5d|" of 42
    result = sprintf(buf, "%-5d|", 42);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("42   |", buf, "sprintf() with \"%-5d|\" should produce the correct string");
    TEST_ASSERT_EQUAL_INT_MESSAGE(6, result, "sprintf() with \"%-5d|\" should return the correct number of characters");

    // Test "%x" of 255
    result = sprintf(buf, "%x", 255);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("ff", buf, "sprintf() with \"%x\" should produce the correct string");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, result, "sprintf() with \"%x\" should return the correct number of characters");

    // Test "%X" of 255
    result = sprintf(buf, "%X", 255);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("FF", buf, "sprintf() with \"%X\" should produce the correct string");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, result, "sprintf() with \"%X\" should return the correct number of characters");

    // Test "%o" of 8
    result = sprintf(buf, "%o", 8);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("10", buf, "sprintf() with \"%o\" should produce the correct string");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, result, "sprintf() with \"%o\" should return the correct number of characters");

    // Test "%+d" of 5
    result = sprintf(buf, "%+d", 5);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("+5", buf, "sprintf() with \"%+d\" should produce the correct string");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, result, "sprintf() with \"%+d\" should return the correct number of characters");
}

/*
 * STEP 21: test_sprintf_strings_and_doubles
 *
 * Verify sprintf() with text and floating point formats, using char buf[64].
 * "%s" of "abc" gives "abc"; "%c" of 'Z' gives "Z"; "%-6s|" of "ab" gives "ab
 * |"; "%6s" of "ab" gives "    ab"; "%.2s" of "abcdef" gives "ab"; "%.2f" of
 * 3.14159 gives "3.14"; "%.3f" of 2.0 gives "2.000"; "%8.2f" of 3.14159 gives
 * "    3.14"; "%f" of 0.5 gives "0.500000"; and the format "100%%" gives
 * "100%".
 *
 * HINT: Count the spaces carefully: a width such as %8.2f pads the number on the left to 8 characters.
 */

void test_sprintf_strings_and_doubles(void)
{
    char buf[64];

    TEST_ASSERT_EQUAL_STRING_MESSAGE("abc", sprintf(buf, "%s", "abc"), "sprintf() with \"%s\" should produce the correct string");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Z", sprintf(buf, "%c", 'Z'), "sprintf() with \"%c\" should produce the correct string");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("ab   |", sprintf(buf, "%-6s|", "ab"), "sprintf() with \"%-6s|\" should produce the correct string");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("    ab", sprintf(buf, "%6s", "ab"), "sprintf() with \"%6s\" should produce the correct string");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("ab", sprintf(buf, "%.2s", "abcdef"), "sprintf() with \"%.2s\" should produce the correct string");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("3.14", sprintf(buf, "%.2f", 3.14159), "sprintf() with \"%.2f\" should produce the correct string");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("2.000", sprintf(buf, "%.3f", 2.0), "sprintf() with \"%.3f\" should produce the correct string");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("    3.14", sprintf(buf, "%8.2f", 3.14159), "sprintf() with \"%8.2f\" should produce the correct string");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("0.500000", sprintf(buf, "%f", 0.5), "sprintf() with \"%f\" should produce the correct string");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("100%%", sprintf(buf, "100%%"), "sprintf() with \"100%%\" should produce the correct string");
}

/*
 * STEP 22: test_sprintf_return_matches_strlen
 *
 * sprintf() returns the number of characters it wrote (not counting the
 * terminator). For each of these calls assert the expected return value AND
 * that it equals strlen(buf): "%d" of 12345 (5); "%s-%s" of "ab" and "cd" (5);
 * "%5.2f" of 3.14159 (5); "%s" of "" (0 - and the buffer must be the empty
 * string); "%s: %s, %s: %d" of "Name", "Ann", "Age", 21, which writes "Name:
 * Ann, Age: 21" (18 characters).
 *
 * HINT: This test uses sprintf() and strlen() together: if either one is wrong, the two numbers disagree.
 */

void test_sprintf_return_matches_strlen(void)
{
    char buf[64];
    int ret;

    ret = sprintf(buf, "%d", 12345);
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, ret, "sprintf() should return the correct number of characters");
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, (int)strlen(buf), "strlen() should match the return value of sprintf()");

    ret = sprintf(buf, "%s-%s", "ab", "cd");
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, ret, "sprintf() should return the correct number of characters");
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, (int)strlen(buf), "strlen() should match the return value of sprintf()");

    ret = sprintf(buf, "%5.2f", 3.14159);
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, ret, "sprintf() should return the correct number of characters");
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, (int)strlen(buf), "strlen() should match the return value of sprintf()");

    ret = sprintf(buf, "%s", "");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, ret, "sprintf() should return the correct number of characters for an empty string");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, (int)strlen(buf), "strlen() should match the return value of sprintf() for an empty string");

    ret = sprintf(buf, "%s: %s, %s: %d", "Name", "Ann", "Age", 21);
    TEST_ASSERT_EQUAL_INT_MESSAGE(18, ret, "sprintf() should return the correct number of characters");
    TEST_ASSERT_EQUAL_INT_MESSAGE(18, (int)strlen(buf), "strlen() should match the return value of sprintf()");
}

/* ======================================================================
 * PART 4: qsort and integration (you choose the inputs - the checklist tells you what must be covered)
 * ====================================================================== */

/*
 * STEP 23: test_qsort_ints
 *
 * Verify qsort() on int arrays, sorted ascending with the given comparator
 * cmp_int. Your test must cover: an unsorted array {5, 2, 9, 1, 7} (becomes
 * {1, 2, 5, 7, 9}); a reversed array; an already sorted array (stays
 * unchanged); an array with duplicates {3, 1, 3, 1}; an array with negative
 * numbers; an array whose SMALLEST element is the LAST one (for example {9, 8,
 * 7, 0}); an array with a single element; and count 0, which must not touch
 * the array (use an array you can check afterwards).
 *
 * HINT: qsort(base, count, sizeof(int), cmp_int);  Compare the whole array with TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE against an array that holds the expected result.
 *       A test that only uses already sorted data can never fail - use unsorted inputs.
 */

void test_qsort_ints(void)
{  
    int arr1[] = {5, 2, 9, 1, 7};
    qsort(arr1, 5, sizeof(int), cmp_int);
    int e_arr1[] = {1, 2, 5, 7, 9};
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(e_arr1, arr1, 5, "qsort() should sort the array {5, 2, 9, 1, 7} into {1, 2, 5, 7, 9}");
    int arr2[] = {9, 8, 7, 6, 5};
    qsort(arr2, 5, sizeof(int), cmp_int);
    int e_arr2[] = {5, 6, 7, 8, 9};
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(e_arr2, arr2, 5, "qsort() should sort the array {9, 8, 7, 6, 5} into {5, 6, 7, 8, 9}");
    int arr3[] = {1, 2, 3, 4, 5};
    qsort(arr3, 5, sizeof(int), cmp_int);
    int e_arr3[] = {1, 2, 3, 4, 5};
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(e_arr3, arr3, 5, "qsort() should leave the already sorted array {1, 2, 3, 4, 5} unchanged");
    int arr4[] = {3, 1, 3, 1};
    qsort(arr4, 4, sizeof(int), cmp_int);
    int e_arr4[] = {1, 1, 3, 3};
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(e_arr4, arr4, 4, "qsort() should sort the array {3, 1, 3, 1} into {1, 1, 3, 3}");
    int arr5[] = {-3, -1, -2, 0, 2};
    qsort(arr5, 5, sizeof(int), cmp_int);
    int e_arr5[] = {-3, -2, -1, 0, 2};
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(e_arr5, arr5, 5, "qsort() should sort the array {-3, -1, -2, 0, 2} into {-3, -2, -1, 0, 2}");
    int arr6[] = {9, 8, 7, 0};
    qsort(arr6, 4, sizeof(int), cmp_int);
    int e_arr6[] = {0, 7, 8, 9};
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(e_arr6, arr6, 4, "qsort() should sort the array {9, 8, 7, 0} into {0, 7, 8, 9}");
    int arr7[] = {42};
    qsort(arr7, 1, sizeof(int), cmp_int);
    int e_arr7[] = {42};
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(e_arr7, arr7, 1, "qsort() should leave the single-element array {42} unchanged");
    int arr8[] = {1, 2, 3, 4, 5};
    qsort(arr8, 0, sizeof(int), cmp_int);
    int e_arr8[] = {1, 2, 3, 4, 5};
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(e_arr8, arr8, 0, "qsort() should not modify the array when count is 0");
}

/*
 * STEP 24: test_qsort_chars
 *
 * Verify qsort() on the characters of a string, using the comparator cmp_char
 * that you write (see below) and strlen() to get the count. Your test must
 * cover: "hello" (becomes "ehllo"); "dcba" (becomes "abcd"); an already sorted
 * string "abc"; a string of equal letters "zzz"; and the empty string ""
 * (stays empty). Sort a char array (not a string literal - literals are read-
 * only).
 *
 * HINT: char word[] = "hello";  qsort(word, strlen(word), sizeof(char), cmp_char);  then TEST_ASSERT_EQUAL_STRING_MESSAGE("ehllo", word, "...");
 */

void test_qsort_chars(void)
{
    char word1[] = "hello";
    qsort(word1, strlen(word1), sizeof(char), cmp_char);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("ehllo", word1, "qsort() should sort the string \"hello\" into \"ehllo\"");

    char word2[] = "dcba";
    qsort(word2, strlen(word2), sizeof(char), cmp_char);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("abcd", word2, "qsort() should sort the string \"dcba\" into \"abcd\"");

    char word3[] = "abc";
    qsort(word3, strlen(word3), sizeof(char), cmp_char);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("abc", word3, "qsort() should leave the already sorted string \"abc\" unchanged");

    char word4[] = "zzz";
    qsort(word4, strlen(word4), sizeof(char), cmp_char);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("zzz", word4, "qsort() should leave the string of equal letters \"zzz\" unchanged");

    char word5[] = "";
    qsort(word5, strlen(word5), sizeof(char), cmp_char);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("", word5, "qsort() should leave the empty string unchanged");
}

/*
 * STEP 25: test_qsort_strings
 *
 * Verify qsort() on an array of strings (char * elements), using the
 * comparators cmp_strlen and cmp_str that you write (see below). Sort {"ccc",
 * "a", "dddd", "bb"} by length: the result is "a", "bb", "ccc", "dddd". Sort
 * {"pear", "apple", "Zed", "app"} alphabetically with strcmp(): the result is
 * "Zed", "app", "apple", "pear" (uppercase first, and a prefix comes before
 * the longer word). Check every element of both results.
 *
 * HINT: Here every element is a char *, so the comparators receive pointers to char * (really char **) - see compare_strptr_asc from the previous assignment. The element size is sizeof(char *).
 *       qsort() is not stable, so these inputs contain no ties (no two strings with the same length).
 */

void test_qsort_strings(void)
{
    char *words_by_length[] = {"ccc", "a", "dddd", "bb"};
    qsort(words_by_length, 4, sizeof(char *), cmp_strlen);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("a", words_by_length[0], "qsort() should sort the strings by length");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("bb", words_by_length[1], "qsort() should sort the strings by length");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("ccc", words_by_length[2], "qsort() should sort the strings by length");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("dddd", words_by_length[3], "qsort() should sort the strings by length");

    char *words_alphabetically[] = {"pear", "apple", "Zed", "app"};
    qsort(words_alphabetically, 4, sizeof(char *), cmp_str);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Zed", words_alphabetically[0], "qsort() should sort the strings alphabetically");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("app", words_alphabetically[1], "qsort() should sort the strings alphabetically");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("apple", words_alphabetically[2], "qsort() should sort the strings alphabetically");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("pear", words_alphabetically[3], "qsort() should sort the strings alphabetically");
}

/*
 * STEP 26: test_roundtrip_sprintf_atoi_atof
 *
 * A round trip: format a number with sprintf(), parse the text back, and
 * expect the original number. For the ints {0, 7, -15, 1234567, -2000000000}
 * use "%d" and check that the text has the expected length (1, 1, 3, 7, 11),
 * that atoi() gives the number back and that atol() gives it back. For the
 * doubles {0.5, -2.25, 100.75, 3.0} use "%.2f" and check that atof() gives the
 * number back. Use loops over arrays.
 *
 * HINT: This test uses sprintf, strlen, atoi, atol and atof together. Use constant arrays for the values and for the expected lengths.
 */

void test_roundtrip_sprintf_atoi_atof(void)
{
    int arr[] = {0, 7, -15, 1234567, -2000000000};
    int expected_lengths[] = {1, 1, 3, 7, 11};
    char buf[64]; 
    TEST_ASSERT_EQUAL_INT_MESSAGE(sizeof(arr)/sizeof(arr[0]), sizeof(expected_lengths)/sizeof(expected_lengths[0]), "Arrays must have the same length");

}

/*
 * STEP 27: test_anagram_with_qsort
 *
 * Write the helper is_anagram(a, b) (see below), then test it. Two strings are
 * anagrams if they contain the same letters in a different order. Your test
 * must cover: "listen" and "silent" (yes); "hello" and "world" (no); "abc" and
 * "abcd" (no - different lengths); "" and "" (yes); "aab" and "abb" (no - same
 * length, different letters); "Listen" and "silent" (no - case matters); "a"
 * and "a" (yes).
 *
 * HINT: The function returns 1 for yes and 0 for no, so TEST_ASSERT_EQUAL_INT_MESSAGE(1, is_anagram("listen", "silent"), "...") works.
 */

void test_anagram_with_qsort(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, is_anagram("listen", "silent"), "listen and silent are anagrams");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, is_anagram("hello", "world"), "hello and world are not anagrams");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, is_anagram("abc", "abcd"), "abc and abcd are not anagrams");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, is_anagram("", ""), "empty strings are anagrams");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, is_anagram("aab", "abb"), "aab and abb are not anagrams");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, is_anagram("Listen", "silent"), "Listen and silent are not anagrams (case matters)");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, is_anagram("a", "a"), "a and a are anagrams");
}
