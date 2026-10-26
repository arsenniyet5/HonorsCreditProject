#include <iostream>
#include "LongestCommonSubsequence.h"

using namespace std;

int main() {
	LongestCommonSubsequence lcs;
	lcs.solve("AGGTAB", "GXTXAYB");
	lcs.solve("ABCB", "BDCAB");
	lcs.solve("", "PYTHON");
	lcs.solve("ABC", "XYZ");
	lcs.solve("DYNAMICS", "DYNAMICS");
}