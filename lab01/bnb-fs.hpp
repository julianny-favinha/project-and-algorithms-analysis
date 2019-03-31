#ifndef bnb_fs_hpp
#define bnb_fs_hpp

#include "optimization.hpp"
#include "input.hpp"

struct BestSolution {
    vector<int> order;
    int sum;
};

vector<int> bound(vector< vector<int> > jobs, int n, vector<int> order, vector<int> remaining);

void branch_and_bound(vector< vector<int> > jobs, int n, vector<int> order, vector<int> remaining);

#endif /* bnb_fs_hpp */
