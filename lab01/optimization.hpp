#ifndef optimization_hpp
#define optimization_hpp

#include <stdio.h>
#include "input.hpp"

// imprime soma total na maquina 2
void total_time(vector<Job> jobs, vector<int> order);

// retorna soma total na maquina 2
int total_time_sum(vector<Job> jobs, vector<int> order);

// retorna vector de tempos finais na maquina 1
vector<int> final_times_m1(vector<Job> jobs, vector<int> order);

// retorna vector de tempos finais na maquina 2
vector<int> final_times_m2(vector<Job> jobs, vector<int> order);

// Calcula o valor para um candidato para limitante dual relaxando o problema em relacao a maquina 1
int calcula_S1(vector<int> m, vector<int> f, int n, int r, vector<int> tempo_maquina1, vector<int> tempo_maquina2);

// Calcula o valor para um candidato para limitante dual relaxando o problema em relacao a maquina 2
int calcula_S2(vector<int> m, vector<int> f1, vector<int> f2, int n, int r, vector<int> tempo_maquina1, vector<int> tempo_maquina2);

// calcula estimativa
void calcula_estimativa(vector<Job> jobs, int n, int r, vector< vector<int> > d);

#endif /* optimization_hpp */
