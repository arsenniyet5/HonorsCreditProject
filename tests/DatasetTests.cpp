#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "UnitTestFramework.h"
#include "EditDistance.h"
#include "LongestCommonSubsequence.h"

/**
 * @file DatasetTests.cpp
 * @brief Unit tests that run EditDistance and LongestCommonSubsequence against
 *        the generated CSV datasets (columns: id, parameter_value, s1, s2, expected).
 *
 * Build (from the Code folder):
 *   g++ -std=c++17 -O2 -I. tests/DatasetTests.cpp EditDistance.cpp LongestCommonSubsequence.cpp tests/UnitTestFramework.cpp -o run_tests.out
 * Run:
 *   ./run_tests.out              (reads ./datasets)
 *   ./run_tests.out path/to/dir  (reads another folder)
 */

using namespace std;

/** @brief One row of a dataset CSV. */
struct DatasetRow {
	int id;
	int parameterValue;
	string s1;
	string s2;
	int expected;
};

/**
 * @brief Loads a dataset CSV. Fields never contain commas or quotes, so a
 *        plain split is enough. Handles the \r\n line endings Python writes.
 * @param path Path to the CSV file.
 * @return The data rows (header skipped); empty if the file cannot be opened.
 */
vector<DatasetRow> loadDataset(const string& path) {
	vector<DatasetRow> rows;
	ifstream file(path);
	if(!file) {
		cout << "Could not open " << path << endl;
		return rows;
	}

	string line;
	getline(file, line); // header
	while(getline(file, line)) {
		if(!line.empty() && line.back() == '\r') {
			line.pop_back();
		}
		if(line.empty()) {
			continue;
		}
		vector<string> fields;
		stringstream ss(line);
		string field;
		while(getline(ss, field, ',')) {
			fields.push_back(field);
		}
		if(fields.size() != 5) {
			cout << "Malformed line in " << path << ": " << line.substr(0, 40) << endl;
			continue;
		}
		rows.push_back({stoi(fields[0]), stoi(fields[1]), fields[2], fields[3], stoi(fields[4])});
	}
	return rows;
}

/**
 * @brief Checks that seq is a subsequence of s.
 */
bool isSubsequence(const string& seq, const string& s) {
	size_t k = 0;
	for(size_t i = 0; i < s.size() && k < seq.size(); i++) {
		if(s[i] == seq[k]) {
			k++;
		}
	}
	return k == seq.size();
}

/**
 * @brief Runs EditDistance::editDistance on every row and compares with the expected distance.
 */
void testEditDistanceDataset(const string& path, size_t expectedRows) {
	log_function_name("testEditDistanceDataset: " + path);
	vector<DatasetRow> rows = loadDataset(path);
	TEST_CHECK(rows.size() == expectedRows);

	EditDistance ed;
	for(const DatasetRow& row : rows) {
		cout << "  id=" << row.id << " parameter_value=" << row.parameterValue
		     << " expected=" << row.expected << endl;
		int actual = ed.editDistance(row.s1, row.s2);
		TEST_CHECK(actual == row.expected);
	}
}

/**
 * @brief Runs LongestCommonSubsequence::lcs on every row. Only the length is
 *        compared with the CSV (an LCS is not unique); the returned string is
 *        checked to be a common subsequence of both inputs with that length.
 */
void testLcsDataset(const string& path, size_t expectedRows) {
	log_function_name("testLcsDataset: " + path);
	vector<DatasetRow> rows = loadDataset(path);
	TEST_CHECK(rows.size() == expectedRows);

	LongestCommonSubsequence lcs;
	for(const DatasetRow& row : rows) {
		cout << "  id=" << row.id << " parameter_value=" << row.parameterValue
		     << " expected=" << row.expected << endl;
		pair<int,string> result = lcs.lcs(row.s1, row.s2);
		TEST_CHECK(result.first == row.expected);
		TEST_CHECK((int)result.second.size() == row.expected);
		TEST_CHECK(isSubsequence(result.second, row.s1) && isSubsequence(result.second, row.s2));
	}
}

int main(int argc, char* argv[]) {
	string dir = (argc > 1) ? argv[1] : "datasets";

	testEditDistanceDataset(dir + "/ed_length.csv", 35);
	testEditDistanceDataset(dir + "/ed_alphabet.csv", 25);
	testEditDistanceDataset(dir + "/ed_similarity.csv", 25);
	testEditDistanceDataset(dir + "/ed_shorter.csv", 30);

	testLcsDataset(dir + "/lcs_length.csv", 30);
	testLcsDataset(dir + "/lcs_alphabet.csv", 25);
	testLcsDataset(dir + "/lcs_similarity.csv", 25);
	testLcsDataset(dir + "/lcs_shorter.csv", 30);

	print_tests_summary();
	print_failed_tests();

	return test_failures_count == 0 ? 0 : 1;
}
