#include "optimization.hpp"

// imprime soma total na maquina 2
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

static void inicializaVetores(vector<int> &d0, vector<int> &d1, vector<int> &m, int n, vector<int> &tempo_maquina1, vector<int> &tempo_maquina2) {
    d0 = vector<int>(n - m.size());
    d1 = vector<int>(n - m.size());
    for (int i = 0; i < m.size(); i++) {
        tempo_maquina1[m[i]] = -1;
    }
    int k = 0;
    for (int i = 0; i < tempo_maquina1.size(); i++) {
        if (tempo_maquina1[i] != -1) {
            d0[k] = tempo_maquina1[i];
            d1[k] = tempo_maquina2[i];
            k++;
        }
    }
}

// calcula S1
int calcula_S1(vector<int> m, vector<int> f, int n, int r, vector<int> tempo_maquina1, vector<int> tempo_maquina2) {
    int s1 = 0;

    // cout << "M" << endl;
    // for (int i = 0; i < m.size(); i++) {
    //     cout << m[i] << " ";
    // }
    // cout << endl;

    // cout << "TEMPO MAQUINA 1" << endl;
    // for (int i = 0; i < tempo_maquina1.size(); i++) {
    //     cout << tempo_maquina1[i] << " ";
    // }
    // cout << endl;

    // cout << "TEMPO MAQUINA 2" << endl;
    // for (int i = 0; i < tempo_maquina2.size(); i++) {
    //     cout << tempo_maquina2[i] << " ";
    // }
    // cout << endl;

    vector<int> d0;
    vector<int> d1;
    inicializaVetores(d0, d1, m, n, tempo_maquina1, tempo_maquina2);

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
        s1 += f[r - 1] + (n - k) * d0[k-r] + d1[k-r];
    }

    return s1;
}

// calcula S2
int calcula_S2(vector<int> m, vector<int> f1, vector<int> f2, int n, int r, vector<int> tempo_maquina1, vector<int> tempo_maquina2) {
    int s2 = 0;

    vector<int> d0;
    vector<int> d1;
    inicializaVetores(d0, d1, m, n, tempo_maquina1, tempo_maquina2);

    sort(d0.begin(), d0.end());
    sort(d1.begin(), d1.end());

    for (int k = r; k < n; k++) {
        s2 += (n - k) * d1[k - r];
    }

    int minimum = d0[0] + f1[r - 1];
    int maximum = max(f2[r - 1], minimum);

    return s2 + (n - r) * maximum;
}

// calcula estimativa
void calcula_estimativa(vector<Job> jobs, int n, int r, vector< vector<int> > d) {
    cout << "n: " << n << " r: " << r << endl;

    vector<int> m(1, 1);
    vector<int> tempo_1 = final_times_m1(jobs, m);
    vector<int> tempo_2 = final_times_m2(jobs, m);

    cout << "TEMPO_1: " << endl;
    for (int j = 0; j < n; j++) {
        cout << tempo_1[j] << " ";
    }
    cout << endl;

    int s1 = calcula_S1(m, tempo_1, n, (int)m.size(), d[0], d[1]);
    int s2 = calcula_S2(m, tempo_1, tempo_2, n, (int)m.size(), d[0], d[1]);
    cout << "Resultado S1: " << s1 << endl << "Resultado S2: " << s2 << endl;
}
