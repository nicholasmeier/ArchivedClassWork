#ifndef MYSTACK_H
#define MYSTACK_H
#define MAX_SIZE 10000

template <class T>
class mystack{
	private:
		T array[MAX_SIZE];
		int index;
	public:
        	mystack(){
			this->index = -1;
		};
	        void push(T x);
        	T top();
	        T pop();
        	bool empty();
	        int size();
};

template<class T>
void mystack<T>::push(T X){
	this->index++;
	array[this->index] = X;
};

template<class T>
T mystack<T>::top(){
	if (!empty()){
		if (this->index != -1)
			return array[this->index];
	}else{
		throw std::runtime_error("Stack is Empty");
	}
};

template<class T>
T mystack<T>::pop(){
	if (!empty()){
		T re = array[this->index];
		this->index--;
		return re;
	}else{
		throw std::runtime_error("Stack is Empty");
	}
};

template<class T>
bool mystack<T>::empty(){
	if (this->index == -1){
		return true;
	}
	return false;
};

template<class T>
int mystack<T>::size(){
	return (this->index+1);
};


#endif
