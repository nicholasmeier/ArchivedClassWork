#ifndef PROJ4_RBTREE_H
#define PROJ4_RBTREE_H

#include "Node.h"
#include <iostream>
#include <queue>
using namespace std;

template <typename Key, typename Element>
class RBTree {
private:
	size_t s; // The size of tree
	Node<Key, Element> * root; //root node
public:
	// Implement each of these methods
	RBTree() {
		this->s = 0;
		this->root = NULL;
	}
	void insert(const Key& k, const Element& e);
	void replace(Node<Key, Element> * n_new, Node<Key, Element> * n_old);
	void rightRotate(Node<Key, Element> * n);
	void leftRotate(Node<Key, Element> * n);
	void validate_nodes(Node<Key, Element> * n);
	Node<Key, Element>* search(const Key& k);
	void del(const Key& k);
	void helpInOrd(ostream& out, Node<Key, Element> * n) const;
	void inorder(ostream& out) const;
	void levelOrder(ostream& out) const;
	size_t size() const;
	Node<Key, Element>* getRoot();
};


// Function to insert a new node with given data
template <typename Key, typename Element>
void RBTree<Key, Element>::insert(const Key& k, const Element& e) {
	Node<Key, Element> *n = new Node<Key, Element>();
	n->setKey(k);
	n->setElement(e);
	if (size() == 0){
		n->setColor(BLACK);
		this->root = n;
		this->s += 1;
		return ;
	}
	Node<Key, Element> * r = this->root;
	while(1){
		if (n->getKey() == r->getKey()){ //if keys are equal, replace the value at the node
			r->setElement(n->getElement());
			return ;
		}else if (n->getKey() < r->getKey()) {
			if (r->leftNode == NULL){
				r->leftNode = n;
				break ;
			}else{
				r = r->leftNode;
			}
		}else if (n->getKey() > r->getKey()) {
			if (r->rightNode == NULL){
				r->rightNode = n;
				break ;
			}else{
				r = r->rightNode;
			}
		}
	}
	n->parent = r;
	
	this->s += 1;
	validate_nodes(n);
}

template <typename Key, typename Element>
void RBTree<Key, Element>::replace(Node<Key, Element> * n_new, Node<Key, Element> * n_old){
	if (n_old->parent == NULL) { this->root = n_new; }
	else {
		if (n_old == n_old->parent->leftNode){
			n_old->parent->leftNode = n_new;
		}else{
			n_old->parent->rightNode = n_new;
		}
	}
	if (n_new != NULL){
		n_new->parent = n_old->parent;
	}
}

template <typename Key, typename Element>
void RBTree<Key, Element>::rightRotate(Node<Key, Element> * n){
	Node<Key, Element> * left = n->leftNode;
	n->leftNode = left->rightNode;
	if (n->leftNode != NULL){
		n->leftNode->parent = n;
	}
	left->parent = n->parent;
	if (n->parent == NULL){
		this->root = left;
	}else if(n == n->parent->leftNode){
		n->parent->leftNode = left;
	}else {
		n->parent->rightNode = left;
	}
	left->rightNode = n;
	n->parent = left;
	/*Node<Key, Element> * left = n->leftNode;
	replace(left, n);
	n->leftNode = left->rightNode;
	if (left->rightNode != NULL){
		left->rightNode->parent = n;
	}
	left->rightNode = n;
	n->parent = left;*/
}

template <typename Key, typename Element>
void RBTree<Key, Element>::leftRotate(Node<Key, Element> * n){
	Node<Key, Element> * right = n->rightNode;
	n->rightNode = right->leftNode;
	if (n->rightNode != NULL){
		n->rightNode->parent = n;
	}
	right->parent = n->parent;
	if (n->parent == NULL){
		this->root = right;
	}else if (n == n->parent->leftNode){
		n->parent->leftNode = right;
	}else{
		n->parent->rightNode = right;
	}
	right->leftNode = n;
	n->parent = right;
	/*
	Node<Key, Element> * right = n->rightNode;
	replace(right, n);
	n->righ
#include <queue>tNode = right->leftNode;
	if (right->leftNode != NULL){
		right->leftNode->parent = n;
	}
	right->leftNode = n;
	n->parent = right;
	*/
}

