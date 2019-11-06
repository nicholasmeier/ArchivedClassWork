#ifndef TREENODE_H
#define TREENODE_H

#include <string>

using namespace std;

class TreeNode
{
	private:
		TreeNode * left;
		TreeNode * right;
		unsigned frequency;
		unsigned char val;
		string charCode;
	public:
		
		//TreeNode();
		TreeNode(unsigned char val, unsigned frequency);

		//returns true if this node is a leaf node, returns false otherwise
		bool isLeafNode(){
			if (this->left == NULL && this->right == NULL){
				return true;
			}
			return false;
		};

		//puts left as the left child and right as the right child of this node
		void join(TreeNode * left, TreeNode * right){
			this->left = left;
			this->right = right;
		};

		TreeNode * getLeft(){
			return this->left;	
		}; //returns the left child
		
		TreeNode * getRight(){
			return this->right;
		}; //returns the right child
		

		//Used for making the huffman codes 
		string getCharCode(){
			return this->charCode;
		}

		void setCharCode(string s){
			this->charCode = s;
		}
		
		unsigned getFrequency(); //returns frequency member variable
		unsigned getVal(); //returns val member variable
};


#endif
