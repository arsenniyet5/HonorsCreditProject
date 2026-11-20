#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "EditDistance.h"
#include "LongestCommonSubsequence.h"

/**
 * @file ComplexityMeasurements.cpp
 * @brief Measures the runtime and space usage of EditDistance and
 *        LongestCommonSubsequence on the generated CSV datasets
 *        (columns: id, parameter_value, s1, s2, expected).
 *
 * Runtime: CPU time measured with clock(), median of REPEATS runs per string pair.
 * Space:   number of DP array cells allocated by the algorithm (getCellsAllocated()).
 * Output:  one CSV line per string pair, printed to stdout
 *          (columns: dataset, id, parameter_value, len_s1, len_s2, time_ms, cells).
 *          Errors are printed to stderr so they do not mix with the CSV.
 *
 * Build (from the Code folder):
 *   g++ -std=c++17 -O2 ComplexityMeasurements.cpp EditDistance.cpp LongestCommonSubsequence.cpp -o complexities.out
 * Run:
 *   ./complexities.out > complexity_measurements.csv             (reads ./datasets)
 *   ./complexities.out path/to/dir > complexity_measurements.csv (reads another folder)
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
		cerr << "Could not open " << path << endl;
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
			cerr << "Malformed line in " << path << ": " << line.substr(0, 40) << endl;
			continue;
		}
		rows.push_back({stoi(fields[0]), stoi(fields[1]), fields[2], fields[3], stoi(fields[4])});
	}
	return rows;
}

/**
 * @brief Returns the median of the measured run times.
 * @param times Run times in milliseconds (copied, so sorting does not change the caller's vector).
 * @return The middle value after sorting.
 */

double median(vector<double> times) {
    sort(times.begin(), times.end());
    return times[times.size()/2];
}

/**
 * @brief Prints one measurement as a CSV line to stdout.
 * @param name Dataset name (e.g. "ed_length").
 * @param row The dataset row that was measured.
 * @param timeMs Median run time in milliseconds.
 * @param cells Number of DP array cells allocated by the algorithm.
 */
void writeRow(const string& name, const DatasetRow& row, double timeMs, int cells) {
    string out = name + "," + to_string(row.id) + "," + to_string(row.parameterValue) + 
                "," + to_string(row.s1.size()) + "," + to_string(row.s2.size()) + "," + 
                to_string(timeMs) + "," + to_string(cells);
    cout << out << endl;
}

/** @brief Number of timed runs per string pair; the median run time is reported. */
const int REPEATS = 5;

/** @brief Converts a clock() interval to milliseconds. */
double clockToMs(clock_t start, clock_t end) {
	return 1000.0 * (double)(end - start) / CLOCKS_PER_SEC;
}

/**
 * @brief Measures EditDistance on every row of <dir>/<name>.csv.
 *        Time: median of REPEATS runs measured with clock().
 *        Space: cells allocated by the algorithm (getCellsAllocated()).
 *        Each result is also checked against the expected value from the CSV.
 * @param dir Folder with the dataset CSV files.
 * @param name Dataset name without the .csv extension.
 */
void measureEditDistance(const string& dir, const string& name) {
	vector<DatasetRow> rows = loadDataset(dir + "/" + name + ".csv");
	EditDistance ed;

	for(const DatasetRow& row : rows) {
		vector<double> times;
		int result = 0;
		for(int r = 0; r < REPEATS; r++) {
			clock_t start = clock();
			result = ed.editDistance(row.s1, row.s2);
			clock_t end = clock();
			times.push_back(clockToMs(start, end));
		}
		if(result != row.expected) {
			cerr << "WRONG RESULT " << name << " id=" << row.id << ": got " << result
			     << ", expected " << row.expected << endl;
		}
		writeRow(name, row, median(times), ed.getCellsAllocated());
	}
}


/**
 * @brief Measures LongestCommonSubsequence on every row of <dir>/<name>.csv.
 *        Same method as measureEditDistance; only the LCS length is checked,
 *        and the timed call includes both the table fill and the traceback.
 * @param dir Folder with the dataset CSV files.
 * @param name Dataset name without the .csv extension.
 */
void measureLcs(const string& dir, const string& name) {
	vector<DatasetRow> rows = loadDataset(dir + "/" + name + ".csv");
	LongestCommonSubsequence lcs;

	for(const DatasetRow& row : rows) {
		vector<double> times;
		int result = 0;
		for(int r = 0; r < REPEATS; r++) {
			clock_t start = clock();
			result = lcs.lcs(row.s1, row.s2).first;
			clock_t end = clock();
			times.push_back(clockToMs(start, end));
		}
		if(result != row.expected) {
			cerr << "WRONG RESULT " << name << " id=" << row.id << ": got " << result
			     << ", expected " << row.expected << endl;
		}
		writeRow(name, row, median(times), lcs.getCellsAllocated());
	}
}

/**
 * @brief Prints the CSV header, then measures every dataset.
 * @param argc Number of command-line arguments.
 * @param argv argv[1] is an optional dataset folder (default: ./datasets).
 * @return 0 on completion.
 */
int main(int argc, char* argv[]) {
	string dir = (argc > 1) ? argv[1] : "datasets";

	cout << "dataset,id,parameter_value,len_s1,len_s2,time_ms,cells" << endl;

	measureEditDistance(dir, "ed_length");
	measureEditDistance(dir, "ed_alphabet");
	measureEditDistance(dir, "ed_similarity");

	measureLcs(dir, "lcs_length");
	measureLcs(dir, "lcs_alphabet");
	measureLcs(dir, "lcs_similarity");

	return 0;
}
