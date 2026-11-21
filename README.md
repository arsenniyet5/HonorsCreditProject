# CS 3358 Honors Credit Project

#### Description: This is the documentation for compiling and running the program

##

### Dependencies

* OS: MacOS, Windows, Linux (either one).
* Standard C++ compiler with C++17 support.
* (Preferable, but not required) "make" build automation tool for makefile.
* (Optional) Python 3, only needed to regenerate the datasets or their statistics.
* All build and run commands need to be executed from inside "Code" folder.

### Build and Run (using makefile for MacOS and Linux)

* Build and run all unit tests:
<pre wrap>
make
</pre>
* Build all classes, unit tests, main manual test, and complexity measurements:
<pre wrap>
make build
</pre>
* Run all unit tests:
<pre wrap>
make run-unit-tests
</pre>
* Build and run EditDistance and LongestCommonSubsequence main manual test:
<pre wrap>
make main-manual-test
</pre>
* Build and run complexity measurements (results are saved to new_complexity_measurements.csv):
<pre wrap>
make complexity-measurements
</pre>

### Build and Run (without makefile for MacOS and Linux)

* Build all classes and unit tests:
<pre wrap>
g++ -std=c++17 -O2 -I. tests/DatasetTests.cpp tests/UnitTestFramework.cpp EditDistance.cpp LongestCommonSubsequence.cpp -o unit-tests.out
</pre>
* Run all unit tests:
<pre wrap>
./unit-tests.out
</pre>
* Build EditDistance and LongestCommonSubsequence main manual test:
<pre wrap>
g++ -std=c++17 main.cpp EditDistance.cpp LongestCommonSubsequence.cpp -o main.out
</pre>
* Run EditDistance and LongestCommonSubsequence main manual test:
<pre wrap>
./main.out
</pre>
* Build complexity measurements:
<pre wrap>
g++ -std=c++17 -O2 ComplexityMeasurements.cpp EditDistance.cpp LongestCommonSubsequence.cpp -o complexities.out
</pre>
* Run complexity measurements:
<pre wrap>
./complexities.out > new_complexity_measurements.csv
</pre>

### Build and Run (without makefile for Windows CMD)

* Build all classes and unit tests:
<pre wrap>
g++ -std=c++17 -O2 -I. tests/DatasetTests.cpp tests/UnitTestFramework.cpp EditDistance.cpp LongestCommonSubsequence.cpp -o unit-tests.exe
</pre>
* Run all unit tests:
<pre wrap>
unit-tests.exe
</pre>
* Build EditDistance and LongestCommonSubsequence main manual test:
<pre wrap>
g++ -std=c++17 main.cpp EditDistance.cpp LongestCommonSubsequence.cpp -o main.exe
</pre>
* Run EditDistance and LongestCommonSubsequence main manual test:
<pre wrap>
main.exe
</pre>
* Build complexity measurements:
<pre wrap>
g++ -std=c++17 -O2 ComplexityMeasurements.cpp EditDistance.cpp LongestCommonSubsequence.cpp -o complexities.exe
</pre>
* Run complexity measurements:
<pre wrap>
complexities.exe > new_complexity_measurements.csv
</pre>

### Datasets (optional, Python 3)

* The datasets are already included in the "datasets" folder. Regenerate them (fixed seed, so the output is the same):
<pre wrap>
python3 datasets/generate_datasets.py
</pre>
* Recompute descriptive statistics of the datasets:
<pre wrap>
python3 datasets/dataset_statistics.py
</pre>


### Measurement results

* The results used in the report are in complexity_measurements.csv.
* New runs are saved to new_complexity_measurements.csv, since runtimes differ slightly between runs and machines.
