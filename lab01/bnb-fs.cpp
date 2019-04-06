#include "bnb-fs.hpp"

// nomes dos arquivos de entrada
char *jobs_file_name;
char *params_file_name;

// melhor solucao ate o momento
BestSolution best_solution = {vector<int>(), INT_MAX};

// melhor limitante dual ate o momento
int best_lower_bound = INT_MAX;

// tempo no qual melhor limitante dual foi obtido
clock_t time_best_lower_bound;

// melhor limitante primal ate o momento
int best_upper_bound = 0;

// tempo no qual melhor limitante primal foi obtido
clock_t time_best_upper_bound;

// tempo de inicio de execucao
clock_t start;

// tempo maximo de execucao
int max_time;

// numero maximo de nos a serem explorados
int max_nodes_count;
int active_nodes_count = 0;

// numero de nos explorados
int nodes_visited_count = 0;

// encontra melhores posicoes para continuar a recursao
vector<int> bound(vector< vector<int> > jobs, int n, vector<int> order, vector<int> remaining) {
    int melhor_estimativa = INT_MAX;
    vector<int> melhores_posicoes;
    melhores_posicoes.reserve(n);

    for (int i = 0; i < remaining.size(); i++) {
        int elemento = remaining[i];

        vector<int> new_order = order;
        new_order.push_back(elemento);

        vector<int> new_remaining = remaining;
        new_remaining.erase(new_remaining.begin() + i);

        // int estimativa_do_elemento = estimate_lower_bound(jobs, n, new_order);
        int estimativa_do_elemento = estimate_bound(jobs, n, new_order, new_remaining);

        if (estimativa_do_elemento == melhor_estimativa) {
            melhores_posicoes.push_back(elemento);
        } else {
            if (estimativa_do_elemento < melhor_estimativa) {
                melhor_estimativa = estimativa_do_elemento;
                melhores_posicoes.clear();
                melhores_posicoes.push_back(elemento);
            }

            if (estimativa_do_elemento < best_lower_bound) {
                best_lower_bound = estimativa_do_elemento;
                // cout << "****** Novo best_lower_bound: " << best_lower_bound << endl;
                
                time_best_lower_bound = clock();
            }
        }    
    }

    // cout << "melhores_posicoes: ";
    // for (int i = 0; i < melhores_posicoes.size(); i++) {
    //     cout << melhores_posicoes[i] << " ";
    // }
    // cout << endl;

    return melhores_posicoes;
}

// recursao
void branch_and_bound(vector< vector<int> > jobs, int n, vector<int> order, vector<int> remaining) {
    // cout << "order: ";
    // for (int i = 0; i < order.size(); i++) {
    //     cout << order[i] << " ";
    // }

    // cout << "/ remaining: ";
    // for (int i = 0; i < remaining.size(); i++) {
    //     cout << remaining[i] << " ";
    // }
    // cout << endl;
    if (order.size() < n-1) {
        // cout << "order" << endl;
        // for (int i = 0; i < order.size(); i++) {
        //     cout << order[i] << " ";
        // }
        // cout << endl;

        vector<int> melhores_posicoes = bound(jobs, n, order, remaining);
        // cout << "melhores_posicoes" << endl;
        // for (int i = 0; i < melhores_posicoes.size(); i++) {
        //     cout << melhores_posicoes[i] << " ";
        // }
        // cout << endl;
        // // cout << melhores_posicoes.size();
        // active_nodes_count += melhores_posicoes.size();

        if (active_nodes_count > max_nodes_count) {
            cout << "ULTRAPASSOU MAX NODES COUNT COM: " << active_nodes_count << endl;
            print_best_solution();
            exit(0);
        }

        for (int i = 0; i < melhores_posicoes.size(); i++) {
            int elemento = melhores_posicoes[i];

            // encontra posicao para remover elemento
            int index = 0;
            for (index = 0; index < remaining.size(); index++) {
                if (remaining[index] == elemento) {
                    break;
                }
            }

            order.push_back(elemento);
            remaining.erase(remaining.begin() + index);
            nodes_visited_count++;

            active_nodes_count -= 1;

            branch_and_bound(jobs, n, order, remaining);

            order.pop_back();
            remaining.push_back(elemento);
        }
    } else {
        order.push_back(remaining[0]);
        remaining.clear();

        int soma = total_time_sum(jobs, order);
        // cout << "Soma folha: " << soma << endl;

        if (soma == best_lower_bound) {
            best_solution.sum = soma;
            best_solution.order = order;
        }

        if (soma < best_solution.sum) {
            best_solution.sum = soma;
            best_solution.order = order;
        }
    }
}

void print_best_solution() {
    clock_t finish = clock();
    double total_time_execution = ((finish - start) / (float)CLOCKS_PER_SEC);
    double total_time_best_lower_bound = ((time_best_lower_bound - start) / (float)CLOCKS_PER_SEC);

    printf("%s,<primal>,%d,%d,<t-primal>,%.2f,%.2f\n", jobs_file_name, best_lower_bound, nodes_visited_count, total_time_best_lower_bound, total_time_execution);
}

int main(int argc, char *argv[]) {
    start = clock();

    jobs_file_name = argv[1];
    params_file_name = argv[2];

    Jobs jobs = read_jobs_file(jobs_file_name);
    Params params = read_params_file(params_file_name);

    max_nodes_count = params.max_nodes_count;
    max_time = params.max_time;

    vector<int> order;
    order.reserve(jobs.count);
    vector<int> remaining;
    remaining.reserve(jobs.count);

    for (int i = 0; i < jobs.count; i++) {
        remaining.push_back(i);
    }

    branch_and_bound(jobs.jobs, jobs.count, order, remaining);

    print_best_solution();

    return 0;
}
