#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

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

void total_time(vector<Job> jobs, vector<int> ordem) {
	int ultimo_tempo_maq1 = 0;
	int ultimo_tempo_maq2 = 0;
	int soma = 0;

	for (int i = 0; i < ordem.size(); i++) {
		ultimo_tempo_maq1 = ultimo_tempo_maq1 + jobs[ordem[i]].time1;
		ultimo_tempo_maq2 = max(ultimo_tempo_maq2, ultimo_tempo_maq1) + jobs[ordem[i]].time2;
		// cout << "maq1: " << ultimo_tempo_maq1 << ", maq2: " << ultimo_tempo_maq2 << endl;
		soma += ultimo_tempo_maq2;
	}

	cout << "Tempo na máquina 2: " << soma << endl;
}

void print_ordem(vector<int> ordem) {
	cout << "Ordem: ";
	for (int i = 0; i < ordem.size(); i ++) {
		cout << ordem[i] + 1 << " ";
	}

	cout << endl;
}

void brute_force(vector<Job> jobs, vector<int> ordem, vector<int> used, int k, int n) {
	if (k == n) {
		print_ordem(ordem);
		total_time(jobs, ordem);
	} else {
		for (int i = 0; i < n; i ++) {
			if (!used[i]) {
				used[i] = 1;
				ordem[k] = i;
				brute_force(jobs, ordem, used, k+1, n);
				used[i] = 0;
			}
		}
	}
}

int main(int argc, char *argv[]) {
    char *input_file_name = argv[1];
    char *param_file_name = argv[2];

    vector<Job> jobs = read_jobs_file(input_file_name);
    // vector<int> params = read_params_file(param_file_name);

    // cout << "JOBS";
    // print_jobs(jobs);

    int n = jobs.size();
    vector<int> used(n, 0);
    vector<int> ordem(n);

    brute_force(jobs, ordem, used, 0, n);

    return 0;
};
