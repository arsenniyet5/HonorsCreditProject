#ifndef UNIT_TEST_FRAMEWORK_H
#define UNIT_TEST_FRAMEWORK_H
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>

/**
 * @file UnitTestFramework.h
 * @brief Declaration of a macros based unit test framework for C++.
 * @author Arsen Niyetkaliyev
 * @date 2026-11-14
 */

/** @brief Maximum number of tests that can be run. */
#define MAX_TESTS 100000

using namespace std;

/** @brief Count of test failures. */
extern int test_failures_count;

/** @brief Count of tests passed. */
extern int tests_passed_count;

/** @brief Array storing the descriptions of test failures. */
extern string test_failures[MAX_TESTS];

/**
 * @brief Logs a test failure to stdout with the given expression, file, and line number.
 * @param expr The expression that failed.
 * @param file The file in which the failure occurred.
 * @param line The line number at which the failure occurred.
 */
void log_failure(const char* expr, const char* file, int line);

/**
 * @brief Logs the name of the currently executing test function to stdout.
 * @param function_name The name of the test function.
 */
void log_function_name(string function_name);

/**
 * @brief Prints a summary of the test results to stdout.
 */
void print_tests_summary();

/**
 * @brief Prints the descriptions of all failed tests to stdout.
 */
void print_failed_tests();

/**
 * @brief Macro to check a test expression (boolean) and log the result.
 * @param expression The test expression to check.
 */
#define TEST_CHECK(expression) \
    if(expression) { \
        tests_passed_count++; \
        cout << __FILE__ << ":" << __LINE__ << " - [PASS] " << #expression << endl; \
    } else { \
        log_failure(#expression, __FILE__, __LINE__); \
    }

/**
 * @brief Macro to check if a test expression (function execution) throws a specific exception and log the result.
 * @param expression The test expression to check.
 * @param exception_type The type of exception expected to be thrown.
 */
#define TEST_THROW(expression, exception_type) \
    try { \
        expression; \
        log_failure(#expression " did not throw " #exception_type, __FILE__, __LINE__); \
    } catch (const exception_type&) { \
        tests_passed_count++; \
        cout << __FILE__ << ":" << __LINE__ << " - [PASS] " << #expression << " threw " #exception_type << endl; \
    } catch(...) { \
        log_failure(#expression " threw an unexpected exception instead of " #exception_type, __FILE__, __LINE__); \
    }

#endif
