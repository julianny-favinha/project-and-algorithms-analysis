#include "bnb-fs.hpp"

// guarda a melhor solucao ate o momento
BestSolution best_solution = {vector<int>(), 90000};

// guarda o melhor limitante dual ate o momento
int best_lower_bound = 90000;

// encontra melhores posicoes para continuar a recursao
vector<int> bound(vector<Job> jobs, vector<int> ordem, vector<int> restante) {
    int melhor_estimativa = INT_MAX;
    vector<int> melhores_posicoes;

    for (int i = 0; i < restante.size(); i++) {
        int elemento = restante[i];

        // *************************************************
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
        // *************************************************

        vector<int> new_order = ordem;
        new_order.push_back(elemento);

        int estimativa_do_elemento;

        if (new_order.size() < jobs.size()) {
            estimativa_do_elemento = estimate_lower_bound(jobs, new_order, new_order.size(), matriz);

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

                cout << "Atualizei melhor estimativa" << endl;
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
void branch_and_bound(vector<Job> jobs, vector<int> ordem, vector<int> restante) {
    cout << "ordem: ";
    for (int i = 0; i < ordem.size(); i++) {
        cout << ordem[i] << " ";
    }
    cout << endl;

    cout << "restante: ";
    for (int i = 0; i < restante.size(); i++) {
        cout << restante[i] << " ";
    }
    cout << endl;

    if (!restante.empty()) {
        vector<int> melhores_posicoes = bound(jobs, ordem, restante);

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
            branch_and_bound(jobs, ordem, restante);
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
            exit(0);
       }
    }
}

int main(int argc, char *argv[]) {
    char *input_file_name = argv[1];
    char *param_file_name = argv[2];

    vector<Job> jobs = read_jobs_file(input_file_name);
    vector<int> params = read_params_file(param_file_name);

//     cout << "Jobs: " << endl;
//     print_jobs(jobs);

    vector<int> ordem;
    vector<int> restante;

    for (int i = 0; i < jobs.size(); i++) {
        restante.push_back(i);
    }

    branch_and_bound(jobs, ordem, restante);

    return 0;
}
