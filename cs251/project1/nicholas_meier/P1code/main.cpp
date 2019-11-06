/**
 *  CS251 - Spring 2018: Project 1
 *  NICHOLAS MEIER
 */

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
/* Use input/output using C++ like notation would use "cin" and "cout" to read/write to stdin/stdout */
    /* ------------------------------- */
    /* process inputs and write output */
    /* ------------------------------- */
    /* Exit the program */
	int c; //Input
	bool firstInt = true; //Checks if the input is on the first int
	int x; //how many numbers are in each line
	int nHorLines = 0; //number of horizontal lines - 1
	int count = 0; //counts how many numbers have been printed in a line
	while (cin >> c){
		if (firstInt){ //initial number and some first number handling
			x = c;
			nHorLines = (6*x)-3;
			firstInt = false;
			for (int i = 0; i < nHorLines; i++){
				cout << '-';
			}
		}else{
			//checks if new line has been started
			if (count == 0){
				cout << "\r\n";
			}

			//set of conditionals that determine how many needed spaces
			if (c < 10){
				cout << "  " << c;
				count++;
			}else{
				if (c <= 99){
					cout << ' ' << c;
					count++;
				}else{
					if (c <= 999){
						cout << c;
						count++;
					}
				}
			}

			//checks if line is finished
			if (count == x){
				cout << "\r\n";
				for (int i = 0; i < nHorLines; i++){
					cout << '-';
				}
				count = 0;
			}else{
				cout << " | ";
			}
		}
		
	}
	return 0;
}
