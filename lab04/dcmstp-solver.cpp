#include "input.hpp"
#include "agm.hpp"
#include "output.hpp"

int iterations = 10;

float best_dual = float(INT_MIN);
float best_primal = float(INT_MAX);
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
	if (iterations == 0 || (best_dual == best_primal && best_primal != float(INT_MAX))) {
		return true;
	}

	iterations--;
	return false;
}

// incrementa o custo das arestas (i,j): c_(i,j) += lambda(i) + lambda(j)
vector<NodeSource> increment_edges_cost(vector<NodeSource> adjacency, vector<float> lambdas) {
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
		if (adjacency[i].adj.size() < 1 || adjacency[i].adj.size() > adjacency[i].max_degree) {
			return false;
		}
	}

	return true;
}

// remove os lambdas somados no custo de cada aresta
vector<NodeSource> remove_lambdas(vector<NodeSource> adjacency, vector<float> lambdas) {
	vector<NodeSource> result = adjacency;

	for (int i = 0; i < result.size(); i++) {
		for (int j = 0; j < result[i].adj.size(); j++) {
			result[i].adj[j].cost = result[i].adj[j].cost - lambdas[i] - lambdas[result[i].adj[j].id];
		}
	}

	return result;
}

// inicializa lambdas
vector<float> first_lambda(int n) {
	vector<float> lambdas(n);

	for (int i = 0; i < lambdas.size(); i++) {
		lambdas[i] = 1.0;
	}

	return lambdas;
}

// atualiza lambdas
vector<float> update_lambdas(vector<float> lambdas, vector<NodeSource> dual, float cost_dual, float cost_primal) {
	int nodes = lambdas.size();

	vector<float> new_lambdas(nodes);

	float alpha = 2.0;
	float beta = 0.03;

	vector<int> s(nodes);
	int s_norm = 0;

	// calcula vetor s
	// calcula norma (ou modulo)
	for (int i = 0; i < nodes; i++) {
		s[i] = dual[i].adj.size() - dual[i].max_degree;
		s_norm += s[i] * s[i];
	}

	// calcula t
	float t = alpha*((1 + beta) * cost_primal - cost_dual) / s_norm;


	// atualiza lambdas
	for (int i = 0; i < nodes; i++) {
		new_lambdas[i] = max(0.0f, lambdas[i] + t * s[i]);
	}

	return new_lambdas;
}

// relaxação lagrangiana / algoritmo do subgradiente
void lagrangean_relaxation(vector<NodeSource> adjacency) {
	vector<float> lambdas = first_lambda(adjacency.size());

	while (stop_subgradient() == false) {
		if (time_expired()) {
			cout << "TEMPO EXPIRADO" << endl;
        	break;
    	}

		// cout << "GRAFO ORIGINAL" << endl;
		// print_graph(adjacency);

		vector<NodeSource> adjacency_with_lambdas = increment_edges_cost(adjacency, lambdas);

		// cout << "GRAFO COM LAMBDAS" << endl;
		// print_graph(adjacency_with_lambdas);
		
		// calcula arvore geradora minima (limitante dual)
		vector<NodeSource> dual_agm_with_lambdas = agm(adjacency_with_lambdas);

		if (time_expired()) {
			cout << "TEMPO EXPIRADO" << endl;
        	break;
    	}

		int cost_dual_with_lambdas = calculate_cost(dual_agm_with_lambdas);

		cout << "GRAFO DUAL" << endl;
		print_graph(dual_agm_with_lambdas);

		vector<NodeSource> dual_agm = remove_lambdas(dual_agm_with_lambdas, lambdas);
		int cost_dual = calculate_cost(dual_agm);

		cout << "CUSTO DUAL: " << cost_dual << endl << endl;

		if (cost_dual > best_dual) {
			best_dual = cost_dual;
		}

		int cost_primal = best_primal;

		if (is_solution(dual_agm)) {
			// cout << "É PRIMAL TAMBÉM" << endl;
			// se o dual é uma solução, então é um limitante primal também
			vector<NodeSource> primal_agm = dual_agm;
			cost_primal = calculate_cost(primal_agm);

			if (cost_primal < best_primal) {
				best_primal = cost_primal;
				best_agm = primal_agm;
			}
		} else {
			vector<NodeSource> primal_agm_with_lambdas = agm_with_degree_restriction(adjacency_with_lambdas);

			if (time_expired()) {
				cout << "TEMPO EXPIRADO" << endl;
	        	break;
    		}

			vector<NodeSource> primal_agm = remove_lambdas(primal_agm_with_lambdas, lambdas);
			cost_primal = calculate_cost(primal_agm);

			cout << "GRAFO PRIMAL" << endl;
			print_graph(primal_agm);

			cout << "CUSTO PRIMAL: " << cost_primal << endl << endl;

			if (cost_primal < best_primal) {
				best_primal = cost_primal;
				best_agm = primal_agm;
			}
		}

		if (time_expired()) {
			cout << "TEMPO EXPIRADO" << endl;
        	break;
    	}

		vector<float> new_lambdas = update_lambdas(lambdas, dual_agm_with_lambdas, cost_dual_with_lambdas, cost_primal);

		lambdas = new_lambdas;
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