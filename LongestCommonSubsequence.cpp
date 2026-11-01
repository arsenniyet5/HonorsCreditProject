#include <iostream>
#include "LongestCommonSubsequence.h"

using namespace std;

LongestCommonSubsequence::LongestCommonSubsequence() {
	this->maxLen = 0;
	this->sequence = "";
}

pair<int,string> LongestCommonSubsequence::lcs(string s1, string s2) {
	int n1 = s1.size();
	int n2 = s2.size();

	ArrayList<ArrayList<int>> countArr(n1+1, ArrayList<int>(n2+1, 0));

	for(int i = 1; i <= n1; i++) {
		for(int j = 1; j <= n2; j++) {
			if(s1[i-1] == s2[j-1]) {
				countArr[i][j] = countArr[i-1][j-1]+1;
			}
			countArr[i][j] = max(countArr[i][j], countArr[i-1][j]);
			countArr[i][j] = max(countArr[i][j], countArr[i][j-1]);
		}
	}

	string temp = "";
	int i = n1;
	int j = n2;
	while(i > 0 && j > 0) {
		if(s1[i-1] == s2[j-1]) {
			temp += s1[i-1];
			i--;
			j--;
		} else if(countArr[i-1][j] >= countArr[i][j-1]) {
			i--;
		} else {
			j--;
		}
	}
	
	string seq = "";
	for(int k = temp.size()-1; k >= 0; k--) {
		seq += temp[k];
	}

	return {countArr[n1][n2],seq};
}

void LongestCommonSubsequence::solve(string s1, string s2) {
	pair<int,string> res = lcs(s1, s2);

	this->maxLen = res.first;
	this->sequence = res.second;

	cout << "Expected Length: " << this->maxLen << endl;
	cout << "Expected LCS String: " << "\"" << this->sequence << "\"" << endl; 
}

