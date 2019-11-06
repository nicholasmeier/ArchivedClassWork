#ifndef HUFFTREE_H
#define HUFFTREE_H

#include "MinHeap.h"
#include "TreeNode.h"
#include <string>
#include <vector>

#define MAXCODELENGTH 256
#define BYTESIZE 8
#define MAXCSIZE 256

using namespace std;

class HuffTree
{
	private:
		vector<string> codes;
		TreeNode * hufftree;
		string codelist[256];
	public:
		HuffTree();

		//build a huffman  tree  give a minheap
		void buildTree(MinHeap * mh);

		//generate codes by traversing the huffman tree
		//and store them in an internal data structure (array of strings for example)
		void generateCodes();
		void coder(TreeNode * p, string s);

		//returns root of the tree
		TreeNode * getRoot();

		//returns huffman code from the ascii code
		string getCharCode(int c);
		~HuffTree();
		
};


#endif
