#ifndef optimization_hpp
#define optimization_hpp

#include <stdio.h>
#include "input.hpp"

// imprime soma total na maquina 2
void print_total_time(vector< vector<int> > jobs, vector<int> order);

// retorna soma total na maquina 2
int total_time_sum(vector< vector<int> > jobs, vector<int> order);

// retorna vector de tempos finais na maquina 1
vector<int> final_times_m1(vector< vector<int> > jobs, vector<int> order);

// retorna vector de tempos finais na maquina 2
vector<int> final_times_m2(vector< vector<int> > jobs, vector<int> order);

// Calcula o valor para um candidato para limitante dual relaxando o problema em relacao a maquina 1
int s1(vector<int> m, vector<int> f, int n, int r, vector<int> time_machine1, vector<int> time_machine2);

// Calcula o valor para um candidato para limitante dual relaxando o problema em relacao a maquina 2
int s2(vector<int> m, vector<int> f1, vector<int> f2, int n, int r, vector<int> time_machine1, vector<int> time_machine2);

// calcula estimativa
int estimate_lower_bound(vector< vector<int> > jobs, vector<int> m, int r);

#endif /* optimization_hpp */
