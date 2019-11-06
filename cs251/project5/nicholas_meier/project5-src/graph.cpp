#include <iostream>
#include <string>
#include <utility>
#include <algorithm>
#include <map>
#include "graph.h"

#define INFI 1000000
/*
	Graph class for use in Project 5.
	This provided code does not include any specific data structure
	for storing a graph, only a list of methods that will be called 
	from main. Based on the project handout and the lecture slides,
	select a method for storing the graph and implement it here.
*/
// Constructor for Graph

using namespace std;
Graph::Graph()
{
	this->numCities = 0;
	this->numRoutes = 0;
	this->numConCom = 0;
}

Graph::Graph(int v, int e){
	this->numCities = v;
	this->numRoutes = e;
	this->numConCom = 0;
}
void Graph::setCityNum(int v){
	this->numCities = v;
}

void Graph::setRouteNum(int e){
	this->numRoutes = e;
}
int Graph::getCityNum(){
	return this->numCities;
}
int Graph::getRouteNum(){
	return this->numRoutes;
}

void Graph::addRoute(string s1, string s2, double d){
	//add s2 to the adjaceny list for s1
	if ((this->cities).find(s1) == (this->cities).end() ){
		vector<pair<string, double>> v;
		pair<string, double> p = make_pair(s2, d);
		v.push_back(p);
		//v.push_back(make_pair<string, double>(s2.c_str(), d));
		this->cities.insert(pair<string, vector<pair<string,double>>>(s1, v));
		cityList.push_back(s1);

		this->marked.insert(pair<string, bool>(s1, false));
		this->disc.insert(pair<string, int>(s1, 0));
		this->low.insert(pair<string,int>(s1, 0));
		this->parent.insert(pair<string,string>(s1, "NULL"));
		
	}else{
		cities.find(s1)->second.push_back(make_pair(s2,d));
	}
	
	//add s1 to the adjacency list for s2
	if ((this->cities).find(s2) == (this->cities).end() ){
		vector<pair<string, double>> v;
		pair<string, double> p = make_pair(s1, d);
		v.push_back(p);
		//v.push_back(make_pair<string, double>(s2.c_str(), d));
		this->cities.insert(pair<string, vector<pair<string,double>>>(s2, v));
		cityList.push_back(s2);

		this->marked.insert(pair<string, bool>(s2, false));
		this->disc.insert(pair<string, int>(s2, 0));
		this->low.insert(pair<string,int>(s2, 0));
		this->parent.insert(pair<string,string>(s2, "NULL"));

	}else{
		cities.find(s2)->second.push_back(make_pair(s1,d));
	}
}


//Used for Debugging
void Graph::printAdjList(int condition){
	switch(condition){
		case 0:	
			for (map<string, vector<pair<string,double>>>::iterator it = cities.begin(); it!=cities.end(); it++){
				cout << it->first << " : ";
				for (int i = 0; i < it->second.size(); i++){
					cout << "(" << get<0>(it->second.at(i)) << ", " << get<1>(it->second.at(i)) << ") ";
				}
				cout << endl;
			}
			break ;
		case 1:
			cout << this->numConCom << endl;
			cout << this->critRoutes.size() << endl;
			for (int i = 0; i < this->critRoutes.size(); i++){
				cout << this->critRoutes.at(i) << endl;
			}
			break;
	}
}

void Graph::eulerianTour(string s){
	cout << s << endl;
	//Kruskal's Alg --> create MST
	
	//Compute Euler Tour
} 

struct CompareSecond{
	bool operator()(const pair<string,double>& left, const pair<string,double>& right) const{
		return left.second < right.second;
	}
};

pair<string, double> Graph::getMin(map<string, double> m, vector<string> e){
	map<string,double> m_c = m;
	for (int i = 0; i < e.size(); i++){
		m_c.erase(e.at(i));
	}
	pair<string, double> min = *min_element(m_c.begin(), m_c.end(), CompareSecond());
	return min;
}

void Graph::findCost(string source, string dest){
	//priority_queue<pair<string, int>
	map<string, double> distList;
	map<string, string> parentList;
	vector<string> q;
	
	vector<string> excl;
	for (int i = 0; i < cityList.size(); i++){
		q.push_back(cityList.at(i));
		distList.insert(pair<string,double>(cityList.at(i), INFI));
		parentList.insert(pair<string,string>(cityList.at(i), "NULL"));
	}
	distList.at(source) = 0; 
	map<string, vector<pair<string,double>>>::iterator it = this->cities.find(source);
	vector<pair<string, double>> adjList = it->second;
	while(q.size() > 0){
		//U CANNOT BE IN Q ffs
		string u = getMin(distList, excl).first;

		if (u.compare(dest) == 0){
			vector<string> o;
			string z = u;
			do {
				o.push_back(z);
				z = parentList.at(z);
			}while( (parentList.find(z) != parentList.end()) && (z.compare(source) != 0));
			cout << source << " ";
			for (int i = o.size()-1; i >= 0; i--){
				cout << o.at(i) << " ";
			}
			cout.precision(2);
			cout << fixed << distList.at(dest) << endl;
			//print out the thing
			break;
		}
		excl.push_back(u);
		vector<string>::iterator rem = find(q.begin(), q.end(), u);
		q.erase(remove(q.begin(), q.end(), u), q.end());
		it = this->cities.find(u);
		adjList = it->second;
		for (int i = 0; i < adjList.size(); i++){
			string v = adjList.at(i).first; // find the string in the adjlist
			if (find(q.begin(), q.end(), v) != q.end()){
				double alt = distList.at(u) + adjList.at(i).second;
				//alt = distance of u from source plus distance between u and v
				if (alt < distList.at(v)){
					distList.at(v) = alt;
					parentList.at(v) = u;
				}
			}
		}
	}
	//TODO print the formatted output, etc
}


