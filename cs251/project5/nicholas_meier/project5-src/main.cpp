#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "graph.h"
 
using namespace std;


 // Part 1 : Find the connected components and the bridge
void part1()
{
	int check = 0;
	string line;
	Graph g;
	while (getline(cin, line)){
		if (check < 2){
			if (check == 1){
				istringstream iss(line);
				vector<string> v;
				do{
					string sub;
					iss >> sub;
					v.push_back(sub);
				}while(iss);
				g.setCityNum(stoi(v.at(0)));
				g.setRouteNum(stoi(v.at(1)));
			}	
			check++;
		}else{
			vector<string> v;
			istringstream iss(line);
			do {
				string sub;
				iss >> sub;
				v.push_back(sub);
			}while(iss);
			g.addRoute(v.at(0), v.at(1), stod(v.at(2)));
		}
	}
	g.analyseGraph();
	g.printAdjList(1);
}

 // Part 2 :  Find a ticket using Dijkstra	
void part2()
{
	int check = 0;
	string line;
	Graph g;
	while (getline(cin, line)){
		if (line.compare("END") == 0){ return; }
		if (check < 2){
			if (check == 1){
				istringstream iss(line);
				vector<string> v;
				do{
					string sub;
					iss >> sub;
					v.push_back(sub);
				}while(iss);
				g.setCityNum(stoi(v.at(0)));
				g.setRouteNum(stoi(v.at(1)));
			}	
		}else{
			if (check < g.getRouteNum()+2){

				vector<string> v;
				istringstream iss(line);
				do {
					string sub;
					iss >> sub;
					v.push_back(sub);
					//cout << sub << endl;
				}while(iss);
				g.addRoute(v.at(0), v.at(1), stod(v.at(2)));
			}else{
				vector<string> v;
				istringstream iss(line);
				do {
					string sub;
					iss >> sub;
					v.push_back(sub);
				}while(iss);
				g.findCost(v.at(0), v.at(1));
			}
		}
		check++;
		//cout << check << endl;
	}
	//g.printAdjList(0);
}

 // Part 3 :Try to do a tour of all cities 
void part3()
{
	int check = 0;
	string line;
	Graph g;
	while (getline(cin, line)){
		if (line.compare("END") == 0){ return; }
		if (check < 2){
			if (check == 1){
				istringstream iss(line);
				vector<string> v;
				do{
					string sub;
					iss >> sub;
					v.push_back(sub);
				}while(iss);
				g.setCityNum(stoi(v.at(0)));
				g.setRouteNum(stoi(v.at(1)));
			}	
		}else{
			if (check < g.getRouteNum()+2){

				vector<string> v;
				istringstream iss(line);
				do {
					string sub;
					iss >> sub;
					v.push_back(sub);
					//cout << sub << endl;
				}while(iss);
				g.addRoute(v.at(0), v.at(1), stod(v.at(2)));
			}else{
				vector<string> v;
				istringstream iss(line);
				do {
					string sub;
					iss >> sub;
					v.push_back(sub);
				}while(iss);
				//run the Euler tour
				//cout << v.at(0);
				g.eulerianTour(v.at(0));
			}
		}
		check++;
		//cout << check << endl;
	}

}
 
 int main()
 {
	 double cmd;
	 cin >> cmd;

	 if(cmd == 1) part1();
	 else if(cmd == 2) part2();
	 else if(cmd == 3) part3();

	 return 0;
 }
