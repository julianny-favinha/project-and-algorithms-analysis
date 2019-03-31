#ifndef input_hpp
#define input_hpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <climits>

using namespace std;

struct Job {
    int time1;
    int time2;
};

// Le o aqruivo com as tarefas
vector<Job> read_jobs_file(char *name);

// Vizualiza o vetor com as tarefas
int print_jobs(vector<Job> jobs);

vector<int> read_params_file(char *name);

#endif /* input_hpp */
