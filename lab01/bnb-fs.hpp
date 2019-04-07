#ifndef bnb_fs_hpp
#define bnb_fs_hpp

#include "optimization.hpp"
#include "input.hpp"

// encontra os nos com melhores estimativas
vector<int> bound(vector< vector<int> > jobs, int n, vector<int> order, vector<int> remaining);

// recursao para encontrar a melhor solucao
void branch(vector< vector<int> > jobs, int n, vector<int> order, vector<int> remaining);

// imprime a melhor solucao encontrada
void print_best_solution();

#endif /* bnb_fs_hpp */
