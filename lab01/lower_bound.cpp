#include <algorithm>

int calcula_S1(vector<vector<int>> f, int n, int r, vector<vector<int>> d) {
	int s1 = 0;

	// ordenar d[0][r+1...n] crescentemente
	int m = n - r;
	vector<int> d0(m);
	for (int k = 0; k < m; k++) {
		d0[k] = d[0][k + r + 1]
	}
	sort(d0.begin(), d0.end());

	for (int k = r + 1; k < n; k++) {
		s1 += f[0][r] + (n - k + 1) * d0[k] + d[k - r - 1];
	}

	return s1;
}

int calcula_S2(vector<vector<int>> f, int n, int r, vector<vector<int>> d) {
	int s2 = 0;

	// ordenar d[1][r+1...n] crescentemente
	int m = n - r;
	vector<int> d1(m);
	for (int k = 0; k < m; k++) {
		d1[k] = d[1][k + r + 1]
	}
	sort(d1.begin(), d1.end());

	int minimum = 0;

	for (int k = r + 1; k < n; k++) {
		s2 += max(f[1][r], f[0][r], minimum) + (n - k + 1) * d1[k - r - 1];
	}

	return s2;
}

