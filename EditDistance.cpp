#include <algorithm>
#include <vector>
#include "EditDistance.h"

using namespace std;

EditDistance::EditDistance() {
	this->minDist = 0;
	this->cellsAllocated = 0;
}

int EditDistance::getCellsAllocated() {
	return this->cellsAllocated;
}

int EditDistance::editDistance(string s1, string s2) {
	string longer = "";
	string shorter = "";
	if(s1.size() > s2.size()) {
		longer = s1;
		shorter = s2;
	} else {
		longer = s2;
		shorter = s1;
	}

	int m = longer.size();
	int n = shorter.size();

	vector<int> prev_row(n+1, 0);
	vector<int> curr_row(n+1, 0);

	this->cellsAllocated = prev_row.size() + curr_row.size();
	
	for(int i = 0; i <= n; i++) {
		prev_row[i] = i;
	}

	for(int i = 1; i <= m; i++) {
		curr_row[0] = i;
		for(int j = 1; j <= n; j++) {
			if(longer[i-1] == shorter[j-1]) {
				curr_row[j] = prev_row[j-1];
			} else {
				int minTerm = min(prev_row[j-1], prev_row[j]);
				minTerm = min(minTerm, curr_row[j-1]);
				curr_row[j] = 1 + minTerm;
			}
		}
		swap(prev_row, curr_row);
	}

	int res = prev_row[n];

	return res;
}

void EditDistance::solve(string s1, string s2) {
	this->minDist = editDistance(s1, s2);

	cout << "Distance: " << this->minDist << endl;
}
