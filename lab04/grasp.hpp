#ifndef grasp_hpp
#define grasp_hpp

#include "agm.hpp"

using namespace std;

struct Agm {
	int cost;
	vector<NodeSource> adjacency;
};

pair< bool, vector<NodeSource> > agm_grasp(int V, int E, vector<NodeSource> adjacency, float interval, clock_t start_time, int max_time);

#endif /* grasp_hpp */
