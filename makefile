all: all-unit-tests

build: build-unit-tests build-main-manual-test build-complexity-measurements

all-unit-tests: build-unit-tests run-unit-tests

run-unit-tests: unit-tests.out
	./unit-tests.out

build-unit-tests: tests/DatasetTests.cpp tests/UnitTestFramework.cpp EditDistance.cpp LongestCommonSubsequence.cpp
	g++ -std=c++17 -O2 -I. tests/DatasetTests.cpp tests/UnitTestFramework.cpp EditDistance.cpp LongestCommonSubsequence.cpp -o unit-tests.out


main-manual-test: build-main-manual-test run-main-manual-test

run-main-manual-test: main.out
	./main.out

build-main-manual-test: main.cpp EditDistance.cpp LongestCommonSubsequence.cpp
	g++ -std=c++17 main.cpp EditDistance.cpp LongestCommonSubsequence.cpp -o main.out


complexity-measurements: build-complexity-measurements run-complexity-measurements

run-complexity-measurements: complexities.out
	./complexities.out > new_complexity_measurements.csv

build-complexity-measurements: ComplexityMeasurements.cpp EditDistance.cpp LongestCommonSubsequence.cpp
	g++ -std=c++17 -O2 ComplexityMeasurements.cpp EditDistance.cpp LongestCommonSubsequence.cpp -o complexities.out