#ifndef MINHEAP_H
#define MINHEAP_H

#include "TreeNode.h"
#include <vector>


using namespace std;

class MinHeap
{
	private:
		int size;
		
	public:
		vector<TreeNode*> nodes;
		MinHeap();
		TreeNode * removeMin(); //returns root of heap
		void insert(TreeNode * val); //adds element to heap
		int getSize(); //returns size of heap
		
		//Utility methods
		int parent(int i);
		int right(int i);
		int left(int i);
		void shrink(int i);
		~MinHeap();
};

#endif
