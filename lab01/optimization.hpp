#ifndef optimization_hpp
#define optimization_hpp

#include <stdio.h>
#include "input.hpp"

// imprime soma total na maquina 2
void print_total_time(vector< vector<int> > &jobs, vector<int> order);

// retorna soma total na maquina 2
int total_time_sum(vector< vector<int> > &jobs, vector<int> order);

// retorna os tempos finais na maquina 1
vector<int> final_times_m1(vector< vector<int> > &jobs, vector<int> order);

// retorna os tempos finais na maquina 2
vector<int> final_times_m2(vector< vector<int> > &jobs, vector<int> order);

// compara dois jobs na estrutura JobsMachine de acordo com o tempo na maquina 1
bool compare_two_jobs_machine1(JobsMachine lhs, JobsMachine rhs);

// compara dois jobs na estrutura JobsMachine de acordo com o tempo na maquina 2
bool compare_two_jobs_machine2(JobsMachine lhs, JobsMachine rhs);

// atualiza o limitante primal
void update_upper_bound(vector< vector<int> > &jobs, int n, vector<int> order, vector<JobsMachine> jobs_machine);

// calcula o valor para um candidato para limitante dual relaxando o problema em relacao a maquina 1
int s1(vector< vector<int> > &jobs, int n, vector<JobsMachine> jobs_machine, vector<int> order, vector<int> remaining, vector<int> final_times_machine1);

// calcula o valor para um candidato para limitante dual relaxando o problema em relacao a maquina 2
int s2(vector< vector<int> > &jobs, int n, vector<JobsMachine> jobs_machine, vector<int> order, vector<int> remaining, vector<int> final_times_machine1, vector<int> final_times_machine2);

vector<JobsMachine> build_jobs_machine(vector< vector<int> > &jobs, vector<int> &remaining);

// calcula estimativas
Bounds estimate_bounds(vector< vector<int> > &jobs, int n, vector<int> order, vector<int> remaining);

#endif /* optimization_hpp */
