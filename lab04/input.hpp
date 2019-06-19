#ifndef input_hpp
#define input_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
#include <set>
#include <climits>
#include <algorithm>
#include <iomanip>
#include <thread>

using namespace std;

struct Node {
	int id;
	int max_degree;
};

struct Edge {
	int src;
	int dest;
	float cost;
};

struct NodeDestiny {
	int id;
	float cost;
};

struct NodeSource {
	int max_degree;
	vector<NodeDestiny> adj;
};

struct Graph {
	int V;
	int E;
	vector<NodeSource> adjacency;
};

Graph read_file(char *name);
void print_graph(vector<NodeSource> adjacency);
vector<int> calculate_degrees(vector<NodeSource> adjacency);

#endif /* input_hpp */