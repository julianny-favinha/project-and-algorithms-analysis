#include "bnb-fs.hpp"

// melhor solucao ate o momento
BestSolution best_solution = {vector<int>(), INT_MAX};

// melhor limitante dual ate o momento
int best_lower_bound = INT_MAX;

// tempo de execucao do branch and bound
clock_t tempo_inicio;

// encontra melhores posicoes para continuar a recursao
vector<int> bound(vector< vector<int> > jobs, int n, vector<int> ordem, vector<int> restante) {
    int melhor_estimativa = INT_MAX;
    vector<int> melhores_posicoes;

    for (int i = 0; i < restante.size(); i++) {
        int elemento = restante[i];

        vector<int> new_order = ordem;
        new_order.push_back(elemento);

        int estimativa_do_elemento;

        if (new_order.size() < n) {
            estimativa_do_elemento = estimate_lower_bound(jobs, n, new_order);

            cout << "estimativa_do_elemento: " << estimativa_do_elemento << endl;
        } else {
            cout << "CHEGOU NUMA FOLHA" << endl;
            estimativa_do_elemento = total_time_sum(jobs, new_order);
        }

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
                cout << "-> Atualizei best_lower_bound: " << best_lower_bound << endl;
            }
        }
    }

    cout << "melhores_posicoes: ";
        for (int i = 0; i < melhores_posicoes.size(); i++) {
            cout << melhores_posicoes[i] << " ";
        }
    cout << endl;

    return melhores_posicoes;
}

// // recursao
void branch_and_bound(vector< vector<int> > jobs, int n, vector<int> ordem, vector<int> restante) {
    cout << "ordem: ";
    for (int i = 0; i < ordem.size(); i++) {
        cout << ordem[i] << " ";
    }

    cout << "/ restante: ";
    for (int i = 0; i < restante.size(); i++) {
        cout << restante[i] << " ";
    }
    cout << endl;

    if (!restante.empty()) {
        vector<int> melhores_posicoes = bound(jobs, n, ordem, restante);

        for (int i = 0; i < melhores_posicoes.size(); i++) {
            int elemento = melhores_posicoes[i];

            // encontra posicao para remover elemento
            int index = 0;
            for (index = 0; index < restante.size(); index++) {
                if (restante[index] == elemento) {
                    break;
                }
            }

            ordem.push_back(elemento);
            restante.erase(restante.begin() + index);
            branch_and_bound(jobs, n, ordem, restante);
        }
    } else {
       int soma = total_time_sum(jobs, ordem);

        if (soma == best_lower_bound || soma < best_solution.sum) {
            best_solution.sum = soma;
            best_solution.order = ordem;

            cout << "****ENCONTREI UMA MELHOR SOLUCAO****" << endl << "ORDEM: ";
            for (int i = 0; i < best_solution.order.size(); i++) {
                cout << best_solution.order[i] << " ";
            }
            cout << endl << "SOMA: " << best_solution.sum << endl;

            clock_t tempo_termino = clock();

            printf("[CLOCK] Tempo total: %.5f segundos\n", ((tempo_termino - tempo_inicio) / (float)CLOCKS_PER_SEC));

            exit(0);
       }
    }
}

int main(int argc, char *argv[]) {
    char *input_file_name = argv[1];
    char *param_file_name = argv[2];

    Input input = read_jobs_file(input_file_name);
    vector<int> params = read_params_file(param_file_name);

    vector<int> ordem;
    vector<int> restante;

    for (int i = 0; i < input.count; i++) {
        restante.push_back(i);
    }

    tempo_inicio = clock();

    branch_and_bound(input.jobs, input.count, ordem, restante);

    return 0;
}
