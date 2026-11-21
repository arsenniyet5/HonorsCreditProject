#ifndef EDIT_DISTANCE_H
#define EDIT_DISTANCE_H

#include <iostream>
#include <string>
#include <utility>

/**
 * @file EditDistance.h
 * @brief Declaration of the EditDistance class.
 * @author Arsen Niyetkaliyev
 * @date 2026-11-18
 */

using namespace std;

class EditDistance {
private:
	/** @brief The minimum edit distance found */
	int minDist;

	/** @brief The number of cells allocated in the DP table */
	int cellsAllocated;

public:
	/** 
	 * @brief Constructor for the EditDistance class. 
	*/
	EditDistance();

	/**
	 * @brief Get the number of cells allocated in the DP table.
	 * @return The number of cells allocated in the DP table.
	 */
	int getCellsAllocated();

	/**
	 * @brief Calculate the edit distance between two strings.
	 * @param s1 First string.
	 * @param s2 Second string.
	 * @return The edit distance between the two strings.
	 */
	int editDistance(string s1, string s2);

	/**
	 * @brief Solve the edit distance problem and print the result.
	 * @param s1 First string.
	 * @param s2 Second string.
	 * @note Calls the editDistance function.
	 */
	void solve(string s1, string s2);
};

#endif
