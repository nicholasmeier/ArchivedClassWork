#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <utility>
#include <map>
#include <vector>

using namespace std;

class Graph
{
	private:
		int numCities;
		int numRoutes;
		map<string, vector<pair<string,double>>> cities;
		
		map<string, bool> marked;
		map<string, int> disc;
		map<string, int> low;
		map<string, string> parent;
		vector<string> critRoutes;
		int numConCom;

		vector<string> cityList;
	public:		
		Graph(int, int);
		Graph();
		void setCityNum(int);
		void setRouteNum(int);
		int getCityNum();
		int getRouteNum(); 
		// Create the graph
		void addRoute(string, string, double);
		
		//utility method for min of maps
		pair<string,double> getMin(map<string,double>, vector<string>);
		
		//used for testing
		void printAdjList(int);

		//part 1: Find the connected components and the bridges
		void analyseGraph();
		void analyseHelper(string);
		//void analyseHelper(int, string, map<string, bool>, vector<pair<string, int>>, vector<pair<string, int>>, map<string, string>);
		
		// Part 2: Find a ticket using Dijkstra	
		void findCost(string, string);
		
		// Part 3: Try to do a tour of all cities
		void eulerianTour(string);  // NOTE : If necessary, you can change the parameters to this function.

};

#endif
