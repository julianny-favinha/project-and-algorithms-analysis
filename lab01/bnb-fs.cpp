#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct Job {
    int time1;
    int time2;
};

vector<Job> read_jobs_file(char *name) {
    ifstream file;
    file.open(name);

    int jobs_number;
    file >> jobs_number;

    vector<Job> jobs;
    int time1;
    int time2;
    while (file >> time1 >> time2) {
        Job job = {time1, time2};
        jobs.push_back(job);
    };

    file.close();

    return jobs;
}

int print_jobs(vector<Job> jobs) {
    for(int i = 0; i < jobs.size(); i++) {
        cout << jobs[i].time1 << " " << jobs[i].time2 << endl;
    }

    return 0;
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

int main(int argc, char *argv[]) {
    char *input_file_name = argv[1];
    char *param_file_name = argv[2];
    
    vector<Job> jobs = read_jobs_file(input_file_name);
    vector<int> params = read_params_file(param_file_name);

    print_jobs(jobs);

    return 0;
};
