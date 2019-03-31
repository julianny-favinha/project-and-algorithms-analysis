#include "bnb-fs.hpp"

// guarda a melhor solucao ate o momento
BestSolution best_solution = {list<int>(), 90000};

// guarda o melhor limitante dual ate o momento
int best_lower_bound = 90000;

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

//     cout << "Jobs: " << endl;
//     print_jobs(jobs);
//
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

    int lower_bound = estimate_lower_bound(jobs, jobs.size(), 1, matriz);
    cout << "Estimativa: " << lower_bound << endl;

    return 0;
}
