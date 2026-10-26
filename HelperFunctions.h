#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <iostream>
#include <string>
#include <stdexcept>

/**
 * @file HelperFunctions.h
 * @brief Declaration and implementation of helper functions for input validation.
 * @author Arsen Niyetkaliyev
 * @date 2026-09-21
 */

/**
 * @brief Checks if the given string represents an integer.
 * @param line The string to check.
 * @return True if the string represents an integer, false otherwise.
 */
inline bool helperIsInteger(const std::string& line) {
	int num = std::stoi(line);
	std::string copy = std::to_string(num);
	return line == copy;
}

/**
 * @brief Checks if the given string represents a single character.
 * @param line The string to check.
 * @return True if the string represents a single character, false otherwise.
 */
inline bool helperIsChar(const std::string& line) {
	return line.size() == 1;
}

/**
 * @brief Validates that the given string represents an integer and throws an error with the provided message otherwise.
 * @param line The string to validate.
 * @param message The error message to throw if the validation fails.
 */
inline void helperValidateInteger(const std::string& line, const std::string& message) {
	if(!helperIsInteger(line)) {
		throw std::runtime_error(message);
	}
}

/**
 * @brief Validates that the given string represents a single character and throws an error otherwise.
 * @param line The string to validate.
 */
inline void helperValidateChar(const std::string& line) {
	if(!helperIsChar(line)) {
		throw std::runtime_error("Invalid input. Input should consist of chars.");
	}
}

/**
 * @brief Swaps the values of two variables.
 * @tparam T The type of the variables.
 * @param a The first variable.
 * @param b The second variable.
 */
template<typename T>
void helperSwap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

#endif
