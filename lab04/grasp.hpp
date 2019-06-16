#ifndef grasp_hpp
#define grasp_hpp

#include "agm.hpp"

using namespace std;

struct Agm {
	int cost;
	vector<NodeSource> adjacency;
};

Agm agm_grasp(int V, int E, vector<NodeSource> adjacency, float interval);

#endif /* grasp_hpp */
