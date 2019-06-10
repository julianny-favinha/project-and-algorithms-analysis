#ifndef agm_hpp
#define agm_hpp

#include "input.hpp"

using namespace std;

vector<NodeSource> agm(vector<NodeSource> adjacency);
vector<NodeSource> agm_with_degree_restriction(vector<NodeSource> adjacency);

#endif /* agm_hpp */
