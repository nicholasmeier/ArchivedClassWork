#include<iostream>
#include "myqueue.h"

using namespace std;

int main(){
	myqueue<int> * q_int = new myqueue<int>();
	myqueue<double> * q_double = new myqueue<double>();
	myqueue<char> * q_char = new myqueue<char>();
	myqueue<string> * q_string = new myqueue<string>();
	
	string s;
	string _int = "int";
	string _double = "double";
	string _char = "char";
	string _string = "string";
	
	const string actions[] = {"enqueue", "front", "dequeue", "empty", "size"};
	
	bool firstLine = true;
	int state = -1;
	while(cin >> s){
		if (firstLine){
			int t0 = s.compare(_int);
			int t1 = s.compare(_double);
			int t2 = s.compare(_char);
			int t3 = s.compare(_string);
			if (!(t0)){ state = 0; }
			if (!(t1)){ state = 1; }
			if (!(t2)){ state = 2; }
			if (!(t3)){ state = 3; }
			firstLine = false;
		}else{
			int t = -1;
			int act = -1;
			for (int z = 0; z < 5; z++){
				t = s.compare(actions[z]);
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
							q_int->enqueue(stoi(val));
							break;
						case 1:
							cout << q_int->front() << "\n";
							break;
						case 2:
							cout << q_int->dequeue() << "\n";
							break;
						case 3:
							if (q_int->empty()){
								cout << 1 << "\n";
							}else{
								cout << 0 << "\n";
							}
							break;
						case 4:
							cout << q_int->size() << "\n";
							break;
					}
					break;	
				case 1:
					switch(act){
						case 0:
							cin >> val;
							q_double->enqueue(stod(val));
							break;
						case 1:
							cout << q_double->front() << "\n";
							break;
						case 2:
							cout << q_double->dequeue() << "\n";
							break;
						case 3:
							if (q_double->empty()){
								cout << 1 << "\n";
							}else{
								cout << 0 << "\n";
							}
							break;
						case 4:
							cout << q_double->size() << "\n";
							break;
					}
					break;
				case 2:
					switch(act){
						case 0:
							cin >> val;
							q_char->enqueue(val[0]);
							break;
						case 1:
							cout << q_char->front() << "\n";
							break;
						case 2:
							cout << q_char->dequeue() << "\n";
							break;
						case 3:
							if (q_char->empty()){
								cout << 1 << "\n";
							}else{
								cout << 0 << "\n";
							}
							break;
						case 4:
							cout << q_char->size() << "\n";
							break;
					}
					break;					
				case 3:
					switch(act){
						case 0:
							cin >> val;
							q_string->enqueue(val);
							break;
						case 1:
							cout << q_string->front() << "\n";
							break;
						case 2:
							cout << q_string->dequeue() << "\n";
							break;
						case 3:
							if (q_string->empty()){
								cout << 1 << "\n";
							}else{
								cout << 0 << "\n";
							}
							break;
						case 4:
							cout << q_string->size() << "\n";
							break;
					}
					break;
			}
		}
	}
	cout << "\n";
	return 0;
}