template <typename Key, typename Element>
void RBTree<Key, Element>::validate_nodes(Node<Key, Element> * n){
	//TODO create local nodes to hold n->parent, n->getGrandpa, n->getUncle()
	while((n != this->root) && (n->getColor() != BLACK) && (n->parent->getColor() == RED)){
		Node<Key, Element> * p = n->parent;
		Node<Key, Element> * g = n->getGrandpa();
		if (p == g->leftNode){
			Node<Key, Element> * u = n->getGrandpa()->rightNode;
			if (u != NULL && u->getColor() == RED){
				g->setColor(RED);
				p->setColor(BLACK);
				u->setColor(BLACK);
				n = g;
			}else if (n == p->rightNode){
				leftRotate(p);
				n = p;
				p = n->parent;
			}
			//need to fix right rotate
			//change replace method
			rightRotate(g);
			//swap(p->getColor(), g->getColor());
			color pc = p->getColor();
			p->setColor(g->getColor());
			g->setColor(pc);
			n = p;
		}else{
			Node<Key, Element> * u = n->getGrandpa()->leftNode;
			if (u != NULL && u->getColor() == RED){
				g->setColor(RED);
				p->setColor(BLACK);
				u->setColor(BLACK);
				n = g;
			}else if(n == p->leftNode){
				rightRotate(p); //nned to fix still
				n = p;
				p = n->parent;
			}
			leftRotate(g);
			//swap(p->getColor(), g->getColor());
			color pc = p->getColor();
			p->setColor(g->getColor());
			g->setColor(pc);
			n = p;			
		}
	}
	root->setColor(BLACK);
	/*if (n->parent == NULL){ //check if the root node is black
		n->setColor(BLACK);
	}else if (n->parent->getColor() == BLACK){
		return ;
	}else if (n->getUncle() != NULL && n->getUncle()->getColor() == RED){
		//parent can't be red if the child is red
		n->parent->setColor(BLACK);
		n->getUncle()->setColor(BLACK);
		n->getGrandpa()->setColor(RED);
		//if (n->getUncle() != NULL) { n->getUncle()->setColor(BLACK); }
		//if (n->getGrandpa() != NULL) { n->getGrandpa()->setColor(RED); }
		validate_nodes(n->parent->parent);
	}else if (n == n->parent->rightNode && n->getGrandpa() != NULL && n->parent == n->getGrandpa()->leftNode){
		leftRotate(n);
	}else if (n == n->parent->rightNode && n->getGrandpa() != NULL && n->parent == n->getGrandpa()->leftNode){
		rightRotate(n);
	}else{
		n->parent->setColor(BLACK);
		if (n->getGrandpa() != NULL){
			n->parent->parent->setColor(RED);
			if (n == n->parent->leftNode && n->parent == n->parent->parent->leftNode){
				rightRotate(n);
			}
			if (n == n->parent->rightNode && n->parent == n->parent->parent->rightNode){
				leftRotate(n);
			}
		}
	}*/
}

// Function to search a node with given data
template <typename Key, typename Element>
Node<Key, Element>* RBTree<Key, Element>::search(const Key& k) {
	Node<Key, Element> *n = this->root;
	while(n != NULL){
		if (n->getKey() > k){
			n = n->leftNode;
		}else if (n->getKey() < k){
			n = n->rightNode;
		}else{
			return n;
		}
	}
	//Not found
	return NULL;
}

// Function to delete a node with given data
template <typename Key, typename Element>
void RBTree<Key, Element>::del(const Key& k) {
}

// Function to do inorder traversal
template <typename Key, typename Element>
void RBTree<Key, Element>::inorder(ostream& out) const {
	Node<Key, Element> * n = this->root;
	helpInOrd(out, n);
}

// Recursive Helper function for inoder traversal
template <typename Key, typename Element>
void RBTree<Key, Element>::helpInOrd(ostream& out, Node<Key, Element> * n) const {
	if (n == NULL){ return ; }
	helpInOrd(out, n->leftNode);
	out << * n << endl;
	helpInOrd(out, n->rightNode);
}

// Function to perform level-order traversal
template <typename Key, typename Element>
void RBTree<Key, Element>::levelOrder(ostream& out) const {
	if (this->root == NULL){ return ; }
	std::queue<Node<Key, Element>*> q;
	q.push(this->root);
	while(!q.empty()){
		Node<Key, Element> * t = q.front();
		out << * t << endl;
		q.pop();
		//if (t->rightNode != NULL){ q.push(t->rightNode); }
		if (t->leftNode != NULL){ q.push(t->leftNode); }
		if (t->rightNode != NULL){ q.push(t->rightNode); }
	}
}

// Function to return the number of nodes in the RBTree
template <typename Key, typename Element>
size_t RBTree<Key, Element>::size() const { return this->s; }

// Function to return a pointer to the root of the RBTree
template <typename Key, typename Element>
Node<Key, Element>* RBTree<Key, Element>::getRoot() { return NULL; }


// The following functions have been provided for you, and do not
// need to be modified:


// Output stream operator overload -- writes the contents of the
// entire RBTree to the specified output stream, using an in-order
// traversal. Note that there must be a similar operator overload
// for Key and Element types in order for this to compile.
// Example usage:
//		RBTree<K,E> rbt;
//		cout << rbt << endl;
template <typename Key, typename Element>
ostream& operator<<(ostream& out, const RBTree<Key, Element>& rbtree) {
	out << rbtree.size() << endl;
	rbtree.inorder(out);
	return out;
}

// Input stream operator overload -- reads the contents of an RBTree
// from the specified input stream, inserting each Node as it is
// read. Note that there must be a similar operator overload for Key
// and Element types in order for this to compile.
// Example usage:
//		RBTree<K,E> rbt;
//		cin >> rbt;
template <typename Key, typename Element>
istream& operator>>(istream& in, RBTree<Key, Element>& rbtree) {
	// Clear existing contents, if any
	rbtree = RBTree<Key, Element>();

	// Read in the number of elements to read
	size_t size;
	in >> size;
	for (size_t i = 0; i < size; i++) {
		// Read in each node as it appears in the stream
		Node<Key, Element> n;
		in >> n;
		// Insert the node into the RBTree
		rbtree.insert(n.getKey(), n.getElement());
	}

	return in;
}

#endif //PROJ4_RBTREE_H
