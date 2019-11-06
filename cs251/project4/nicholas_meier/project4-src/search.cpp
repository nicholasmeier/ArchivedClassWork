#include <fstream>
#include <iostream>
#include <vector>
#include <locale>
#include <algorithm>
#include "RBTree.h"
using namespace std;

// Input stream operator overload for node elements
istream& operator>>(istream& in, vector<pair<string, int>>& e);

bool sortFunc (pair<string, int> i, pair<string, int> j){
	if (i.second == j.second){
		return (i.first.compare(j.first) < 0);
	}else{
		return (i > j);
	}
}


int main(int argc, char** argv) {
	// Implement your search code here
	//args [0] = inputfile, [1<] = wordlist
	if (argc < 3){
		cout << "usage: search <input> <word> <word> ... " << endl;
	}
	
	RBTree<string, vector<pair<string, int>>> rbt;
	filebuf fb;
	fb.open(argv[1], ios::in);
	istream in(&fb);
	in >> rbt;
	locale loc;
	vector<vector<pair<string,int>>> foundlist;
	for (int i = 2; i < argc; i++){
		string s = argv[i];
		for (string::size_type j = 0; j < s.length(); j++){
			if (isupper(s[j])){ tolower(s[j], loc); }
		}
		Node<string, vector<pair<string, int>>> * n = rbt.search(s);
		if (n != NULL){
			foundlist.push_back(n->getElement());
		}
	}
	if (foundlist.size() == 0){
		cout << "Not found" << endl;
		return 0;
	}
	vector<pair<string,int>> intersect;
	if (foundlist.size() == 1){
		intersect = foundlist.at(0);
		sort(intersect.begin(), intersect.end(), sortFunc);
		for (int i = 0; i < intersect.size(); i++){
			pair<string,int> j = intersect.at(i);
			cout << j.first << " " << j.second << endl;
		}
		return 0;
	}else{
		sort(foundlist.at(0).begin(), foundlist.at(0).end(), sortFunc);
		sort(foundlist.at(1).begin(), foundlist.at(1).end(), sortFunc);
		set_intersection(foundlist.at(0).begin(),foundlist.at(0).end(), 
				foundlist.at(1).begin(), foundlist.at(1).end(), 
				back_inserter(intersect));
		for (int i = 2; i < foundlist.size(); i++){
			vector<pair<string,int>> temp;
			sort(foundlist.at(i).begin(), foundlist.at(i).end(), sortFunc);
			set_intersection(foundlist.at(0).begin(),foundlist.at(0).end(),
					intersect.begin(), intersect.end(), back_inserter(temp));
			intersect = temp;
		}
		if (intersect.size() == 0){
			cout << "Not found" << endl;
			return 0;
		}else{
			sort(intersect.begin(), intersect.end(), sortFunc);
			for (int i = 0; i < intersect.size(); i++){
				pair<string, int> j = intersect.at(i);
				cout << j.first << " " << j.second << endl;
			}
			return 0; 
		}
	}
	
	cout << "Not found" << endl;
	//Need to find the URLS that appear in every list in the list of lists
	return 0;
}

// This function overloads the input stream operator for Node
// elements. It has been provided to you and does not need to be
// altered.
istream& operator>>(istream& in, vector<pair<string, int>>& e) {
	// Clear any existing contents
	e.clear();

	// Read the number of elements
	size_t n;
	in >> n;
	for (int i = 0; i < n; i++) {
		// Read each member of each pair the vector stores
		string u;
		in >> u;
		int f;
		in >> f;
		// Add to vector
		e.push_back(make_pair(u, f));
	}

	return in;
}
