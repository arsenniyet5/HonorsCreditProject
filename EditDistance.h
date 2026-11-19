#ifndef EDIT_DISTANCE_H
#define EDIT_DISTANCE_H

#include <iostream>

using namespace std;

class EditDistance {
private:
	int minDist;

public:
	EditDistance();
	int editDistance(string s1, string s2);
	void solve(string s1, string s2);
};

#endif