void Graph::analyseGraph(){
	for (map<string,bool>::iterator it = this->marked.begin(); it!=this->marked.end(); it++){
		if (it->second == false){
			analyseHelper(it->first);
			this->numConCom++;
		}
	}
	sort(this->critRoutes.begin(), this->critRoutes.end());		
}

void Graph::analyseHelper(string city){
	static int time = 0;
	this->marked.at(city) = true;
	this->disc.at(city) = ++time;
	this->low.at(city) = ++time;
	map<string, vector<pair<string,double>>>::iterator it = this->cities.find(city);
	vector<pair<string, double>> adjList = it->second;
	for (int i = 0; i < adjList.size(); i++){
		string adj = adjList.at(i).first;
		if (this->marked.at(adj) == false){
			this->parent.at(adj) = city;
			analyseHelper(adj);
			this->low.at(city) = min(this->low.at(city), this->low.at(adj));
			if (this->low.at(adj) > this->disc.at(city)){
				string crit;
				if (city.compare(adj) > 0){
					crit = adj + " " + city;
				}else{
					crit = city + " " + adj;
				}
				//cout << crit << endl;
				this->critRoutes.push_back(crit);
			}
		}else if(adj.compare(this->parent.at(city)) != 0){
			this->low.at(city) = min(this->low.at(city), this->disc.at(adj));
		}
	}

}

/*

void Graph::analyseGraph(){ 
	
	
	//Need to rethink this a bit. Rework the matrices to c++ string thing
	map<string, bool> marked;//Keep track of visited vertices
	vector<pair<string, int>> disc;//store discovery times of visited veritcies
	vector<pair<string, int>> low; //used to compare discovery times
	map<string, string> parent; //store parent verticies of visted vertices
	for (int i = 0; i < cityList.size(); i++){
		marked.insert(pair<string, bool>(cityList.at(i),false));
		disc.push_back(make_pair(cityList.at(i), -1));
		low.push_back(make_pair(cityList.at(i), -1));
		parent.insert(pair<string, string>(cityList.at(i), "NULL"));
		//parent.insert(make_pair<cityList.at(i), "NULL">);
	}
	for (int i = 0; i < this->numCities; i++){
		if (!(marked.at(cityList.at(i)))){
			analyseHelper(i, cityList.at(i), marked, disc, low, parent);
		}
	}
}

void Graph::dfs(string city){
	this->marked.at(city).second = false;
	map<string,vector<pair<string,double>>>:iterator it = this->cities.find(city);
	vector<pair<string,double>> adjList = it->second;
	for (int i = 0; i < adjList.size(); i++){
			
	}
}

void Graph::analyseHelper(int index, string city, map<string, bool> marked, vector<pair<string, int>> disc, vector<pair<string,int>> low, map<string, string> parent){
	static int time = 0;
	marked.at(city) = true;
	disc.at(index).second = low.at(index).second = ++time;
	int x = 0;
	map<string, vector<pair<string,double>>>::iterator it = this->cities.find(city);
	vector<pair<string, double>> adjList = it->second;
	//index = index of city
	//city = current vertex
	for (int j = 0; j < adjList.size(); j++){
		int adjInd = find(cityList.begin(), cityList.end(), adjList.at(j).first) - cityList.begin();
		//adjInd = index of city from adjlist in citylist
		if (!marked.at(adjList.at(j).first)){
			parent.at(adjList.at(j).first) = city;
			analyseHelper(adjInd, adjList.at(j).first, marked, disc, low, parent);
			low.at(index).second = min(low.at(index).second, low.at(adjInd).second);
			if (low.at(adjInd).second > disc.at(index).second){
				string crit = city + " " + cityList.at(adjInd);
				critRoutes.push_back(crit);
				cout << crit << endl;
			}
			//parent.at(city) = adjList.at
		}else if(cityList.at(adjInd).compare(parent.at(city)) != 0){
			low.at(index).second = min(low.at(index).second, disc.at(adjInd).second);
		}
	}

}
*/
