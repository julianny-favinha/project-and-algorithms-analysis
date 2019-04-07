#include "input.hpp"

Jobs read_jobs_file(char *name) {
    ifstream file;
    file.open(name);

    int n;
    file >> n;

    vector<int> machine1(n);
    vector<int> machine2(n);

    int time1;
    int time2;
    int i = 0;
    while (file >> time1 >> time2) {
        machine1[i] = time1;
        machine2[i] = time2;
        i++;
    };

    vector< vector<int> > jobs;

    jobs.push_back(machine1);
    jobs.push_back(machine2);

    Jobs input = {jobs, n};
    return input;
}

void print_jobs(vector< vector<int> > jobs) {
    for (int i = 0; i < (int)jobs.size(); i++) {
        cout << "Machine " << i+1 << endl;
        for (int j = 0; j < (int)jobs[i].size(); j++) {
            cout << "Job " << j+1 << ": " << jobs[i][j] << endl;
        }
        cout << endl;
    }
}

Params read_params_file(char *name) {
    ifstream file;
    file.open(name);

    int max_nodes_count = 0;
    int max_time = 0;

    int value1;
    int value2;

    while (file >> value1 >> value2) {
        max_nodes_count = value1;
        max_time = value2;
    };

    file.close();

    Params params = {max_nodes_count, max_time};
    return params;
}

void print_params(Params params) {
    cout << "Max number os nodes to be explored: " << params.max_nodes_count << endl;
    cout << "Max time of execution: " << params.max_time << "s" <<  endl;
}

// int main(int argc, char *argv[]) {
//     char *jobs_file_name = argv[1];
//     char *params_file_name = argv[2];

//     Jobs jobs = read_jobs_file(jobs_file_name);
//     Params params = read_params_file(params_file_name);

//     print_jobs(jobs.jobs);
//     print_params(params);

//     return 0;
// }
