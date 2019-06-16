#ifndef agm_hpp
#define agm_hpp

#include "input.hpp"

using namespace std;

int calculate_cost(vector<NodeSource> adjacency);
vector< pair< pair<int, int>, int > > create_adjacency_edges(vector<NodeSource> adjacency);
vector<NodeSource> transform(vector<NodeSource> adjacency, vector< pair< pair<int, int>, int > > agm_edges, int nodes);
bool sort_cost_ascending(const pair< pair<int, int>, int > &rhs, const pair< pair<int, int>, int > &lhs);
void make_set(vector<int> *c);
int find_set(vector<int> c, int i);
void union_set(vector<int> *c, int i, int j);
vector<int> find_vertices(vector<int> c, int component);
int sum_degrees(vector<NodeSource> adjacency, vector<int> vertices_ci);
vector<NodeSource> agm(vector<NodeSource> adjacency);
vector<NodeSource> agm_with_degree_restriction(int V, int E, vector<NodeSource> adjacency);

#endif /* agm_hpp */
