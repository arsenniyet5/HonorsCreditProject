#ifndef LONGEST_COMMON_SUBSEQUENCE_H
#define LONGEST_COMMON_SUBSEQUENCE_H

#include <iostream>
#include <vector>
#include "ArrayList.h"

using namespace std;

class LongestCommonSubsequence {
private:
	int maxLen;
	string sequence;

public:
	LongestCommonSubsequence();
	pair<int,string> lcs(string s1, string s2);
	void solve(string s1, string s2);
};

#endif
