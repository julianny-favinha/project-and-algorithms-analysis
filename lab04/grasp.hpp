#ifndef grasp_hpp
#define grasp_hpp

#include "agm.hpp"

using namespace std;

struct Agm {
	int cost;
	vector<NodeSource> adjacency;
};

Agm agm_grasp(int V, int E, vector<NodeSource> adjacency, vector< pair< pair<int, int>, int > > adjacency_edges, float interval);

#endif /* grasp_hpp */
