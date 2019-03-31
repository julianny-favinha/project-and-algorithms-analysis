#include "optimization.hpp"

// retorna soma total na maquina 2
int total_time_sum(vector< vector<int> > jobs, vector<int> order) {
    int last_time_machine1 = 0;
    int last_time_machine2 = 0;
    int sum = 0;

    for (int i = 0; i < order.size(); i++) {
        last_time_machine1 = last_time_machine1 + jobs[0][order[i]];
        last_time_machine2 = max(last_time_machine2, last_time_machine1) + jobs[1][order[i]];
        sum += last_time_machine2;
    }

    return sum;
}

// retorna vector de tempos finais na maquina 1
vector<int> final_times_m1(vector< vector<int> > jobs, vector<int> order) {
    vector<int> final_times(order.size());

    int last_time_machine1 = 0;

    for (int i = 0; i < order.size(); i++) {
        last_time_machine1 = last_time_machine1 + jobs[0][order[i]];
        final_times[i] = last_time_machine1;
    }

    return final_times;
}

// retorna vector de tempos finais na maquina 2
vector<int> final_times_m2(vector< vector<int> > jobs, vector<int> order) {
    vector<int> final_times(order.size());

    int last_time_machine1 = 0;
    int last_time_machine2 = 0;

    for (int i = 0; i < order.size(); i++) {
        last_time_machine1 = last_time_machine1 + jobs[0][order[i]];
        last_time_machine2 = max(last_time_machine2, last_time_machine1) + jobs[1][order[i]];
        final_times[i] = last_time_machine2;
    }

    return final_times;
}

static void initialize(vector<int> &d0, vector<int> &d1, vector<int> &order, int n, vector<int> &time_machine1, vector<int> &time_machine2) {
    d0 = vector<int>(n - order.size());
    d1 = vector<int>(n - order.size());

    for (int i = 0; i < order.size(); i++) {
        time_machine1[order[i]] = -1;
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
int s1(vector< vector<int> > jobs, int n, vector<int> order, vector<int> final_times_machine1) {
    int value = 0;
    int r = order.size();

    // cout << "M" << endl;
    // for (int i = 0; i < order.size(); i++) {
    //     cout << order[i] << " ";
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
    initialize(d0, d1, order, n, jobs[0], jobs[1]);

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
        // cout << "final_times_machine1[r-1]: " <<  final_times_machine1[r - 1] << " (n-k+1): " << (n - k) << " d0[k-r]: "<<  d0[k-r] << " d1[k-r]: "<<  d1[k-r] << endl;
        value += final_times_machine1[r-1] + (n-k) * d0[k-r] + d1[k-r];
    }

    return value;
}

// calcula S2
int s2(vector< vector<int> > jobs, int n, vector<int> order, vector<int> final_times_machine1, vector<int> final_times_machine2) {
    int value = 0;
    int r = order.size();

    vector<int> d0;
    vector<int> d1;
    initialize(d0, d1, order, n, jobs[0], jobs[1]);

    sort(d0.begin(), d0.end());
    sort(d1.begin(), d1.end());

    for (int k = r; k < n; k++) {
        value += (n - k) * d1[k - r];
    }

    int minimum = d0[0] + final_times_machine1[r-1];
    int maximum = max(final_times_machine2[r-1], minimum);

    return value + (n-r) * maximum;
}

// calcula estimativa
int estimate_lower_bound(vector< vector<int> > jobs, int n, vector<int> order) {
    // cout << "m: ";
    // for (int i = 0; i < m.size(); i++) {
    //     cout << m[i] << " ";
    // }
    // cout << endl;

    vector<int> time_machine1 = final_times_m1(jobs, order);

    // cout << "time_machine1: ";
    // for (int i = 0; i < time_machine1.size(); i++) {
    //     cout << time_machine1[i] << " ";
    // }

    vector<int> time_machine2 = final_times_m2(jobs, order);

    // cout << "/ time_machine2: ";
    // for (int i = 0; i < time_machine2.size(); i++) {
    //     cout << time_machine2[i] << " ";
    // }

    int sum_times_machine2 = total_time_sum(jobs, order);
    // cout << "/ sum_times_machine2: " << sum_times_machine2 << endl;

    int value_s1 = s1(jobs, n, order, time_machine1);
    int value_s2 = s2(jobs, n, order, time_machine1, time_machine2);
    // cout << "s1: " << value_s1 << ", s2: " << value_s2 << endl;
    return sum_times_machine2 + max(value_s1, value_s2);
}
