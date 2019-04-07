#ifndef bnb_fs_hpp
#define bnb_fs_hpp

#include "optimization.hpp"
#include "input.hpp"

vector<int> bound(vector< vector<int> > jobs, int n, vector<int> order, vector<int> remaining);

void branch(vector< vector<int> > jobs, int n, vector<int> order, vector<int> remaining);

void print_best_solution();

#endif /* bnb_fs_hpp */
