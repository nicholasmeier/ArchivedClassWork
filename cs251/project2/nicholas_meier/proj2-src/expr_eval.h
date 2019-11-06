#ifndef EXPR_EVAL_H
#define EXPR_EVAL_H

#include <string>
#include <sstream>
#include <math.h>
#include "mystack.h"

#define PI 3.14159265

using namespace std;

class expr_eval{
  private:
        // Declare which ever necessary variables and methods
	void repeatOps(char z);
	int prec(char z);
	void doOp();
	mystack<double> * stk_val;
	mystack<char> * stk_ops;
  public:
        double eval(string, double);
};

double expr_eval::eval(string s, double d){
	stringstream ss;
	double ind = -1;
	stk_val = new mystack<double>();
	stk_ops = new mystack<char>();
	bool intNext = false;
	for (int x = 0; x < s.length(); x++){
		char c = s[x];

		//Determine if the next double in the input is part of a previously read double
		if (x != s.length()-1 && ( (s[x+1] >= '0' && s[x+1] <= '9') || (s[x+1] == '.') || (s[x+1] == 'x') ) ){
			intNext = true;
		}else{
			intNext = false;
		}

		//create the double
		if ( ( (c >= '0' && c <= '9') || (c == '.') ) ){
			ss << c;
			if (!intNext){
				ss << " ";
				ss >> ind;
				stk_val->push(ind);
			}
			if (x == s.length()-1){
				while( (stk_val->size() > 1) && (!(stk_ops->empty())) ){
					doOp();
				}
			}
		}else{
			if (c == 'x'){
				stk_val->push(d);
			}else{
				if (c == '('){
					stk_ops->push(c);
				}else{
					if (c == ')'){
						while((stk_ops->top() != '(') && ( !( stk_val->empty() ) ) ){ 
							doOp(); 
						}
						if (stk_ops->top() == '(') { 
							stk_ops->pop();
							if (!(stk_ops->empty()) && !(stk_val->empty()) && (x == s.length()-1)){
								while(!(stk_ops->empty())){
									doOp();
								}
							}
						}
					}else{
					
						if (c == 's' || c == 'c' || c == 't' || c == 'l'){
							x += 2;
						}
						repeatOps(c);
						stk_ops->push(c);
					}
				}
			}
		}
	}
	//repeatOps('$');
	return stk_val->top();	
}

void expr_eval::repeatOps(char z){
	while( ((stk_val->size() > 1)) && (!(stk_ops->empty())) && (prec(z) <= prec(stk_ops->top()))){
		doOp();
	}
}

void expr_eval::doOp(){
	char z = stk_ops->pop();
	double y = stk_val->pop();
	if ( (z == 'l') || (z == 's') || (z == 'c') || (z == 't') ){
		switch(z){
			case 'l':
				stk_val->push(log10(y));
				return ;
			case 's':
				stk_val->push(sin(y*PI/180));
				return ;
			case 'c':
				stk_val->push(cos(y*PI/180));
				return ;
			case 't':
				stk_val->push(tan(y*PI/180));
				return ;	
		}
	}
	double x = stk_val->pop();
	switch(z){
		case '+':
			stk_val->push(x+y);
			break;
		case '-':
			stk_val->push(x-y);
			break;
		case '*':
			stk_val->push(x*y);
			break;
		case '/':
			stk_val->push(x/y);
			break;
		case '^':
			stk_val->push(pow(x,y));
			break;
		case 'l':
			stk_val->push(log10(y));
			break;
		case 's':
			stk_val->push(sin(y));
			break;
		case 'c':
			stk_val->push(cos(y));
			break;
		case 't':
			stk_val->push(tan(y));
			break;
	}
}

int expr_eval::prec(char z){
	switch(z) {
		case '(':
			return 0;
		case '+':
			return 1;
		case '-':
			return 1;
		case '*':
			return 2;
		case '/':
			return 2;
		case '^':
			return 3;
		case 'l':
			return 4;
		case 's':
			return 5;
		case 'c':
			return 5;
		case 't':
			return 5;
	}
}


#endif
