#include<iostream>
#include <string>
#include "mystack.h"

using namespace std;

int main(){
	mystack<int> * stk_int = new mystack<int>();
	mystack<double> * stk_double = new mystack<double>();
	mystack<char> * stk_char = new mystack<char>;
	mystack<string> * stk_string = new mystack<string>;
	//mystack stacks[] = {stk_int, stk_double, stk_char, stk_str};

	string s;
	string _int = "int";
	string _double = "double";
	string _char = "char";
	string _string = "string";

	string _push = "pus";
	string _pop = "pop";
	string _empty = "emp";
	string _size = "siz";
	string _top = "top";
	const string actions[] = {"pus","pop", "emp", "siz", "top"};
	
	bool firstLine = true;
	int state = -1;
	while (cin >> s){
		if (firstLine){
			int t0 = s.compare(_int);
			int t1 = s.compare(_double);
			int t2 = s.compare(_char);
			int t3 = s.compare(_string);
			if (!(t0)){
				state = 0;
			}
			if (!(t1)){
				state = 1;
				//stk_double = new mystack<double>();
			}
			if (!(t2)){ state = 2; }
			if (!(t3)){ state = 3; }
			firstLine = false;
		}else{	
			int t = -1;
			int act = -1;
			for (int z = 0; z < 5; z++){
				t = s.compare(0,3,actions[z]);
				if (!(t)){
					act = z;
					break;
				}
			}
			string val;
			switch(state){
				case 0:
					switch(act){
						case 0:
							cin >> val;			
							//cout << val << "\n";
							stk_int->push(stoi(val));
							break;
						case 1:
							cout << stk_int->pop() << "\n";
							break;
						case 2:
							if (stk_int->empty()){
								cout << 1 << "\n";
							}else{ 
								cout << 0 << "\n";		
							}	
							//cout << stk_int->empty() << "\n";
							break;
						case 3:
							cout << stk_int->size() << "\n";
							break;
						case 4:
							cout << stk_int->top() << "\n";
						}
					break;
				case 1:
					switch(act){
						case 0:
							cin >> val;
							//cout << val << "\n";
							stk_double->push(stod(val));
							break;
						case 1:
							cout << stk_double->pop() << "\n";
							break;
						case 2:
							if (stk_double->empty()){
								cout << 1 << "\n";
							}else{
								cout << 0 << "\n";
							}	
							//cout << stk_double->empty() << "\n";
							break;
						case 3:
							cout << stk_double->size() << "\n";
							break;
						case 4:
							cout << stk_double->top() << "\n";
						}
					break;
				case 2:
					switch(act){
						case 0:
							cin >> val;
							stk_char->push(val[0]);
							break;
						case 1:
							cout << stk_char->pop() << "\n";
							break;
						case 2:
							if (stk_char->empty()){
								cout << 1 << "\n";
							}else{
								cout << 0 << "\n";
							}	
							//cout << stk_double->empty() << "\n";
							break;
						case 3:
							cout << stk_char->size() << "\n";
							break;
						case 4:
							cout << stk_char->top() << "\n";
						}
					break;
				case 3:
					switch(act){
						case 0:
							cin >> val;
							stk_string->push(val);
							break;
						case 1:
							cout << stk_string->pop() << "\n";
							break;
						case 2:
							if (stk_string->empty()){
								cout << 1 << "\n";
							}else{
								cout << 0 << "\n";
							}	
							//cout << stk_double->empty() << "\n";
							break;
						case 3:
							cout << stk_string->size() << "\n";
							break;
						case 4:
							cout << stk_string->top() << "\n";
						}
					break;

			}
		}
	}	
	cout << "\n";
	//for exceptions use try catch
	// try{} catch (exception e){};
	return 0;
}
