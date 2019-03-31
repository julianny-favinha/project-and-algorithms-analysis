#include "input.hpp"

vector< vector<int> > read_jobs_file(char *name) {
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

    return jobs;
}

void print_jobs(vector< vector<int> > jobs) {
    for (int i = 0; i < jobs.size(); i++) {
        for (int j = 0; j < jobs[i].size(); j++) {
            cout << jobs[i][j] << " ";
        }
        cout << endl;
    }
}

vector<int> read_params_file(char *name) {
    ifstream file;
    file.open(name);
    
    vector<int> items;
    int item;
    while (file >> item) {
        items.push_back(item);
    };
    
    file.close();
    
    return items;
}

// int main(int argc, char *argv[]) {
//     char *input_file_name = argv[1];
//     char *param_file_name = argv[2];

//     vector< vector<int> > d = read_jobs_file(input_file_name);

//     print_jobs(d);
//     return 0;
// }
