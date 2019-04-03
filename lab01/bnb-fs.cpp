#include "bnb-fs.hpp"

// melhor solucao ate o momento
BestSolution best_solution = {vector<int>(), INT_MAX};

// melhor limitante dual ate o momento
int best_lower_bound = INT_MAX;

// tempo de execucao do branch and bound
clock_t start;

// numero maximo de nos a serem explorados
int max_nodes_count;

// tempo maximo de execucao
int max_time;

// encontra melhores posicoes para continuar a recursao
vector<int> bound(vector< vector<int> > jobs, int n, vector<int> order, vector<int> remaining) {
    int melhor_estimativa = INT_MAX;
    vector<int> melhores_posicoes;
    melhores_posicoes.reserve(n);

    for (int i = 0; i < remaining.size(); i++) {
        int elemento = remaining[i];

        vector<int> new_order = order;
        new_order.push_back(elemento);

        int estimativa_do_elemento = estimate_lower_bound(jobs, n, new_order);

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

// // recursao
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
        vector<int> melhores_posicoes = bound(jobs, n, order, remaining);
        // cout << melhores_posicoes.size();
        if (melhores_posicoes.size() > max_nodes_count) {
            cout << "LIMITE DE NOS ATINGIDO: melhores_posicoes.size() = " << melhores_posicoes.size() << " e max_nodes_count = " << max_nodes_count << endl;
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
    cout << "****ENCONTREI UMA MELHOR SOLUCAO****" << endl << "ORDEM: ";
    for (int i = 0; i < best_solution.order.size(); i++) {
        cout << best_solution.order[i] << " ";
    }
    cout << endl << "SOMA: " << best_solution.sum << endl;
}

int main(int argc, char *argv[]) {\
    start = clock();

    char *jobs_file_name = argv[1];
    char *params_file_name = argv[2];

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

    clock_t finish = clock();
    printf("Total time: %.5fs\n", ((finish - start) / (float)CLOCKS_PER_SEC));

    return 0;
}
