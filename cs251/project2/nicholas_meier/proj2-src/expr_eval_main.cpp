#include <iostream>
#include <iomanip>
#include <string>
#include "expr_eval.h"


using namespace std;

int main(){
	expr_eval * ee = new expr_eval();
	bool hasXVal = false;
	double x = 0;
	string s;
	while(cin >> s){
		for (int i = 0; i < s.length(); i++){
			if (s[i] == 'x'){
				hasXVal = true;
				break;	
			}	
		}
		if (hasXVal){
			cin >> x;	
		}
		cout << defaultfloat << setprecision(2) << ee->eval(s, x) << '\n';
	}
}
