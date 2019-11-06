#include <iostream>
#include <climits>

#include "MinHeap.h"



//implement the methods in MinHeap.h

MinHeap::MinHeap()
{
	this->size = 0;
}

void MinHeap::insert(TreeNode * val)
{
		
	nodes.push_back(val);
	this->size++;
	int c = this->size-1;
	int p = parent(c);
	while (c > 0) {
		if (nodes.at(c)->getFrequency() >= nodes.at(p)->getFrequency()){
			break;
		}
		TreeNode * temp = nodes.at(c);
		nodes.at(c) = nodes.at(p);
		nodes.at(p) = temp;
		c = p;
		p = parent(c);
	}
		/*
		while (i != 0 && (nodes.at(parent(i))->getFrequency() >= nodes.at(i)->getFrequency())){
			TreeNode * tempx = nodes.at(parent(i));
			TreeNode * tempy = nodes.at(i);
			nodes.at(parent(i)) = tempy;
			nodes.at(i) = tempx;
			i = parent(i);
		}*/
}

int MinHeap::getSize()
{
	return this->size;
}

TreeNode * MinHeap::removeMin()
{
	//ut << this->size << "SIZE";
	if (this->size <= 0){ return NULL; }
	TreeNode * min = nodes.at(0);
	this->size--;
	if (this->size == 0){
		nodes.pop_back();
		return min;
	}

	nodes.at(0) = nodes.at(this->size);
	nodes.pop_back();
	int p = 0;
	int l = left(p);
	int r = right(p);
	while (l < this->size){
		int minC = l;
		if (r < this->size && nodes.at(r)->getFrequency() < nodes.at(l)->getFrequency()){
			minC = r;
		}
		if (nodes.at(p)->getFrequency() <= nodes.at(minC)->getFrequency()){
			break;
		}
		TreeNode * temp = nodes.at(minC);
		nodes.at(minC) = nodes.at(p);
		nodes.at(p) = temp;
		p = minC;
		l = left(p);
		r = right(p);	
	}
	return min;
	
	/*
	if (this->size == 1){
		this->size--;
		nodes.pop_back();
		return min;
	}else{
		this->size--;
		nodes.at(0) = nodes.at(this->size);
		nodes.erase(nodes.begin()+(this->size));
		//nodes.pop_front();
		shrink(0);
		return min;
	}*/
}

void MinHeap::shrink(int i){
	//cout << "Shrink Called"; 
	int li = left(i);
	int ri = right(i);
	int mini = i;
	if (li < this->size && nodes.at(li)->getFrequency() <= nodes.at(i)->getFrequency()){
		mini = li;
	}
	if (ri < this->size && nodes.at(ri)->getFrequency() <= nodes.at(mini)->getFrequency()){
		mini = ri;
	}
	if (mini != i){
		TreeNode * tempx = nodes.at(i);
		TreeNode * tempy = nodes.at(mini);
		nodes.at(mini) = tempy;
		nodes.at(i) = tempx;
		shrink(mini);
	}
}
int MinHeap::parent(int c){ return (((c)-1)/2); }
int MinHeap::left(int p){ return (2*(p) + 1); }
int MinHeap::right(int p){ return (2*(p) + 2); }
