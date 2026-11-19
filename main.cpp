#include <iostream>
#include "LongestCommonSubsequence.h"
#include "EditDistance.h"

using namespace std;

int main() {
	EditDistance ed;
	ed.solve("KITTEN", "SITTING");
	ed.solve("EXECUTION", "INTENTION");

	LongestCommonSubsequence lcs;
	lcs.solve("AGGTAB", "GXTXAYB");
	lcs.solve("ABCB", "BDCAB");
	lcs.solve("", "PYTHON");
	lcs.solve("ABC", "XYZ");
	lcs.solve("DYNAMICS", "DYNAMICS");

	return 0;
}