#include "optimization.hpp"

// imprime soma total na maquina 2
void print_total_time(vector<Job> jobs, vector<int> order) {
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

// retorna soma total na maquina 2
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

// retorna vector de tempos finais na maquina 1
vector<int> final_times_m1(vector<Job> jobs, vector<int> order) {
    vector<int> final_times(order.size());

    int last_time_machine1 = 0;

    for (int i = 0; i < order.size(); i++) {
        int machine = order[i];
        last_time_machine1 = last_time_machine1 + jobs[machine].time1;
        final_times[i] = last_time_machine1;
    }

    return final_times;
}

// retorna vector de tempos finais na maquina 2
vector<int> final_times_m2(vector<Job> jobs, vector<int> order) {
    vector<int> final_times(order.size());

    int last_time_machine1 = 0;
    int last_time_machine2 = 0;

    for (int i = 0; i < order.size(); i++) {
        int machine = order[i];
        last_time_machine1 = last_time_machine1 + jobs[machine].time1;
        last_time_machine2 = max(last_time_machine2, last_time_machine1) + jobs[machine].time2;
        final_times[i] = last_time_machine2;
    }

    return final_times;
}

static void initialize(vector<int> &d0, vector<int> &d1, vector<int> &m, int n, vector<int> &time_machine1, vector<int> &time_machine2) {
    d0 = vector<int>(n - m.size());
    d1 = vector<int>(n - m.size());
    for (int i = 0; i < m.size(); i++) {
        time_machine1[m[i]] = -1;
    }
    int k = 0;
    for (int i = 0; i < time_machine1.size(); i++) {
        if (time_machine1[i] != -1) {
            d0[k] = time_machine1[i];
            d1[k] = time_machine2[i];
            k++;
        }
    }
}

// calcula S1
int s1(vector<int> m, vector<int> f, int n, int r, vector<int> time_machine1, vector<int> time_machine2) {
    int value = 0;

    // cout << "M" << endl;
    // for (int i = 0; i < m.size(); i++) {
    //     cout << m[i] << " ";
    // }
    // cout << endl;

    // cout << "TEMPO MAQUINA 1" << endl;
    // for (int i = 0; i < time_machine1.size(); i++) {
    //     cout << time_machine1[i] << " ";
    // }
    // cout << endl;

    // cout << "TEMPO MAQUINA 2" << endl;
    // for (int i = 0; i < time_machine2.size(); i++) {
    //     cout << time_machine2[i] << " ";
    // }
    // cout << endl;

    vector<int> d0;
    vector<int> d1;
    initialize(d0, d1, m, n, time_machine1, time_machine2);

    // cout << "d0 ANTES DE ORDENAR" << endl;
    // for (int i = 0; i < d0.size(); i++) {
    //     cout << d0[i] << " ";
    // }
    // cout << endl;

    sort(d0.begin(), d0.end());

    // cout << "d0 DEPOIS DE ORDENAR" << endl;
    // for (int i = 0; i < d0.size(); i++) {
    //     cout << d0[i] << " ";
    // }
    // cout << endl;

    // cout << "d1" << endl;
    // for (int i = 0; i < d1.size(); i++) {
    //     cout << d1[i] << " ";
    // }
    // cout << endl;

    for (int k = r; k < n; k++) {
        cout << "f[r-1]: " <<  f[r - 1] << " (n-k+1): " << (n - k) << " d0[k-r]: "<<  d0[k-r] << " d1[k-r]: "<<  d1[k-r] << endl;
        value += f[r - 1] + (n - k) * d0[k-r] + d1[k-r];
    }

    return value;
}

// calcula S2
int s2(vector<int> m, vector<int> f1, vector<int> f2, int n, int r, vector<int> time_machine1, vector<int> time_machine2) {
    int value = 0;

    vector<int> d0;
    vector<int> d1;
    initialize(d0, d1, m, n, time_machine1, time_machine2);

    sort(d0.begin(), d0.end());
    sort(d1.begin(), d1.end());

    for (int k = r; k < n; k++) {
        value += (n - k) * d1[k - r];
    }

    int minimum = d0[0] + f1[r - 1];
    int maximum = max(f2[r - 1], minimum);

    return value + (n - r) * maximum;
}

// calcula estimativa
int estimate_lower_bound(vector<Job> jobs, vector<int> m, int r, vector< vector<int> > d) {
    cout << "m: ";
    for (int i = 0; i < m.size(); i++) {
        cout << m[i] << " ";
    }
    cout << endl;

    vector<int> time_machine1 = final_times_m1(jobs, m);

    cout << "time_machine1: ";
    for (int i = 0; i < time_machine1.size(); i++) {
        cout << time_machine1[i] << " ";
    }
    cout << endl;

    vector<int> time_machine2 = final_times_m2(jobs, m);

    cout << "time_machine2: ";
    for (int i = 0; i < time_machine2.size(); i++) {
        cout << time_machine2[i] << " ";
    }
    cout << endl;

    int sum_times_machine2 = total_time_sum(jobs, m);
    cout << "sum_times_machine2: " << sum_times_machine2 << endl;

    int value_s1 = s1(m, time_machine1, jobs.size(), m.size(), d[0], d[1]);
    int value_s2 = s2(m, time_machine1, time_machine2, jobs.size(), m.size(), d[0], d[1]);
    cout << "S1: " << value_s1 << ", S2: " << value_s2 << endl;

    return sum_times_machine2 + max(value_s1, value_s2);
}
