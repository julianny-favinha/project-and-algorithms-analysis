#include "bnb-fs.hpp"

// nomes dos arquivos de entrada
char *jobs_file_name;
char *params_file_name;

// melhor limitante dual ate o momento
int best_dual = INT_MAX;

// tempo no qual melhor limitante dual foi obtido
clock_t time_best_lower_bound;

// melhor limitante primal ate o momento
int best_primal = INT_MAX;

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
    vector<int> melhores_posicoes;
    melhores_posicoes.reserve(n);

    vector<int> lowers;
    lowers.reserve(remaining.size());

    for (int i = 0; i < remaining.size(); i++) {
        int elemento = remaining[i];

        vector<int> new_order = order;
        new_order.push_back(elemento);

        vector<int> new_remaining = remaining;
        new_remaining.erase(new_remaining.begin() + i);

        Bounds bounds = estimate_bounds(jobs, n, new_order, new_remaining);
        lowers.push_back(bounds.lower_bound);

        if (bounds.upper_bound < best_primal) {
            best_primal = bounds.upper_bound;
            time_best_upper_bound = clock();
        }
    }

    int melhor_estimativa_lower_bound = best_primal;

    for (int i = 0; i < lowers.size(); i++) {
        int elemento = remaining[i];

        // cout << lowers[i] << " " << melhor_estimativa_lower_bound << endl;

        if (lowers[i] == melhor_estimativa_lower_bound) {
            melhores_posicoes.push_back(elemento);
        } else {
            if (lowers[i] < melhor_estimativa_lower_bound) {
                melhor_estimativa_lower_bound = lowers[i];
                melhores_posicoes.clear();
                melhores_posicoes.push_back(elemento);
            }      
        }

        if (lowers[i] < best_dual) {
            best_dual = lowers[i];                
            time_best_lower_bound = clock();
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

        if (nodes_visited_count > max_nodes_count) {
            cout << "ULTRAPASSOU MAX NODES COUNT COM: " << nodes_visited_count << endl;
            print_best_solution();
            exit(EXIT_FAILURE);
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

        if (soma < best_primal) {
            best_primal = soma;
            time_best_upper_bound = clock();
        }
    }
}

void print_best_solution() {
    clock_t finish = clock();
    double total_time_execution = ((finish - start) / (float)CLOCKS_PER_SEC);
    double total_time_best_lower_bound = ((time_best_lower_bound - start) / (float)CLOCKS_PER_SEC);
    double total_time_best_upper_bound = ((time_best_upper_bound - start) / (float)CLOCKS_PER_SEC);

    printf("%s,%d,%d,%d,%.2f,%.2f,%.2f\n", jobs_file_name, best_primal, best_dual, nodes_visited_count, total_time_best_upper_bound, total_time_best_lower_bound, total_time_execution);
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
