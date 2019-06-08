#ifndef input_hpp
#define input_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
#include <set>

using namespace std;

struct Node {
	int id;
	int max_degree;
};

struct Edge {
	int src;
	int dest;
	int cost;
};

struct NodeDestiny {
	int id;
	int cost;
};

struct NodeSource {
	int max_degree;
	vector<NodeDestiny> adj;
};

extern vector<Edge> edges_list;

// 
vector<NodeSource> read_file(char *name);

#endif /* input_hpp */