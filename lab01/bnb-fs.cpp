#include "input.cpp"

struct BestSolution {
	list<int> order;
	int sum;
};

// guarda a melhor solucao ate o momento
BestSolution best_solution = {list<int>(), INT_MAX};

// guarda o melhor limitante dual ate o momento
int best_lower_bound = INT_MAX;

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

// calcula S1
int calcula_S1(vector<int> f, int n, int r, vector< vector<int> > d) {
    int s1 = 0;

    // cout << "D ANTES DE ORDERNAR" << endl;
    // for (int i = 0; i < d.size(); i++) {
    //     for (int j = 0; j < d[i].size(); j++) {
    //         cout << d[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    
    // ordenar d[0][r+1...n] crescentemente
    int m = n - r;
    vector<int> d0(m);
    for (int k = 0; k < m; k++) {
        d0[k] = d[0][k + r];
    }

    sort(d0.begin(), d0.end());

    cout << "d0 DEPOIS DE ORDENAR" << endl;
    for (int i = 0; i < d0.size(); i++) {
        cout << d0[i] << " ";
    }
    cout << endl;
    
    for (int k = r; k < n; k++) {
        cout << "f[r-1]: " <<  f[r - 1] << " (n-k+1): " << (n - k) << " d0[k-r]: "<<  d0[k-r] << " d[1][k]: "<<  d[1][k] << endl;
        s1 += f[r - 1] + (n - k) * d0[k-r] + d[1][k];
    }
    
    return s1;
}

// calcula S2
//int calcula_S2(vector<vector<int>> f, int n, int r, vector<vector<int>> d) {
//    int s2 = 0;
//
//    // ordenar d[1][r+1...n] crescentemente
//    int m = n - r;
//    vector<int> d1(m);
//    for (int k = 0; k < m; k++) {
//        d1[k] = d[1][k + r + 1];
//    }
//    sort(d1.begin(), d1.end());
//
//    int minimum = 0;
//
//    for (int k = r + 1; k < n; k++) {
//        s2 += max(f[1][r], f[0][r], minimum) + (n - k + 1) * d1[k - r - 1];
//    }
//
//    return s2;
//}

// calcula estimativa
void calcula_estimativa(vector<Job> jobs, int n, int r, vector< vector<int> > d) {
    cout << "n: " << n << " r: " << r << endl;

    vector<int> v(1, 0);
    vector<int> tempo_1 = final_times_m1(jobs, v);

    cout << "TEMPO_1: " << endl;
    for (int j = 0; j < n; j++) {
     cout << tempo_1[j] << " ";
    }
    cout << endl;

    int resultado_S1 = calcula_S1(tempo_1, n, r, d);
//    int resultado_S2 = calcula_S2(jobs, n, r, d);
    cout << resultado_S1 << "  " << endl;
}

// encontra melhores posicoes para continuar a recursao
//vector<int> encontra_melhores_posicoes(list<int> ordem, list<int> restante) {
//    int melhor_estimativa = INT_MAX;
//    vector<int> melhores_posicoes;
//
//    for (int i = 0; i < restante.size(); i++) {
//        int elemento = restante[i];
//
//        int estimativa_do_elemento = calcula_estimativa();
//
//        if (estimativa_do_elemento == melhor_estimativa) {
//            melhores_posicoes.push_back(elemento);
//        } else {
//            if (estimativa_do_elemento < melhor_estimativa) {
//                melhor_estimativa = estimativa_do_elemento;
//                melhores_posicoes.clear();
//                melhores_posicoes.push_back(elemento);
//            }
//        }
//    }
//
//    return melhores_posicoes;
//}

// recursao
//BestSolution branch_and_bound(vector<Job> jobs, list<int> ordem, list<int> restante) {
//    if (!restante.empty()) {
//        list<int> melhores_posicoes = encontra_melhores_posicoes(ordem, restante);
//
//        for (int i = 0; i < melhores_posicoes.size(); i++) {
//            int elemento = melhores_posicoes[i];
//            branch_and_bound(jobs, ordem.push_back(elemento), restante.remove(elemento));
//        }
//    } else {
//        int soma = total_time_sum(jobs, ordem);
//
//        if (soma == best_lower_bound || soma < best_solution_sum) {
//            best_solution.sum = soma;
//            best_solution.order = ordem;
//
//            cout << "ENCONTREI UMA MELHOR SOLUCAO" << endl << "Ordem: ";
//            // TODO: IMPRIMIER LISTA
//            cout << endl << "SOMA: " << best_solution.sum << endl;
//            exit(0);
//        }
//    }
//}

int main(int argc, char *argv[]) {
    char *input_file_name = argv[1];
    char *param_file_name = argv[2];

    vector<Job> jobs = read_jobs_file(input_file_name);
    vector<int> params = read_params_file(param_file_name);

    // cout << "Jobs: " << endl;
    // print_jobs(jobs);

//    list<int> ordem;
//    list<int> restante;
//
//    for (int i = 0; i < jobs.size(); i++) {
//        restante.push_back(i);
//    }
//
//    BestSolution best_solution = branch_and_bound(jobs, ordem, restante);
//
//    cout << "MELHOR SOLUCAO" << endl << "ORDEM: ";
//    for (int i = 0; i < best_solution.order.size(); i++) {
//        cout << best_solution.order[i] << " ";
//    }
//    cout << endl;
//
//    cout << "SOMA: " << best_solution.sum << endl;
//    vector<vector<int>> f, int n, int r, vector<vector<int>> d
    

    vector<int> linha1(3);
    vector<int> linha2(3);

    linha1[0] = 2;
    linha1[1] = 3;  
    linha1[2] = 2;


    linha2[0] = 1;
    linha2[1] = 1;  
    linha2[2] = 3;

    vector< vector<int> > matriz;

    matriz.push_back(linha1);
    matriz.push_back(linha2);

    calcula_estimativa(jobs, jobs.size(), 1, matriz);

    return 0;
};
