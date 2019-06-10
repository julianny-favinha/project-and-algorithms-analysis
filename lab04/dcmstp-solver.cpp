#include "input.hpp"
#include "agm.hpp"
#include "output.hpp"

int iterations = 1;

int best_dual = INT_MIN;
int best_primal = INT_MAX;
vector<NodeSource> best_agm;

// tempo de inicio de execucao
clock_t start;

// tempo maximo de execucao
int max_time;

// verifica se tempo expirou
bool time_expired() {
    clock_t finish = clock();
    int total_time_execution = (int) ((finish - start) / (float)CLOCKS_PER_SEC);

    return total_time_execution > max_time;
}

// retorna true quando ocorreram todas as iterações OU se o limitante dual é igual ao primal
bool stop_subgradient() {
	if (iterations == 0 || (best_dual == best_primal && best_dual != INT_MAX)) {
		return true;
	}

	iterations--;
	return false;
}

// calcula custo de uma lista de adjacencias
int calculate_cost(vector<NodeSource> adjacency) {
	int cost = 0;
	int nodes = adjacency.size();

	for (int i = 0; i < nodes; i++) {
		for (int j = 0; j < adjacency[i].adj.size(); j++) {
			if (i < adjacency[i].adj[j].id) {
				cost += adjacency[i].adj[j].cost;
			}
		}
	}

	return cost;
}

// incrementa o custo das arestas (i,j): c_(i,j) = c_(i,j) + lambda(i) + lambda(j)
vector<NodeSource> increment_edges_cost(vector<NodeSource> adjacency, vector<int> lambdas) {
	vector<NodeSource> adjacency2 = adjacency;

	for (int u = 0; u < adjacency2.size(); u++) {
		for (int v = 0; v < adjacency2[u].adj.size(); v++) {
			adjacency2[u].adj[v].cost += lambdas[u] + lambdas[adjacency2[u].adj[v].id];
		}
	}

	return adjacency2;
}

// verifica se a lista de adjacencias é uma solução viavel
bool is_solution(vector<NodeSource> adjacency) {
	for (int i = 0; i < adjacency.size(); i++) {
		if (adjacency[i].adj.size() > adjacency[i].max_degree) {
			return false;
		}
	}

	return true;
}

vector<NodeSource> remove_lambdas(vector<NodeSource> adjacency, vector<int> lambdas) {
	vector<NodeSource> result = adjacency;

	for (int i = 0; i < result.size(); i++) {
		for (int j = 0; j < result[i].adj.size(); j++) {
			result[i].adj[j].cost = result[i].adj[j].cost - lambdas[i] - lambdas[result[i].adj[j].id];
		}
	}

	return result;
}

// inicializa lambda
vector<int> first_lambda(int n) {
	vector<int> lambdas(n);

	for (int i = 0; i < lambdas.size(); i++) {
		lambdas[i] = 1;
	}

	return lambdas;
}

// atualiza lambda com o passo u_k
// TODO qual o tamanho do passo?
void update_lambdas(vector<int> *lambdas) {
	for (int i = 0; i < lambdas->size(); i++) {
		(*lambdas)[i] += 1;
	}
}

// relaxação lagrangiana / algoritmo do subgradiente
void lagrangean_relaxation(vector<NodeSource> adjacency) {
	vector<int> lambdas = first_lambda(adjacency.size());

	while (stop_subgradient() == false) {
		if (time_expired()) {
        	break;
    	}

		cout << "GRAFO ORIGINAL" << endl;
		print_graph(adjacency);

		vector<NodeSource> adjacency_with_lambdas = increment_edges_cost(adjacency, lambdas);

		cout << "GRAFO COM LAMBDAS" << endl;
		print_graph(adjacency_with_lambdas);
		
		// calcula arvore geradora minima (limitante dual)
		vector<NodeSource> dual_agm_with_lambdas = agm(adjacency_with_lambdas);

		if (time_expired()) {
        	break;
    	}

		cout << "GRAFO AGM" << endl;
		print_graph(dual_agm_with_lambdas);

		vector<NodeSource> dual_agm = remove_lambdas(dual_agm_with_lambdas, lambdas);
		int cost_dual = calculate_cost(dual_agm);

		cout << "CUSTO AGM" << endl;
		cout << cost_dual << endl;

		if (cost_dual > best_dual) {
			best_dual = cost_dual;
		}

		if (is_solution(dual_agm)) {
			cout << "É PRIMAL TAMBÉM" << endl;
			// se o dual é uma solução, então é um limitante primal também
			vector<NodeSource> primal_agm = dual_agm;
			int cost_primal = calculate_cost(primal_agm);

			if (cost_primal < best_primal) {
				best_primal = cost_primal;
				best_agm = primal_agm;
			}
		} else {
			cout << "NÃO É PRIMAL" << endl;
			vector<NodeSource> primal_agm_with_lambdas = agm_with_degree_restriction(adjacency_with_lambdas);
			vector<NodeSource> primal_agm = remove_lambdas(primal_agm_with_lambdas, lambdas);
			int cost_primal = calculate_cost(primal_agm);

			if (cost_primal < best_primal) {
				best_primal = cost_primal;
				best_agm = primal_agm;
			}
		}

		update_lambdas(&lambdas);
	}
}

int main(int argc, char *argv[]) {
	start = clock();

	char *file_name = argv[1];
	int time = atoi(argv[2]);
	char *method = argv[3];

	vector<NodeSource> adjacency = read_file(file_name);
	max_time = time;

	if (string(method) == "l") {
		lagrangean_relaxation(adjacency);
		cout << file_name << "," << best_dual << "," << best_primal << endl;
	} else {
		cout << "IMPLEMENTAR META-HEURÍSTICA" << endl;
		cout << file_name << "," << best_primal << endl;
	}

	save_output(file_name, best_agm);

	return 0;
}