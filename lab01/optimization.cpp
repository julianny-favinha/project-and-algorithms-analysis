#include "optimization.hpp"

int estimate_upper_bound = INT_MAX;

// retorna soma total na maquina 2
int total_time_sum(vector< vector<int> > &jobs, vector<int> order) {
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
vector<int> final_times_m1(vector< vector<int> > &jobs, vector<int> order) {
    vector<int> final_times(order.size());

    int last_time_machine1 = 0;

    for (int i = 0; i < order.size(); i++) {
        last_time_machine1 = last_time_machine1 + jobs[0][order[i]];
        final_times[i] = last_time_machine1;
    }

    return final_times;
}

// retorna vector de tempos finais na maquina 2
vector<int> final_times_m2(vector< vector<int> > &jobs, vector<int> order) {
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

bool compare_two_jobs_machine1(JobsMachine lhs, JobsMachine rhs) {
    return lhs.time1 < rhs.time1;
}

bool compare_two_jobs_machine2(JobsMachine lhs, JobsMachine rhs) {
    return lhs.time2 < rhs.time2;
}

vector<JobsMachine> monta_estrutura(vector< vector<int> > &jobs, vector<int> &remaining) {
    vector<JobsMachine> nova_estrutura(remaining.size());

    int i = 0;
    for (int j = 0; j < remaining.size(); j++) {
        JobsMachine jobsMachine = {remaining[j], jobs[0][remaining[j]], jobs[1][remaining[j]]};
        nova_estrutura[i] = jobsMachine;
        i++;
    }

    return nova_estrutura;
}

void atualiza_upper_bound(vector< vector<int> > &jobs, int n, vector<int> order, vector<JobsMachine> nova_estrutura) {
    vector<int> escalonamento(n);
    int k = 0;

    for (int i = 0; i < order.size(); i++) {
        escalonamento[k] = order[i];
        k++;
    }

    for (int i = 0; i < nova_estrutura.size(); i++) {
        escalonamento[k] = nova_estrutura[i].number;
        k++;
    }

    int upper_bound = total_time_sum(jobs, escalonamento);

    if (upper_bound < estimate_upper_bound) {
        estimate_upper_bound = upper_bound;
    }

    // cout << "ORDEM: ";
    // for (int i = 0; i < escalonamento.size(); i++) {
    //     cout << escalonamento[i] << " ";
    // }
    // cout << endl;

    // cout << "estimate_upper_bound" << estimate_upper_bound << endl;
}

// calcula S1
int s1(vector< vector<int> > &jobs, int n, vector<JobsMachine> nova_estrutura, vector<int> order, vector<int> remaining, vector<int> final_times_machine1) {
    sort(nova_estrutura.begin(), nova_estrutura.end(), compare_two_jobs_machine1);

    // cout << "NOVA ESTRUTURA DEPOIS DE ORDERNAR" << endl;
    // for (int i = 0; i < nova_estrutura.size(); i++) {
    //     cout << nova_estrutura[i].number << " " << nova_estrutura[i].time1 << " " << nova_estrutura[i].time2 << endl;
    // }

    vector<int> d0(remaining.size());
    vector<int> d1(remaining.size());

    for (int i = 0; i < nova_estrutura.size(); i++) {
        d0[i] = nova_estrutura[i].time1;
        d1[i] = nova_estrutura[i].time2;
    }

    atualiza_upper_bound(jobs, n, order, nova_estrutura);

    int value = 0;
    int r = order.size();

    for (int k = r; k < n; k++) {
        value += final_times_machine1[r-1] + (n-k) * d0[k-r] + d1[k-r];
    }

    return value;
}

// calcula S2
int s2(vector< vector<int> > &jobs, int n, vector<JobsMachine> nova_estrutura, vector<int> order, vector<int> remaining, vector<int> final_times_machine1, vector<int> final_times_machine2) {
    int value = 0;
    int r = order.size();

    sort(nova_estrutura.begin(), nova_estrutura.end(), compare_two_jobs_machine1);

    vector<int> d0(remaining.size());
    for (int i = 0; i < nova_estrutura.size(); i++) {
        d0[i] = nova_estrutura[i].time1;
    }

    sort(nova_estrutura.begin(), nova_estrutura.end(), compare_two_jobs_machine2);

    vector<int> d1(remaining.size());
    for (int i = 0; i < nova_estrutura.size(); i++) {
        d1[i] = nova_estrutura[i].time2;
    }

    atualiza_upper_bound(jobs, n, order, nova_estrutura);

    for (int k = r; k < n; k++) {
        value += (n - k) * d1[k - r];
    }

    int minimum = d0[0] + final_times_machine1[r-1];
    int maximum = max(final_times_machine2[r-1], minimum);

    return value + (n-r) * maximum;
}

// calcula estimativa
Bounds estimate_bounds(vector< vector<int> > &jobs, int n, vector<int> order, vector<int> remaining) {
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

    vector<JobsMachine> nova_estrutura = monta_estrutura(jobs, remaining);
    // cout << "NOVA ESTRUTURA" << endl;
    // for (int i = 0; i < nova_estrutura.size(); i++) {
    //     cout << nova_estrutura[i].number << " " << nova_estrutura[i].time1 << " " << nova_estrutura[i].time2 << endl;
    // }

    int value_s1 = s1(jobs, n, nova_estrutura, order, remaining, time_machine1);
    int value_s2 = s2(jobs, n, nova_estrutura, order, remaining, time_machine1, time_machine2);
    // cout << "s1: " << value_s1 << ", s2: " << value_s2 << endl;

    int estimate_lower_bound = sum_times_machine2 + max(value_s1, value_s2);

    Bounds bounds = {estimate_lower_bound, estimate_upper_bound};
    estimate_upper_bound = INT_MAX;
    return bounds;
}
