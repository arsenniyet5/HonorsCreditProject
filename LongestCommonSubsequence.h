#ifndef LONGEST_COMMON_SUBSEQUENCE_H
#define LONGEST_COMMON_SUBSEQUENCE_H

#include <iostream>
#include <string>
#include <utility>

using namespace std;

class LongestCommonSubsequence {
private:
	int maxLen;
	string sequence;
	int cellsAllocated;

public:
	LongestCommonSubsequence();
	int getCellsAllocated();
	pair<int,string> lcs(string s1, string s2);
	void solve(string s1, string s2);
};

#endif
