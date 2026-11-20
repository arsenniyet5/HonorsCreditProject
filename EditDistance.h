#ifndef EDIT_DISTANCE_H
#define EDIT_DISTANCE_H

#include <iostream>
#include <string>
#include <utility>

using namespace std;

class EditDistance {
private:
	int minDist;
	int cellsAllocated;

public:
	EditDistance();
	int getCellsAllocated();
	int editDistance(string s1, string s2);
	void solve(string s1, string s2);
};

#endif
