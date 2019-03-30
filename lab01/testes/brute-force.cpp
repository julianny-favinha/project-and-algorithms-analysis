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

/*
Calculates the sum of the end times of each job executed on machine 2
*/
void total_time(vector<Job> jobs, vector<int> order) {
	int last_time_machine1 = 0;
	int last_time_machine2 = 0;
	int sum = 0;

	for (int i = 0; i < order.size(); i++) {
		int machine = order[i];
		last_time_machine1 = last_time_machine1 + jobs[machine].time1;
		last_time_machine2 = max(last_time_machine2, last_time_machine1) + jobs[machine].time2;
		sum += last_time_machine2;
		cout << "m1: " << last_time_machine1 << ", m2: " << last_time_machine2 << ", sum: " << sum << endl;
	}

	cout << "Time elapsed @ machine 2: " << sum << endl;
}

/*
Calculates array of final times each job executed on machine 2
*/
vector<int> final_times_m2(vector<Job> jobs, vector<int> order, int k) {
	vector<int> final_times(order.size());

	int last_time_machine1 = 0;
	int last_time_machine2 = 0;

	for (int i = 0; i < order.size() && i < k; i++) {
		int machine = order[i];
		last_time_machine1 = last_time_machine1 + jobs[machine].time1;
		last_time_machine2 = max(last_time_machine2, last_time_machine1) + jobs[machine].time2;
		final_times[i] = last_time_machine2;
	}

	return final_times;
}

void print_jobs_order(vector<int> order, int k) {
	cout << "Order: ";
	for (int i = 0; i < order.size() && i < k; i ++) {
		cout << order[i] + 1 << " ";
	}

	cout << endl;
}

void print_vector(vector<int> v, int k) {
	for (int i = 0; i < v.size() && i < k; i ++) {
		cout << v[i] << " ";
	}

	cout << endl;
}

int sum_vector(vector<int> v) {
	int sum = 0;
	for (int i = 0; i < v.size(); i ++) {
		sum += v[i];
	}

	return sum;
}

void brute_force(vector<Job> jobs, vector<int> order, vector<int> used, int k, int n) {
	if (k == n) {
		cout << "FOLHA ";
		print_jobs_order(order, k);

		// CALCULA SOMA DOS TEMOS FINAIS EM M2
		cout << "TOTAL SUM: ";
		cout << sum_vector(final_times_m2(jobs, order, k)) << endl;
	} else {
		for (int i = 0; i < n; i ++) {
			if (!used[i]) {
				used[i] = 1;
				order[k] = i;

				// print_jobs_order(order, k+1);
				// cout << "Final times in M2: ";
				// print_vector(final_times_m2(jobs, order, k+1), k+1);

				brute_force(jobs, order, used, k+1, n);

				used[i] = 0;
			}
		}
	}
}

int main(int argc, char *argv[]) {
    char *input_file_name = argv[1];
    char *param_file_name = argv[2];

    vector<Job> jobs = read_jobs_file(input_file_name);
    vector<int> params = read_params_file(param_file_name);

    // cout << "Jobs: " << endl;
    // print_jobs(jobs);

    int n = jobs.size();
    vector<int> used(n, 0);
    vector<int> order(n);
    brute_force(jobs, order, used, 0, n);

    return 0;
};
