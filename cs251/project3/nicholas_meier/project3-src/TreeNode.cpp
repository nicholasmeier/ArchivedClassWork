#include <iostream>
#include "TreeNode.h"

#include <string>


TreeNode::TreeNode(unsigned char val, unsigned int frequency)
{
	this->frequency = frequency;
	this->val = val;
	this->charCode = "-1";
	this->right = NULL;
	this->left = NULL;
}

unsigned TreeNode::getFrequency()
{
	return frequency;
}
unsigned TreeNode::getVal()
{
	return val;
}
