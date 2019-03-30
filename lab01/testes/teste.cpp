#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <list>


using namespace std;

struct BestSolution {
	list<int> order;
	int sum;
};

int print_stack(stack<int> stack_jobs) {
	vector<int> jobs;

	while (!stack_jobs.empty()) {
		int elem = stack_jobs.top();
		stack_jobs.pop();
		jobs.push_back(elem);
	}

	cout << "PERMUTA ";
	for(int i = jobs.size() - 1; i >= 0; i--) {
		cout << jobs[i] << " ";
	}
	cout << endl;

	return 0;
}

void print(list<int> const &list) {
	for (auto it = list.cbegin(); it != list.cend(); it++) {
		cout << *it << " ";
	}

	cout << endl;
}

int main() {
	// int n = 3;
	// vector< vector<int> > elements(n, vector<int>(n));

	// for (int i = 0; i < n; i++) {
	// 	for (int j = 0; j < n; j++) {
	// 		elements[i][j] = i-j;
	// 	}
	// }

	// for (int i = 0; i < n; i++) {
	// 	for (int j = 0; j < n; j++) {
	// 		cout << elements[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }

	// sort(elements[1].begin(), elements[1].end());

	// cout << "SORTING" << endl;
	
	// for (int i = 0; i < n; i++) {
	// 	for (int j = 0; j < n; j++) {
	// 		cout << elements[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }

	// cout << "LINHA" << endl;
	// vector<int> linha = elements[0];

	// for (int j = 0; j < n; j++) {
	// 	cout << linha[j] << " ";
	// }
	// cout << endl;

	// int n = 3;
	// vector<int> used(n, 0);

	// vector<int> v;
	// vector<int> u;

	// for (int i = 0; i < 1000000000; i++) {
	// 	v.push_back(1);
	// }

	// u = v;

	// v[0] = 10;

	// cout << v[0] << endl;
	// cout << u[0] << endl;

	BestSolution solution = {list<int>(), -4};

	solution.order.push_back(1);
	solution.order.push_back(10);

	print(solution.order);

	// cout << solution.sum << endl;

	return 0;
}
