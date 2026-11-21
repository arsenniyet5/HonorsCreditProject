#include <iostream>
#include "LongestCommonSubsequence.h"
#include "EditDistance.h"

using namespace std;

void manualTest() {
	cout << "Testing EditDistance and LongestCommonSubsequence implementations." << endl;
	cout << endl;
	
	cout << "Enter ED for EditDistance or LCS for LongestCommonSubsequence or E to exit: ";
	string method;
	
	while(getline(cin, method)) {
		if(method != "ED" && method != "LCS" && method != "E") {
			cout << "Invalid input. Please enter ED or LCS or E.";
		} else if(method == "E") {
			cout << "Exiting the program." << endl;
			break;
		} else {
			cout << "Enter s1: ";
			string s1;
			getline(cin, s1);

			cout << "Enter s2: ";
			string s2;
			getline(cin, s2);

			if(method == "ED") {
				EditDistance ed;
				ed.solve(s1, s2);
			} else {
				LongestCommonSubsequence lcs;
				lcs.solve(s1, s2);
			}
		}
		
		cout << endl;
		cout << "Enter ED for EditDistance or LCS for LongestCommonSubsequence or E to exit: ";
	}
}

int main() {

	manualTest();

	return 0;
}
