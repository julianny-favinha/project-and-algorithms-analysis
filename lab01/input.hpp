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

struct Input {
	vector< vector<int> > jobs;
	int count;
};

// le o aqruivo com as tarefas
Input read_jobs_file(char *name);

// imprime o vetor com as tarefas
void print_jobs(vector< vector<int> > jobs);

vector<int> read_params_file(char *name);

#endif /* input_hpp */
