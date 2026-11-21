#include <iostream>
#include <vector>
#include "LongestCommonSubsequence.h"

/**
 * @file LongestCommonSubsequence.cpp
 * @brief Implementation of the LongestCommonSubsequence class.
 * @author Arsen Niyetkaliyev
 * @date 2026-10-26
 */

using namespace std;

LongestCommonSubsequence::LongestCommonSubsequence() {
	this->maxLen = 0;
	this->sequence = "";
	this->cellsAllocated = 0;
}

int LongestCommonSubsequence::getCellsAllocated() {
	return this->cellsAllocated;
}

pair<int,string> LongestCommonSubsequence::lcs(string s1, string s2) {
	int n1 = s1.size();
	int n2 = s2.size();
	
	// Full (M+1) x (N+1) table, needed for the traceback: O(MN) space
	vector<vector<int>> countArr(n1+1, vector<int>(n2+1, 0));

	this->cellsAllocated = 0;
	for(int i = 0; i <= n1; i++) {
		this->cellsAllocated += countArr[i].size();
	}

	// Fill the table
	for(int i = 1; i <= n1; i++) {
		for(int j = 1; j <= n2; j++) {
			if(s1[i-1] == s2[j-1]) {
				countArr[i][j] = countArr[i-1][j-1]+1;
			}
			countArr[i][j] = max(countArr[i][j], countArr[i-1][j]);
			countArr[i][j] = max(countArr[i][j], countArr[i][j-1]);
		}
	}

	// Traceback from the bottom-right cell
	string temp = "";
	int i = n1;
	int j = n2;
	while(i > 0 && j > 0) {
		if(s1[i-1] == s2[j-1]) {
			temp += s1[i-1];  // match: part of the LCS, move diagonally
			i--;
			j--;
		} else if(countArr[i-1][j] >= countArr[i][j-1]) {
			i--;
		} else {
			j--;
		}
	}
	
	// Characters were collected backwards, so reverse them
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

	cout << "LCS Length: " << this->maxLen << endl;
	cout << "LCS String: " << "\"" << this->sequence << "\"" << endl; 
}
