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

// retorna os tempos finais na maquina 1
vector<int> final_times_m1(vector< vector<int> > &jobs, vector<int> order) {
    vector<int> final_times(order.size());

    int last_time_machine1 = 0;

    for (int i = 0; i < order.size(); i++) {
        last_time_machine1 = last_time_machine1 + jobs[0][order[i]];
        final_times[i] = last_time_machine1;
    }

    return final_times;
}

// retorna os tempos finais na maquina 2
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
    vector<JobsMachine> jobs_machine(remaining.size());

    for (int i = 0; i < remaining.size(); i++) {
        JobsMachine jobsMachine = {remaining[i], jobs[0][remaining[i]], jobs[1][remaining[i]]};
        jobs_machine[i] = jobsMachine;
    }

    return jobs_machine;
}

void update_upper_bound(vector< vector<int> > &jobs, int n, vector<int> order, vector<JobsMachine> jobs_machine) {
    vector<int> scheduling;
    vector<int> remaining(jobs_machine.size());

    for (int i = 0; i < jobs_machine.size(); i++) {
        remaining[i] = jobs_machine[i].number;
    }

    scheduling.insert(scheduling.end(), order.begin(), order.end());
    scheduling.insert(scheduling.end(),remaining.begin(), remaining.end());

    int upper_bound = total_time_sum(jobs, scheduling);

    if (upper_bound < estimate_upper_bound) {
        estimate_upper_bound = upper_bound;
    }
}

// calcula S1
int s1(vector< vector<int> > &jobs, int n, vector<JobsMachine> jobs_machine, vector<int> order, vector<int> remaining, vector<int> final_times_machine1) {
    sort(jobs_machine.begin(), jobs_machine.end(), compare_two_jobs_machine1);

    vector<int> d0(remaining.size());
    vector<int> d1(remaining.size());

    for (int i = 0; i < jobs_machine.size(); i++) {
        d0[i] = jobs_machine[i].time1;
        d1[i] = jobs_machine[i].time2;
    }

    update_upper_bound(jobs, n, order, jobs_machine);

    int value = 0;
    int r = order.size();

    for (int k = r; k < n; k++) {
        value += final_times_machine1[r-1] + (n-k) * d0[k-r] + d1[k-r];
    }

    return value;
}

// calcula S2
int s2(vector< vector<int> > &jobs, int n, vector<JobsMachine> jobs_machine, vector<int> order, vector<int> remaining, vector<int> final_times_machine1, vector<int> final_times_machine2) {
    int value = 0;
    int r = order.size();

    sort(jobs_machine.begin(), jobs_machine.end(), compare_two_jobs_machine1);

    vector<int> d0(remaining.size());
    for (int i = 0; i < jobs_machine.size(); i++) {
        d0[i] = jobs_machine[i].time1;
    }

    sort(jobs_machine.begin(), jobs_machine.end(), compare_two_jobs_machine2);

    vector<int> d1(remaining.size());
    for (int i = 0; i < jobs_machine.size(); i++) {
        d1[i] = jobs_machine[i].time2;
    }

    update_upper_bound(jobs, n, order, jobs_machine);

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
    vector<int> time_machine2 = final_times_m2(jobs, order);

    int sum_times_machine2 = total_time_sum(jobs, order);

    vector<JobsMachine> jobs_machine = monta_estrutura(jobs, remaining);

    int value_s1 = s1(jobs, n, jobs_machine, order, remaining, time_machine1);
    int value_s2 = s2(jobs, n, jobs_machine, order, remaining, time_machine1, time_machine2);

    int estimate_lower_bound = sum_times_machine2 + max(value_s1, value_s2);

    Bounds bounds = {estimate_lower_bound, estimate_upper_bound};
    estimate_upper_bound = INT_MAX;
    return bounds;
}
