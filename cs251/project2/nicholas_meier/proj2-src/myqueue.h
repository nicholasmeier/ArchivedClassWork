#ifndef MYQUEUE_H
#define MYQUEUE_H
#define MAX_SIZE 10000

template <class T>
class myqueue{
	private:
        	T array[MAX_SIZE];
		int indEnd;
		int indFro;
		int n;	
	public:
        	myqueue(){
			this->indEnd = 0;
			this->indFro = 0;
			this->n = 0;
		};
	    	void enqueue(T x);
	    	T front();
        	T dequeue();
    		bool empty();
	        int size();
};

template<class T>
void myqueue<T>::enqueue(T x){
	array[this->indEnd] = x;
	this->indEnd++;
};

template<class T>
T myqueue<T>::front(){
	if (!empty()){
		return array[this->indFro];
	}else{
		throw std::runtime_error("Queue is empty");
	}
};

template<class T>
T myqueue<T>::dequeue(){
	if (!empty()){
		T re = array[this->indFro];
		this->indFro++;
		return re;
	}else{
		throw std::runtime_error("Queue is empty");
	}
};

template<class T>
bool myqueue<T>::empty(){
	if (indEnd == indFro){
		return true;
	}else{
		return false;	
	}
};

template<class T>
int myqueue<T>::size(){
	return (this->indEnd - this->indFro);
}
#endif
