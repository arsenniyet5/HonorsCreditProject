#include "UnitTestFramework.h"

/**
 * @file UnitTestFramework.cpp
 * @brief Implementation of the macro based unit test framework used for running and logging tests.
 * @author Arsen Niyetkaliyev
 * @date 2026-11-20
 */

int test_failures_count = 0;
int tests_passed_count = 0;
string test_failures[MAX_TESTS];

void log_failure(const char* expr, const char* file, int line) {
    string fileNameStr(file);
    string message = fileNameStr + ":" + to_string(line) + " - [FAILED]: (" + expr + ")";
    cout << message << endl;
    test_failures[test_failures_count] = message;
    test_failures_count++;

}

void log_function_name(string function_name) {
    cout << __FILE__ << ":" << __LINE__ << " - [TESTING_FUNCTION] " << function_name << endl;
}

void print_tests_summary() {
    double success_rate = 0;
    if(tests_passed_count + test_failures_count > 0) {
        success_rate = (double)tests_passed_count / (tests_passed_count + test_failures_count) * 100;
    }
    cout << "\n====================================\n";
    cout << "        TESTS SUMMARY         \n";
    cout << "====================================\n";
    cout << "Total Tests Run:  " << (tests_passed_count + test_failures_count) << endl;
    cout << "Passed Tests:     " << tests_passed_count << endl;
    cout << "Failed Tests:     " << test_failures_count << endl;
    cout << "Success Rate:     " << success_rate << "%" << endl;
    cout << "====================================\n";
}

void print_failed_tests() {
    if(test_failures_count > 0) {
        cout << "\n====================================\n";
        cout << "        FAILED TESTS         \n";
        cout << "====================================\n";
        for(int i = 0; i < test_failures_count; i++) {
            cout << test_failures[i] << endl;
        }
        cout << "====================================\n";
    }
}
