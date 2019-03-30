#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

struct Job {
    int time1;
    int time2;
};

vector<Job> read_jobs_file(char *name) {
    ifstream file;
    file.open(name);

    int n;
    file >> n;

    vector<Job> jobs(n);

    int time1;
    int time2;
    int i = 0;
    while (file >> time1 >> time2) {
        Job job = {time1, time2};
        jobs[i] = job;
        i++;
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