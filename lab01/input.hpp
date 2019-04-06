#ifndef input_hpp
#define input_hpp

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <climits>
#include <chrono>
#include <time.h>
#include <cstdlib>
#include <queue>

using namespace std;

struct Jobs {
	vector< vector<int> > jobs;
	int count;
};

struct Params {
	int max_nodes_count;
	int max_time;
};

struct BestSolution {
    vector<int> order;
    int sum;
};

struct Bounds {
	int lower_bound;
	int upper_bound;
};

// le o aqruivo com as tarefas
Jobs read_jobs_file(char *name);

// imprime o vetor com as tarefas
void print_jobs(vector< vector<int> > jobs);

// le o arquivo com o numero maximo de nos a serem explorados na arvore e o tempo maximo de execucao
Params read_params_file(char *name);

void print_params(Params params);

#endif /* input_hpp */
