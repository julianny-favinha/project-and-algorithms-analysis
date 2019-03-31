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

// le o aqruivo com as tarefas
vector< vector<int> > read_jobs_file(char *name);

// imprime o vetor com as tarefas
void print_jobs(vector< vector<int> > jobs);

vector<int> read_params_file(char *name);

#endif /* input_hpp */
