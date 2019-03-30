#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

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

int total_time_sum(vector<Job> jobs, vector<int> order) {
	int last_time_machine1 = 0;
	int last_time_machine2 = 0;
	int sum = 0;

	for (int i = 0; i < order.size(); i++) {
		int machine = order[i];
		last_time_machine1 = last_time_machine1 + jobs[machine].time1;
		last_time_machine2 = max(last_time_machine2, last_time_machine1) + jobs[machine].time2;
		sum += last_time_machine2;
	}

	return sum;
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