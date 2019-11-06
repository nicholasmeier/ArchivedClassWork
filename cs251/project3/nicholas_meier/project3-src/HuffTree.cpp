#include <iostream>
#include "HuffTree.h"

HuffTree::HuffTree(){

}


void HuffTree::buildTree(MinHeap * mh){
	while( mh->getSize() > 1){
		TreeNode * a = mh->removeMin();
		TreeNode * b = mh->removeMin();
		if (a != NULL && b != NULL){
			TreeNode * internal = new TreeNode(0, a->getFrequency()+b->getFrequency());
			internal->join(a, b);
			mh->insert(internal);
		}
	}
	hufftree = mh->removeMin();
}

//generate huffman codes by traversing the huffman tree
//stores the codes in TreeNode object;
void HuffTree::generateCodes(){
	string s = "";
	TreeNode * p = this->hufftree;
	coder(p, s);
	/*
	TreeNode * l = p->getLeft();
	TreeNode * r = p->getRight();
	
	if (l->isLeafNode()){
		
	}else{
		s.push_back('0');
		p = l;
		
		if (r->isLeafNode()){
			s.pop_back();
		}else{
			s.pop_back();
		}
	}*/
}

void HuffTree::coder(TreeNode * p, string s){
	if (p->isLeafNode()){
		codelist[p->getVal()] = s;
		//printf("symbol %c has code %s", p->getVal(), s.c_str());
	}else{

		s.push_back('1');
		coder(p->getLeft(), s);
		s.pop_back();
	
		s.push_back('0');
		coder(p->getRight(), s);
		s.pop_back();
	}
	/*
	TreeNode * i = p;	
	TreeNode * l = p->left;
	while (l != NULL){
		if (l->isLeafNode()){
			
		}
	}
	*/
}

//get the root of tree
TreeNode * HuffTree::getRoot(){
	return this->hufftree;
}

string HuffTree::getCharCode(int c){
	return codelist[c];
}
