#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <list>

using namespace std;

struct Job {
    int time1;
    int time2;
};

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

    vector< vector<int> > d;

    d.push_back(machine1);
    d.push_back(machine2);

    return d;
}

// void print_total_time(vector< vector<int> > d, vector<int> order) {
//     int last_time_machine1 = 0;
//     int last_time_machine2 = 0;
//     int sum = 0;

//     for (int i = 0; i < order.size(); i++) {
//         int machine = order[i];
//         last_time_machine1 = last_time_machine1 + jobs[machine].time1;
//         last_time_machine2 = max(last_time_machine2, last_time_machine1) + jobs[machine].time2;
//         sum += last_time_machine2;
//         cout << "m1: " << last_time_machine1 << ", m2: " << last_time_machine2 << ", sum: " << sum << endl;
//     }

//     cout << "Time elapsed @ machine 2: " << sum << endl;
// }

int main(int argc, char *argv[]) {
    char *input_file_name = argv[1];
    char *param_file_name = argv[2];

    vector< vector<int> > d = read_jobs_file(input_file_name);

    for (int i = 0; i < d.size(); i++) {
    	for (int j = 0; j < d[i].size(); j++) {
    		cout << d[i][j] << " ";
    	}
    	cout << endl;
    }

    cout << "oi";

	return 0;
}
