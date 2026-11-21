#ifndef LONGEST_COMMON_SUBSEQUENCE_H
#define LONGEST_COMMON_SUBSEQUENCE_H

#include <iostream>
#include <string>
#include <utility>

/**
 * @file LongestCommonSubsequence.h
 * @brief Declaration of the LongestCommonSubsequence class.
 * @author Arsen Niyetkaliyev
 * @date 2026-10-26
 */

using namespace std;

class LongestCommonSubsequence {
private:
	/** @brief The length of the longest common subsequence */
	int maxLen;

	/** @brief The longest common subsequence */
	string sequence;

	/** @brief The number of cells allocated in the DP table */
	int cellsAllocated;

public:
	/**
	 * @brief Constructor for a LongestCommonSubsequence object.
	 */
	LongestCommonSubsequence();

	/**
	 * @brief Returns the number of cells allocated in the DP table.
	 * @return The number of cells allocated in the DP table.
	 */
	int getCellsAllocated();

	/**
	 * @brief Computes the length of the longest common subsequence of two strings and the subsequence itself.
	 * @param s1 The first string.
	 * @param s2 The second string.
	 * @return A pair containing the length of the longest common subsequence and the subsequence itself.
	 */
	pair<int,string> lcs(string s1, string s2);

	/**
	 * @brief Solves the longest common subsequence problem for two strings and prints the result.
	 * @param s1 The first string.
	 * @param s2 The second string.
	 * @note Calls the lcs method.
	 */
	void solve(string s1, string s2);
};

#endif
