#ifndef grasp_hpp
#define grasp_hpp

#include "agm.hpp"

using namespace std;

vector<NodeSource> agm_grasp(int V, int E, vector<NodeSource> adjacency, float interval, int times);

#endif /* grasp_hpp */
