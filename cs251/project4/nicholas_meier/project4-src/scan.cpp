#include <iostream>
#include <fstream>
#include <vector> 
#include <algorithm>
#include "SimpleHTMLParser.h"
#include "Node.h"
#include "RBTree.h"
using namespace std;

// Output stream operator overload for node elements
ostream& operator<<(ostream& out, const vector<pair<string, int>>& e);

int main(int argc, char** argv) {
	if (argc != 3){
		cout << "Usage: scan <input> <output>" << endl;
	}
	
	RBTree<string, vector<pair<string, int>>> rbt;
	ifstream inputfile(argv[1]);
	string line; //url
	SimpleHTMLParser parser;
	bool firstLine = true;
	int URLS = 0;
	while(getline(inputfile, line)){
		if (firstLine){
			string::size_type sz;
			URLS = stoi(line, &sz);	
			firstLine = false;
		} else if (parser.parse(line)){
			string words = parser.getText();
			
			//split the list of words
			vector<string> v;
			string::size_type j = words.find(' ');
			string::size_type i = 0;
			while (j != string::npos) {
				v.push_back(words.substr(i, j-1));
				i = j++;
				j = words.find(' ', j);
				if (j == string::npos) { v.push_back(words.substr(i, words.length())); }
			}
			for (int x = 0; x < v.size(); x++){
				Node<string, vector<pair<string, int>>> * n = rbt.search(v.at(x));
				if (n == NULL){
					vector<pair<string, int>> z;
					z.push_back(make_pair(line, 1));
					rbt.insert(v.at(x), z);
				}else{
					vector<pair<string, int>> z = n->getElement();
					/*auto it = find_if( z.begin(), z.end(), 
					[](const pair<string, int>& element) {
						return element.first == line;
					});
					*/
					for (int y = 0; y < z.size(); y++){
						if (!(z.at(y).first.compare(line))){
							z.at(y).second += 1;
							break ; 
						}
						if (y == z.size()-1){
							rbt.search(v.at(x))->getElement().push_back(make_pair(line, 1));
						}
					}
					//URL not found
				}
				//check if node is in rbt, if not insert it. ye
				// if node is null then make it a thing
			}
			
						
		}
	}
	filebuf fb;
	fb.open(argv[2], ios::out);
	ostream out(&fb);
	out << rbt << endl;
	return 0;
}


// This function overloads the output stream operator for Node
// elements. It has been provided to you and does not need to be
// altered.
ostream& operator<<(ostream& out, const vector<pair<string, int>>& e) {
	// Write the number of elements in the vector
	out << e.size() << endl;
	for (auto& uf : e)
		// Write each member of each pair the vector stores
		out << uf.first << " " << uf.second << endl;
	return out;
}
